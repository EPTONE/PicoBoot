/* standered c Library */
#include <stdio.h>
#include <errno.h>

/* deps  */
#include "ff.h"

/* code base */
#include "filesystem.h"
#include "../../Conf/conf.h"
#include "tf_card.h"
#include "../Err/err.h"

/* filesystem pointer */

FATFS sd;
FRESULT ffr;

int filesystem_init() {

  pico_fatfs_set_config(&sdconf);

  for (unsigned int i = 0; i <= 5; i++) {
  
    ffr = f_mount(&sd, "", 1);
    if (ffr == FR_OK) {
      break;
    }

    printf("failed to mount sdcard, retry %d/5 | ERROR CODE: %d", i, ffr);
    pico_fatfs_reboot_spi();
  }
  if (ffr != FR_OK) {
    err(ffr);      
  }

  return 0;
}

void filesystem_deinit(FIL *file_fp) {
  f_close(file_fp);
  
  for (unsigned int i = 0; i <= 5; i++) {
  
    ffr = f_unmount("");
    if (ffr == FR_OK) {
      break;
    }

    printf("failed to unmount sdcard, retry %d/5 | ERROR CODE: %d", i, ffr);
  }
  if (ffr != FR_OK) {
    err(ffr);      
  }
  
}
