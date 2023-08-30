///AT+NSOCL=0
#include "BC20.h"
#include "main.h"
#include "string.h"
#include "oled.h"
char *strx,*extstrx;
char atstr[BUFLEN];
int err;    //全局变量
int socketnum=0;//当前的socket号码
BC20 BC20_Status;

#define SERVERIP "47.104.238.90"
#define SERVERPORT 1001

void Clear_Buffer(void)//清空串口2缓存
{
    printf(buf_uart2.buf);  //清空前打印信息
    Delay(300);
    buf_uart2.index=0;
    memset(buf_uart2.buf,0,BUFLEN);
}

int BC20_Init(void)
{
    int errcount = 0;
    err = 0;    //判断模块卡是否就绪最重要
    printf("start init BC20\r\n");
    Uart2_SendStr("AT\r\n");
    Delay(300);
    printf(buf_uart2.buf);      //打印收到的串口信息
    printf("get back BC20\r\n");
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    Clear_Buffer();
    while(strx==NULL)
    {
        printf("\r\n单片机正在连接到模块...\r\n");
        OLED_ShowString(0,3,"START NBIOT...");
        Clear_Buffer();
        Uart2_SendStr("AT\r\n");
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    }

    Uart2_SendStr("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR");//只要卡不错误 基本就成功
    if(strx==NULL)
    {
        printf("我的卡号是 : %s \r\n",&buf_uart2.buf[8]);
        OLED_ShowString(0,3,"NB SIMCARD [OK]");
        Clear_Buffer();
        Delay(300);
    }
    else
    {
        err = 1;
        printf("卡错误 : %s \r\n",buf_uart2.buf);
        OLED_ShowString(0,3,"NB SIMCARD [..]");
        Clear_Buffer();
        Delay(300);
    }

    Uart2_SendStr("AT+CGATT=1\r\n");//激活网络，PDP
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返OK
    Clear_Buffer();
    if(strx)
    {
        Clear_Buffer();
        printf("init PDP OK\r\n");
        Delay(300);
    }
    Uart2_SendStr("AT+CGATT?\r\n");//查询激活状态
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//返1 表明激活成功 获取到IP地址了
    Clear_Buffer();
    errcount = 0;
    while(strx==NULL)
    {
        OLED_ShowString(0,3,"NB Regist [...]");
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CGATT?\r\n");//获取激活状态
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//返回1,表明注网成功
        if(errcount>100)     //防止死循环
        {
            OLED_ShowString(0,3,"NB Regist  [NO]");
            err=1;
            errcount = 0;
            break;
        }
    }


    Uart2_SendStr("AT+QBAND?\r\n"); //允许错误值
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    if(strx)
    {
        printf("========BAND========= \r\n %s \r\n",buf_uart2.buf);

        //OLED_ShowString(0,5,(u8 *)buf_uart2.buf+13);
        Clear_Buffer();
        Delay(300);
    }

    Uart2_SendStr("AT+CSQ\r\n");//查看获取CSQ值
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSQ");//返回CSQ
    if(strx)
    {
        printf("信号质量:%s\r\n",buf_uart2.buf);
        Clear_Buffer();
        Delay(300);
    }
    Uart2_SendStr("AT+CEREG?\r\n");
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 0,1");//返回注册状态
    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 1,1");//返回注册状态
    Clear_Buffer();
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)//两个返回值都没有
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CEREG?\r\n");//判断运营商
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 0,1");//返回注册状态
        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG: 1,1");//返回注册状态
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC20_PDPACT(void)//激活场景，为连接服务器做准备
{
    int errcount = 0;
    Uart2_SendStr("AT+CGPADDR=1\r\n");//激活场景
    Delay(300);
    Clear_Buffer();
    Uart2_SendStr("AT+CGATT?\r\n");//激活场景
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//注册上网络信息
    Clear_Buffer();
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CGATT?\r\n");//激活场景
        Delay(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT: 1");//一定要终端入网
        if(errcount>100)     //防止死循环
        {
            errcount = 0;
            break;
        }
    }
    Uart2_SendStr("AT+QICLOSE=0\r\n");//判断连接状态，返回1就是成功
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
    Uart2_SendStr(atstr);//发送 socketIP和端口后面跟对应数据长度以及数据
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QIOPEN: 0,0");//返回OK
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QIOPEN: 0,0");//返回OK
        if(errcount>100)     //防止死循环
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
    Uart2_SendStr(atstr);//发送0 socketIP和端口后面跟对应数据长度以及数据
    Delay(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
        if(errcount>100)     //防止死循环
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
        GPIO_SetBits(GPIOC,GPIO_Pin_6);		//发送失败重启单片机和复位模块
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
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QIRD");//返回+QIRD:，表明接收到TCP服务器发回的数据
    if(strx)
    {
        Delay(300);
        Clear_Buffer();

    }
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QIRD=%d,%d\r\n",socketnum,100);
    Uart2_SendStr(atstr); //允许错误值
    Delay(300);
    Clear_Buffer();
}

