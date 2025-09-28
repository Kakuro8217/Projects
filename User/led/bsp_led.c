/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   
#include "./beep/bsp_beep.h"

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*设置引脚速率为2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
		
		 // 初始状态：绿灯亮，红灯灭（表示程序启动）
    GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN);   // 绿灯亮
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN);     // 红灯灭

}

typedef enum {
    SYSTEM_NORMAL = 0,     // 系统正常
    SYSTEM_VOLTAGE_LOW,    // 电压过低
    SYSTEM_VOLTAGE_HIGH,   // 电压过高
    SYSTEM_ERROR           // 程序异常
} System_Status;

static System_Status system_status = SYSTEM_NORMAL;


void LED_SetStatus(System_Status status)
{
    switch(status) {
        case SYSTEM_NORMAL:
            // 绿灯亮，红灯灭
            GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN);
            GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN);
            break;
            
        case SYSTEM_VOLTAGE_LOW:
        case SYSTEM_VOLTAGE_HIGH:
            // 绿灯灭，红灯常亮（电压异常）
            GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN);
            GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN);
            break;
            
        case SYSTEM_ERROR:
            // 绿灯灭，红灯闪烁（程序异常）
            GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN);
            GPIO_ToggleBits(LED1_GPIO_PORT, LED1_PIN);  // 切换红灯状态
						BEEP(ON);
            break;
    }
}

static System_Status Voltage_Check(float voltage)
{
    if (voltage < VOLTAGE_MIN) {
        return SYSTEM_VOLTAGE_LOW;
    } else if (voltage > VOLTAGE_MAX) {
        return SYSTEM_VOLTAGE_HIGH;
    } else {
        return SYSTEM_NORMAL;
    }
}
/*********************************************END OF FILE**********************/
