#include "usart.h"
#include <stm32l1xx.h>
void Clear_Buffer(void);//��ջ���	
int BC20_Init(void);
void BC20_PDPACT(void);
void BC20_ConUDP(void);
void BC20_RECData(void);
void BC20_Senddata(uint8_t *len,uint8_t *data);
typedef struct
{
   uint8_t CSQ;    
   uint8_t Socketnum;   //���
   uint8_t reclen[10];   //��ȡ�����ݵĳ���
   uint8_t res;      
   uint8_t recdatalen[10];
   uint8_t recdata[100];
} BC20;

