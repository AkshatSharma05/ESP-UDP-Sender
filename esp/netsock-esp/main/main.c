#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#define PORT 5000

static const char *TAG = "udp-listener";

static void udp_server_task(){
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) { ESP_LOGE(TAG, "Failed to create socket!"); }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5000);

    // struct msghdr msg;
    // char buf[100];

    /*
       When  a  socket is created with socket(2), it exists in a name space (address family) but has no address assigned to it.  bind() assigns the address
       specified by addr to the socket referred to by the file descriptor sockfd.  addrlen specifies the size, in bytes, of the address  structure  pointed
       to by addr.  Traditionally, this operation is called “assigning a name to a socket”.
    */
    int err = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
    if(err < 0){ ESP_LOGE(TAG, "Failed to bind socket!"); }
    ESP_LOGI(TAG, "Socket bound to port %d!", PORT);

    while(1){
        ESP_LOGI(TAG, "Waiting for data....");
        // int len = recvmsg(sockfd, &msg, NULL);
        // int len = recvfrom(sockfd, &buf, sizeof(buf), NULL, )
        // if(len < 0){ ESP_LOGE(TAG, "No msg received!"); }
        vTaskDelay(100/portTICK_PERIOD_MS);

    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    /* Configure the peripheral according to the LED type */
    xTaskCreate(udp_server_task, "udp-server", 4096*2, NULL, 1, NULL);
}
