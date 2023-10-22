#include "FreeRTOS.h"
#include "task.h"

#include "rcc.h"
#include "gpio.h"

#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      256
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      256

TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

gpio_t led1, led2;

void gpio_tgl_init(void)
{
	led1.port = gpio_port_a;
	led1.pin = gpio_pin_1;
	led1.mode = gpio_mode_out10mhz;
	led1.oconfig = gpio_oconfig_pp;

	led2.port = gpio_port_a;
	led2.pin = gpio_pin_2;
	led2.mode = gpio_mode_out10mhz;
	led2.oconfig = gpio_oconfig_pp;

	gpio_init(&led1);
	gpio_init(&led2);
}

void task1_task(void *pvParameters)
{
    while(1)
    {
        gpio_tgl(&led1);
        vTaskDelay(250);
        gpio_tgl(&led1);
        vTaskDelay(250);
    }
}

void task2_task(void *pvParameters)
{
    while(1)
    {
        gpio_tgl(&led1);
        vTaskDelay(500);
        gpio_tgl(&led1);
        vTaskDelay(500);
    }
}

int main(void)
{
	rcc_init();
	/*NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);*/
	/*SystemInit();*/
	/*SystemCoreClockUpdate();*/
	gpio_tgl_init();

	/* create two task */
    xTaskCreate((TaskFunction_t )task2_task,
                        (const char*    )"task2",
                        (uint16_t       )TASK2_STK_SIZE,
                        (void*          )NULL,
                        (UBaseType_t    )TASK2_TASK_PRIO,
                        (TaskHandle_t*  )&Task2Task_Handler);

    xTaskCreate((TaskFunction_t )task1_task,
                    (const char*    )"task1",
                    (uint16_t       )TASK1_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK1_TASK_PRIO,
                    (TaskHandle_t*  )&Task1Task_Handler);
    vTaskStartScheduler();

	while(1)
	{}
}
