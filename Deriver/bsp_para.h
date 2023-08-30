#ifndef _BSP_PARA_H_
#define _BSP_PARA_H_

typedef struct
{
	uint8_t mode; 				//����ģʽ
	uint8_t devAddr;			//�豸�Ӻ�
	uint8_t devNum;				//�豸�Ӻ�����
	uint8_t devAddrAsc[12];		//�豸���
	
	uint16_t ai1Up;				//ģ����1����
	uint16_t ai1Down;			//ģ����1����
	
	uint16_t ai2Up;				//ģ����2����
	uint16_t ai2Down;			//ģ����2����
	
	uint16_t ai3Up;				//ģ����3����
	uint16_t ai3Down;			//ģ����3����
	
	uint8_t ai1Addr;			//ģ����1������ַ
	uint8_t ai2Addr;			//ģ����2������ַ
	uint8_t ai3Addr;			//ģ����3������ַ
	
	uint8_t ai1Enable;			//ģ����1ʹ��
	uint8_t ai2Enable;			//ģ����2ʹ��
	uint8_t ai3Enable;			//ģ����3ʹ��
	
	uint8_t diAddr;				//������������ַ
	
	uint16_t rptTime;			//�ϱ�ʱ��
	
	uint64_t baudRate;			//���ڲ�����
	uint64_t dataBit;			//����ֹͣλ
	uint64_t parity;			//����У��λ
	uint64_t stopBit;			//����ֹͣλ
	
	uint8_t ip[16];				//������IP��ַ
	uint16_t port;				//�������˿ں�
	
	uint8_t di1;
	uint8_t di2;

}PARA_TYPEDEF;

extern PARA_TYPEDEF para_t;

void bsp_para_init(void);

void bsp_para_default(void);
uint8_t bsp_para_set(const char *cmd, uint8_t* buf_out);
uint8_t bsp_para_get(const char* describe, const char *cmd, uint8_t* buf_out);

#endif
