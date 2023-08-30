#include "FreeRTOS_communication.h"

#define MUTEX_TEST_EN 1
#define EVENT_TEST_EN 0
#define EVENT_NOTFICATION_EN   0
#define SOFEWARETIME_EN    0
#define FREEHEAPSIZE_EN    0
/***************ª•≥‚¡ø≤‚ ‘*********************/
#if MUTEX_TEST_EN
//// ??????
int sharedCounter = 0;
// ???
SemaphoreHandle_t xMutex;

void TaskA(void *pvParameters) {
	 // ?????
    xMutex = xSemaphoreCreateMutex();
    while (1) {
        // ?????
//        if (xSemaphoreTake(xMutex, pdMS_TO_TICKS(100)) == pdTRUE) 
//					{
//            // ??????,???????
//            sharedCounter++;
//            printf("TaskA: sharedCounter = %d\n", sharedCounter);
//            // ?????
//            xSemaphoreGive(xMutex);
//        }
        // ??????,????????
				printf("TaskA running\r\n");
			//	delay_ms(1000);
			 vTaskDelay(pdMS_TO_TICKS(1000));
				printf("Test delay_ms\r\n");
			//	delay_ms(100);
			 vTaskDelay(pdMS_TO_TICKS(100));
				printf("Test delay_ms100\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void TaskB(void *pvParameters) {
    while (1) {
        // ?????
//        if (xSemaphoreTake(xMutex, pdMS_TO_TICKS(100)) == pdTRUE) 
//					{
//            // ??????,???????
//            sharedCounter--;
//            printf("TaskB: sharedCounter = %d\n", sharedCounter);
//            // ?????
//            xSemaphoreGive(xMutex);
//        }
				
				printf("TaskB running\r\n");
        // ??????,????????
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
#endif

/***************ª•≥‚¡ø≤‚ ‘*********************/

/**************** ¬º˛◊È≤‚ ‘*********************/
#if EVENT_TEST_EN

#define DATA_READY_BIT (1 << 0)
#define DATA_READY_BIT1 (1 << 1)
#define DATA_READY_BIT2 (1 << 2)
// ????????????
EventGroupHandle_t  xEventGroup;

// ??A,?????????
void TaskA(void *pvParameters) {
	    // ?????
    xEventGroup = xEventGroupCreate();
    while (1) {
        // ???????????????
        // ...

        // ???????,???????DATA_READY_BIT?
        xEventGroupSetBits(xEventGroup, DATA_READY_BIT);
			
        printf("Release event0 position\r\n");
        // ??A??????
        vTaskDelay(pdMS_TO_TICKS(1000));
			
			        // ???????,???????DATA_READY_BIT?
        xEventGroupSetBits(xEventGroup, DATA_READY_BIT1);
			
        printf("Release event1 position\r\n");
        // ??A??????
        vTaskDelay(pdMS_TO_TICKS(1000));
			
			        // ???????,???????DATA_READY_BIT?
       xEventGroupSetBits(xEventGroup, DATA_READY_BIT2);
			
        printf("Release event2 position\r\n");
        // ??A??????
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ??B,??????????????
void TaskB(void *pvParameters) {
    while (1) {
        // ???????DATA_READY_BIT????
        EventBits_t uxBits = xEventGroupWaitBits(xEventGroup, DATA_READY_BIT|DATA_READY_BIT1|DATA_READY_BIT2, pdTRUE, pdFALSE, 0);
        // EventBits_t uxBits = xEventGroupGetBits(xEventGroup);
        // ??DATA_READY_BIT??????
        if (uxBits & DATA_READY_BIT) {
            // ????????
            printf("Sending data via0 UART...\n");

            // ??DATA_READY_BIT?,?????????
           // xEventGroupClearBits(xEventGroup, DATA_READY_BIT);
        }
				 else if (uxBits & DATA_READY_BIT1) {
            // ????????
            printf("Sending data via1 UART...\n");

            // ??DATA_READY_BIT?,?????????
            //xEventGroupClearBits(xEventGroup, DATA_READY_BIT1);
        }
				 else if (uxBits & DATA_READY_BIT2) {
            // ????????
            printf("Sending data via2 UART...\n");

            // ??DATA_READY_BIT?,?????????
           // xEventGroupClearBits(xEventGroup, DATA_READY_BIT2);
        }

//      	#if DEBUG_LOG_PRINTF  
//			/* ???????????????,????FreeRTOS??INCLUDE_uxTaskGetStackHighWaterMark??
//			 * uxTaskGetStackHighWaterMark()????????????????, ?????????????
//			 * ??????????ì???(High Water Mark)î?????????,????????????*/
//			  LOG_D("Reference TaskB stack size = %d", ((int )configMINIMAL_STACK_SIZE-(int)uxTaskGetStackHighWaterMark(NULL))*2);
//	#endif

        // ??B??????
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
#endif
/**************** ¬º˛◊È≤‚ ‘*********************/

/**************** ¬º˛Õ®÷™≤‚ ‘*********************/
#if      EVENT_NOTFICATION_EN  
		// ????
TaskHandle_t xTaskAHandle, xTaskBHandle;
// ??????
TaskHandle_t TaskSenderHandle;
TaskHandle_t TaskReceiverHandle;

// ??????
#define TEMPERATURE_HIGH_NOTIFY (1 << 0)

// ??A??
void TaskA(void *pvParameters) {
	    // ????1???
     xTaskBHandle = xTaskGetHandle("TaskB");
    for (;;) {
        // ????????
        int temperature = 38;
        printf("Task A: Temperature = %d\n", temperature);
        
        if (temperature > 30) {
            // ???????????B
            xTaskNotifyGive(xTaskBHandle);
            printf("Task A: Sent temperature high notification to Task B\n");
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // ??1???????
    }
}

// ??B??
void TaskB(void *pvParameters) {
	 xTaskAHandle = xTaskGetHandle("TaskA");
    for (;;) {
        // ???????
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Task B: Received temperature high notification\n");
        
        // ?????,??????????

        printf("Task B: Adjusted fan speed\n");
    }
}

void TaskSender(void* pvParameters) {
    uint32_t ulValueToSend = 5; // ??????
  	uint32_t ulValueToSend1 = 10; // ??????
    BaseType_t xResult;
    for (;;) {
    // ????
    xResult = xTaskNotify(TaskReceiverHandle, ulValueToSend, eSetValueWithOverwrite);

    if (xResult == pdPASS) {
        // ??????
        printf("TaskSender: Notification sent ulValueToSend successfully\r\n");
    }

    vTaskDelay(pdMS_TO_TICKS(3000)); // ??1???????
	      // ????
 //   vTaskDelete(NULL);
		
	  xResult = xTaskNotify(TaskReceiverHandle, ulValueToSend1, eSetValueWithOverwrite);
    if (xResult == pdPASS) {
        // ??????
        printf("TaskSender: Notification sent ulValueToSend1 successfully\r\n");
    }

    vTaskDelay(pdMS_TO_TICKS(3000)); // ??1???????
	}
	
}

void TaskReceiver(void* pvParameters) {
    uint32_t ulReceivedValue;
    BaseType_t xResult;

    for (;;) {
        // ????
        xResult = xTaskNotifyWait(0, 0, &ulReceivedValue, portMAX_DELAY);

        if (xResult == pdTRUE) {
            // ??????
            // ulReceivedValue ?????????
            printf("TaskReceiver: Notification received successfully\r\n");

            // ?? eNotifyAction ??????
            if (ulReceivedValue ) {
                // ...
                printf("TaskReceiver: Processing notification %d\r\n",ulReceivedValue);
            }
//						else if (ulReceivedValue == 1) {
//                // ...
//                printf("1\r\n");
//            }
//					  else if (ulReceivedValue == 2) {
//                // ...
//                printf("2\r\n");
//            }
//						else if (ulReceivedValue == 3) {
//                // ...
//                printf("3\r\n");
//            }
//						else if (ulReceivedValue == 4) {
//                // ...
//                printf("4\r\n");
//            }
//					  else if (ulReceivedValue == 5) {
//                // ...
//                printf("5\r\n");
//            }
//						else if (ulReceivedValue == 6) {
//                // ...
//                printf("6\r\n");
//            }
        }
	//			printf("TaskReceiver: doning other tasks\r\n");
    }
}


#endif
/**************** ¬º˛Õ®÷™≤‚ ‘*********************/

/****************»Ìº˛∂® ±∆˜≤‚ ‘********************/
#if SOFEWARETIME_EN

TaskHandle_t TaskHandle;
TimerHandle_t SoftwareTimer1, SoftwareTimer2;

void SoftwareTimerCallback1(TimerHandle_t xTimer) {
    static BaseType_t xLEDState = pdFALSE;
    xLEDState = !xLEDState;
     printf("LED1State = %d\r\n",(int32_t)xLEDState);
}

void SoftwareTimerCallback2(TimerHandle_t xTimer) {
    static BaseType_t xLEDState = pdFALSE;
    xLEDState = !xLEDState;
     printf("LED2State = %d\r\n",(int32_t)xLEDState);
}

void TaskFunction(void *pvParameters) {
    SoftwareTimer1 = xTimerCreate("SoftwareTimer1", pdMS_TO_TICKS(500), pdTRUE, (void *)0, SoftwareTimerCallback1);
    SoftwareTimer2 = xTimerCreate("SoftwareTimer2", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, SoftwareTimerCallback2);
	  if (xTimerStart(SoftwareTimer1, pdMS_TO_TICKS(1000)) == pdPASS && xTimerStart(SoftwareTimer2, pdMS_TO_TICKS(1000)) == pdPASS) 
		{
			
		}

//    if (SoftwareTimer1 != NULL && SoftwareTimer2 != NULL) 
//		{
//        if (xTimerStart(SoftwareTimer1, pdMS_TO_TICKS(1000)) == pdPASS && xTimerStart(SoftwareTimer2, pdMS_TO_TICKS(1000)) == pdPASS) 
//				{
//            while (1) 
//						{
//                vTaskDelay(pdMS_TO_TICKS(1000));
//            }
//        }
//    }

    while (1) 
		{
			vTaskDelay(pdMS_TO_TICKS(1000));
			printf("TaskFunction other function running\r\n");
       // vTaskDelay(portMAX_DELAY);
    }
}
#endif
/****************»Ìº˛∂® ±∆˜≤‚ ‘********************/

/****************xPortGetFreeHeapSize ªÒ»° £”‡∂—ø’º‰≤‚ ‘****************/
// ???????
#if FREEHEAPSIZE_EN
typedef struct {
    uint8_t data[50]; // ??????????100??
} DataPacket;

void vDataProcessingTask(void *pvParameters) 
{
    while (1) {
        // ???????????
        DataPacket *packet = (DataPacket *)pvPortMalloc(sizeof(DataPacket));
        if (packet == NULL) {
            // ????,?????????
					   size_t freeHeapSize = xPortGetFreeHeapSize();
            // ?????????????
            printf("Free heap size: %u bytes\n", (unsigned int)freeHeapSize);
            // ...
        } else {
            // ∑÷≈‰≥…π¶£¨¥¶¿Ì ˝æ›∞¸
            // ...
            
            // ??????????
            size_t freeHeapSize = xPortGetFreeHeapSize();
            // ?????????????
            printf("Free heap size: %u bytes\n", (unsigned int)freeHeapSize);
            
            // ????????
           // vPortFree(packet);
        }
        
        // ??????,??????????
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void vDataProcessingTaskA(void *pvParameters) 
{
    while (1) 
		{
        // ???????????
        DataPacket *packet = (DataPacket *)pvPortMalloc(sizeof(DataPacket));
        if (packet == NULL) 
				{
            // ????,?????????
					   size_t freeHeapSize = xPortGetFreeHeapSize();
            // ?????????????
            printf("ATask test Free heap size: %u bytes\n", (unsigned int)freeHeapSize);
            // ...
        }
				else 
				{
            // ∑÷≈‰≥…π¶£¨¥¶¿Ì ˝æ›∞¸
            // ...
            
            // ??????????
            size_t freeHeapSize = xPortGetFreeHeapSize();
            // ?????????????
            printf("A Free heap size: %u bytes\n", (unsigned int)freeHeapSize);
					
					  // ????????
            vPortFree(packet);
	#if DEBUG_LOG_PRINTF  
			/* ???????????????,????FreeRTOS??INCLUDE_uxTaskGetStackHighWaterMark??
			 * uxTaskGetStackHighWaterMark()????????????????, ?????????????
			 * ??????????ì???(High Water Mark)î?????????,????????????*/
			     LOG_D("Reference TaskA stack size = %d", (int)uxTaskGetStackHighWaterMark(NULL));
	#endif           
        }
        
        // ??????,??????????
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void vDataProcessingTaskB(void *pvParameters) 
{
    while (1) 
		{
        // ???????????
        DataPacket *packet = (DataPacket *)pvPortMalloc(sizeof(DataPacket));
        if (packet == NULL) 
				{
            // ????,?????????
					   size_t freeHeapSize = xPortGetFreeHeapSize();
            // ?????????????
            printf("BTask test Free heap size: %u bytes\n", (unsigned int)freeHeapSize);
            // ...
        }
				else 
				{
            // ∑÷≈‰≥…π¶£¨¥¶¿Ì ˝æ›∞¸
            // ...
            
            // ??????????
            size_t freeHeapSize = xPortGetFreeHeapSize();
            // ?????????????
            printf("B Free heap size: %u bytes\n", (unsigned int)freeHeapSize);
					
					  // ????????
            vPortFree(packet);
	#if DEBUG_LOG_PRINTF  
			/* ???????????????,????FreeRTOS??INCLUDE_uxTaskGetStackHighWaterMark??
			 * uxTaskGetStackHighWaterMark()????????????????, ?????????????
			 * ??????????ì???(High Water Mark)î?????????,????????????*/
			     LOG_D("Reference TaskB stack size = %d", (int)uxTaskGetStackHighWaterMark(NULL));
	#endif           
        }
        
        // ??????,??????????
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
#endif
/****************xPortGetFreeHeapSize ªÒ»° £”‡∂—ø’º‰≤‚ ‘****************/