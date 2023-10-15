#include <stdio.h>             
#include <string.h>            
#include "freertos/FreeRTOS.h" 
#include "esp_system.h"        
#include "esp_wifi.h"          
#include "esp_log.h"           
#include "esp_event.h"         
#include "nvs_flash.h"         
#include "lwip/err.h"          
#include "lwip/sys.h"          

const char *ssid = "SHAW-EEAD50";
const char *pass = "0MGSM253KPJ9";
int retry_num = 0;

void infinite_loop() {
  while (1) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    printf("Current time: %02d:%02d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
  if (event_id == WIFI_EVENT_STA_START){
    printf("__________________WIFI3 CONNECTING....\n");
  }  else if (event_id == WIFI_EVENT_STA_CONNECTED){    
    printf("__________________WiFi3 CONNECTED\n");
  }  else if (event_id == WIFI_EVENT_STA_DISCONNECTED){    
    printf("__________________WiFi3 lost connection\n");
    if (retry_num < 5){
      esp_wifi_connect();
      retry_num++;
      printf("__________________Retrying2 to Connect...\n");
    }
  }  else if (event_id == IP_EVENT_STA_GOT_IP)  {
    printf("__________________WiFi3 got IP...\n\n");
    infinite_loop();
  }
}


void wifi_connection(){
  esp_netif_init();
  esp_event_loop_create_default();     
  esp_netif_create_default_wifi_sta(); 
  wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_initiation); 
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
  wifi_config_t wifi_configuration = { .sta = { .ssid = "SHAW-EEAD50", .password = "0MGSM253KPJ9", } };
  strcpy((char *)wifi_configuration.sta.ssid, ssid);
  strcpy((char *)wifi_configuration.sta.password, pass);
  esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
  esp_wifi_start();
  esp_wifi_set_mode(WIFI_MODE_STA);
  printf("______ wifi_init_softap finished. SSID:%s  password:%s", ssid, pass);
  esp_wifi_connect();
}

void app_main(void){
  nvs_flash_init();
  wifi_connection();
}