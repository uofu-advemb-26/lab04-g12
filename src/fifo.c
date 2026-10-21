#include "fifo.h"
#include "FreeRTOS.h"
#include <queue.h>

void fifo_worker_handler(QueueHandle_t requests, QueueHandle_t results,
                         int id) {
  for (;;) {
    struct request_msg data = {};

    if (xQueueReceive(requests, &data, portMAX_DELAY) == pdTRUE) {
      data.output = data.input + 5;
      data.handled_by = id;
      xQueueSendToBack(results, &data, portMAX_DELAY);
    }
  }
}
