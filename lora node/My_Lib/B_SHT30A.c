#include "B_SHT30A.h"


vid SHT30_DelayMs(uint8 k)
{
  do
  {
    SHT30_Delay1us(1000);
  }while(k--);
}

vid SHT30_Delay1us(uint16 vTemp)
{
  do
  {
    asm("nop"); //һ��asm("nop")��������ʾ�������Դ���100ns
    asm("nop");
    asm("nop");
    asm("nop");
   asm("nop");
   asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
   asm("nop");
  }while(vTemp--);
}

/*-------------------------------------------------------
����������������ʼ��
��    ������
�� �� ֵ����
---------------------------------------------------------*/
vid SHT30_Init(vid)
{
  SHT30_SCL0_O;                          	//����SCLKΪ���
  SHT30_SDA0_O;                          	//����SDAΪ���
  
  SHT30_SCL0_H;
  SHT30_SCL0_L;
  
  SHT30_SDA0_H;
  SHT30_SDA0_L;
  
  SHT30_Soft_Reset();
  
  
  SHT30_DelayMs(1);
  
  SHT30_ClearStaus();
  
  SHT30_DelayMs(1);
  
  
}


uint16 SHT30_ReadSerialNumber()
{
  uint8 	vRval = 0x00;
  uint16 	vVal = 0x00;
  uint8 	vCrc = 0x00;
  
  uint8 i = 0;
  uint8 vBuf[3];
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);								//��ַд
  vRval |= SHT30_SendByte( 0x37 );			//��ȡ״̬�Ĵ���
  vRval |= SHT30_SendByte( 0x80 );
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);								//��ַ��
  
  for(i=0; i<3; i++)
  {
    vBuf[i] = SHT30_RecvByte();                				//�洢����
    if (i == 2)
    {
      
      SHT30_SendACK(1);                         			//���һ��������Ҫ��NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         			//��ӦACK
    }
  }
  
  SHT30_Stop();
  
  if (vRval>0)
  {
    return 0xFFFF;
  }
  
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal |= vBuf[0];
    vVal <<= 8;
    vVal |= vBuf[1];
  }
  
  return vVal;
}


/*---------------------------------------------------------------------
��������: ��ʼ�ź�
����˵��: ��	
��������: ��
---------------------------------------------------------------------*/
vid SHT30_Start()
{
  SHT30_SDA0_H;                         //����������
  SHT30_Delay1us(1);
  SHT30_SCL0_H;                         //����ʱ����
  SHT30_Delay1us(1);                     //��ʱ
  SHT30_SDA0_L;                         //�����½���
  SHT30_Delay1us(10);                     //��ʱ
  SHT30_SCL0_L;                         //����ʱ����
  SHT30_Delay1us(10);
}

/*---------------------------------------------------------------------
��������: ֹͣ�ź�
����˵��: ��	
��������: ��
---------------------------------------------------------------------*/
vid SHT30_Stop()
{
  SHT30_SCL0_L;                         //����ʱ����
  SHT30_Delay1us(1);
  SHT30_SDA0_L;                         //����������
  SHT30_Delay1us(1);                     //��ʱ
  SHT30_SCL0_H;
  SHT30_Delay1us(10);                     //��ʱ
  SHT30_SDA0_H;                         //����������
  SHT30_Delay1us(10);
}

/*---------------------------------------------------------------------
��������: ����Ӧ���ź�
����˵��: ack [in] - Ӧ���ź�(0:ACK 1:NAK)
��������: ��
---------------------------------------------------------------------*/
vid SHT30_SendACK(uint8 ack)
{
  if (ack&0x01)	SHT30_SDA0_H;		//дӦ���ź�
  else	SHT30_SDA0_L;
  
  SHT30_Delay1us(1);
  SHT30_SCL0_H;                         //����ʱ����
  SHT30_Delay1us(5);                     //��ʱ
  SHT30_SCL0_L;                         //����ʱ����
  SHT30_SDA0_H;
  SHT30_Delay1us(20);                     //��ʱ
}

/*---------------------------------------------------------------------
��������: ����Ӧ���ź�
����˵��: ��
��������: ����Ӧ���ź�
---------------------------------------------------------------------*/
uint8 SHT30_RecvACK()
{
  uint8 CY = 0x00;
  uint16 vConter = 1000;
  SHT30_SDA0_H;
  
  SHT30_SDA0_I;
  vConter = 1000;
  SHT30_SCL0_H;                         //����ʱ����
  SHT30_Delay1us(1);                     //��ʱ
  while (vConter)
  {
    vConter--;
    CY |= SHT30_SDA0_DAT;                 //��Ӧ���ź�
    
    if(!CY)	break;
  }
  SHT30_Delay1us(20);                     //��ʱ
  SHT30_SCL0_L;
  SHT30_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
��������: ��IIC���߷���һ���ֽ�����
����˵��: dat [in] - д�ֽ�
��������: 0 - �ɹ�   1- ����
---------------------------------------------------------------------*/
uint8 SHT30_SendByte(uint8 dat)
{
  uint8 vRval = 0x01;
  uint8 i;
  
  for (i=0; i<8; i++)         			//8λ������
  {
    if (dat&0x80)	SHT30_SDA0_H;
    else	SHT30_SDA0_L;                   //�����ݿ�
    
    SHT30_Delay1us(1);             		//��ʱ
    SHT30_SCL0_H;                		//����ʱ����
    SHT30_Delay1us(5);             		//��ʱ
    SHT30_SCL0_L;                		//����ʱ����
    SHT30_Delay1us(1);             		//��ʱ
    dat <<= 1;              			//�Ƴ����ݵ����λ
  }
  
  vRval = SHT30_RecvACK();
  
  return vRval;
}

/*---------------------------------------------------------------------
��������: ��IIC���߽���һ���ֽ�����
����˵��: ��
��������: �����ֽ�
---------------------------------------------------------------------*/
uint8 SHT30_RecvByte()
{
  uint8 i;
  uint8 dat = 0;
  uint16 vConter = 100;
  
  SHT30_SDA0_H;                         //ʹ���ڲ�����,׼����ȡ����,
  
  SHT30_SDA0_I;
  for (i=0; i<8; i++)         	        //8λ������
  {
    SHT30_SCL0_H;                       //����ʱ����
    SHT30_Delay1us(1);             			//��ʱ
    vConter = 100;
    SHT30_SCL0_I;
    while (SHT30_SCL0_DAT==0)
    {
      vConter--;
      if (vConter == 0) break;
    }
    SHT30_SCL0_O;
    SHT30_Delay1us(3);
    
    dat |= SHT30_SDA0_DAT;              //������             
    SHT30_SCL0_L;                       //����ʱ����
    SHT30_Delay1us(1);             			//��ʱ
    
    if (i<7) dat <<= 1;	
  }
  SHT30_SDA0_O;
  
  return dat;
}


/*---------------------------------------------------------------------
��������: SHT30���β���
����˵��: vBuf [out] - ������ȡ���
��������: 0 - �ɹ�  ����1����
---------------------------------------------------------------------*/
uint8 SHT30_Single_Measure(uint8 *vBuf)
{
  uint8 vRval = 0;
  uint8 i = 0;
  
  SHT30_Start();
  
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SINGLE_H_MEASURE_EN>>8)&0xFF );				//ʹ�ܸ߾��Ȳɼ�
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SINGLE_H_MEASURE_EN)&0xFF );
  SHT30_Stop();
  
  if (vRval)	return vRval;
  
  SHT30_SCL0_H;
  SHT30_DelayMs(15);														//15Ms
  
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);					//��ַ��
  
  if (vRval)	return vRval;
  
  for(i=0; i<6; i++)
  {
    vBuf[i] = SHT30_RecvByte();                	//�洢����
    if (i == 5)
    {
      
      SHT30_SendACK(1);                         //���һ��������Ҫ��NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         //��ӦACK
    }
  }
  
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
��������: SHT30�������ڲ���
����˵��: vBuf [in] - ��������
��������: 0 - �ɹ�  ����1����
---------------------------------------------------------------------*/
uint8 SHT30_Periodic_Measure(uint16 vCmd)
{
  uint8 vRval = 0;
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (vCmd>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (vCmd)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
��������: SHT30���ڲ��� ��ȡ����
����˵��: vBuf [out] - ��������
��������: 0 - �ɹ�  ����1����
---------------------------------------------------------------------*/
uint8 SHT30_Periodic_Measure_Read(uint8 *vBuf)
{
  uint8 vRval = 0;
  
  uint8 i = 0;
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_PERIODOC_MEASURE_READ>>8)&0xFF );				//��ȡ���ڲɼ�����
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_PERIODOC_MEASURE_READ)&0xFF );
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);					//��ַ��
  
  if (vRval)	return vRval;
  
  for(i=0; i<6; i++)
  {
    vBuf[i] = SHT30_RecvByte();                	//�洢����
    if (i == 5)
    {
      
      SHT30_SendACK(1);                         //���һ��������Ҫ��NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         //��ӦACK
    }
  }
  
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
��������: SHT30����������λ (����ģʽ�¹ر���������)
����˵��: ��
��������: 0 - �ɹ�  ����1����
---------------------------------------------------------------------*/
uint8 SHT30_Soft_Reset()
{
  uint8 vRval = 0;
  
  SHT30_Start();
  
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SOFT_RESET>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SOFT_RESET)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
��������: SHT30��������
����˵��: vCmd [in] - 0x306D�򿪼���  0x3066�رռ���
��������: 0 - �ɹ�  ����1����
---------------------------------------------------------------------*/
uint8 SHT30_Heater(uint16 vCmd)
{
  uint8 vRval = 0;
  
  SHT30_Start();
  
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (vCmd>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (vCmd)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
��������: SHT30��ȡ״̬�Ĵ���
����˵��: ��
��������: ����״̬�Ĵ���
---------------------------------------------------------------------*/
uint16 SHT30_ReadStaus()
{
  uint8 	vRval = 0x00;
  uint16 	vVal = 0x00;
  uint8 	vCrc = 0x00;
  
  uint8 i = 0;
  uint8 vBuf[3];
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);								//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_READ_STATUS>>8)&0xFF );				//��ȡ״̬�Ĵ���
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_READ_STATUS)&0xFF );
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);								//��ַ��
  
  if (vRval>0)
  {
    return 0xFFFF;
  }
  
  for(i=0; i<3; i++)
  {
    vBuf[i] = SHT30_RecvByte();                				//�洢����
    if (i == 2)
    {
      
      SHT30_SendACK(1);                         			//���һ��������Ҫ��NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         			//��ӦACK
    }
  }
  
  SHT30_Stop();
  
  if (vRval>0)
  {
    return 0xFFFF;
  }
  
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal |= vBuf[0];
    vVal <<= 8;
    vVal |= vBuf[1];
  }
  
  return vVal;
}

/*---------------------------------------------------------------------
��������: SHT30���״̬�Ĵ���
����˵��: ��
��������: 0 - �ɹ�  ����1����
---------------------------------------------------------------------*/
uint8 SHT30_ClearStaus()
{
  uint8 vRval = 0;
  SHT30_Start();
  
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//��ַд
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_CLEAR_STATUS>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_CLEAR_STATUS)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

uint8 SHT30_CheckCrc8(uint8 *vDat, uint8 vLen)  
{  
  uint8 vBit;        // bit mask
  uint8 vCrc = 0xFF; // calculated checksum
  uint8 vByteCtr;    // byte counter
  
  // calculates 8-Bit checksum with given polynomial
  for(vByteCtr = 0; vByteCtr < vLen; vByteCtr++)
  {
    vCrc ^= (vDat[vByteCtr]);
    for(vBit = 8; vBit > 0; --vBit)
    {
      if(vCrc & 0x80) vCrc = (vCrc << 1) ^ POLYNOMIAL;
      else           vCrc = (vCrc << 1);
    }
  }
  
  return vCrc;
}

/*---------------------------------------------------------------------
��������: SHT30 �����������
����˵��:  vBuf [in] - ������ȡ���
vTemSymbol [out] - �����¶ȷ���  0-���¶�  1-���¶�
vTem [out] - �¶�
vHum [out] - ʪ��

��������: ��
---------------------------------------------------------------------*/
void SHT30_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{
  uint16 	vVal = 0x00;
  uint8 	vCrc = 0x00;
  float		vTemp = 0.00;
  
  //�¶�
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal = vBuf[0];
    vVal<<=8;
    vVal |= vBuf[1];
    
    vTemp = 175.0*vVal/(65536.0-1.0);
    
    if (vTemp >= 45)
    {
      *vTemSymbol = 1;
      *vTem = (vTemp - 45.0)*10.0;
    }
    else
    {
      *vTemSymbol = 0;
      *vTem = (45.0 - vTemp)*10.0;
    }
    
  }
  
  vBuf += 3;
  vVal = 0x00;
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal = vBuf[0];
    vVal<<=8;
    vVal |= vBuf[1];
    
    vTemp = 100.0*vVal/(65536.0-1.0);
    *vHum = (vTemp*10);
  }
}

/*---------------------------------------------------------------------
��������: SHT30 �����������
����˵��:  vTemSymbol [out] - �����¶ȷ��� 0-���¶�  1-���¶�
vTem [out] - �¶�
vHum [out] - ʪ��

��������: ��
---------------------------------------------------------------------*/
void SHT30_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{
  uint8 vDat[8];
  uint8 vRval = 0;
  
  vRval = SHT30_Single_Measure(vDat);
  if (!vRval) SHT30_calc(vDat, vTemSymbol, vTem, vHum);
  
}




