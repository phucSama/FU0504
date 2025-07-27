#ifndef Log_h
#define Log_h

#include <Arduino.h>
#include "Hex.h"

#define LOG_PORT Serial
#define LOG_BAUD_RATE 115200
#define LOG_MAX_LEN 128

#define LOG_INIT() LOG_PORT.begin(LOG_BAUD_RATE)
#define LOG(x) LOG_PORT.print(x)
#define LOG_LN(x) LOG_PORT.println(x)
#define LOG_HEX(x) LOG_PORT.print(x, HEX)
#define LOG_CR() LOG_PORT.println()
#define LOG_VALUE(x, y) LOG_PORT.print(x); LOG_PORT.println(y)
#define LOG_VALUE_HEX(x, y) LOG_PORT.print(x); LOG_PORT.println(y, HEX)
#define LOG_VALUE_BIN(x, y) LOG_PORT.print(x); LOG_PORT.println(y, BIN)

#define LOG_IF(condition, x) if (condition) LOG_PORT.println(x)
#define LOG_IF2(condition, x, y) if (condition) LOG_PORT.println(x); else LOG_PORT.println(y)

#define LOG_BUFF(x, y) LOG.hexBuff(x, y)
#define LOG_BUFF2(x, y) LOG(y); LOG(" byte 0x"); LOG.hexBuff(x, y)
#define LOG_MAC(x) LOG.mac(x);

class LogHelper {
public:
    LogHelper();
    void hexBuff(INT8U *pByteBuff, INT8U len);
    void mac(INT8U *pMacBuff);
private:
    INT8U _logBuff[LOG_MAX_LEN];
};

extern LogHelper LOG;

#endif
