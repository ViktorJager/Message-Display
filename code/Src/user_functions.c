#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/**
* @brief    Filter a string and return substring between specified tokens
* @param    const char *str
*           const char *p1
*           const char *p2
* @note     tbc
* @retval   *str 
*/
char * extract_between(const char *str, const char *p1, const char *p2)
{
  const char *i1 = strstr(str, p1);
  if (i1 != NULL)
  {
    const size_t pl1 = strlen(p1);
    const char *i2 = strstr(i1 + pl1, p2);
    if (p2 != NULL)
    {
      /* Found both markers, extract text. */
      const size_t mlen = i2 - (i1 + pl1);
      char *ret = malloc(mlen + 1);
      if (ret != NULL)
      {
        memcpy(ret, i1 + pl1, mlen);
        ret[mlen] = '\0';
        return ret;
      }
    }
  }
  return NULL;
}

/**
* @brief    
* @param    
* @note     
* @retval   
*/
char * filter_msg(char msg[]) {
  return extract_between(msg, "<p>", "</p>");
}


void update_display(void)
{
  char * usrMsg = (char *) wifi_get_msg();
  printf("%s\r\n", usrMsg);
  
  // Copy string and print
  //char msgStr[80+1] = {0};
  char row1[20] = {0};
  char row2[20] = {0};
  char row3[20] = {0};
  char row4[20] = {0};
  
  for(int i = 0; i < 20; i++)
  {
    //msgStr[i] = *(usrMsg+i);
    row1[i] = *(usrMsg+0+i);
    row2[i] = *(usrMsg+20+i);
    row3[i] = *(usrMsg+40+i);
    row4[i] = *(usrMsg+60+i);
  }
  
  lcd_write(1, row1);
  lcd_write(2, row2);
  lcd_write(3, row3);
  lcd_write(4, row4);
}