#include"Delay.h"


/*---------------------------------------------------------------------
 功能描述: 微秒延时
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
vid delay_us(vid)
{ 
  asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
  asm("nop");
  asm("nop");
  asm("nop"); 
   asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
  asm("nop");
  asm("nop");
  asm("nop");  
   asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
  asm("nop");
  asm("nop");
  asm("nop");  
  
  
}


/*---------------------------------------------------------------------
 功能描述: 毫秒延时
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
vid delay_ms(uint16 time)
{
  uint16 i;
  while(time--)
  {
    for(i=900;i>0;i--)
    {
      delay_us();
    }
  }
}
