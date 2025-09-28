/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   
#include "./beep/bsp_beep.h"

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
		
		 // ��ʼ״̬���̵���������𣨱�ʾ����������
    GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN);   // �̵���
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN);     // �����

}

typedef enum {
    SYSTEM_NORMAL = 0,     // ϵͳ����
    SYSTEM_VOLTAGE_LOW,    // ��ѹ����
    SYSTEM_VOLTAGE_HIGH,   // ��ѹ����
    SYSTEM_ERROR           // �����쳣
} System_Status;

static System_Status system_status = SYSTEM_NORMAL;


void LED_SetStatus(System_Status status)
{
    switch(status) {
        case SYSTEM_NORMAL:
            // �̵����������
            GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN);
            GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN);
            break;
            
        case SYSTEM_VOLTAGE_LOW:
        case SYSTEM_VOLTAGE_HIGH:
            // �̵��𣬺�Ƴ�������ѹ�쳣��
            GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN);
            GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN);
            break;
            
        case SYSTEM_ERROR:
            // �̵��𣬺����˸�������쳣��
            GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN);
            GPIO_ToggleBits(LED1_GPIO_PORT, LED1_PIN);  // �л����״̬
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
