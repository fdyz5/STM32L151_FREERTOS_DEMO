#ifndef _AT_H
#define _AT_H
#include <stdint.h>
extern uint8_t g_ucRssi;
extern uint8_t g_ucTimeout;  //³¬Ê±
extern uint32_t g_uiTime;

void AT_ResetSta(void);
void AT_SendCmd(char *_Cmd);
int AT_DELAY(uint32_t _period);
int AT_START(void);	
int AT_OK(char *_Cmd);
int AT_CPIN(char op);
int AT_CGSN(char op);
int AT_CIMI(char op);
int AT_CCID(char op);
int AT_CSQ(char op);
int AT_CREG(char op);
int AT_CGATT(char op);
int AT_QIOPEN(char op);
int AT_QICLOSE(char op);
int AT_M26_UDP(char op);
int AT_CFUN(char op);
int AT_CESQ(char op);
int AT_CEREG(char op);
int AT_QGACT(char op);
int AT_CSCON(char op);
int AT_QSOC(char op);
int AT_QSOCON(char op);
int AT_CCLK(char op);
int AT_QICFG(char *_cmd);
int AT_QIOPEN(char op);
int AT_ATI(char op);
int AT_BC26_EC20_QIOPEN(char op);
int AT_L610_MIPOPEN(char op);
int AT_BC26_UDP(char op);
int AT_L610_UDP(char op);
int AT_QLWSERV(char op);
int AT_QLWCONF(char op);
int AT_QLWADDOBJ(char op);
int AT_QLWOPEN(char op);
int AT_QLWCFG(char op);
int AT_BC26_LwM2M(char op);
int AT_EC20_UDP(char op);
void AT_ResetUpdataSta(void);
int AT_UPDATA(char op);
int AT_ACK(char *_Cmd, char *_Ack, char *_buf, uint16_t _usLen, uint32_t _usTimeout, uint8_t times);
int AT_BC260Y_AEP(char op);
int AT_BC260Y_UDP(char op);
int AT_BC260Y_CCLK(char op);
int AT_BC260Y_CGPADDR(void);
int AT_BC260Y_QIOPEN(char op);
int AT_BC260Y_NCDPOPEN(char op);
int AT_OK_E(char *_Cmd,const char *resp, int sec, int times);
int AT_BC260Y_CESQ(char op);
int AT_BC260Y_CEREG(void);
#endif
