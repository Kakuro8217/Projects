#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./adc/bsp_adc.h"
#include "./beep/bsp_beep.h"


// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint32_t ADC_ConvertedValue[3];

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[3]; 

// 电压正常范围定义 - 根据你的需求调整
#define VOLTAGE_MIN    1.0f    // 最低正常电压
#define VOLTAGE_MAX    2.5f    // 最高正常电压

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

typedef enum {
    SYSTEM_NORMAL = 0,     // 系统正常
    SYSTEM_VOLTAGE_LOW,    // 电压过低
    SYSTEM_VOLTAGE_HIGH,   // 电压过高
} System_Status;

static void LED_SetStatus(System_Status status)
{
    switch(status) {
        case SYSTEM_NORMAL:
            // 绿灯亮，红灯灭
            LED2(ON);
            LED1(OFF);
			BEEP(BEEP_OFF);
            break;
            
        case SYSTEM_VOLTAGE_LOW:
        case SYSTEM_VOLTAGE_HIGH:
            // 绿灯灭，红灯常亮（电压异常）
            LED2(OFF);
            LED1(ON);
			BEEP(BEEP_ON);
            break;
            
			}
}

static System_Status system_status = SYSTEM_NORMAL;

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
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  Debug_USART_Config();
	LED_GPIO_Config();
	BEEP_GPIO_Config();
	Rheostat_Init();	
	
	printf("系统启动完成...\r\n");
  printf("电压正常范围: %.1fV - %.1fV\r\n", VOLTAGE_MIN, VOLTAGE_MAX);
	
  while (1)
  {
    Delay(0xfffff);  
    
    ADC_ConvertedValueLocal[0] =(float)((uint16_t)ADC_ConvertedValue[0]*3.3/4096); 
    ADC_ConvertedValueLocal[1] =(float)((uint16_t)ADC_ConvertedValue[1]*3.3/4096);
    ADC_ConvertedValueLocal[2] =(float)((uint16_t)ADC_ConvertedValue[2]*3.3/4096);
    
		// 分别检查三个通道
    System_Status status_ch0 = Voltage_Check(ADC_ConvertedValueLocal[0]);
    System_Status status_ch1 = Voltage_Check(ADC_ConvertedValueLocal[1]);
    System_Status status_ch2 = Voltage_Check(ADC_ConvertedValueLocal[2]);
    
    // 综合判断：任一通道异常即报警
    if (status_ch0 == SYSTEM_VOLTAGE_LOW || status_ch1 == SYSTEM_VOLTAGE_LOW || status_ch2 == SYSTEM_VOLTAGE_LOW) {
        system_status = SYSTEM_VOLTAGE_LOW;
    } else if (status_ch0 == SYSTEM_VOLTAGE_HIGH || status_ch1 == SYSTEM_VOLTAGE_HIGH || status_ch2 == SYSTEM_VOLTAGE_HIGH) {
        system_status = SYSTEM_VOLTAGE_HIGH;
    } else {
        system_status = SYSTEM_NORMAL;
    }
		
		// 设置LED状态
    LED_SetStatus(system_status);
		
    // 每10次循环打印一次，减少串口输出频率
    static int count = 0;
    if(count++ >= 10) {
        count = 0;
        printf("ADC1: %f V, 状态: ", ADC_ConvertedValueLocal[0]);
        
        switch(system_status) {
            case SYSTEM_NORMAL: printf("正常\r\n"); break;
            case SYSTEM_VOLTAGE_LOW: printf("电压过低!\r\n"); break;
            case SYSTEM_VOLTAGE_HIGH: printf("电压过高!\r\n"); break;
				}
				
				printf("ADC2: %f V, 状态: ", ADC_ConvertedValueLocal[1]);
        
        switch(system_status) {
            case SYSTEM_NORMAL: printf("正常\r\n"); break;
            case SYSTEM_VOLTAGE_LOW: printf("电压过低!\r\n"); break;
            case SYSTEM_VOLTAGE_HIGH: printf("电压过高!\r\n"); break;
        }
				
				printf("ADC3: %f V, 状态: ", ADC_ConvertedValueLocal[2]);
        
        switch(system_status) {
            case SYSTEM_NORMAL: printf("正常\r\n"); break;
            case SYSTEM_VOLTAGE_LOW: printf("电压过低!\r\n"); break;
            case SYSTEM_VOLTAGE_HIGH: printf("电压过高!\r\n"); break;
				}
				printf("==================\r\n");
    }
				
	}
}
