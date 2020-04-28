#include "machine.h"
extern UART_HandleTypeDef huart1;
//�������� Arduino ��ʾ���������� MCU ����ֲ��
void Receive_command()
{
    unsigned char inByte,temp;
    if(Serial.available() > 0)
    {
        inByte = Serial.read();
        if(inByte==0xAA)
        {
            Receive_item=0;
        }
        Receive_data_temp[Receive_item]=inByte;
        Receive_item++;
        if(Receive_item>15)
        {
            Receive_item=0;
        }
        if(inByte==0x55)
        {
            temp=0; //�Խ��ܵ����ݽ����ۼ�У��
            for(int i=0; i<14; i++)
            {
                temp += Receive_data_temp[i];
            }
            if(temp==Receive_data_temp[14])
            {
                for(int j=0; j<16; j++)
                {
                    Receive_data[j]=Receive_data_temp[j];
                } //������ܵ���������ȷ�� copy �� Receive_data ������
            }
            Uart.Receive_flag=1;
        }
    }
}

void machine_speed(uint32_t pspeed)
{

}