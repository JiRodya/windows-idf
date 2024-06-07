#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

// Enum for different types of sensors
typedef enum {
    SENSOR_TEMPERATURE,
    SENSOR_PH,
    SENSOR_TDS,
    SENSOR_SALINITY,
    SENSOR_TURBIDITY,
    SENSOR_DO  // Dissolved Oxygen
} sensor_type_t;

typedef enum {
    SENSOR_STATUS_OK,
    SENSOR_STATUS_ERROR,
    SENSOR_STATUS_CALIBRATING,
    SENSOR_STATUS_NOT_INITIALIZED
} sensor_status_t;

// Sensor data might include value and status
typedef struct {
    float value;
    sensor_status_t status;  // Operational status using the defined enumeration
} sensor_data_t;

// Sensor structure forward declaration
typedef struct sensor_t sensor_t;

// Function pointer types for sensor operations
typedef uint8_t (*sensor_init_func_t)(sensor_t *sensor);
typedef sensor_data_t (*sensor_read_func_t)(sensor_t *sensor, uint8_t param);
typedef uint8_t (*sensor_calibrate_t)(sensor_t *sensor);

// Sensor structure with function pointers
struct sensor_t {
    sensor_type_t type;
    void* handle;  // Specific data needed for each sensor type
    sensor_init_func_t init_func;  // Function pointer to initialize the sensor
    sensor_read_func_t read_func;  // Function pointer to read from the sensor
    sensor_calibrate_t calibrate_func;  // Function pointer to calibrate the sensor
};

// Function declarations using the above function pointer types
uint8_t init_sensor(sensor_t *sensor);
sensor_data_t read_sensor(sensor_t *sensor, uint8_t param);
uint8_t calibrate_sensor(sensor_t *sensor);

#endif // SENSORS_H
