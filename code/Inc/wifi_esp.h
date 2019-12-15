//// "main-functions"
//void init_wifi_module();
//String fetch_latest_msg();
//
//// "sub-functions"
//void connect_to_AP(String SSID, String Password);
void tcp_connect();
//
char * send_get_request();
//String msg_filter(str1);

void wifi_configure(void);
void wifi_connect(void);
char * wifi_get_msg(void);