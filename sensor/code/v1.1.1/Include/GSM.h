#ifndef GSM_H
#define GSM_H
#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe
#include "stm8l15x.h"
#define SIMCOM_TIME_OUT					60

#define buffer_size  128

typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	UsartReceiveData[buffer_size];

}Uart_Types;


//����ע��״̬


//����ע��״̬


#define	SIMCOM_NET_NOT 						0    	//����δ���
#define	SIMCOM_POWER_ON                                         1
#define	SIMCOM_READY_YES                                        2
#define	SIMCOM_CLOSE_ECHO                                       3
#define	SIMCOM_CARD_DET                                         4
#define	SIMCOM_NET_CLOSE                                        5
#define	SIMCOM_NET_PASS_THROUGH                                 6
#define	SIMCOM_Connect_Platform                                 7
#define	SIMCOM_NET_ERROR					0XFF	//����
#define	SIMCOM_NET_OK						8	//���ӳɹ�
//#define	SIMCOM_READY_YES					1     //�������
//#define	SIMCOM_ATE0   						2    	//�رջ���






//#define	SIMCOM_SIM_OK  						3    	//SIM��ok
//#define	SIMCOM_Network_Intensity_READY		                4  	//�ź�����Ҫ��
//#define	SIMCOM_GPRS_READY  					5    	//gprs ok
//#define	SIMCOM_NET_PORT_CLOSE				        6    	//�ر�tcp����
//#define	SIMCOM_NET_TRANSPARENT				        7    	//͸��ģʽ
//
//#define	SIMCOM_Network_Normal_ModeT         8
//

//#define	SIMCOM_NET_IDLE						14
//
//
//#define	SIMCOM_NetOpen_READY				11     //net open
//
//#define	SIMCOM_NET_OK2     13

unsigned char Get_Network_status(void);
void SIMCOM_Register_Network(void);
void test(void);
void Set_Network_status(void);
#endif