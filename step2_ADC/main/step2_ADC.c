#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define POT_ADC_CHANNEL ADC1_CHANNEL_0  // GPIO36 (ADC1 Channel 0) for the potentiometer
#define V_REF 1100                       // ADC reference voltage in mV

void readPotTask(void *pvParameter) {
    esp_adc_cal_characteristics_t adc1_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &adc1_chars);
    
    while (1) {
        // Read ADC value
        int raw_value = adc1_get_raw(POT_ADC_CHANNEL);
        
        // Convert the ADC value to a voltage between 0 and 5
        uint32_t voltage = esp_adc_cal_raw_to_voltage(raw_value, &adc1_chars);
        //float scaled_value = (voltage / 1000.0) * (5.0 / 3.3);  // Adjusted scaling factor for 3.3V input
         // Scale the voltage to a range of 0 to 5
        float scaled_value = (voltage / 1000.0) * (5.1 / 3.3);  // Fine-tuned for better accuracy


        printf("Potentiometer Value: %.2f V\n", scaled_value);
        
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