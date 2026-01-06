#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#define PORT 5000

static const char *TAG = "udp-listener";

static void udp_server_task(){
    struct sockaddr_storage addr_from;
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) { ESP_LOGE(TAG, "Failed to create socket!"); }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    uint8_t buf[1024];
    int len;

    /*
       When  a  socket is created with socket(2), it exists in a name space (address family) but has no address assigned to it.  bind() assigns the address
       specified by addr to the socket referred to by the file descriptor sockfd.  addrlen specifies the size, in bytes, of the address  structure  pointed
       to by addr.  Traditionally, this operation is called “assigning a name to a socket”.
    */

    int err = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
    if(err < 0){ ESP_LOGE(TAG, "Failed to bind socket!"); }

    // ESP_LOGI(TAG, "Socket bound to port %d and addr: %d!", PORT, addr.sin_addr.s_addr);
    ESP_LOGI(TAG, "Socket bound to %s:%d", inet_ntoa(addr.sin_addr), PORT);  //inet_ntoa -> Internet host representation to dot representation

    while(1){
        ESP_LOGI(TAG, "Waiting for data....");

        socklen_t socklen = sizeof(addr_from);

        if((len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &addr_from, &socklen)) < 0){
            ESP_LOGE(TAG, "No msg received!");
        }

        // buf[len] = 0;
        if(len > 0){
            for(int i = 0; i < len; i++){
                // printf("0x%02X, ", bmap[i]);
                ESP_LOGI(TAG, "0x%02X %d", buf[i], len);
            }
        }
    }
    if (sockfd != -1) {
        ESP_LOGE(TAG, "Shutting down socket and restarting...");
        shutdown(sockfd, 0);
        close(sockfd);
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(udp_server_task, "udp-server", 4096, NULL, 1, NULL);
}
