#ifndef Command_h
#define Command_h

#include <Arduino.h>

// ID lệnh
#define COMMAND_ID_CONFIG   0xCF
#define COMMAND_ID_SET      0x01
#define COMMAND_ID_MOVE     0x02
#define COMMAND_ID_GRIPPER  0x03
#define COMMAND_ID_BALL     0x04
#define COMMAND_ID_TEST     0x75

// Tham số lệnh SET
#define SET_SPEED_INC    0x01 // 01 01 
#define SET_SPEED_DEC    0x02 // 01 02

// Tham số lệnh MOVE
#define MOVE_STOP        0x00 // 02 00 
#define MOVE_FORWARD     0x01 // 02 01 
#define MOVE_BACKWARD    0x02 // 02 02 
#define MOVE_TURN_LEFT   0x03 // 02 03 
#define MOVE_TURN_RIGHT  0x04 // 02 04 

// Tham số lệnh CLAMP
#define GRIPPER_CLAMP        0x00 // 03 00 
#define GRIPPER_RELEASE      0x01 // 03 01 
#define GRIPPER_WRIST_UP     0x02 // 03 02 
#define GRIPPER_WRIST_DOWN   0x03 // 03 03 
#define GRIPPER_ARM_CW       0x04 // 03 04 
#define GRIPPER_ARM_CCW      0x05 // 03 05 

// Tham số lệnh BALL
#define BALL_UP        0x00 // 04 00 
#define BALL_DOWN      0x01 // 04 01 
#define BALL_HOLD      0x02 // 04 02 
#define BALL_RELEASE   0x03 // 04 03 

// Tham số lệnh CONFIG
#define COMMAND_CONFIG_INFO  0x1F // CF 1F
#define COMMAND_CONFIG_RESET 0x00 // CF 00

// Tham số lệnh TEST
#define COMMAND_TEST_MOTOR 0x01 // 75 01
#define COMMAND_TEST_SERVO 0x02 // 75 02

#define TEST_MOTOR_STOP 0x00 // 75 01 00
#define TEST_MOTOR_CW   0x01 // 75 01 01
#define TEST_MOTOR_CCW  0x02 // 75 01 02
#define TEST_MOTOR_INC  0x03 // 75 01 03
#define TEST_MOTOR_DEC  0x04 // 75 01 04

#define TEST_SERVO_STOP 0x00 // 75 02 00
#define TEST_SERVO_CW   0x01 // 75 02 01
#define TEST_SERVO_CCW  0x02 // 75 02 02
#define TEST_SERVO_PWM  0x03 // 75 02 03
#define TEST_SERVO_POS  0x04 // 75 02 04 => theo góc 0 - 180

// Nơi gọi lệnh
#define COMMAND_TARGET_UART 0x00
#define COMMAND_TARGET_BLUETOOTH 0x01
#define COMMAND_TARGET_REMOTE 0x02

#define COMMAND_MAX_LEN   20
struct command_t {
  uint8_t id; // Mã lệnh
  uint8_t data[COMMAND_MAX_LEN]; // Tham số lệnh
  uint8_t target; // Nguồn gọi lệnh (UART, BlueTooth, Controller)
};

#endif
