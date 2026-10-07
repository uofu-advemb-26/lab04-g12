
#include "FreeRTOS.h"
#include "signaling.h"
#include <semphr.h>

void signal_handle_calculation(SemaphoreHandle_t request,
                               SemaphoreHandle_t response,
                               struct signal_data *data) {
  // Wait for a request
  xSemaphoreTake(request, portMAX_DELAY);
  // Add 5 to data
  data->output = data->input + 5;
  // give response
  xSemaphoreGive(response);
  return;
}

BaseType_t signal_request_calculate(SemaphoreHandle_t request,
                                    SemaphoreHandle_t response,
                                    struct signal_data *data) {
  // set input data
  // send ready signal (give request semaphore)
  xSemaphoreGive(request);
  // wait for response
  return xSemaphoreTake(response, (TickType_t)500);
}
