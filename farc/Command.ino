#include "Command.h"

command_t command;

void doCommand() {
  switch (command.id) {
    case (COMMAND_ID_SET):
      if (command.data[0] == SET_SPEED_INC) incSpeed();
      else if (command.data[0] == SET_SPEED_DEC) decSpeed();
      break;

    case (COMMAND_ID_CONFIG):
      changeConfig(command.data);
      if (command.target == COMMAND_TARGET_UART) LOG_LN(getConfig());
      else if (command.target == COMMAND_TARGET_BLUETOOTH) SerialBT.println(getConfig());
      break;

    case (COMMAND_ID_MOVE):
      robot_move(command.data[0]);
      break;

    //case (COMMAND_ID_GRIPPER):
    //  if (command.data[0] == GRIPPER_CLAMP) gripper_clamp(true);
    //  else if (command.data[0] == GRIPPER_RELEASE) gripper_clamp(false);
    //  else if (command.data[0] == GRIPPER_WRIST_UP) gripper_wrist(true);
    //  else if (command.data[0] == GRIPPER_WRIST_DOWN) gripper_wrist(false);
    //  else if (command.data[0] == GRIPPER_ARM_CW) arm_rotate(true);
    //  else if (command.data[0] == GRIPPER_ARM_CCW) arm_rotate(false);
    //  break;

    //case (COMMAND_ID_BALL):
    //  if (command.data[0] == BALL_UP) basket_slide(true);
    //  else if (command.data[0] == BALL_DOWN) basket_slide(false);
    //  else if (command.data[0] == BALL_HOLD) ball_drop(true);
    //  else if (command.data[0] == BALL_RELEASE) ball_drop(false);
    //  break;

    case (COMMAND_ID_TEST):
      if (command.data[0] == COMMAND_TEST_MOTOR) testMotor();
      else if (command.data[0] == COMMAND_TEST_SERVO) testServo();
      break;

    default:
      LOG_VALUE("Undefine CMD: 0x", command.id);
      break;
  }
}
/*--------------------------------*/
void moveRobot(int8_t direction) {
  Serial.print("move: "); Serial.print(" direction: ");  Serial.println(direction);
}
/*--------------------------------*/
void incSpeed() {
  deviceConfig.RobotSpeed += 1;
  Serial.print("incSpeed: "); Serial.println(deviceConfig.RobotSpeed);
}
/*--------------------------------*/
void decSpeed() {
  deviceConfig.RobotSpeed -= 1;
  Serial.print("decSpeed: "); Serial.println(deviceConfig.RobotSpeed);
}
/*--------------------------------*/
void testMotor() {
  uint8_t action = command.data[1];
  uint8_t motor_id = command.data[2];
  
  if (action == TEST_MOTOR_STOP) {
    viaBoard.motor_stop(motor_id);
  }
  else if (action == TEST_MOTOR_CW) {
    viaBoard.motor_stop(motor_id);
    viaBoard.motor_rotate(motor_id, true);
  }
  else if  (action == TEST_MOTOR_CCW) {
    viaBoard.motor_stop(motor_id);
    viaBoard.motor_rotate(motor_id, false);
  }
  else if  (action == TEST_MOTOR_INC) {
    viaBoard.motor_incSpeed(motor_id);
  }
  else if  (action == TEST_MOTOR_DEC) {
    viaBoard.motor_decSpeed(motor_id);
  }

  else {
    Serial.println("Undefine test action"); 
  }
}
/*--------------------------------*/
void testServo() {
  uint8_t action = command.data[1];
  uint8_t servo_id = command.data[2];
  uint16_t value = command.data[3];
  value = (value << 8) | command.data[4];

  if (action == TEST_SERVO_STOP) {
    viaBoard.servo_stop(servo_id);
  }
  else if (action == TEST_SERVO_PWM) {
    viaBoard.servo_pwm(servo_id, value);
  }
  else if (action == TEST_SERVO_CW) {
    viaBoard.servo_rotate(servo_id, true);
  }
  else if  (action == TEST_SERVO_CCW) {
    viaBoard.servo_rotate(servo_id, false);
  }
  else if  (action == TEST_SERVO_POS) {
    viaBoard.servo_angle(servo_id, value);
  }

  else {
    Serial.println("Undefine test action"); 
  }
}
/*--------------------------------*/


