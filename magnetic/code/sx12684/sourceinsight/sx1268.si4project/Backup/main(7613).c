#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
//
#include "math.h"
#include "sx126x-board.h"
#include "gpio.h"
#include "delay.h"
#include "spi.h"
#include "radio.h"
#include "sx126x.h"
#include "sx126x-board.h"
#include "crc.h"
#include "lora_params.h"


extern bool IrqFired;
static RadioEvents_t RadioEvents;
extern const struct Radio_s Radio;
States_t State = LOWPOWER;

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";
uint8_t TX_Buffer[BUFFERSIZE];
uint8_t RX_Buffer[BUFFERSIZE];
int8_t RssiValue = 0;
int8_t SnrValue = 0;
uint16_t  crc_value;
uint16_t BufferSize = BUFFERSIZE;


void OnTxDone( void )
{  
  //Lora_Process();
  Radio.Standby();
  Radio.Rx( RX_TIMEOUT_VALUE ); //??��??����?
  
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
  BufferSize = size;
  memcpy( RX_Buffer, payload, BufferSize );
  RssiValue = rssi;
  SnrValue = snr;
  
  Radio.Standby();
  rf_receive_analy(BufferSize);
  if(EnableMaster)
  {
    if(memcmp(RX_Buffer,PongMsg,4)==0)
    {
      LedToggle();//LED����??
      
    }
    
    TX_Buffer[0] = 'P';
    TX_Buffer[1] = 'I';
    TX_Buffer[2] = 'N';
    TX_Buffer[3] = 'G'; 
    
    crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????��?3?��a����?����y?Y�㨹CRC?��
    TX_Buffer[4]=crc_value>>8;
    TX_Buffer[5]=crc_value;
    Radio.Send( TX_Buffer, 6);
  }
  else
  {
    if(memcmp(RX_Buffer,PingMsg,4)==0)
    {
      LedToggle();//LED����??
      
      TX_Buffer[0] = 'P';
      TX_Buffer[1] = 'O';
      TX_Buffer[2] = 'N';
      TX_Buffer[3] = 'G'; 
      
      crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????��?3?��a����?����y?Y�㨹CRC?��
      TX_Buffer[4]=crc_value>>8;
      TX_Buffer[5]=crc_value;
      Radio.Send( TX_Buffer, 6);
    }
    else
    {
      Radio.Rx( RX_TIMEOUT_VALUE ); 
    }   
  }
}

void OnTxTimeout( void )
{
  
}

void OnRxTimeout( void )
{
  Radio.Standby();
  if(EnableMaster)
  {
    TX_Buffer[0] = 'P';
    TX_Buffer[1] = 'I';
    TX_Buffer[2] = 'N';
    TX_Buffer[3] = 'G'; 
    
    crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????��?3?��a����?����y?Y�㨹CRC?��
    TX_Buffer[4]=crc_value>>8;
    TX_Buffer[5]=crc_value;
    Radio.Send( TX_Buffer, 6);
  }
  else
  {
    Radio.Rx( RX_TIMEOUT_VALUE ); 
  }
}

void OnRxError( void )
{
  
  
}

void radio_init()
{
 // Radio initialization
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;
  LedToggle();
  
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  
  //    Radio.WriteBuffer(0x06C0,data,2);
  //    Radio.ReadBuffer(0x06C0,test,2);
  
//#if defined( USE_MODEM_LORA )
  
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
  
  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, false );
}
void Ping_Pong()
{
  if(EnableMaster)
  {
    TX_Buffer[0] = 'P';
    TX_Buffer[1] = 'I';
    TX_Buffer[2] = 'N';
    TX_Buffer[3] = 'G'; 
    
    crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????��?3?��a����?����y?Y�㨹CRC?��
    TX_Buffer[4]=crc_value>>8;
    TX_Buffer[5]=crc_value;
    Radio.Send( TX_Buffer, 6);
  }
  else
  {
    Radio.Rx( RX_TIMEOUT_VALUE ); 
  }
  
}


int main( void )
{   
  //    uint8_t data[2]={0x12,0x34};
  //    static uint8_t test[2];
  Uart1_Init(9600, 0);
  HW_int();//MCU��a?�����?��3?��??��
  DelayMs(1000);
  
  radio_init();
  Ping_Pong();
  while( 1 )
  {
    extern  uint8_t tx_complete;     
    Radio.IrqProcess( ); // Process Radio IRQ
    Lora_Process();
  }
}




