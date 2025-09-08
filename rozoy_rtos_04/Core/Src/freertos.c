/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "stdio.h"
/* USER CODE BEGIN */
#include "ensi_uart.h"
#include "semphr.h"
//SemaphoreHandle_t xSemaphore;
QueueHandle_t xQueue;
TickType_t tick, tickReceived;
/**
* @brief Creation of software system architecture
*/
void app_init(void);

/**
* @brief Task 1 for test. Rename task with useful name for a real application
*/
static void task1(void *pvParameters);

/**
* @brief Task 2 for test. Rename task with useful name for a real application
*/
static void task2(void *pvParameters);

/**
* @brief Task 3 for test. Rename task with useful name for a real application
*/
static void task3(void *pvParameters);


void app_init(void) {
    /* application tasks creation */
	//xSemaphore = xSemaphoreCreateBinary();
	xQueue = xQueueCreate(5, sizeof(int));
	xTaskCreate(task1,       				/* Function that implements the task. */
				"Tache 1",          		/* Text name for the task. */
				configMINIMAL_STACK_SIZE,  	/* Stack size in words, not bytes. */
				NULL,    					/* Parameter passed into the task. */
				tskIDLE_PRIORITY+2,			/* Priority at which the task is created. */
				NULL);      				/* Used to pass out the created task's handle. */

	xTaskCreate(task2,       				/* Function that implements the task. */
				"Tache 2",          		/* Text name for the task. */
				configMINIMAL_STACK_SIZE,  	/* Stack size in words, not bytes. */
				NULL,    					/* Parameter passed into the task. */
				tskIDLE_PRIORITY+1,			/* Priority at which the task is created. */
				NULL);      				/* Used to pass out the created task's handle. */

	xTaskCreate(task3,       				/* Function that implements the task. */
				"Tache 3",          		/* Text name for the task. */
				configMINIMAL_STACK_SIZE,  	/* Stack size in words, not bytes. */
				NULL,    					/* Parameter passed into the task. */
				tskIDLE_PRIORITY+1,			/* Priority at which the task is created. */
				NULL);      				/* Used to pass out the created task's handle. */
}



static void task1(void *pvParameters) {
     // Task init

	 // Task infinite loop
	while(1) {
		 tick = xTaskGetTickCount();
		 HAL_GPIO_WritePin(Task1_GPIO_Port, Task1_Pin, GPIO_PIN_SET);
		 //xSemaphoreGive(xSemaphore);
		 xQueueSend(xQueue, &tick, (TickType_t)10);
		 HAL_GPIO_WritePin(Task1_GPIO_Port, Task1_Pin, GPIO_PIN_RESET);
		 vTaskDelay(3000);
	}
}


static void task2(void *pvParameters) {
     // Task init
	uint8_t buffer_tick[20];
	 // Task infinite loop
	while(1) {

		 //xSemaphoreTake(xSemaphore, portMAX_DELAY);
		 HAL_GPIO_WritePin(Task2_GPIO_Port, Task2_Pin, GPIO_PIN_SET);
		 //ENSI_UART_PutString((uint8_t*)"\r\n22222222");
		 //xQueueReceive(xQueue, &tickReceived, 1000);
		 //sprintf((char*)buffer_tick, "\n\r%lu", tick);

		 if (xQueueReceive(xQueue, &tickReceived, pdMS_TO_TICKS(portMAX_DELAY)) == pdTRUE) { // Delay infini (Attention activé vTaskSuspend)
		     // Un message a été reçu
		     sprintf((char*)buffer_tick, "\r\nReçu : %lu", (unsigned long)tickReceived);
		 } else {
		     // Timeout : aucun message reçu dans le délai
		     sprintf((char*)buffer_tick, "\r\nTimeout : aucun message");
		 }


		 //itoa(tickReceived, buffer_tick, 10);
		 ENSI_UART_PutString((const uint8_t*)buffer_tick);
		 HAL_GPIO_WritePin(Task2_GPIO_Port, Task2_Pin, GPIO_PIN_RESET);

	}
}


static void task3(void *pvParameters) {

	 // Task infinite loop
	while(1) {
	     // Task init
		 HAL_GPIO_WritePin(Task3_GPIO_Port, Task3_Pin, GPIO_PIN_SET);
		 ENSI_UART_PutString((uint8_t*)"\r\nTask3Working");
		 HAL_GPIO_WritePin(Task3_GPIO_Port, Task3_Pin, GPIO_PIN_RESET);
		 vTaskDelay(1000);
	}
}


/* USER CODE END */

