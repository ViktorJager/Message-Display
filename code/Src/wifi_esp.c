///* Hello wifi_esp8266 */

#include "main.h"
#include "usart.h"
#include <string.h>
#include "user_functions.h"

#define TIMOUT_UART 5000
#define BUFF_SIZE 1200

//#define SSID "bENNE"
//#define PASSWORD "Krikor69"


/**
* @brief    Configures a new ESP module 
* @param    None
* @note     None
* @retval   None 
*/
void wifi_configure(void){
  // This only needs to be run when configuring a new ESP unit
  // set_wifimode:      WiFi Mode = STA
  // set_tcpconnection: TCP/UDP Connections = Single
  
  uint8_t set_wifimode[] =       "AT+CWMODE=1\r\n";
  uint8_t set_tcpconnection[] =  "AT+CIPMUX=0\r\n";
  
  HAL_UART_Transmit(&huart3, set_wifimode, sizeof(set_wifimode), TIMOUT_UART);
  HAL_UART_Transmit(&huart3, set_tcpconnection, sizeof(set_tcpconnection), TIMOUT_UART); 
}


/**
* @brief    Connects to the specified network
* @param    None
* @note     Connect to wifi: "SSID","PASSWORD"
*           uint8_t usr_pass[] = "AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n";
* @retval   None 
*/
void wifi_connect(void){
  uint8_t usr_pass[] = "AT+CWJAP=\"bENNE\",\"Krikor69\"\r\n";
  
  HAL_UART_Transmit(&huart3, usr_pass, sizeof(usr_pass), TIMOUT_UART);
}


/**
* @brief    Fetch msg from webiste and store in array
* @param    None
* @note     Returns a pointer to 
* @retval   None 
*/
char * wifi_get_msg(void){
  
  uint8_t arr[BUFF_SIZE]; 
  uint8_t * recieve_buffer;
  recieve_buffer = (uint8_t *) malloc(sizeof(arr));
  
  uint8_t setup_tcp[] = "AT+CIPSTART=\"TCP\",\"simple-door-screen-sweet-bear.eu-gb.mybluemix.net\",80\r\n";
  uint8_t send_bytes[] = "AT+CIPSEND=82\r\n";
  uint8_t get_request[] = "GET /message HTTP/1.1\r\nHost: simple-door-screen-sweet-bear.eu-gb.mybluemix.net\r\n\r\n";
  
  HAL_UART_Transmit(&huart3, setup_tcp, sizeof(setup_tcp), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, send_bytes, sizeof(send_bytes), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, get_request, sizeof(get_request), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  
  printf("%s\r\n", recieve_buffer);
  // Filter GET response
  char * filtered_msg = filter_msg(recieve_buffer);
  for(int i = 0; i < 1200; i++){
    *(recieve_buffer + i) = NULL;
  }
  
  free(recieve_buffer);
  //printf(filtered_msg);
  
  /* DEBUG
  char testMsg[] = "Find out the size of a piece of text ...because (byte)size matters! Long Live Mutti";
  char * filtered_msg;
  filtered_msg = testMsg;
  */
  
  //lcd_write(1, filtered_msg);
  
  
  // Copy string and extend
  char msgStr[81] = {0};
  int i = 0;
  while(*(filtered_msg+i) != '\0')
  {
    msgStr[i] = *(filtered_msg+i);
    i++;
  }
  msgStr[i] = '\0';
  
  //printf(msgStr);
  
  //PRINTOUTS FOR DEBUGGING
  printf("%s\r\n", recieve_buffer);
  printf("/********************/\n");  
  printf(filtered_msg);
  printf("\r\n/********************/\n");  
  printf(msgStr);
  printf("\r\n/********************/\n");
  
  
  
  return msgStr;
}


void tcp_connect(void){
  uint8_t recieve_buffer[BUFF_SIZE] = {0};
//  printf("%d\r\n", recieve_buffer);
  
  uint8_t AT_COMMAND_setup_tcp[] = "AT+CIPSTART=\"TCP\",\"simple-door-screen-sweet-bear.eu-gb.mybluemix.net\",80\r\n";
  HAL_UART_Transmit(&huart3, AT_COMMAND_setup_tcp, sizeof(AT_COMMAND_setup_tcp), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  
  printf("%s\r\n", recieve_buffer);
}

char * send_get_request(){
  uint8_t recieve_buffer[BUFF_SIZE] = {0};
  uint8_t send_bytes[] = "AT+CIPSEND=82\r\n";
  uint8_t get_request[] = "GET /message HTTP/1.1\r\nHost: simple-door-screen-sweet-bear.eu-gb.mybluemix.net\r\n\r\n";
  
  
  HAL_UART_Transmit(&huart3, send_bytes, sizeof(send_bytes), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, get_request, sizeof(get_request), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  
  printf("%s\r\n", recieve_buffer);
  
  char * filtered_msg = filter_msg(recieve_buffer);
  printf(filtered_msg);
  //free(recieve_buffer);
  return filtered_msg;
}

void send_get_request2(){
  uint8_t recieve_buffer[BUFF_SIZE] = {0};
  
  
  uint8_t send_bytes[] = "AT+CIPSEND=82\r\n";
  uint8_t get_request[] = "GET /message HTTP/1.1\r\nHost: simple-door-screen-sweet-bear.eu-gb.mybluemix.net\r\n\r\n";
  
  HAL_UART_Transmit(&huart3, send_bytes, sizeof(send_bytes), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, get_request, sizeof(get_request), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  
  // Filter GET response
  char * filtered_msg = filter_msg(recieve_buffer);
  //printf(filtered_msg);
  
  /* DEBUG
  char testMsg[] = "Find out the size of a piece of text ...because (byte)size matters! Long Live Mutti";
  char * filtered_msg;
  filtered_msg = testMsg;
  */
  
  lcd_write(1, filtered_msg);
  
  
  // Copy string and extend
  char msgStr[81] = {0};
  int i = 0;
  while(*(filtered_msg+i) != '\0')
  {
    msgStr[i] = *(filtered_msg+i);
    i++;
  }
  msgStr[i] = '\0';
  
  //printf(msgStr);
  
  //PRINTOUTS FOR DEBUGGING
    printf("%s\r\n", recieve_buffer);
    printf("/********************/\n");  
    printf(filtered_msg);
    printf("\r\n/********************/\n");  
    printf(msgStr);
    printf("\r\n/********************/\n");
  

}

void wifi_get_msg2(void){
  
  uint8_t recieve_buffer[BUFF_SIZE] = {0};
  
  uint8_t setup_tcp[] = "AT+CIPSTART=\"TCP\",\"simple-door-screen-sweet-bear.eu-gb.mybluemix.net\",80\r\n";
  uint8_t send_bytes[] = "AT+CIPSEND=82\r\n";
  uint8_t get_request[] = "GET /message HTTP/1.1\r\nHost: simple-door-screen-sweet-bear.eu-gb.mybluemix.net\r\n\r\n";
  
  HAL_UART_Transmit(&huart3, setup_tcp, sizeof(setup_tcp), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, send_bytes, sizeof(send_bytes), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, get_request, sizeof(get_request), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  
  // Filter GET response
  char * filtered_msg = filter_msg(recieve_buffer);
  //printf(filtered_msg);
  
  /* DEBUG
  char testMsg[] = "Find out the size of a piece of text ...because (byte)size matters! Long Live Mutti";
  char * filtered_msg;
  filtered_msg = testMsg;
  */
  
  printf("%s\r\n", recieve_buffer);
  printf("=========================\r\n");
  printf("%s\r\n", filtered_msg);
  
  lcd_write(1, filtered_msg);
}

/** BACKUP
* @brief    Fetch msg from webiste and store in array
* @param    None
* @note     Returns a pointer to 
* @retval   None 

char * wifi_get_msg(void){
  
  uint8_t recieve_buffer[BUFF_SIZE] = {0};
  
  uint8_t setup_tcp[] = "AT+CIPSTART=\"TCP\",\"simple-door-screen-sweet-bear.eu-gb.mybluemix.net\",80\r\n";
  uint8_t send_bytes[] = "AT+CIPSEND=82\r\n";
  uint8_t get_request[] = "GET /message HTTP/1.1\r\nHost: simple-door-screen-sweet-bear.eu-gb.mybluemix.net\r\n\r\n";
  
  HAL_UART_Transmit(&huart3, setup_tcp, sizeof(setup_tcp), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, send_bytes, sizeof(send_bytes), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  HAL_UART_Transmit(&huart3, get_request, sizeof(get_request), TIMOUT_UART);
  HAL_UART_Receive(&huart3, recieve_buffer, BUFF_SIZE, TIMOUT_UART);
  
  // Filter GET response
  char * filtered_msg = filter_msg(recieve_buffer);
  //printf(filtered_msg);
  
   DEBUG
  char testMsg[] = "Find out the size of a piece of text ...because (byte)size matters! Long Live Mutti";
  char * filtered_msg;
  filtered_msg = testMsg;
  
  
  lcd_write(1, filtered_msg);
  
  
  // Copy string and extend
  char msgStr[81] = {0};
  int i = 0;
  while(*(filtered_msg+i) != '\0')
  {
    msgStr[i] = *(filtered_msg+i);
    i++;
  }
  msgStr[i] = '\0';
  
  //printf(msgStr);
  
  //PRINTOUTS FOR DEBUGGING
    printf("%s\r\n", recieve_buffer);
    printf(filtered_msg);
    printf("\r\n----------------\n");  
    printf(msgStr);
    printf("\r\n----------------\n");
  
  return msgStr;
}
*/