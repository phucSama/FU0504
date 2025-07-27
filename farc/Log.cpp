#include "Log.h"

LogHelper::LogHelper() {}

void LogHelper::hexBuff(INT8U *pByteBuff, INT8U len) {
  Hex::hexBuff(pByteBuff, &_logBuff[0], len);
  _logBuff[len*2] = EOS_CHAR;
  LOG_LN((char*)_logBuff);
}
/*--------------------------------*/
void LogHelper::mac(INT8U *pMacBuff) {
  Hex::hexMAC(pMacBuff, &_logBuff[0]);
  _logBuff[6*3-1] = EOS_CHAR;
  LOG_LN((char*)_logBuff);
}
/*--------------------------------*/

LogHelper LOG;
