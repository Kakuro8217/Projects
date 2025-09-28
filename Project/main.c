#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./adc/bsp_adc.h"
#include "./beep/bsp_beep.h"


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint32_t ADC_ConvertedValue[3];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal[3]; 

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

typedef enum {
    SYSTEM_NORMAL = 0,     // ϵͳ����
    SYSTEM_VOLTAGE_LOW,    // ��ѹ����
    SYSTEM_VOLTAGE_HIGH,   // ��ѹ����
    SYSTEM_ERROR           // �����쳣
} System_Status;

static System_Status system_status = SYSTEM_NORMAL;


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();
	LED_GPIO_Config();
	BEEP_GPIO_Config();
	Rheostat_Init();	
	
	printf("ϵͳ�������...\r\n");
  printf("��ѹ������Χ: %.1fV - %.1fV\r\n", VOLTAGE_MIN, VOLTAGE_MAX);
	
  while (1)
  {
    Delay(0xffffee);  
    
    ADC_ConvertedValueLocal[0] =(float)((uint16_t)ADC_ConvertedValue[0]*3.3/4096); 
    ADC_ConvertedValueLocal[1] =(float)((uint16_t)ADC_ConvertedValue[1]*3.3/4096);
    ADC_ConvertedValueLocal[2] =(float)((uint16_t)ADC_ConvertedValue[2]*3.3/4096);
    
		// ����LED״̬
    LED_SetStatus(system_status);
		
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[0]); 
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[1]); 
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[2]); 
    
    printf("\r\n The current ADC1 value = %f V \r\n",ADC_ConvertedValueLocal[0]); 
    printf("\r\n The current ADC2 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
    printf("\r\n The current ADC3 value = %f V \r\n",ADC_ConvertedValueLocal[2]);
  
	  // ��ӡϵͳ״̬
        switch(system_status) {
            case SYSTEM_NORMAL:
                printf("ϵͳ״̬: ����\r\n");
                break;
            case SYSTEM_VOLTAGE_LOW:
                printf("ϵͳ״̬: ��ѹ����!\r\n");
                break;
            case SYSTEM_VOLTAGE_HIGH:
                printf("ϵͳ״̬: ��ѹ����!\r\n");
                break;
            case SYSTEM_ERROR:
                printf("ϵͳ״̬: �����쳣!\r\n");
                break;
					}
				printf("==================\r\n");
	}
}
