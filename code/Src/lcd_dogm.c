/** 
******************************************************************************
  @brief  LCD user implemented functions
  @file   lcd.c
  @author Viktor Jäger
  @ver    1.0
  @date   27-February-2019
  @brief  Function/s for writing to the LCD EA DOGS104-A through SPI interface.
          This file contains basic user default setup and transmit functions.
          The SPI transmission is done through interrupts.
******************************************************************************
*/

#include "main.h"
#include "spi.h"
#include "gpio.h"


ITStatus SPIReady = RESET;


/**
  @func   void lcd_write(uint8_t RS, uint8_t data)
  @brief  This function wites to the display or an instruction to the LCD 
          display through SPI as an interrupt. The data to be sent is first
          divided into a lower and an upper part along with a startbyte.
          The data is then sent as 3 bytes {startbyte, lowerdata, upperdata}.
  @param  uint8_t RS,   RS = 1 write to display 
                        RS = 0 write instuction
          uint8_t data, data to be sent
  @return none
*/
void lcd_send_data(uint8_t RS, uint8_t data)
{
  uint8_t startbyte;
  /* 
    Startbyte = 1111 1[RW][RS]0,
    where 5 msb are 1's, RW is 0,
      RS = 1 for writing to display
      RS = 0 for writing an instruction
  
    NOTE: Display is LSB and reads least significant bit first, therefore,
    we need to flip the bits before sending.      
  */
  
  /*
    STARTBYTE WRITE INSTRUCTION
    1111 1000 = 0xFA   The byte that the display should recieve
    0001 1111 = 0x5F   Translated LSB byte to be sent via SPI
  */
  uint8_t startbyte_instr = 0x1F;
  
  /*
    STARTBYTE WRITE DISPLAY
    1111 1010 = 0xFA   The byte that the display should recieve
    0101 1111 = 0x5F   Translated LSB byte to be sent via SPI
  */
  uint8_t startbyte_display = 0x5F;
  
  /*
    RS decides the startbyte:
      RS = 1 --> write to display
      RS = 0 --> write instuction
  */
  if(RS == 1){
    startbyte = startbyte_display;
  } else {
    startbyte = startbyte_instr;
  }
  
  /*
    Split up byte to transmit into two bytes of lower and upper data
    ex. Transmit of data:
    uint8_t data = 0x42 [= 0100 0010]
        -> separate the lower and upper part into separate variables
    uint8_t lower_data = [0000 0010] = 0x02
    uint8_t upper_data = [0000 0100] = 0x04
  */
  uint8_t lower_data = data & 0x0F;
  uint8_t upper_data = ((data >> 4)&0x0F);
  
  /* Assemble the bytes into tx-array = {startbyte, lowerdata, upperdata} */
  uint8_t buffer[3] = {startbyte, lower_data, upper_data};

  /* Transmit startbyte, lower_data, upper_data */
  HAL_SPI_Transmit_IT(&hspi1, (uint8_t *)buffer, 3);
  
  while(SPIReady != SET);
  SPIReady = RESET;
}

/**
  @func   void lcd_init(void)
  @brief  Basic initialization of LCD
  @param  void
  @return none
*/
void lcd_init(void){
  
  uint8_t function_set =                 0x3A;   // 8bit datalen
  uint8_t clear_display =                0x01;   // clear display
  uint8_t ex_function_set =              0x09;   // 4 line display
  uint8_t entry_mode_set =               0x06;   // bottom view
  uint8_t bias_setting =                 0x1E;   // BS1=1
  uint8_t function_set2  =               0x39;   // 8bit data len
  uint8_t internal_OSC =                 0x1B;   // BS0=1->Bias=1/6
  uint8_t follower_control =             0x6E;   // Divider on n set value
  uint8_t power_control =                0x56;   // Booster on, set contrast
  uint8_t contrast_set =                 0x7A;   // Set contrast
  uint8_t function_set3 =                0x38;   // 8bit data len
  uint8_t display_ON =                   0x0F;   // Display, cursor, blink = on
  
  lcd_send_data(0, function_set);
  lcd_send_data(0, clear_display);
  lcd_send_data(0, ex_function_set);
  lcd_send_data(0, entry_mode_set);
  lcd_send_data(0, bias_setting);
  lcd_send_data(0, function_set2);
  lcd_send_data(0, internal_OSC);
  lcd_send_data(0, follower_control);
  lcd_send_data(0, power_control);
  lcd_send_data(0, contrast_set);
  lcd_send_data(0, function_set3);
  lcd_send_data(0, display_ON);
}

/**
  @func   void lcd_clear(void)
  @brief  clear the display on current cursor
  @param  void
  @return none
*/
void lcd_clear(void)
{
  lcd_send_data(0,0x01);
}

/**
  @brief  Updates the display of LCD.
  @param  void
  @return none
*/
void lcd_update_display(void)
{
  
}

/**
  @brief  Writes up to 20 char's to specified row.
  @param  uint8_t row, char message[]
  @return none
*/
void lcd_write(uint8_t row, char message[])
{

// Add test for message 0-20 char, err out of bounds
  switch(row) 
  {
    
  case 1:
    // Write to ROW1
    lcd_send_data(0, 0x80);
    for(int i = 0; i < strlen(message); i++){
      lcd_send_data(1, message[i]);
    } break;
    
  case 2:
    // Write to ROW2
    lcd_send_data(0, 0x80+0x20);
    for(int i = 0; i < strlen(message); i++){
      lcd_send_data(1, message[i]);
    } break;
    
  case 3:
    // Write to ROW1
    lcd_send_data(0, 0x80+0x40);
    for(int i = 0; i < strlen(message); i++){
      lcd_send_data(1, message[i]);
    } break;
    
  case 4:
    // Write to ROW1
    lcd_send_data(0, 0x80+0x60);
    for(int i = 0; i < strlen(message); i++){
      lcd_send_data(1, message[i]);
    } break;
    
  default :
    printf("Err: row out of bounds");
  }
  
}


/**
  @brief  Set the backlight ON
  @param  void
  @return none
*/
void lcd_bklgt_set(void)
{
  HAL_GPIO_WritePin(GPIOA, BKLGT_EN1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, BKLGT_EN2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, BKLGT_EN3_Pin, GPIO_PIN_SET);
}

/**
  @brief  Reet the backlight OFF
  @param  void
  @return none
*/
void lcd_bklgt_reset(void)
{
  HAL_GPIO_WritePin(GPIOA, BKLGT_EN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, BKLGT_EN2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, BKLGT_EN3_Pin, GPIO_PIN_RESET);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *SpiHandle)
{
  SPIReady = SET;
}
