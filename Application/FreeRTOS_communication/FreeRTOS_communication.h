/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： __FREERTOS_COMM_H.h
* 文件标识：
* 内容摘要： __FREERTOS_COMM_H相关函数声明
* 其它说明：
* 当前版本：
* 作    者： woody
* 完成日期： 2020.09.07
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
************************************************************************/
#ifndef __FREERTOS_COMM_H
#define __FREERTOS_COMM_H
#include "stm32l1xx.h"
#include "MainConfig.h" 

extern SemaphoreHandle_t xMutex;
void TaskA(void *pvParameters);
void TaskB(void *pvParameters);
void TaskC(void *pvParameters);
void TaskSender(void* pvParameters); 
void TaskReceiver(void* pvParameters);
void TaskFunction(void *pvParameters);
void vDataProcessingTask(void *pvParameters);
void vDataProcessingTaskA(void *pvParameters);
void vDataProcessingTaskB(void *pvParameters);
#endif  /* __FREERTOS_COMM_H */


































