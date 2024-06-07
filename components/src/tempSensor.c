#include "tempSensor.h"
#include "driver/gpio.h"
#include "onewire_bus.h"

// Define your GPIO pin for the 1-Wire bus
#define DS18B20_GPIO_PIN 4

esp_err_t init_one_wire_bus(onewire_bus_handle_t* bus_handle) {
    // Initialize the onewire bus configuration
    onewire_bus_config_t config = {
        .bus_gpio_num = DS18B20_GPIO_PIN,
    };

    // Initialize the onewire bus rmt configuration
    onewire_bus_rmt_config_t rmt_config = {
        .max_rx_bytes = 8,  // example value, adjust as needed
    };

    // Create a new onewire bus with the given configurations
    return onewire_new_bus_rmt(&config, &rmt_config, bus_handle);
}

uint8_t temp_init(sensor_t *sensor) {
    onewire_bus_handle_t b_handler;
    esp_err_t result = init_one_wire_bus(&b_handler);
    if (result == ESP_OK) {
        sensor->handle = b_handler;
    }
    return result == ESP_OK ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR;
}

sensor_data_t temp_read(sensor_t *sensor, uint8_t unit) {
    sensor_data_t data = {0.0, SENSOR_STATUS_OK};
    onewire_bus_handle_t handle = (onewire_bus_handle_t)sensor->handle;

    // Send a reset pulse and check for presence
    if (onewire_bus_reset(handle) != ESP_OK) {
        data.status = SENSOR_STATUS_ERROR;
        return data;
    }

    // Skip ROM command to address the sensor directly if it's the only one on the bus
    uint8_t command = 0xCC;
    onewire_bus_write_bytes(handle, &command, 1);

    // Start temperature conversion
    command = 0x44;
    onewire_bus_write_bytes(handle, &command, 1);

    // Wait for the conversion to complete
    vTaskDelay(pdMS_TO_TICKS(750));

    // Read the scratchpad
    if (onewire_bus_reset(handle) != ESP_OK) {
        data.status = SENSOR_STATUS_ERROR;
        return data;
    }
    command = 0xCC;
    onewire_bus_write_bytes(handle, &command, 1);
    command = 0xBE;
    onewire_bus_write_bytes(handle, &command, 1);

    uint8_t scratchpad[9];
    if (onewire_bus_read_bytes(handle, scratchpad, sizeof(scratchpad)) != ESP_OK) {
        data.status = SENSOR_STATUS_ERROR;
        return data;
    }

    // Calculate the temperature from the scratchpad data
    int16_t raw_temp = (scratchpad[1] << 8) | scratchpad[0];
    data.value = raw_temp * 0.0625;  // DS18B20 has a resolution of 0.0625 degrees Celsius

    if (unit == TEMPERATURE_FAHRENHEIT){
        data.value = (data.value * 9 / 5) + 32;
    }

    return data;
}
