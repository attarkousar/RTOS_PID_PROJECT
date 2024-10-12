#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define POT_ADC_CHANNEL ADC1_CHANNEL_0  // GPIO36 (ADC1 Channel 0) for the potentiometer
#define V_REF 3300                       // ADC reference voltage in mV

void readPotTask(void *pvParameter) {
    esp_adc_cal_characteristics_t adc1_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &adc1_chars);
    
    while (1) {
        // Read ADC value
        int raw_value = adc1_get_raw(POT_ADC_CHANNEL);
        
        // Convert the ADC value to a voltage
        //uint32_t voltage = esp_adc_cal_raw_to_voltage(raw_value, &adc1_chars);
        
        // Adjust scaling to reach full 0–5V range
        float scaled_value = (float)(raw_value) * ((float)V_REF / 4095.0) / 1000.0; // Slightly adjusted scaling factor and offset


        printf("Potentiometer Value: %.2f V , Raw value %d\n", scaled_value, raw_value);
        
        vTaskDelay(pdMS_TO_TICKS(500));  // Delay 500 ms
    }
}

void app_main() {
    // Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POT_ADC_CHANNEL, ADC_ATTEN_DB_11);
    
    // Create task to read potentiometer
    xTaskCreate(&readPotTask, "readPotTask", 2048, NULL, 5, NULL);
}