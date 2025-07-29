#include "farc.h"

/*--------------------------------*/
void setup() {
  LOG_INIT();
 
  readConfig(); // read config from EEPROM
  LOG_LN(getConfig());

  viaBoard.init();
  LOG_LN("ViaBoard inited");
  robot_init();

  initPS2(); // Kết nối GamePad PS2
}
/*--------------------------------*/
void loop() {
  checkUart();
  checkBT();
  read_limit_switch();
  checkPS2();
}
/*--------------------------------*/
