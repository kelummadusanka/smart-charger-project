
#ifndef __TWI_H__
#define __TWI_H__

  #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)

    #define TWI_TWAR            TWAR 
    #define TWI_TWBR            TWBR 
    #define TWI_TWDR            TWDR
    #define TWI_TWCR            TWCR 
    #define TWI_TWSR            TWSR 

  #endif


  #ifndef SUCCESS
    #define SUCCESS             0
  #endif

  #ifndef ERROR
    #define ERROR               1
  #endif 

  #define TWI_START_ACK         0x08  
  #define TWI_REP_START_ACK     0x10  
  #define TWI_FLAG_ARB_LOST     0x38  

  #define TWI_MT_SLAW_ACK       0x18  
  #define TWI_MT_SLAW_NACK      0x20  
  #define TWI_MT_DATA_ACK       0x28  
  #define TWI_MT_DATA_NACK      0x30  
  // Master Receiver Mode
  #define TWI_MR_SLAR_ACK       0x40  
  #define TWI_MR_SLAR_NACK      0x48 
  #define TWI_MR_DATA_ACK       0x50 
  #define TWI_MR_DATA_NACK      0x58  
  

  #define TWI_SR_SLAW_ACK       0x60  
  #define TWI_SR_ALMOA_ACK      0x68  
  #define TWI_SR_GCALL_ACK      0x70 
  #define TWI_SR_ALMGA_ACK      0x78 
  #define TWI_SR_OA_DATA_ACK    0x80 
  #define TWI_SR_OA_DATA_NACK   0x88 
  #define TWI_SR_GC_DATA_ACK    0x90 
  #define TWI_SR_GC_DATA_NACK   0x98  
  #define TWI_SR_STOP_RSTART    0xA0 

  #define TWI_ST_OA_ACK         0xA8  
  #define TWI_ST_ALMOA_ACK      0xB0 
  #define TWI_ST_DATA_ACK       0xB8  
  #define TWI_ST_DATA_NACK      0xC0  
  #define TWI_ST_DATA_LOST_ACK  0xC8 


  #define TWI_FREQ(BIT_RATE, PRESCALER) { TWI_TWBR = BIT_RATE; TWI_TWSR |= (TWI_TWSR & 0x03) | PRESCALER; }

  #define TWI_START()                   { TWI_TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA); }

  #define TWI_STOP()                    { TWI_TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO); }


  #define TWI_ENABLE()                  { TWI_TWCR = (1 << TWEN) | (1 << TWINT); }


  #define TWI_WAIT_TILL_TWINT_IS_SET()  { while (!(TWI_TWCR & (1 << TWINT))); }


  #define TWI_STATUS                    ( TWI_TWSR & 0xF8 )

  void TWI_Init(void);


  char TWI_MT_Start(void);


  char TWI_MT_Send_SLAW(char);


  char TWI_MT_Send_Data(char);

  char TWI_MR_Send_SLAR(char);


  void TWI_Stop(void);
  
#endif
