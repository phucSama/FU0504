#ifndef VIA_ROBOT_h
#define VIA_ROBOT_h

#include <Arduino.h>
#include "PCA9685.h"

#define NUM_OF_MOTORS 4
#define MOTOR_1_CHANNEL_A 8
#define MOTOR_1_CHANNEL_B 9
#define MOTOR_2_CHANNEL_A 10
#define MOTOR_2_CHANNEL_B 11
#define MOTOR_3_CHANNEL_A 12
#define MOTOR_3_CHANNEL_B 13
#define MOTOR_4_CHANNEL_A 14
#define MOTOR_4_CHANNEL_B 15

#define NUM_OF_SERVO 6
#define SERVO_1_CHANNEL 2
#define SERVO_2_CHANNEL 3
#define SERVO_3_CHANNEL 4
#define SERVO_4_CHANNEL 5
#define SERVO_5_CHANNEL 6
#define SERVO_6_CHANNEL 7

#define NOTIFY_LED 13

#define MOTOR_SPEED_MIN 1
#define MOTOR_SPEED_MAX 30
#define MOTOR_SPEED_DEFAULT 15

#define MOTOR_PWM_MIN 756
#define MOTOR_PWM_MAX 4095

#define MOTOR_DIR_CW 0
#define MOTOR_DIR_CCW 1

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2600
#define SERVO_360_CW    280   // Xung servo quay chiều kim đồng hồ 
#define SERVO_360_CCW   370  // Xung servo quay ngược chiều kim đồng hồ

#define SERVO_FREQUENCY 50

//=====================
#define MOTOR_LEFT_ID 0
#define MOTOR_RIGHT_ID 0


class ViaBoard {
public:
    void init();

    void motor_speed(uint8_t motor_id, uint8_t motor_speed); // thiết lập tham số tốc độ
    void motor_incSpeed(uint8_t motor_id);
    void motor_decSpeed(uint8_t motor_id);
    void motor_rotate(uint8_t motor_id, bool isCW);
    void motor_pwm(uint8_t motor_id, bool isCW, uint16_t pwmValue);
    void motor_stop(uint8_t motor_id);

    void servo_stop(uint8_t servo_id);
    void servo_pwm(uint8_t servo_id, uint16_t pwm_value);
    void servo_angle(uint8_t servo_id, uint16_t angle);
    void servo_rotate(uint8_t servo_id, bool isCW);

private:
    PCA9685 _pwm16;
    uint8_t _motorSpeeds[NUM_OF_MOTORS];
    uint16_t angle2pwm(uint16_t angle);
};  

#endif
