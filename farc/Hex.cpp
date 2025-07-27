#include "Hex.h"

const char hexLetters[] = "0123456789ABCDEF";

void Hex::hex2(INT8U value, INT8U *pHexBuff) {
  pHexBuff[0] = hexLetters[HI_NIBBLE(value)];
  pHexBuff[1] = hexLetters[LO_NIBBLE(value)];
}
/*--------------------------------*/
void Hex::hex3(INT16U value, INT8U *pHexBuff) {
  pHexBuff[0] = hexLetters[LO_NIBBLE(HI_BYTE(value))];
  Hex::hex2(LO_BYTE(value), pHexBuff + 1);
}
/*--------------------------------*/
void Hex::hex4(INT16U value, INT8U *pHexBuff) {
  Hex::hex2(HI_BYTE(value), pHexBuff);
  Hex::hex2(LO_BYTE(value), pHexBuff + 2);
}
/*--------------------------------*/
void Hex::hex8(INT32U value, INT8U *pHexBuff) {
  Hex::hex4(HI_WORD(value), pHexBuff);
  Hex::hex4(LO_WORD(value), pHexBuff + 4);
}
/*--------------------------------*/
void Hex::hexBuff(INT8U *pByteBuff, INT8U *pHexBuff, INT8U len) {
  for (INT8U i=0; i < len; i++) Hex::hex2(pByteBuff[i], pHexBuff + i*2);
}
/*--------------------------------*/
void Hex::hexDelimiter(INT8U *pByteBuff, INT8U *pHexBuff, INT8U len, INT8U delimiterChar) {
  for (INT8U i=0; i < len; i++) {
    Hex::hex2(pByteBuff[i], pHexBuff + i*3);
    if (i < len-1) pHexBuff[i*3 + 2] = delimiterChar;
  }
}
/*--------------------------------*/
void Hex::hexMAC(INT8U *pByteBuff, INT8U *pHexBuff) {
  Hex::hexDelimiter(pByteBuff, pHexBuff, 6, COLON_CHAR);
}
/*--------------------------------*/
INT8U Hex::halfByte(INT8U hexChar) {
  return (hexChar >= '0' && hexChar <= '9') ? hexChar - '0' :
         (hexChar >= 'A' && hexChar <= 'F') ? hexChar - 'A' + 10 :
         (hexChar >= 'a' && hexChar <= 'f') ? hexChar - 'a' + 10 : 0;
}
/*--------------------------------*/
INT8U Hex::fullByte(INT8U charH, INT8U charL) {
  return (halfByte(charH) << 4) | halfByte(charL);
}
/*--------------------------------*/
INT8U  Hex::byteAt(INT8U *pHexBuff) {
  return Hex::fullByte(pHexBuff[0], pHexBuff[1]);
}
INT16U Hex::wordAt(INT8U *pHexBuff) {
  return ((INT16U)Hex::byteAt(pHexBuff) << 8) | Hex::byteAt(pHexBuff + 2);  
}
/*--------------------------------*/
INT32U Hex::dwordAt(INT8U *pHexBuff) {
  return ((INT32U)Hex::wordAt(pHexBuff) << 16) | Hex::wordAt(pHexBuff + 4);  
}
/*--------------------------------*/
