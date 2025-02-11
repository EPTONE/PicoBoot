/* pico-sdk */
#include <pico/stdlib.h>

/* code base */
//#include "ConfigParser/config_parser.h"
#include "Filesystem/filesystem.h"
#include "Loader/loader.h"

int main(void) {
  stdio_init_all();
  filesystem_init();
//  conf_parse("/sd/.sys/boot.conf");
  load_app("pico_fatfs_test.bin");
}
