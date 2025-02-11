/* err will use the leds to blink in a certain pattern while also sending printf debug info to the console
 * we'll use error codes, and if we can errno to produce identiers with these errors codes
 * either way the led on the pico will flash at certain periods indication what error it is
 * we can attempt to make then like morse code for any gigga nerds out there and it will make sense considering
 * whats bascally being done is essentally that.*/

#include<hardware/gpio.h>
#include <pico/time.h>

#include "err.h"

#define LED_PIN 25

void err(int16_t code) {
   
   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, true);

   if (code <= -1) { // led will remain on if code less then or equal to -1
      gpio_put(LED_PIN, true);
      while(true);
   }

   while(true) {
      
     for(int32_t i = 0; i <= code; i++) { // code number will blink and then pause for five seconds
        gpio_put(LED_PIN, true);
        sleep_ms(250);
        gpio_put(LED_PIN, false);
     }

      sleep_ms(4000);
   }
}
