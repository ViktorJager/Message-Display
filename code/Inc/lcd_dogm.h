/**
  ******************************************************************************
  @file   lcd.h
  @author Viktor Jäger
  @ver    1.0
  @date   27-February-2019
  @brief  This file contains all the functions prototypes for the lcd
  ******************************************************************************
*/

void lcd_init(void);
void lcd_send_data(uint8_t RS, uint8_t data);
void lcd_clear(void);
void lcd_update_display(void);
void lcd_write(uint8_t row, char message[]);

void lcd_bklgt_set(void);
void lcd_bklgt_reset(void);

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *SpiHandle);