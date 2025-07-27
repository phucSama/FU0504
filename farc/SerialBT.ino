//BluetoothSerial SerialBT;
#define BT_NAME "UNION_ROBOT"

#define BT_BUFF_LEN  20
#define BT_END_CHAR  13
#define BT_TIMEOUT   1000

uint8_t btData[BT_BUFF_LEN];
uint8_t btLen = 0;
uint32_t btLastRead;
bool btWaitEnd = false;

void initSerialBT() {
  SerialBT.register_callback(btCallback);
  SerialBT.begin(BT_NAME); //Bluetooth device name
  LOG_LN("Bluetooth inited");
}
/*--------------------------------*/
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if(event == ESP_SPP_SRV_OPEN_EVT){
    LOG("BT Client connected: "); LOG_MAC(param->srv_open.rem_bda); // print Bluetooth Address
    LED_ON();
  }
  else if (event == ESP_SPP_CLOSE_EVT ) {
    LOG_LN("BT Client disconnected");
    LED_OFF();
  }
}
/*--------------------------------*/
void checkBT() {
  if (btWaitEnd) {
    if (millis() - btLastRead > BT_TIMEOUT) {
      btWaitEnd = false;
      btLen = 0;
      LOG_LN("Bluetooth timeout");
    }
  }

  if (SerialBT.available() > 0) {
    uint8_t data = SerialBT.read();
    if (btLen == 0) {
      btLastRead = millis();
      btWaitEnd = true;
    }
    if ((data == BT_END_CHAR) || (btLen+1 == BT_BUFF_LEN))  {
      btWaitEnd = false;
      doBTCommand();
    }
    else btData[btLen++] = data;
  }
}
//---------------------------------------------------
void doBTCommand() {
  command.target = COMMAND_TARGET_BLUETOOTH;
  command.id = btData[0];
  memcpy(command.data, btData+1, btLen-1);
  doCommand();
  btLen = 0;
}
//---------------------------------------------------
