#include "oled2.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

u8 OLED_GRAM[136][4];

//���Ժ���
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
		}
}

//��Ļ��ת180��
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
}

//��ʱ
void IIC_delay(void)
{
	u8 t=3;
	while(t--);
}

//��ʼ�ź�
void I2C_Start(void)
{
	OLED_SDIN_Set();
	OLED_SCLK_Set();
	IIC_delay();
	OLED_SDIN_Clr();
	IIC_delay();
	OLED_SCLK_Clr();
	IIC_delay();
}

//�����ź�
void I2C_Stop(void)
{
	OLED_SDIN_Clr();
	OLED_SCLK_Set();
	IIC_delay();
	OLED_SDIN_Set();
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
	u8 t;
	OLED_SDIN_Set();
	IIC_delay();
	OLED_SCLK_Set();
	IIC_delay();
	while(IIC_READ_SDA)
	{
		t++;
		if(t>250)
		{
			I2C_Stop();
		}
	}
	OLED_SCLK_Clr();
	IIC_delay();
}

//д��һ���ֽ�
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		if(dat&0x80)//��dat��8λ�����λ����д��
		{
			OLED_SDIN_Set();
    }
		else
		{
			OLED_SDIN_Clr();
    }
		IIC_delay();
		OLED_SCLK_Set();
		IIC_delay();
		OLED_SCLK_Clr();
		dat<<=1;
  }
}

//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


//����OLED��ʾ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

//�ر�OLED��ʾ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//�ر���Ļ
}

//�����Դ浽OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<4;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
	   OLED_WR_Byte(0x08,OLED_CMD);   //���õ�����ʼ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
	   for(n=0;n<120;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
//��������
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<4;i++)
	{
	   for(n=0;n<120;n++)
			{
			 OLED_GRAM[n][i]=0;//�����������
			}
  }
	OLED_Refresh();//������ʾ
}

//���� 
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}

//���һ����
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

//����
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2;
	if(x1==x2)    //������
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i);
			}
  }
	else if(y1==y2)   //������
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1);
			}
  }
	else      //��б��
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
			{
			  OLED_DrawPoint(x1+i,y1+i*k/10);
			}
	}
}
//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
		else return;
				for(m=0;m<8;m++)           //д������
				{
					if(temp&0x80)OLED_DrawPoint(x,y);
					else OLED_ClearPoint(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //����
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

////��ʾ2������
////x,y :�������	 
////len :���ֵ�λ��
////size:�����С
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
			}
  }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk1[chr1][i];}//����16*16����
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//����24*24����
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//����32*32����
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//����64*64����
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(120,6,t,16); //д��һ�����ֱ�����OLED_GRAM[][]������
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //��ʾ���
				 {
					for(i=0;i<136;i++)
						{
							for(n=0;n<4;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<136;i++)   //ʵ������
		{
			for(n=0;n<4;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//����д�����ݵ���ʼλ��
void OLED_WR_BP(u8 x,u8 y)
{
	x+=8;
	OLED_WR_Byte(0xb0+y,OLED_CMD);//��������ʼ��ַ
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
}

//x0,y0���������
//x1,y1���յ�����
//BMP[]��Ҫд���ͼƬ����
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	u32 j=0;
	u8 x=0,y=0;
	for(y=y0;y<y1;y++)
	 {
		 OLED_WR_BP(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 OLED_WR_Byte(BMP[j],OLED_DATA);
			 j++;
     }
	 }
}
//OLED�ĳ�ʼ��
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);
	
	delay_ms(200);
	
	OLED_WR_Byte(0xAE,OLED_CMD);    /*display off*/

	OLED_WR_Byte(0x08,OLED_CMD);    /*set lower column address*/       
	OLED_WR_Byte(0x10,OLED_CMD);    /*set higher column address*/

	OLED_WR_Byte(0xA8,OLED_CMD);    /*multiplex ratio*/
	OLED_WR_Byte(0x1b,OLED_CMD);    /*duty = 1/28*/

	OLED_WR_Byte(0xad,OLED_CMD);    /*External or Internal IREF Selection */
	OLED_WR_Byte(0x40,OLED_CMD);

	OLED_WR_Byte(0xD3,OLED_CMD);    /*set display offset*/
	OLED_WR_Byte(0x12,OLED_CMD);

	OLED_WR_Byte(0xA0,OLED_CMD);    /*set segment remap*/

	OLED_WR_Byte(0xC8,OLED_CMD);    /*Com scan direction*/

	OLED_WR_Byte(0xA6,OLED_CMD);    /*normal / reverse*/

	OLED_WR_Byte(0x40,OLED_CMD);    /*set display start line*/

	OLED_WR_Byte(0xA4,OLED_CMD);  

	OLED_WR_Byte(0x81,OLED_CMD);    /*contract control*/
	OLED_WR_Byte(0x30,OLED_CMD);    /*  30  */  

	OLED_WR_Byte(0xD9,OLED_CMD);    /*set pre-charge period*/
	OLED_WR_Byte(0x22,OLED_CMD);

	OLED_WR_Byte(0xD5,OLED_CMD);    /*set osc division*/
	OLED_WR_Byte(0x41,OLED_CMD); 

	OLED_WR_Byte(0xDA,OLED_CMD);    /*set COM pins*/
	OLED_WR_Byte(0x10,OLED_CMD);

	OLED_WR_Byte(0x20,OLED_CMD);   /*Set Memory Addressing Mode */
	OLED_WR_Byte(0x02,OLED_CMD);   /*Page Addressing Mode*/

	OLED_WR_Byte(0xdb,OLED_CMD);    /*set vcomh*/
	OLED_WR_Byte(0x30,OLED_CMD);

	OLED_WR_Byte(0x8d,OLED_CMD);    /*set charge pump enable*/
	OLED_WR_Byte(0x52,OLED_CMD);     /*  52-8.0V  12-7.5V */

	OLED_WR_Byte(0xAF,OLED_CMD);    /*display ON*/    
	OLED_Clear();
}
