#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tempSensor.h"
#include "sensors.h"

// Sensor declaration
sensor_t temp_sensor;

void init_sensors(void) {
    // Initialize water temperature sensor
    temp_sensor.type = SENSOR_TEMPERATURE;
    temp_sensor.init_func = temp_init;
    temp_sensor.read_func = temp_read;

    // Initialize the temperature sensor
    uint8_t init_status = temp_sensor.init_func(&temp_sensor);
    printf("Temperature sensor initialization status: %d\n", init_status);
}

void temperature_task(void *pvParameters) {
    sensor_t *temp_sensor = (sensor_t *)pvParameters;

    while (1) {
        // Read temperature sensor
        sensor_data_t temperature = temp_sensor->read_func(temp_sensor, 0);  // Unit is not used here
        printf("Temperature: %.2f\n", temperature.value);
        // Delay for some time before reading again (example: 1 second)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void) {
    init_sensors();
    xTaskCreate(temperature_task, "temperature_task", 2048, &temp_sensor, 5, NULL);
}
