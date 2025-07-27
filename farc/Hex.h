#ifndef Hex_h
#define Hex_h

#include <Arduino.h>

#define INT32U unsigned long
#define INT16U word
#define INT8U byte

#define EOS_CHAR 0x00
#define SPACE_CHAR ' '
#define COLON_CHAR ':'
#define COMMA_CHAR ','

#define LO_NIBBLE(x) (x & 0x0F)
#define HI_NIBBLE(x) (x >> 4)
#define LO_BYTE(x)   ((INT8U)(x & 0xFF))
#define HI_BYTE(x)   ((INT8U)(x >> 8))
#define LO_WORD(x)   ((INT16U)(x & 0xFFFF))
#define HI_WORD(x)   ((INT16U)(x >> 16))

class Hex {
public:
    static void hex2(INT8U value, INT8U *pHexBuff);
    static void hex3(INT16U value, INT8U *pHexBuff);
    static void hex4(INT16U value, INT8U *pHexBuff);
    static void hex8(INT32U value, INT8U *pHexBuff);
    static void hexBuff(INT8U *pByteBuff, INT8U *pHexBuff, INT8U len);
    static void hexDelimiter(INT8U *pByteBuff, INT8U *pHexBuff, INT8U len, INT8U delimiterChar);
    static void hexMAC(INT8U *pByteBuff, INT8U *pHexBuff);

    static INT8U halfByte(INT8U hexChar);
    static INT8U fullByte(INT8U charH, INT8U charL);

    static INT8U  byteAt(INT8U *pHexBuff);
    static INT16U wordAt(INT8U *pHexBuff);
    static INT32U dwordAt(INT8U *pHexBuff);
};

#endif
