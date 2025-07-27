#include "ViaBoard.h"

void ViaBoard::init() {
  for (uint8_t i=0; i++; i<NUM_OF_MOTORS) {
    _motorSpeeds[i] = MOTOR_SPEED_DEFAULT;
  } 
  
  _pwm16 = PCA9685();
  _pwm16.begin();
  _pwm16.setFreq(SERVO_FREQUENCY); 
}

/*--------------------------------*/
void ViaBoard::motor_speed(uint8_t motor_id, uint8_t motor_speed) {
  uint8_t speed = constrain(motor_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  _motorSpeeds[motor_id] = speed;
  Serial.print("motor_speed: "); Serial.print(" M-ID: ");  Serial.print(motor_id); Serial.print(" - Speed: "); Serial.println(speed);
}
/*--------------------------------*/
void ViaBoard::motor_incSpeed(uint8_t motor_id) {
  uint8_t speed = constrain(_motorSpeeds[motor_id]+1, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  _motorSpeeds[motor_id] = speed;
  Serial.print("motor_incSpeed: "); Serial.print(" M-ID: ");  Serial.print(motor_id); Serial.print(" - Speed: "); Serial.println(speed);
}
/*--------------------------------*/
void ViaBoard::motor_decSpeed(uint8_t motor_id) {
  uint8_t speed = constrain(_motorSpeeds[motor_id]-1, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  _motorSpeeds[motor_id] = speed;
  Serial.print("motor_decSpeed: "); Serial.print(" M-ID: ");  Serial.print(motor_id); Serial.print(" - Speed: "); Serial.println(speed);
}
/*--------------------------------*/
void ViaBoard::motor_rotate(uint8_t motor_id, bool isCW) {
  Serial.print("motor_rotate: "); Serial.print(" M-ID: ");  Serial.print(motor_id); Serial.print(" - isCW: "); Serial.println(isCW);

  // Đổi từ tốc độ sang giá trị pwm
  uint16_t pwmValue = map(_motorSpeeds[motor_id], MOTOR_SPEED_MIN, MOTOR_SPEED_MAX, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
  Serial.print("Speed: "); Serial.println(_motorSpeeds[motor_id]); 
  Serial.print("pwmValue: "); Serial.println(pwmValue); 
  
  // Tính ID của kênh PWM
  uint8_t baseID = MOTOR_1_CHANNEL_A + motor_id*2;
  uint8_t channelA = baseID + (isCW ? 0 : 1);
  uint8_t channelB = baseID + (isCW ? 1 : 0);

  _pwm16.setPWM(channelA, pwmValue);
  _pwm16.setPWM(channelB, 0);
}
/*--------------------------------*/
// Chỉ định giá trị PWM
void ViaBoard::motor_pwm(uint8_t motor_id, bool isCW, uint16_t pwmValue) {
  Serial.print("motor_pwm: "); Serial.print(" M-ID: ");  Serial.print(motor_id); Serial.print(" - isCW: "); Serial.println(isCW);
  Serial.print("pwmValue: "); Serial.println(pwmValue); 
  
  // Tính ID của kênh PWM
  uint8_t baseID = MOTOR_1_CHANNEL_A + motor_id*2;
  uint8_t channelA = baseID + (isCW ? 0 : 1);
  uint8_t channelB = baseID + (isCW ? 1 : 0);

  _pwm16.setPWM(channelA, pwmValue);
  _pwm16.setPWM(channelB, 0);
}
/*--------------------------------*/
void ViaBoard::motor_stop(uint8_t motor_id) {
  Serial.print("motor_stop: "); Serial.print(" M-ID: ");  Serial.println(motor_id);

  // Tính ID của kênh PWM
  uint8_t channelA = MOTOR_1_CHANNEL_A + motor_id*2;
  uint8_t channelB = channelA + 1;
  _pwm16.setPWM(channelA, 0);
  _pwm16.setPWM(channelB, 0);
}
/*--------------------------------*/
void ViaBoard::servo_stop(uint8_t servo_id) {
  uint8_t channel = SERVO_1_CHANNEL + servo_id;
  Serial.print("servo_stop: "); Serial.println(channel); 
  _pwm16.setPWM(channel, 0);
}
/*--------------------------------*/
void ViaBoard::servo_pwm(uint8_t servo_id, uint16_t pwm_value) {
  Serial.print("servo_pwm: "); Serial.print(" S-ID: ");  Serial.print(servo_id); Serial.print(" - pwm_value: "); Serial.println(pwm_value);
  uint8_t channel = SERVO_1_CHANNEL + servo_id;
  Serial.print("channel: "); Serial.println(channel); 
  _pwm16.setPWM(channel, pwm_value);
}
/*--------------------------------*/
void ViaBoard::servo_angle(uint8_t servo_id, uint16_t angle) {
  uint16_t pwm_value = angle2pwm(angle); // angle to step
  Serial.print("servo_angle: "); Serial.print(" S-ID: ");  Serial.println(servo_id); Serial.print(" - angle: "); Serial.println(angle);
  servo_pwm(servo_id, pwm_value);
}
/*--------------------------------*/
uint16_t ViaBoard::angle2pwm(uint16_t angle) {
  uint16_t pulse_wide, pwm_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pwm_value = int(float(pulse_wide) / 1000000 * SERVO_FREQUENCY * 4096);
  return pwm_value;
}
/*--------------------------------*/
void ViaBoard::servo_rotate(uint8_t servo_id, bool isCW) {
  uint16_t pwm_value = isCW ? SERVO_360_CW : SERVO_360_CCW;
  servo_pwm(servo_id, pwm_value);
}
/*--------------------------------*/

