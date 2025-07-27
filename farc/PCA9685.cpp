#include "PCA9685.h"
#include <Wire.h>

PCA9685::PCA9685() {
  _i2cAddr = PCA9685_I2C_ADDRESS;
}

PCA9685::PCA9685(const uint8_t addr) {
  _i2cAddr = addr;
}

void PCA9685::begin() {
  Wire.begin();
  writeReg(PCA9685_MODE1, MODE1_RESTART); // Restart enabled
  delay(10);
  
  setFreq(DEFAUT_PWM_FREQ);
}

void PCA9685::setFreq(float freq) {
  freq = constrain(freq, 1, 3500); // Datasheet limit is 3052=50MHz/(4*4096)

  float prescaleval = ((PCA9685_INTERNAL_OSC / (freq * 4096.0)) + 0.5) - 1;
  prescaleval = constrain(prescaleval, PCA9685_PRESCALE_MIN, PCA9685_PRESCALE_MAX); 
  uint8_t prescale = (uint8_t)prescaleval;
  Serial.print("prescale: "); Serial.println(prescale);

  uint8_t oldmode = readReg(PCA9685_MODE1);
  uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
  writeReg(PCA9685_MODE1, newmode);                             // go to sleep
  writeReg(PCA9685_PRESCALE, prescale); // set the prescaler
  writeReg(PCA9685_MODE1, oldmode);
  delay(5);
  // This sets the MODE1 register to turn on auto increment.
  writeReg(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
}

void PCA9685::setPWM(uint8_t channel, uint16_t pwmValue) {
  uint16_t on = (pwmValue == 4095) ? 4096 : 0;
  uint16_t off = (pwmValue == 4095) ? 0 : ((pwmValue == 0) ? 4096 : pwmValue);
  uint8_t addr = PCA9685_LED0_ON_L + 4 * channel;
  uint8_t buffer[4];
  buffer[0] = on;
  buffer[1] = on >> 8;
  buffer[2] = off;
  buffer[3] = off >> 8;
  writeRegs(addr, buffer, 4);
}

/******************* Low level I2C interface */
uint8_t PCA9685::readReg(uint8_t addr) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);
  Wire.endTransmission(false);

  Wire.requestFrom(_i2cAddr, (uint8_t)1); // Yêu cầu 1 byte
  uint8_t data = 0;
  while (Wire.available()) {
    data = Wire.read();
  }
  return data;
}

void PCA9685::writeReg(uint8_t addr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);  // Gửi địa chỉ thanh ghi
  Wire.write(data);  // Gửi dữ liệu
  Wire.endTransmission(); // Kết thúc giao tiếp
}

void PCA9685::writeRegs(uint8_t addr, uint8_t* buff, uint8_t length) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);  // Gửi địa chỉ thanh ghi
  for (uint8_t i = 0; i < length; i++) {
    Wire.write(buff[i]);
  }
  Wire.endTransmission(); // Kết thúc truyền
}
