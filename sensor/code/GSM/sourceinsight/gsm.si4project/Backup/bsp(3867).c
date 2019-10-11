/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 *
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/
#include "bsp.h"
#include "uart1.h"
#include "stm8l15x_dma.h"
#include "GSM.h"

#define ADC_RATIO              ((uint16_t) 733) /*ADC_RATIO = ( 3 * 1000 * 1000)/4095 */

#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DR_ADDRESS       (uint16_t)0x5231  /* USART1 Data register Address */

extern  Uart_Types uart_str;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ADCdata = 0;
void delay_ms(uint32_t num)//不是很精确
{
  u16 i = 0;
  while(num--)
  {
    for (i=0; i<2654; i++);
  }
}

void GPIO_Initial(void)
{
  
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
  GPIO_Init( PORT_LED, PIN_LED, GPIO_Mode_Out_PP_High_Fast );
  GPIO_Init( PORT_KEY, PIN_KEY, GPIO_Mode_Out_PP_High_Fast ); 
  GPIO_Init( PORT_POWER_ON, PIN_POWER_ON, GPIO_Mode_Out_PP_High_Fast );     
  GPIO_Init( PORT_PWRKEY_IN, PIN_PWRKEY_IN, GPIO_Mode_Out_PP_Low_Fast );   
  GPIO_Init( PORT_SENSOR_EN, PIN_SENSOR_EN, GPIO_Mode_Out_PP_Low_Fast ); 

//  GPIO_Init(PORT_KEY,PIN_KEY,GPIO_Mode_In_FL_IT);
//  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling_Low);
  
}


void GSM_HardwareInit(unsigned char flag)
{
   
    if(flag == ON)
    {
		GPIO_SetBits( PORT_POWER_ON, PIN_POWER_ON ); 
		GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
		GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
		GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
		//GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
		GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
		GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );                
		delay_ms(3000);
         
        delay_ms(3000);
//delay_ms(4000);
delay_ms(4000);
delay_ms(4000);
//delay_ms(2000);
//delay_ms(2000);
//delay_ms(2000);
//delay_ms(2000);
	}
//	else
//	{
//		GPIO_ResetBits( PORT_POWER_ON, PIN_POWER_ON ); 
//		delay_ms(2000);
//		GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN ); 
//		delay_ms(2000);
//		delay_ms(2000);
//
//	}

}
void Sensor_HardwareInit(unsigned char flag)
{

 
    if(flag == ON)
    {
		GPIO_SetBits( PORT_SENSOR_EN, PIN_SENSOR_EN );


	}
	else
	{
		GPIO_ResetBits( PORT_SENSOR_EN, PIN_SENSOR_EN );


	}

}

void RTC_Config(uint16_t time,unsigned char flag) 
{
  if(flag == ON)
  {
	  RTC_DeInit(); //初始化默认状态 
	  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
	  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2); //选择RTC时钟源LSI 38K、2=19K
	  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
	  
	  RTC_WakeUpCmd(DISABLE);
	  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
	  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_17bits);
	  RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断
	  RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值
	  RTC_WakeUpCmd(ENABLE);

  }
  else
  {
	  RTC_WakeUpCmd(DISABLE);
	  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
	  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_17bits);
	  RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断
	  RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值


  }

}



extern u8 CurrentMode ;
void EnterStopMode(void) 
{
  disableInterrupts(); 
//  SX1276_LoRa_SetMode( LORA_MODE_SLEEP );//lora enter sleep mode
//  
//  //GPIOA
//  GPIO_Init( PORT_SX127X_TX_CTRL, PIN_SX127X_RX_CTRL|PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow ); //lora tx and rx enbale signals 
//  GPIO_Init( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow );
//  GPIO_Init( GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow );
//  
//  //GPIOB
//  GPIO_Init(GPIOB, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
//  GPIO_Init(GPIOB, PIN_SX127X_M0, GPIO_Mode_Out_PP_Low_Slow);
//  GPIO_Init(GPIOB, PIN_SX127X_M1, GPIO_Mode_Out_PP_Low_Slow);
//  GPIO_SetBits( PORT_SX127X_CSN, PIN_SX127X_CSN ); 
//  
//  //GPIOC
//  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
//  GPIO_Init( PORT_SX127X_DIO3, GPIO_Pin_0|PIN_SX127X_DIO3|PIN_SX127X_DIO4|PIN_SX127X_DIO5, GPIO_Mode_Out_PP_Low_Slow );  
  
  //GPIOD
  GPIO_Init( GPIOD, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow );   
  
  //close clk
  TIM2_DeInit();
  TIM3_DeInit();
  TIM4_DeInit();
  ADC_DeInit(ADC1);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);
  while((TIM2->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,DISABLE);
  
  TIM3_Cmd(DISABLE); 
  while((TIM3->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
  
  TIM4_Cmd(DISABLE); 
  while((TIM4->CR1 & TIM4_CR1_CEN) != 0x00);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,DISABLE);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1,DISABLE);  
  
  PWR_UltraLowPowerCmd(ENABLE); //low power enable
  PWR_FastWakeUpCmd(ENABLE);  //wake up enable
  
  RTC_Config(196,ON);//1:55.2s 
  enableInterrupts();
  halt();  //enter stop mode
}


static void DMA_Config(void)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

  /* Deinitialize DMA channels */
  DMA_GlobalDeInit();

  DMA_DeInit(DMA1_Channel1);
  DMA_DeInit(DMA1_Channel2);

  /* DMA channel Rx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_RX, (uint16_t)uart_str.UsartReceiveData , (uint16_t)USART_DR_ADDRESS,
           buffer_size, DMA_DIR_PeripheralToMemory, DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc, DMA_Priority_Low, DMA_MemoryDataSize_Byte);


  /* Enable the USART Tx/Rx DMA requests */
  USART_DMACmd(USART1, USART_DMAReq_RX, ENABLE);
  /* Global DMA Enable */
  DMA_GlobalCmd(ENABLE);

  /* Enable the USART Tx DMA channel */
// DMA_Cmd(USART_DMA_CHANNEL_TX, ENABLE);
  /* Enable the USART Rx DMA channel */
  //DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);         
}
void DMA_START_RX(void)
{   DMA_ClearFlag(DMA1_FLAG_TC2);
    DMA_ClearFlag(DMA1_FLAG_HT2);
    DMA_Cmd(USART_DMA_CHANNEL_RX, DISABLE);
    DMA_SetCurrDataCounter(USART_DMA_CHANNEL_RX, buffer_size); 
    DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);
}


void HardwareInit()
{
  disableInterrupts();
  SystemClock_Init();     // 系统时钟初始化
  GPIO_Initial(); 
  Uart1_Init(9600);// 初始化GPIO
  DMA_Config();
  LED_Init();             //调试LED初始化
GSM_HardwareInit(ON);
  Sensor_HardwareInit(OFF);
enableInterrupts();
}
void LED_Init(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}
void USART_SendStr(unsigned char *Str) 
{
  while(*Str!=0)
  {
    USART_SendData8(USART1, *Str);
    while(!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
    Str++;
  }
}
void USART_SenByte(unsigned char *Str,unsigned char len) 
{
  while(len>0)
  {
    USART_SendData8(USART1, *Str);
    while(!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
    Str++;
    len--;
  }
}

void adcInit(ADC_Channel_TypeDef num)
{
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
  /* Initialize and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
  
  /* ADC channel used for IDD measurement */
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* Disable SchmittTrigger for ADC_Channel, to save power */
  ADC_SchmittTriggerConfig(ADC1, num, DISABLE);
  
  /* Enable ADC1 Channel used for IDD measurement */
  ADC_ChannelCmd(ADC1, num, ENABLE);
  
  /* Start ADC1 Conversion using Software trigger*/
  ADC_SoftwareStartConv(ADC1);
  
  
}
uint32_t adcGet(ADC_Channel_TypeDef num)
{
  unsigned char i;
  uint32_t tmp;
  /* Waiting until press Joystick Up */
  /* Wait until End-Of-Convertion */
  adcInit(num);
  tmp = 0;
  for(i=0;i<samplecount;i++)
  {
	  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	  {}
	  
	  /* Get conversion value */
	  tmp = ADC_GetConversionValue(ADC1);
	  
	  /* Calculate voltage value in uV over capacitor  C67 for IDD measurement*/
	  tmp = tmp + (uint32_t)((uint32_t)ADCdata * (uint32_t)ADC_RATIO);
	  /* Waiting Delay 200ms */
	  delay_ms(200);
	  
	  /* DeInitialize ADC1 */


  }
  ADCdata = tmp /samplecount;
  ADC_DeInit(ADC1);
  
  /* Disable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  return ADCdata;

}


