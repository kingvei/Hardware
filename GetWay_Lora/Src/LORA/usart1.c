#include "USART1.h"
#include "systick_timer.h"
#include "stdio.h"	 	 
#include "string.h"	  
#include "lora_cfg.h"
#include "lora_app.h"
#include "stm32f1xx_hal.h"
#include "stdarg.h"	
extern UART_HandleTypeDef huart1;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F429������
//����3��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2016/6/17
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 

//���ڷ��ͻ����� 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	      //���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
#ifdef USART1_RX_EN   								      //���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				      //���ջ���,���USART1_MAX_RECV_LEN���ֽ�.

UART_HandleTypeDef UART3_Handler;                         //UART���

//��ʼ��IO,����3
//bound:������
void usart1_init(u32 bound)
{	
	//GPIO�˿�����
  huart1.Instance = USART1;
  huart1.Init.BaudRate = bound;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
#if USART1_RX_EN 
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);    //���������ж�
	HAL_NVIC_EnableIRQ(USART1_IRQn);				      //ʹ��USART1�ж�ͨ��
	HAL_NVIC_SetPriority(USART1_IRQn,2,3);			      //��ռ���ȼ�2�������ȼ�3
#endif	

	USART1_RX_STA=0;				                      //���� 
	
  
}

//timer=10ms
//ͨ���жϽ�������2���ַ�֮���ʱ������timer�������ǲ���һ������������.
//���2���ַ����ռ������timer,����Ϊ����1����������.Ҳ���ǳ���timerû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART1_RX_STA=0;  

//����3�жϷ������
void USART1_IRQHandler(void)
{
	  u8 Res;
	  if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  HAL_UART_Receive(&UART3_Handler,&Res,1,1000);
		  if((USART1_RX_STA&0x8000)==0)                //�������һ������,��û�б�����,���ٽ�����������
		  { 
			 if(USART1_RX_STA<USART1_MAX_RECV_LEN)     //�����Խ�������
			 {
				 
				 if(!Lora_mode)//����ģʽ��(������ʱ����ʱ)
				 {
					  TIM7->CNT = 0;                       //���������
					  if(USART1_RX_STA==0)                 //���û�н����κ�����
					  {
						;// HAL_TIM_Base_Start(&TIM7_Handler);//������ʱ��7 
					  }
					 
				 }
				 USART1_RX_BUF[USART1_RX_STA++]=Res;  //��¼���յ���ֵ
				  
			 }
			 else
			 {
				  USART1_RX_STA|=1<<15;			       //ǿ�Ʊ�ǽ������
			 }
		  }
	  }
//	  HAL_UART_IRQHandler(&UART3_Handler);	
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART1_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)                     //ѭ����������
	{
		while((USART1->SR&0X40)==0);     //ѭ������,ֱ���������   
		USART1->DR=USART1_TX_BUF[j];  
	}
}

//����3�����ʺ�У��λ����
//bps:�����ʣ�1200~115200��
//parity:У��λ���ޡ�ż���棩
void usart1_set(u8 bps,u8 parity)
{

    static u32 bound=0;
	
	switch(bps)
	{
		case LORA_TTLBPS_1200:   bound=1200;     break;
		case LORA_TTLBPS_2400:   bound=2400;     break;
		case LORA_TTLBPS_4800:   bound=4800;     break;
		case LORA_TTLBPS_9600:   bound=9600;     break;
		case LORA_TTLBPS_19200:  bound=19200;    break;
		case LORA_TTLBPS_38400:  bound=38400;    break;
		case LORA_TTLBPS_57600:  bound=57600;    break;
		case LORA_TTLBPS_115200: bound=115200;   break;
	}
    
   
    __HAL_UART_DISABLE(&UART3_Handler);//�رմ���
	
	UART3_Handler.Init.BaudRate = bound;
	UART3_Handler.Init.StopBits = UART_STOPBITS_1; 
	
	if(parity==LORA_TTLPAR_8N1)//��У��
	{
		UART3_Handler.Init.WordLength= UART_WORDLENGTH_8B;    
		UART3_Handler.Init.Parity = UART_PARITY_NONE;
	}else if(parity==LORA_TTLPAR_8E1)//żУ��
	{
		UART3_Handler.Init.WordLength= UART_WORDLENGTH_9B;    
		UART3_Handler.Init.Parity = UART_PARITY_EVEN;
	}else if(parity==LORA_TTLPAR_8O1)//��У��
	{
		UART3_Handler.Init.WordLength = UART_WORDLENGTH_9B;    
		UART3_Handler.Init.Parity = UART_PARITY_ODD;
	}
    HAL_UART_Init(&UART3_Handler);	
	
} 

//���ڽ���ʹ�ܿ���
//enable:0,�ر� 1,��
void usart1_rx(u8 enable)
{
	
	 __HAL_UART_DISABLE(&UART3_Handler); //ʧ�ܴ��� 
	
	 if(enable)
	 {
		 UART3_Handler.Init.Mode=UART_MODE_TX_RX;//�շ�ģʽ
	 }else
	 {
		  UART3_Handler.Init.Mode = UART_MODE_TX;//ֻ����  
	 }
	 
	 HAL_UART_Init(&UART3_Handler); //HAL_UART_Init()��ʹ��UART3
	
}

#endif












