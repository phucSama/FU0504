//#define LED_BUILTIN 13

// Công tắc hành trình
#define UPPER_LIMIT_SWITCH   25
#define LOWER_LIMIT_SWITCH   32

// Khai báo đấu nối các motor và servo và board VIA
#define ROBOT_MOTTOR_LEFT   0
#define ROBOT_MOTTOR_RIGHT  1
#define CLIMB_MOTOR         2  // Kéo cáp Treo robot lên cao
#define SLIDE_MOTOR         3  // Kéo dài/thu ngắn slide

// Không dùng tay gắp
///#define ARM_MOTOR           2 // Canh tay
///#define GRIPPER_SERVO_LEFT  0 // Servo 180
///#define GRIPPER_SERVO_RIGHT 1 // Servo 180
///#define GRIPPER_SERVO_WRIST 2 // Servo 360
/*--------------------------------*/
#define BALL_DOOR_SERVO      0 // Servo 180 - Cửa mở giỏ banh
#define FRUIT_DOOR_SERVO     1 // Servo 180 - Cửa mở đổ trái cây (nông sản)
#define RELEASE_CABLE_SERVO  2 // Servo 360
#define UNDEFINE_SERVO       3 // Servo 360 - Chưa có chức năng được gán


// Trạng thái di chuyển
#define MOVE_STATE_IDLE     0
#define MOVE_STATE_FORWARD  1
#define MOVE_STATE_BACKWARD 2
#define MOVE_STATE_LEFT     3
#define MOVE_STATE_RIGHT    4

// Trạng thái slider
#define SLIDE_STATE_IDLE     0
#define SLIDE_STATE_UP       1
#define SLIDE_STATE_DOWN     2
#define SLIDE_STATE_HOLD     3

#define SOFT_SPEED_TIME     1000

#define DOOR_OPEN true
#define DOOR_CLOSE false

uint8_t move_state = MOVE_STATE_IDLE;
uint8_t slide_state = SLIDE_STATE_IDLE;
//bool clamp_state = false;
bool ball_door_state = DOOR_CLOSE;
bool fruit_door_state = DOOR_CLOSE;

bool isTouchTop = false;         // trạng thái switch TOP - giới hạn trên
bool isTouchBottom = false;      // trạng thái switch BOTTOM - giới hạn dưới
bool isHoldSlider = false;    // trạng thái giữ Slider không cho tuột

bool isSoftSpeed;
uint16_t softSpeed;
uint16_t softInterval;
uint32_t softBegin; // thoi gian bat dau sofSpeed
/*--------------------------------*/
void robot_init() {
//  pinMode(LED_BUILTIN, OUTPUT);

  // Khởi tạo công tắc hành trình
  pinMode(UPPER_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(LOWER_LIMIT_SWITCH, INPUT_PULLUP);

  // Tốc độ motor bánh xe trái và phải
  viaBoard.motor_speed(ROBOT_MOTTOR_LEFT, deviceConfig.RobotSpeed);
  viaBoard.motor_speed(ROBOT_MOTTOR_RIGHT, deviceConfig.RobotSpeed);

  // Tốc độ motor slider
  viaBoard.motor_speed(SLIDE_MOTOR, deviceConfig.SliderNormalSpeed);

  // Tốc độ motor tối đa
  viaBoard.motor_speed(CLIMB_MOTOR, deviceConfig.RobotSpeedMax);

  robot_stop();
}
/*--------------------------------*/
void robot_change_speed(bool isUp) {
  deviceConfig.RobotSpeed = constrain(deviceConfig.RobotSpeed + (isUp ? 1 : -1), deviceConfig.RobotSpeedMin, deviceConfig.RobotSpeedMax);
  viaBoard.motor_speed(ROBOT_MOTTOR_LEFT, deviceConfig.RobotSpeed);
  viaBoard.motor_speed(ROBOT_MOTTOR_RIGHT, deviceConfig.RobotSpeed);
}
/*--------------------------------*/
void robot_move(uint8_t dir) {
  if (dir == MOVE_STOP) {
    robot_stop();
  }
  else 
  {
    // Xu ly khoi dong mem
    if (isSoftSpeed) {
      softInterval = 1000 / (deviceConfig.RobotSpeed - deviceConfig.RobotSpeedMin + 1);
      if (millis() - softBegin > softInterval) {
        softSpeed += 1;
        viaBoard.motor_speed(ROBOT_MOTTOR_LEFT, softSpeed);
        viaBoard.motor_speed(ROBOT_MOTTOR_RIGHT, softSpeed);
        softBegin = millis();
      }
      if (softSpeed >= deviceConfig.RobotSpeed) isSoftSpeed = false; // Ket thuc che do khoi dong mem
    }

    if (dir == MOVE_FORWARD) {
      if ((move_state == MOVE_STATE_FORWARD) || (move_state == MOVE_STATE_IDLE)) {  // tránh cháy motor
        viaBoard.motor_rotate(ROBOT_MOTTOR_LEFT, true);
        viaBoard.motor_rotate(ROBOT_MOTTOR_RIGHT, false);
        move_state = MOVE_STATE_FORWARD;
      }
    }
    else if (dir == MOVE_BACKWARD) {
      if ((move_state == MOVE_STATE_BACKWARD) || (move_state == MOVE_STATE_IDLE)) {  // tránh cháy motor
        viaBoard.motor_rotate(ROBOT_MOTTOR_LEFT, false);
        viaBoard.motor_rotate(ROBOT_MOTTOR_RIGHT, true);
        move_state = MOVE_STATE_BACKWARD;
      }
    }
    else if (dir == MOVE_TURN_LEFT) {
      if ((move_state == MOVE_STATE_LEFT) || (move_state == MOVE_STATE_IDLE)) {  // tránh cháy motor
        viaBoard.motor_rotate(ROBOT_MOTTOR_LEFT, false);
        viaBoard.motor_rotate(ROBOT_MOTTOR_RIGHT, false);
        move_state = MOVE_STATE_LEFT;
      }
    }
    else if (dir == MOVE_TURN_RIGHT) {
      if ((move_state == MOVE_STATE_RIGHT) || (move_state == MOVE_STATE_IDLE)) {  // tránh cháy motor
        viaBoard.motor_rotate(ROBOT_MOTTOR_LEFT, true);
        viaBoard.motor_rotate(ROBOT_MOTTOR_RIGHT, true);
        move_state = MOVE_STATE_RIGHT;
      }
    }
  }
}
/*--------------------------------*/
void robot_stop() {
    viaBoard.motor_stop(ROBOT_MOTTOR_LEFT);
    viaBoard.motor_stop(ROBOT_MOTTOR_RIGHT);
    move_state = MOVE_STATE_IDLE;

    // Khoi tao che do khoi dong mem
    isSoftSpeed = true;
    softSpeed = deviceConfig.RobotSpeedMin;
    softBegin = millis();

    delayMicroseconds(500);
}
/*--------------------------------*/
// Kẹp/thả tay gắp - Servo 180
//void gripper_clamp(bool isClamp) {
//  if (isClamp) {
//    viaBoard.servo_angle(GRIPPER_SERVO_LEFT, deviceConfig.GripperClampL);
//    viaBoard.servo_angle(GRIPPER_SERVO_RIGHT, deviceConfig.GripperClampR);
//    Serial.println("Kep Tay gap");
//  }
//  else {
//    viaBoard.servo_angle(GRIPPER_SERVO_LEFT, deviceConfig.GripperReleaseL);
//    viaBoard.servo_angle(GRIPPER_SERVO_RIGHT, deviceConfig.GripperReleaseR); 
//    Serial.println("Tha Tay gap");
//  }
//}  
/*--------------------------------*/
// Kẹp/thả tay gắp - Servo 180
//void gripper_clamp_toggle() {
//  clamp_state = !clamp_state;
//  gripper_clamp(clamp_state);
//}  
/*--------------------------------*/

// Gập cổ tay gắp ngữa lên - Servo 360
//void gripper_wrist(bool isUp) {
  //uint8_t value = isUp ? deviceConfig.WristUpTime : deviceConfig.WristDownTime;
//  viaBoard.servo_rotate(GRIPPER_SERVO_WRIST, isUp);
//  if (isUp) Serial.println("Ngua tay gap");
//  else Serial.println("Gap tay gap");
//}
/*--------------------------------*/
// Gập cổ tay gắp ngữa lên - Servo 360
//void gripper_wrist_stop() {
//  viaBoard.servo_stop(GRIPPER_SERVO_WRIST);
//  Serial.println("Dung Gap/Ngua Tay gap");
//}
/*--------------------------------*/

//Xoay cánh tay về phía trước/sau 
//void arm_rotate(bool isCW) {
//  viaBoard.motor_rotate(ARM_MOTOR, isCW); 
//  if (isCW) Serial.println("Quay phai Arm");
//  else Serial.println("Quay trai Arm");
//};
/*--------------------------------*/
//Dừng Xoay cánh tay 
//void arm_stop() {
//  viaBoard.motor_stop(ARM_MOTOR); 
//  Serial.println("Dung Tay Arm");
//};```````
/*--------------------------------*/
void read_limit_switch() {
  isTouchTop = (digitalRead(UPPER_LIMIT_SWITCH) == LOW);
  isTouchBottom = (digitalRead(LOWER_LIMIT_SWITCH) == LOW);
  //if (isTouchTop) Serial.println("Top");
  //if (isTouchBottom) Serial.println("Bottom");

  // Dừng khi xuống thấp nhất
  if ((slide_state == SLIDE_STATE_DOWN) && isTouchBottom) basket_stop(); 

  // Giữ khi đến điểm cao nhất
  if ((slide_state == SLIDE_STATE_UP) && isTouchTop) basket_hold();
}
/*--------------------------------*/

// Thu ngắn/kéo dài tay nâng giỏ bóng
void basket_slide(bool isLiftUp) {
  // Kéo nâng lên
  if (isLiftUp) {
    // Chỉ thực hiện khi không ở trạng thái HOLD 
    if (!isHoldSlider) {
      viaBoard.motor_rotate(SLIDE_MOTOR, true); // theo chiều kim đồng hồ
      slide_state = SLIDE_STATE_UP;
      Serial.println("Keo dai gio bong");
    }
  }
  else // Hạ xuống
  {
    if (isTouchBottom) basket_stop(); // đụng giới hạn dưới => dừng
    else {
      isHoldSlider = false; // bỏ chế độ dừng khi hạ xuống
      slide_state == SLIDE_STATE_IDLE;
      viaBoard.motor_rotate(SLIDE_MOTOR, false); // ngược chiều kim đồng hồ
      slide_state = SLIDE_STATE_DOWN;
      Serial.println("Thu ngan gio bong");
    }
  }
};   
/*--------------------------------*/
// Dừng Thu ngắn/kéo dài tay nâng giỏ bóng
void basket_stop() {
  if (!isHoldSlider) {
    viaBoard.motor_stop(SLIDE_MOTOR);
    slide_state = SLIDE_STATE_IDLE;
    Serial.println("Dung slide Gio bong");
  }
};   
/*--------------------------------*/
void fast_slide(bool isFast) {
  uint16_t speed = isFast ? deviceConfig.SliderFastSpeed : deviceConfig.SliderNormalSpeed;
  viaBoard.motor_speed(SLIDE_MOTOR, speed);
};   
/*--------------------------------*/
// Giữ không cho giỏ bóng tuột
void basket_hold() {
  viaBoard.motor_pwm(SLIDE_MOTOR, true, deviceConfig.SliderHoldPWM);
  slide_state = SLIDE_STATE_HOLD;
  isHoldSlider = true;
  Serial.println("Giu slide Gio bong");
};   
/*--------------------------------*/
// Đổ bóng vào thùng chứa - Servo 360
//void ball_drop(bool isDrop) {
//  viaBoard.servo_rotate(BALL_SERVO, isDrop);
//  if (isDrop) Serial.println("Do bong");
//  else Serial.println("Giu bong");
//}
/*--------------------------------*/
// Dừng đổ bóng vào thùng chứa - Servo 360
//void ball_stop() {
//  viaBoard.servo_stop(BALL_SERVO);
//  Serial.println("Dung Do/Giu Gio bong");
//}
/*--------------------------------*/
// Đóng mở cửa đổ bóng
void ball_door_toggle() {
  ball_door_state = !ball_door_state;
  uint8_t angle = ball_door_state==DOOR_OPEN ? deviceConfig.BallDoorOpen : deviceConfig.BallDoorClose;
  viaBoard.servo_angle(BALL_DOOR_SERVO, angle);
}  
/*--------------------------------*/
// Đóng mở cửa đổ trái cây (nông sản)
void fruit_door_toggle() {
  fruit_door_state = !fruit_door_state;
  uint8_t angle = fruit_door_state==DOOR_OPEN ? deviceConfig.FruitDoorOpen : deviceConfig.FruitDoorClose;
  viaBoard.servo_angle(FRUIT_DOOR_SERVO, angle);        
}  

/*--------------------------------*/
// Giữ dây Treo robot
void robot_hold() {
  viaBoard.motor_pwm(CLIMB_MOTOR, true, deviceConfig.RobotHoldPWM);
};   
/*--------------------------------*/
// Kéo dây Treo robot
void robot_climb(bool isClimb) {
  viaBoard.motor_rotate(CLIMB_MOTOR, isClimb);
};   
/*--------------------------------*/
// Dừng dây treo robot
void robot_climb_stop() {
  viaBoard.motor_stop(CLIMB_MOTOR);
};   
/*--------------------------------*/
// Gỡ dây treo khỏi slider - Servo 360
void release_cable(bool isRelease) {
  viaBoard.servo_rotate(RELEASE_CABLE_SERVO, isRelease);
}
/*--------------------------------*/
// Dừng Gỡ dây treo
void release_cable_stop() {
  viaBoard.servo_stop(RELEASE_CABLE_SERVO);
}
/*--------------------------------*/

// Chưa định nghĩa (dùng đa mục đích) - Servo 360
void undefine_servo(bool isCW) {
  viaBoard.servo_rotate(UNDEFINE_SERVO, isCW);
}
/*--------------------------------*/
// Dừng servo
void undefine_servo_stop() {
  viaBoard.servo_stop(UNDEFINE_SERVO);
}
/*--------------------------------*/
void arm_servo_hold() {
  viaBoard.servo_pwm(UNDEFINE_SERVO, 325);
}
/*--------------------------------*/
