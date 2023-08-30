///AT+NSOCL=0
#include "BC20.h"
#include "main.h"
#include "string.h"
#include "oled.h"
char *strx,*extstrx;
char atstr[BUFLEN];
int err;    //ȫ�ֱ���
int socketnum=0;//��ǰ��socket����
BC20 BC20_Status;

#define SERVERIP "47.104.238.90"
#define SERVERPORT 1001

void Clear_Buffer(void)//��մ���2����
{
    printf(buf_uart2.buf);  //���ǰ��ӡ��Ϣ
    Delay(300);
    buf_uart2.index=0;
    memset(buf_uart2.buf,0,BUFLEN);
}

int BC20_Init(void)
{
    int errcount = 0;
    err = 0;    //�ж�ģ�鿨�Ƿ��������Ҫ
    printf("start init BC20\r\n");
    Uart2_SendStr("AT\r\n");
    Delay(300);
    printf(buf_uart2.buf);      //��ӡ�յ��Ĵ�����Ϣ
    printf("get back BC20\r\n");
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    Clear_Buffer();
    while(strx==NULL)
    {
        printf("\r\n��Ƭ���������ӵ�ģ��...\r\n");
        OLED_ShowString(0,3,"START NBIOT...");
        Clear_Buffer();
        Uart2_SendStr("AT\r\n");
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    }

    Uart2_SendStr("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR");//ֻҪ�������� �����ͳɹ�
    if(strx==NULL)
    {
        printf("�ҵĿ����� : %s \r\n",&buf_uart2.buf[8]);
        OLED_ShowString(0,3,"NB SIMCARD [OK]");
        Clear_Buffer();
        Delay(300);
    }
    else
    {
        err = 1;
        printf("������ : %s \r\n",buf_uart2.buf);
        OLED_ShowString(0,3,"NB SIMCARD [..]");
        Clear_Buffer();
        Delay(300);
    }

    Uart2_SendStr("AT+CGATT=1\r\n");//�������磬PDP
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//��OK
    Clear_Buffer();
    if(strx)
    {
        Clear_Buffer();
        printf("init PDP OK\r\n");
        Delay(300);
    }
    Uart2_SendStr("AT+CGATT?\r\n");//��ѯ����״̬
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//��1 ��������ɹ� ��ȡ��IP��ַ��
    Clear_Buffer();
    errcount = 0;
    while(strx==NULL)
    {
        OLED_ShowString(0,3,"NB Regist [...]");
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CGATT?\r\n");//��ȡ����״̬
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
        if(errcount>100)     //��ֹ��ѭ��
        {
            OLED_ShowString(0,3,"NB Regist  [NO]");
            err=1;
            errcount = 0;
            break;
        }
    }


    Uart2_SendStr("AT+QBAND?\r\n"); //�������ֵ
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    if(strx)
    {
        printf("========BAND========= \r\n %s \r\n",buf_uart2.buf);

        //OLED_ShowString(0,5,(u8 *)buf_uart2.buf+13);
        Clear_Buffer();
        Delay(300);
    }

    Uart2_SendStr("AT+CSQ\r\n");//�鿴��ȡCSQֵ
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSQ");//����CSQ
    if(strx)
    {
        printf("�ź�����:%s\r\n",buf_uart2.buf);
        Clear_Buffer();
        Delay(300);
    }
    Uart2_SendStr("AT+CEREG?\r\n");
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 0,1");//����ע��״̬
    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 1,1");//����ע��״̬
    Clear_Buffer();
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)//��������ֵ��û��
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CEREG?\r\n");//�ж���Ӫ��
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 0,1");//����ע��״̬
        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 1,1");//����ע��״̬
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC20_PDPACT(void)//�������Ϊ���ӷ�������׼��
{
    int errcount = 0;
    Uart2_SendStr("AT+CGPADDR=1\r\n");//�����
    Delay(300);
    Clear_Buffer();
    Uart2_SendStr("AT+CGATT?\r\n");//�����
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//ע����������Ϣ
    Clear_Buffer();
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CGATT?\r\n");//�����
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//һ��Ҫ�ն�����
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    Uart2_SendStr("AT+QICLOSE=0\r\n");//�ж�����״̬������1���ǳɹ�
    Delay(300);
    Clear_Buffer();
    printf("\r\njust close old socket\r\n");
}

void BC20_ConUDP(void)
{
    int errcount = 0;
    Uart2_SendStr("AT+CGSN=1\r\n");//TCP Socket
    Delay(300);
    Clear_Buffer();
    printf("socket number = %d\r\n",socketnum);
    memset(atstr,0,BUFLEN);
    OLED_ShowString(0,3,"CONNECT TO [..]");
    sprintf(atstr,"AT+QIOPEN=1,%d,\"UDP\",\"%s\",%d,1234,0\r\n",socketnum,SERVERIP,SERVERPORT);
    Uart2_SendStr(atstr);//���� socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QIOPEN: 0,0");//����OK
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QIOPEN: 0,0");//����OK
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    OLED_ShowString(0,3,"CONNECT TO [OK]");
    Clear_Buffer();
}

void BC20_Senddata(uint8_t *len,uint8_t *data)
{
    int errcount=0;
    memset(atstr,0,BUFLEN);
    OLED_ShowString(0,3,"SEND DATA  [..]");
    sprintf(atstr,"AT+QISEND=%d,%s,%s\r\n",socketnum,len,data);
    Uart2_SendStr(atstr);//����0 socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    OLED_ShowString(0,3,"SEND DATA  [OK]");
    strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR");
    if(strx)
    {
        OLED_ShowString(0,3,"SEND DATA  [NO]");
        GPIO_SetBits(GPIOC,GPIO_Pin_6);		//����ʧ��������Ƭ���͸�λģ��
        Delay(500);
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        Delay(300);
        NVIC_SystemReset();
    }
    Clear_Buffer();

}

/*
+QSONMI=0,7
AT+QSORF=0,100
0,47.104.238.90,1001,7,63736c65643131,0
*/
void BC20_RECData(void)
{
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QIRD");//����+QIRD:���������յ�TCP���������ص�����
    if(strx)
    {
        Delay(300);
        Clear_Buffer();

    }
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QIRD=%d,%d\r\n",socketnum,100);
    Uart2_SendStr(atstr); //�������ֵ
    Delay(300);
    Clear_Buffer();
}

