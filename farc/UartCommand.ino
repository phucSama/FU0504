#define UART_PORT  Serial

#define UART_BUFF_LEN  20
#define UART_END_CHAR  13
#define UART_TIMEOUT   1000

uint8_t uartData[UART_BUFF_LEN];
uint8_t uartLen = 0;
uint32_t uartLastRead;
bool uartWaitEnd = false;

void checkUart() {
  if (uartWaitEnd) {
    if (millis() - uartLastRead > UART_TIMEOUT) {
      uartWaitEnd = false;
      uartLen = 0;
      LOG_LN("UART timeout");
    }
  }

  if (UART_PORT.available() > 0) {
    uint8_t data = UART_PORT.read();
    if (uartLen == 0) {
      uartLastRead = millis();
      uartWaitEnd = true;
    }
    if ((data == UART_END_CHAR) || (uartLen+1 == UART_BUFF_LEN))  {
      uartWaitEnd = false;
      doUartCommand();
    }
    else uartData[uartLen++] = data;
  }
}
//---------------------------------------------------
void doUartCommand() {
  command.target = COMMAND_TARGET_UART;
  command.id = uartData[0];
  memcpy(command.data, uartData+1, uartLen-1);
  doCommand();
  uartLen = 0;
}