
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : basic.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : basic program body
* Description    : --
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "basic.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/

	
	
/* Private functions  ==  ˽�к������� ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void BASIC_Memcpy(u8 *des, u8 *src, u32 len)
{
	u32 i;
	for(i=0; i<len; i++)
	{
		*(des + i) = *(src + i);
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/