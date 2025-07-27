#include "PS2X_lib.h"

/******************************************************************
 * Cài đặt chân cho thư viện :
 * - Trên mạch Motorshield của VIA Makerbot BANHMI, có header 6 chân
 *   được thiết kế để cắm tay cầm PS2.
 * Sơ đồ chân header và sơ đồ GPIO tương ứng:
 *   MOSI | MISO | GND | 3.3V | CS | CLK
 *    12     13    GND   3.3V   15   14
 ******************************************************************/
#define PS2_DAT 12 // MISO
#define PS2_CMD 13 // MOSI
#define PS2_SEL 15 // SS
#define PS2_CLK 14 // SLK

//-------- Map lai phim --------//
#define PS2_LEFT   PSB_PAD_LEFT
#define PS2_RIGHT  PSB_PAD_RIGHT
#define PS2_UP     PSB_PAD_UP
#define PS2_DOWN   PSB_PAD_DOWN

#define PS2_SELECT PSB_SELECT 
#define PS2_START  PSB_START
//#define PS2_MODE    // Phim khong phan hoi

#define PS2_X  PSB_SQUARE
#define PS2_Y  PSB_TRIANGLE
#define PS2_A  PSB_CROSS
#define PS2_B  PSB_CIRCLE

#define PS2_L1 PSB_L1
#define PS2_L2 PSB_L2
#define PS2_R1 PSB_R1
#define PS2_R2 PSB_R2

#define PS2_LX PSS_LX
#define PS2_LY PSS_LY
#define PS2_RX PSS_RX
#define PS2_RY PSS_RY

/******************************************************************
 * Lựa chọn chế độ cho tay cầm PS2 :
 *   - pressures = đọc giá trị analog từ các nút bấm
 *   - rumble    = bật/tắt chế độ rung
 ******************************************************************/
#define pressures false
#define rumble false

#define PS2_BUFF_LEN  20

PS2X ps2x; // khởi tạo class PS2x
uint8_t old_lx, old_ly, old_rx, old_ry;
bool x_pressed = false;
bool y_pressed = false;
bool start_pressed = false;

void initPS2() {
  int error = -1;
  for (int i = 0; i < 10; i++) // thử kết nối với tay cầm ps2 trong 10 lần
  {
    delay(500);  //added delay to give wireless ps2 module some time to startup, before configuring it
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    if (error == 0) {
      LOG_LN("Ket noi tay cam PS2 thanh cong");
      break;
    }
    else if (error == 1) {
      LOG_LN("Khong tim thay Gamepad");
    }
    else if (error == 2) {
      LOG_LN("Khong gui duoc lenh");
    }
    else if (error == 3) {
      LOG_LN("Khong vao duoc Pressures mode");
    }
    Serial.print(".");
  }
}
/*--------------------------------*/
void checkPS2() {
  ps2x.read_gamepad(false, false); // gọi hàm để đọc tay điều khiển

  if ((ps2x.Analog(PS2_LX) != old_lx) || (ps2x.Analog(PS2_LY) != old_ly)) on_left_js_change();
  if ((ps2x.Analog(PS2_RX) != old_rx) || (ps2x.Analog(PS2_RY) != old_ry)) on_right_js_change();
  if (ps2x.NewButtonState()) on_button_change();
}
/*--------------------------------*/
void on_left_js_change() {
  Serial.print("Left JS = "); Serial.print(ps2x.Analog(PS2_LX), DEC); Serial.print(","); Serial.println(ps2x.Analog(PS2_LY), DEC);
  old_lx = ps2x.Analog(PS2_LX);
  old_ly = ps2x.Analog(PS2_LY);    
}
/*--------------------------------*/
void on_right_js_change() {
  Serial.print("Right JS = "); Serial.print(ps2x.Analog(PS2_RX), DEC); Serial.print(","); Serial.println(ps2x.Analog(PS2_RY), DEC);
  old_rx = ps2x.Analog(PS2_RX);
  old_ry = ps2x.Analog(PS2_RY);    
}
/*--------------------------------*/
void on_button_change() {
  // DPAD
  if (ps2x.ButtonPressed(PS2_LEFT)) robot_move(MOVE_TURN_LEFT);
  else if (ps2x.ButtonReleased(PS2_LEFT)) robot_stop();
  if (ps2x.ButtonPressed(PS2_RIGHT)) robot_move(MOVE_TURN_RIGHT);
  else if (ps2x.ButtonReleased(PS2_RIGHT)) robot_stop();
  if (ps2x.ButtonPressed(PS2_UP)) robot_move(MOVE_FORWARD);
  else if (ps2x.ButtonReleased(PS2_UP)) robot_stop();
  if (ps2x.ButtonPressed(PS2_DOWN)) robot_move(MOVE_BACKWARD);
  else if (ps2x.ButtonReleased(PS2_DOWN)) robot_stop();

  // SELECT - chế độ trượt nhanh
  //if (ps2x.ButtonPressed(PS2_SELECT)) fast_slide(true); // Thiết lập tốc đột trượt nhanh
  //else if (ps2x.ButtonReleased(PS2_SELECT)) fast_slide(false); // Thiết lập tốc đột trượt bình thường
  // SELECT => Phím lật trạng thái - Giữ giỏ bóng
  if (ps2x.Button(PS2_SELECT)) basket_hold();


  // Nhấn giữ START => tăng giảm tốc độ
  if (ps2x.ButtonPressed(PS2_START)) start_pressed = true;
  else if (ps2x.ButtonReleased(PS2_START)) start_pressed = false;

  if (start_pressed) {
    // Nếu phím START đang nhấn giữ => tăng/giảm tốc độ 
    if (ps2x.Button(PS2_L1)) robot_change_speed(true);
    if (ps2x.Button(PS2_L2)) robot_change_speed(false);
  }
  else {
      // L1, L2 => Nâng hạ giỏ bong
    if (ps2x.ButtonPressed(PS2_L1)) basket_slide(true);
    else if (ps2x.ButtonReleased(PS2_L1)) basket_stop();
    if (ps2x.ButtonPressed(PS2_L2)) basket_slide(false);
    else if (ps2x.ButtonReleased(PS2_L2)) basket_stop();
  }

  // X
  if (ps2x.ButtonPressed(PS2_X)) x_pressed = true;
  else if (ps2x.ButtonReleased(PS2_X)) x_pressed = false;
  // Y => xoay servo 360 chưa định nghĩa
  if (ps2x.ButtonPressed(PS2_Y)) y_pressed = true;
  else if (ps2x.ButtonReleased(PS2_Y)) y_pressed = false;

  if (x_pressed) {
    // Nếu phím X đang nhấn giữ => Xoay servo 360 kéo cáp treo robot 
    if (ps2x.ButtonPressed(PS2_R1)) release_cable(true);
    else if (ps2x.ButtonReleased(PS2_R1)) release_cable_stop();
    if (ps2x.ButtonPressed(PS2_R2)) release_cable(false);
    else if (ps2x.ButtonReleased(PS2_R2)) release_cable_stop();

    // Nếu phím X đang nhấn giữ => gập/ngua kẹp 
    //if (ps2x.ButtonPressed(PS2_R1)) gripper_wrist(true);
    //else if (ps2x.ButtonReleased(PS2_R1)) gripper_wrist_stop();
    //if (ps2x.ButtonPressed(PS2_R2)) gripper_wrist(false);
    //else if (ps2x.ButtonReleased(PS2_R2)) gripper_wrist_stop();
  }
  else {
    if (y_pressed) {
      // Nếu phím Y đang nhấn giữ => treo robot
      if (ps2x.ButtonPressed(PS2_R1)) robot_climb(true);
      else if (ps2x.ButtonReleased(PS2_R1)) robot_climb_stop();
      if (ps2x.ButtonPressed(PS2_R2)) robot_climb(false);
      else if (ps2x.ButtonReleased(PS2_R2)) robot_climb_stop();
    }
    // Không có phím nào bấm
    else {
      // R1, R2 => xoay servo 360
      if (ps2x.ButtonPressed(PS2_R1)) undefine_servo(true);
      else if (ps2x.ButtonReleased(PS2_R1)) arm_servo_hold();
      if (ps2x.ButtonPressed(PS2_R2)) undefine_servo(false);
      else if (ps2x.ButtonReleased(PS2_R2)) arm_servo_hold();
    }

     // R1, R2 => xoay tay Arm
    //if (ps2x.ButtonPressed(PS2_R1)) arm_rotate(true);
    //else if (ps2x.ButtonReleased(PS2_R1)) arm_stop();
    //if (ps2x.ButtonPressed(PS2_R2)) arm_rotate(false);
    //else if (ps2x.ButtonReleased(PS2_R2)) arm_stop();
  } 
  
  // A => Phím lật trạng thái - Đóng/mở cửa bóng
  if (ps2x.Button(PS2_A)) ball_door_toggle();
  // A => Phím lật trạng thái - Kẹp/thả tay gắp
  //if (ps2x.Button(PS2_A)) gripper_clamp_toggle();

  // B => Phím lật trạng thái - Đóng/mở cửa trái cây
  // if (ps2x.Button(PS2_B)) fruit_door_toggle();

  // B => Kẹp/thả tay gắp
  if (ps2x.Button(PS2_B)) fruit_door_toggle(); 
}
//---------------------------------------------------

