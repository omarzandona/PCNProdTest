#ifndef SNP_H
#define SNP_H

#include "windows.h"
#include "assert.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef SNP_EXPORTS
#define SNP_API _declspec(dllexport)
#else
#define SNP_API _declspec(dllimport)
#endif

#define TEXTSIZE           257
#define BROADCAST          0xFF

#define SNP_KERNEL_VERSION     1
#define SNP_OS_VERSION         2
#define SNP_FW_VERSION         3
#define SNP_SW_VERSION         4
#define SNP_ENABLE_PC          5
#define SNP_DISABLE_PC         6
#define SNP_GET_CNT_VAL        7
#define SNP_RESET_CNT          8
#define SNP_GET_TIME           9
#define SNP_SET_TIME           10
#define SNP_ENABLE_DIA_CTR     11
#define SNP_DISABLE_DIA_CTR    12
#define SNP_GET_DIA_STATUS     13
#define SNP_SET_IP_ADDRESS     14
#define SNP_POLLING            15
#define SNP_SET_LED            16
#define SNP_ENABLE_AUTOLED     17
#define SNP_DISABLE_AUTOLED    18
#define SNP_SET_INPUT0         19
#define SNP_SET_INPUT1         20
#define SNP_SET_OUTPUT0        21
#define SNP_SET_OUTPUT1        22
#define SNP_GET_DOORSTATUS     23
#define SNP_SET_INST_HEIGHT    24
#define SNP_SET_DOORSTATUS     25

SNP_API int SNP_initCommunication(char *port, unsigned long BaudRate, unsigned char ByteSize, unsigned char Parity, unsigned char StopBits);
SNP_API void SNP_endCommunication();
SNP_API int SNP_command(unsigned char dest, unsigned char command, char *pInBuf, char *pOutBuf, unsigned int OutBufLen);
SNP_API void SNP_getLastError(char *pOutError, unsigned int outErrorLen);
SNP_API void SNP_setTimeout(unsigned int timeOut);

#define SNP_getKernelVersion(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_KERNEL_VERSION, NULL, pOutBuf, OutBufLen)
#define SNP_getOSVersion(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_OS_VERSION, NULL, pOutBuf, OutBufLen)
#define SNP_getFWVersion(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_FW_VERSION, NULL, pOutBuf, OutBufLen)
#define SNP_getSWVersion(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_SW_VERSION, NULL, pOutBuf, OutBufLen)
#define SNP_enablePCN(dest) SNP_command(dest, SNP_ENABLE_PC, NULL, NULL, 0)
#define SNP_disablePCN(dest) SNP_command(dest, SNP_DISABLE_PC, NULL, NULL, 0)
#define SNP_getCounters(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_GET_CNT_VAL, NULL, pOutBuf, OutBufLen)
#define SNP_resetCounters(dest) SNP_command(dest, SNP_RESET_CNT, NULL, NULL, 0)
#define SNP_getDateTime(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_GET_TIME, NULL, pOutBuf, OutBufLen)
#define SNP_setDateTime(dest, pInBuf) SNP_command(dest, SNP_SET_TIME, pInBuf, NULL, 0)
#define SNP_enableDiagnosticCtr(dest) SNP_command(dest, SNP_ENABLE_DIA_CTR, NULL, NULL, 0)
#define SNP_disableDiagnosticCtr(dest) SNP_command(dest, SNP_DISABLE_DIA_CTR, NULL, NULL, 0)
#define SNP_getDiagnosticStatus(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_GET_DIA_STATUS, NULL, pOutBuf, OutBufLen)
#define SNP_setIPAddress(dest, pInBuf) SNP_command(dest, SNP_SET_IP_ADDRESS, pInBuf, NULL, 0)
#define SNP_polling(dest) SNP_command(dest, SNP_POLLING, NULL, NULL, 0)
#define SNP_setLed(dest, pInBuf) SNP_command(dest, SNP_SET_LED, pInBuf, NULL, 0)
#define SNP_enableAutoled(dest) SNP_command(dest, SNP_ENABLE_AUTOLED, NULL, NULL, 0)
#define SNP_disableAutoled(dest) SNP_command(dest, SNP_DISABLE_AUTOLED, NULL, NULL, 0)
#define SNP_setInput0(dest, pInBuf) SNP_command(dest, SNP_SET_INPUT0, pInBuf, NULL, 0)
#define SNP_setInput1(dest, pInBuf) SNP_command(dest, SNP_SET_INPUT1, pInBuf, NULL, 0)
#define SNP_setOutput0(dest, pInBuf) SNP_command(dest, SNP_SET_OUTPUT0, pInBuf, NULL, 0)
#define SNP_setOutput1(dest, pInBuf) SNP_command(dest, SNP_SET_OUTPUT1, pInBuf, NULL, 0)
#define SNP_getDoorStatus(dest, pOutBuf, OutBufLen) SNP_command(dest, SNP_GET_DOORSTATUS, NULL, pOutBuf, OutBufLen)
//#define SNP_setInstHeight(dest, pInBuf) SNP_command(dest, SNP_SET_INST_HEIGHT, pInBuf, NULL, 0) // Disponibile solo con la versione 2.3.11.2 dellp'imgserver
#define SNP_setDoorStatus(dest, pInBuf) SNP_command(dest, SNP_SET_DOORSTATUS, pInBuf, NULL, 0)
#ifdef __cplusplus
}
#endif

#endif /* SNP_H */
