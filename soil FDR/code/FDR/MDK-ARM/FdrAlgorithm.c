#include "FdrAlgorithm.h"
#include "EEPROM.h"
#include "adc.h"
factor_stru factor_usr;
float SoilHumid(unsigned char status,float AdcValueVol)
{
	static unsigned char index;
	unsigned int result;
	float tmp3;
	static float last_temp;
	if(status == CALIBRATION)
	{
		factor_usr.humid=factor_usr.a0*AdcValueVol*AdcValueVol+factor_usr.a2;
		factor_usr.humid=factor_usr.humid+factor_usr.a1*AdcValueVol;	
	}
	else
	{
		factor_usr.humid=factor_usr.a0*AdcValueVol*AdcValueVol+factor_usr.a2;
		factor_usr.humid=factor_usr.humid+factor_usr.a1*AdcValueVol;	
	}
	if(AdcValueVol<1.515)
	{
		factor_usr.humid = 0;
	   last_temp = 0;
  }
	last_temp = last_temp +factor_usr.humid;
	index++;

	/************�˲�ȥ��������ֹ���ݹ���Ƶ������****************/
	if(index<48)
	{
		tmp3 = last_temp/index;
	}
	else
	{
		tmp3 = last_temp/index;
    index = 0;	
		last_temp = 0;		
	}
   
	if(tmp3<0)
	{
		result = (unsigned int)(tmp3);
		result = ~result+1;
	}
	else 
		result = (unsigned int)(tmp3*10);
	factor_usr.humid = result;
	return (unsigned int) (factor_usr.humid);
}

/*
	Rt=R0��1+A*t+B*t^2��0-850   A=0.00394 B -5.802*10-7 c=-4.274*10-12
	Rt=R0[1+A*t+Bt^2+C(t-100)^3] -200-0
*/
float SoilTemperature(unsigned char status,float AdcValueVol1,float AdcValueVol2)
{
	float resistor;
	float A = 0.0012;
	float B = 2.3162;
	float C = -243.61;	
	float tmp1,tmp2,tmp3;
	static float last_temp;
	static unsigned char index;
	unsigned int result;
	//AdcValueVol1 = ((uint32_t)(AdcValueVol1*1000))/1000.0;
//	AdcValueVol2 = ((uint32_t)(AdcValueVol2*1000))/1000.0;
	resistor = (AdcValueVol1*499/AdcValueVol2)-499;
	resistor = ((uint32_t)(resistor*10000));
	resistor=resistor/10000.0;

	
	
	tmp1 = A*resistor*resistor;
	tmp2 = B*resistor;
	tmp3 = tmp1 + tmp2;
  tmp3 = tmp3 + C;
	tmp3 = tmp3*10;
	last_temp = last_temp +tmp3;
	index++;
	/************�˲�ȥ��������ֹ���ݹ���Ƶ������****************/
	if(index<48)
	{
		tmp3 = last_temp/index;
	}
	else
	{
		tmp3 = last_temp/index;
    index = 0;	
		last_temp = 0;		
	}
   tmp3 = tmp3 -30;
	if(tmp3<0)
	{
		result = (unsigned int)(tmp3);
		result = ~result+1;
	}
	else 
		result = (unsigned int)(tmp3);
	
	
	return result;
}