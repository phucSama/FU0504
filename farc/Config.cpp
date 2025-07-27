#include "Config.h"
#include <EEPROM.h>

config_t deviceConfig;

//----------------------------------------------
void readConfig() {
  EEPROM.begin(sizeof(config_t));
  EEPROM.get(EEPROM_OFFSET, deviceConfig);
  EEPROM.end();
  if (isEmptyConfig()) defaultConfig();
}
//----------------------------------------------
void writeConfig() {
  EEPROM.begin(sizeof(config_t));
  EEPROM.put(EEPROM_OFFSET, deviceConfig);
  EEPROM.commit();
  EEPROM.end();
}
//----------------------------------------------
void defaultConfig() {
  deviceConfig.RobotSpeedMin = DEFAULT_ROBOT_SPEED_MIN;
  deviceConfig.RobotSpeedMax = DEFAULT_ROBOT_SPEED_MAX;
  deviceConfig.RobotSpeed = DEFAULT_ROBOT_SPEED;

  deviceConfig.BallDoorOpen = DEFAULT_BALL_DOOR_OPEN;      
  deviceConfig.BallDoorClose = DEFAULT_BALL_DOOR_CLOSE; 
  deviceConfig.FruitDoorOpen = DEFAULT_FRUIT_DOOR_OPEN; 
  deviceConfig.FruitDoorClose= DEFAULT_FRUIT_DOOR_CLOSE;

  deviceConfig.SliderNormalSpeed = DEFAULT_SLIDER_SPEED; 
  deviceConfig.SliderFastSpeed= DEFAULT_SLIDER_FAST_SPEED;
  deviceConfig.SliderHoldPWM = DEFAULT_SLIDER_HOLD_PWM;

  deviceConfig.SliderHoldPWM = DEFAULT_ROBOT_HOLD_PWM;
}
//----------------------------------------------
bool isEmptyConfig() {
  byte *data = (byte*)&deviceConfig;
  for (int16_t index = 0; index < sizeof(config_t); index++) {
    if (*data != 0xFF) return false;
  }
  return true;
}
//----------------------------------------------
String getConfig() {
  String configStr = "Device Settings";

  configStr += "\nRobotSpeedMin: " + String(deviceConfig.RobotSpeedMin);
  configStr += "\nRobotSpeedMax: " + String(deviceConfig.RobotSpeedMax);
  configStr += "\nRobotSpeed: " + String(deviceConfig.RobotSpeed);

  configStr += "\nBallDoorOpen: " + String(deviceConfig.BallDoorOpen);
  configStr += "\nBallDoorClose: " + String(deviceConfig.BallDoorClose);
  configStr += "\nFruitDoorOpen: " + String(deviceConfig.FruitDoorOpen);
  configStr += "\nFruitDoorClose: " + String(deviceConfig.FruitDoorClose);

  configStr += "\nSliderNormalSpeed: " + String(deviceConfig.SliderNormalSpeed);
  configStr += "\nSliderFastSpeed: " + String(deviceConfig.SliderFastSpeed);
  configStr += "\SliderHoldPWM: " + String(deviceConfig.SliderHoldPWM);

  configStr += "\RobotHoldPWM: " + String(deviceConfig.RobotHoldPWM);

  return configStr;
}
//----------------------------------------------
void changeConfig(uint8_t *configData) {
  uint8_t configType = configData[0];
  if (configType == CONFIG_RESET) defaultConfig();
  else if (configType == CONFIG_WRITE) writeConfig();

  else if (configType == CONFIG_ROBOT_SPEED) {
    deviceConfig.RobotSpeedMin = *(uint8_t*)(configData + 1);
    deviceConfig.RobotSpeedMax = *(uint8_t*)(configData + 2);
    deviceConfig.RobotSpeed = *(uint8_t*)(configData + 3);
  }

  else if (configType == CONFIG_BALL_DOOR) {
    deviceConfig.BallDoorOpen = *(uint8_t*)(configData + 1);
    deviceConfig.BallDoorClose = *(uint8_t*)(configData + 2);
  }

  else if (configType == CONFIG_FRUIT_DOOR) {
    deviceConfig.FruitDoorOpen = *(uint8_t*)(configData + 1);
    deviceConfig.FruitDoorClose = *(uint8_t*)(configData + 2);
  }

  else if (configType == CONFIG_SLIDER_SPEED) {
    deviceConfig.SliderNormalSpeed = *(uint8_t*)(configData + 1);
    deviceConfig.SliderFastSpeed = *(uint8_t*)(configData + 2);
    deviceConfig.SliderHoldPWM = *(uint16_t*)(configData + 3);
  }

  else if (configType == CONFIG_ROBOT_HOLD) {
    deviceConfig.RobotHoldPWM = *(uint16_t*)(configData + 1);
  }
}
//----------------------------------------------
