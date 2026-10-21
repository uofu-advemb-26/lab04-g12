#include "fifo.h"
#include "FreeRTOS.h"
#include <queue.h>

void fifo_worker_handler(QueueHandle_t requests, QueueHandle_t results,
                         int id) {
  // struct request_msg data = {};                        
  // BaseType_t result = xQueueReceive(requests, &data, 1000);
  // if (result == pdTRUE) {
  //   data.output = data.input + 5;
  //   data.handled_by = id;
  //   xQueueSendToBack(results, &data, portMAX_DELAY);
  // }
  return;
}
