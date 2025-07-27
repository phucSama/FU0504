#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>

#define EEPROM_OFFSET       0

// Default value
#define DEFAULT_ROBOT_SPEED_MIN   1
#define DEFAULT_ROBOT_SPEED_MAX   30
#define DEFAULT_ROBOT_SPEED       20

#define DEFAULT_SLIDER_SPEED      5
#define DEFAULT_SLIDER_FAST_SPEED 25
#define DEFAULT_SLIDER_HOLD_PWM   200

#define DEFAULT_BALL_DOOR_OPEN      112
#define DEFAULT_BALL_DOOR_CLOSE     0
#define DEFAULT_FRUIT_DOOR_OPEN     30
#define DEFAULT_FRUIT_DOOR_CLOSE    80

#define DEFAULT_ROBOT_HOLD_PWM   2048

//#define DEFAULT_GR_CLAMP_L       90
//#define DEFAULT_GR_CLAMP_R       90
//#define DEFAULT_GR_RELEASE_L     160
//#define DEFAULT_GR_RELEASE_R     20

// Tham số Config
#define CONFIG_RESET        0x00 // Factory Reset
#define CONFIG_WRITE        0x99 // Ghi cau hinh
#define CONFIG_ROBOT_SPEED  0x01 // Cấu hình tốc độ Robot
#define CONFIG_SLIDER_SPEED 0x02 // Cấu hình tốc độ trượt
#define CONFIG_BALL_DOOR    0x03 // Cấu hình góc đóng/mở cửa khay bóng
#define CONFIG_FRUIT_DOOR   0x04 // Cấu hình góc đóng/mở cửa khay trái cây
#define CONFIG_ROBOT_HOLD   0x05 // Cấu hình PWM giữ robot

struct config_t {
  uint8_t RobotSpeedMin;
  uint8_t RobotSpeedMax;
  uint8_t RobotSpeed;

  uint8_t BallDoorOpen;        // Giá trị góc servo khi cửa đổ bóng mở 
  uint8_t BallDoorClose;       // Giá trị góc servo khi cửa đổ bóng đòng
  uint8_t FruitDoorOpen;       // Giá trị góc servo khi cửa đổ trái cây mở 
  uint8_t FruitDoorClose;      // Giá trị góc servo khi cửa đổ trái cây đóng

  uint8_t SliderNormalSpeed;   // Giá trị tốc độ kéo thanh trượt 
  uint8_t SliderFastSpeed;     // Giá trị tốc độ kéo thanh trượt nhanh 
  uint16_t SliderHoldPWM;       // Giá trị PWM thiết lập giữ motor chống lại lực kéo xuống 

  uint16_t RobotHoldPWM;       // Giá trị PWM thiết lập giữ motor chống lại lực kéo xuống 

  //uint16_t GripperClampL;     // Giá trị góc servo bên trái khi kẹp 
  //uint16_t GripperClampR;     // Giá trị góc servo bên phải khi kẹp
  //uint16_t GripperReleaseL;   // Giá trị góc servo bên trái khi thả 
  //uint16_t GripperReleaseR;   // Giá trị góc servo bên phải khi thả

};

extern config_t deviceConfig;

void readConfig();
void writeConfig();
void defaultConfig();
bool isEmptyConfig();
String getConfig();
void changeConfig(uint8_t *configData);

#endif
