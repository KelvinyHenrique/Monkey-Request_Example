#include <iostream>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "mkrequests.h"

static const char *TAG = "HTTP_CLIENT";

extern "C"
{
  void app_main();
}

static void http_test_task(void *pvParameters)
{
  string body = "{\"temperatura\":\"25\"}";
  string webserver = "httpbin.org";
  string webpath = "/post";
  string params = "nome=kelviny&sobrenome=henrique";
  Requests *request = new Requests();
  cout << request->get(webserver, webpath, params) << std::endl;
  cout << request->post(webserver, webpath, params, body) << std::endl;
  cout << request->put(webserver, webpath, params, body) << std::endl;
  cout << request->patch(webserver, webpath, params, body) << std::endl;
  cout << request->del(webserver, webpath, params) << std::endl;
  cout << request->head(webserver, webpath, params) << std::endl;
  ESP_LOGI(TAG, "Finish http example");
  vTaskDelete(NULL);
}

void app_main(void)
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(example_connect());
  ESP_LOGI(TAG, "Connected to AP, begin http example");
  xTaskCreate(&http_test_task, "http_test_task", 8192, NULL, 5, NULL);
}
