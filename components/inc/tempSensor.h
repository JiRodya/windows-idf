#ifndef COMP_TEMP_SENSOR
#define COMP_TEMP_SENSOR

#include "onewire_bus.h"
#include "sensors.h"



typedef enum {
    TEMPERATURE_CELSIUS,
    TEMPERATURE_FAHRENHEIT
} temperature_unit_t;


// Function prototypes to match generic sensor interface
uint8_t temp_init(sensor_t* sensor);
uint8_t temp_calibrate(sensor_t* sensor);

esp_err_t init_one_wire_bus(onewire_bus_handle_t* bus_handle);
sensor_data_t temp_read(sensor_t* sensor, uint8_t unit);


#endif // COMP_TEMP_SENSOR


