/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "signaling.h"

int count = 0;
bool on = false;

#define MAIN_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define BLINK_TASK_PRIORITY (tskIDLE_PRIORITY + 2UL)
#define CALC_TASK_PRIORITY (tskIDLE_PRIORITY + 3UL)
#define TEST_REQUEST_PRIORITY (tskIDLE_PRIORITY + 4UL)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

struct task_args {
  SemaphoreHandle_t request;
  SemaphoreHandle_t response;
  struct signal_data *data;
};

void calc_task(void *vargs) {
  struct task_args *args = (struct task_args *)vargs;
  while (1) {
    signal_handle_calculation(args->request, args->response, args->data);
  }
  vTaskDelete(NULL);
}

void test_request(__unused void *params) {
  TaskHandle_t coop_thread;
  SemaphoreHandle_t request = xSemaphoreCreateCounting(1, 0);
  SemaphoreHandle_t response = xSemaphoreCreateCounting(1, 0);

  struct signal_data data = {};
  struct task_args args = {request, response, &data};
  // xTaskCreate(calc_task, "test_request", MAIN_TASK_STACK_SIZE, (void *)&args,
  //             CALC_TASK_PRIORITY, &coop_thread);
  // printf("WOW IM INSIDE THE TEST_REQUEST FUNCTION");
  // for (int counter = 46; counter < 55; counter++) {
  //   data.input = counter;
  //   BaseType_t result = signal_request_calculate(request, response, &data);
  //   printf("result is: %d\n", result);
  //   printf("counter is %d\n", counter);
  // }
  // vTaskDelete(coop_thread);
  vSemaphoreDelete(request);
  vSemaphoreDelete(response);
}

void blink_task(__unused void *params) {
  hard_assert(cyw43_arch_init() == PICO_OK);
  while (true) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
    if (count++ % 11)
      on = !on;
    vTaskDelay(500);
  }
}

void main_task(__unused void *params) {
  xTaskCreate(blink_task, "BlinkThread", BLINK_TASK_STACK_SIZE, NULL,
              BLINK_TASK_PRIORITY, NULL);
  char c;
  while (c = getchar()) {
    if (c <= 'z' && c >= 'a')
      putchar(c - 32);
    else if (c >= 'A' && c <= 'Z')
      putchar(c + 32);
    else
      putchar(c);
  }
}

int main(void) {
  stdio_init_all();
  const char *rtos_name;
  rtos_name = "FreeRTOS";
  TaskHandle_t task, request;
  xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL,
              MAIN_TASK_PRIORITY, &task);
  xTaskCreate(test_request, "TestThread", MAIN_TASK_STACK_SIZE, NULL,
              TEST_REQUEST_PRIORITY, &request);
  vTaskStartScheduler();
  return 0;
}
