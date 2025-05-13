#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink and GPIO INPUT pin,
   or you can edit the following line and set a number here.
*/
#define GPIO_INPUT_IO 4

/*set pin 4, and then put it into pin_bit_mask to configure those pins at the same time*/
#define GPIO_INPUT_PIN_SEL (1ULL<<GPIO_INPUT_IO) 
//GPIO_INPUT_PIN_SEL                000000000000000000000000000000000010000

#define ESP_INTR_FLAG_DEFAULT 0 // No interrupt flag

static const char *TAG = "example"; // definition of TAG " example: Turning the LED OFF! "

#define BLINK_GPIO 2
#define GPIO_OUTPUT_PIN_SEL (1ULL<<BLINK_GPIO)

static uint8_t s_led_state = 0; // __uint_least8_t = uint8_t
volatile bool led_toggle_requested = false;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    
        led_toggle_requested=true;
        esp_rom_printf(" Interrupt triggered!\n");
}
static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

void app_main(void)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};

    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

/* way 2: gpio_set_intr_type(GPIO_INPUT_IO, GPIO_INTR_POSEDGE);*/
    

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO, gpio_isr_handler,(void*)GPIO_INPUT_IO);
   
    while(1)
    {
        if (led_toggle_requested)
        {
            int n=5;
            while (n--)
            {
                ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
                blink_led();
                s_led_state = !s_led_state;
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
            led_toggle_requested=false;
        }
        else
        {
            vTaskDelay(10 / portTICK_PERIOD_MS); // Nhường CPU nếu không có việc
        }
    }
}
