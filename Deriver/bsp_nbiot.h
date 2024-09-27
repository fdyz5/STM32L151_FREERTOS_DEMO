#ifndef _BSP_NBIOT_H_
#define _BSP_NBIOT_H_

int8_t 	bsp_nbiot_link(void);
int8_t  bsp_nbiot_get_socket(void);
int8_t  bsp_nbiot_send(uint8_t n,uint8_t *sbuf,uint8_t len);
int8_t  bsp_nbiot_recv(uint8_t socket,uint8_t tcmd);
int8_t  bsp_nbiot_get_time(uint8_t *pbuf);
int32_t bsp_nbiot_get_unix(void);


#endif
