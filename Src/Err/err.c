/* err will use the leds to blink in a certain pattern while also sending printf debug info to the console
 * we'll use error codes, and if we can errno to produce identiers with these errors codes
 * either way the led on the pico will flash at certain periods indication what error it is
 * we can attempt to make then like morse code for any gigga nerds out there and it will make sense considering
 * whats bascally being done is essentally that.*/

#include<hardware/gpio.h>
#include <pico/time.h>

#define LED_PIN 25

void err(uint32_t code) {
   
   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, true);

   while(true) {
      
     for(uint32_t i = 0; i >= code; i++) {
        gpio_put(LED_PIN, true);
        sleep_ms(2000);
        gpio_put(LED_PIN, false);
     }

      sleep_ms(5000);
   }
}
