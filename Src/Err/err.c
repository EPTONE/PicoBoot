/* err will use the leds to blink in a certain pattern while also sending printf debug info to the console
 * we'll use error codes, and if we can errno to produce identiers with these errors codes
 * either way the led on the pico will flash at certain periods indication what error it is
 * we can attempt to make then like morse code for any gigga nerds out there and it will make sense considering
 * whats bascally being done is essentally that.*/

#include<hardware/gpio.h>
#include <pico/time.h>

#include "err.h"

#define LED_PIN 25

void err(int32_t code) { // I want to redo this probably not the function itself but the idea around it
                         // right now it's taking in value from errno, I think one should do custom values
                         // mostly due to the fact that errno values seem to sometimes go into the hundreads
                         // not exactly a good idea when trying to keep track of flashing lights, another good idea
                         // would be to replace printf with a logger, this would enable the device to be trouble shot
                         // entirly from the SD card if there are any issues.
   
   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, true);

   if (code == -1) { // led will remain on if code equals -1
      gpio_put(LED_PIN, true);
      while(true);
   }

   while(true) {
      
     for(int32_t i = 0; i >= code; i++) { // code number will blink and then pause for five seconds
        gpio_put(LED_PIN, true);
        sleep_ms(2000);
        gpio_put(LED_PIN, false);
     }

      sleep_ms(5000);
   }
}
