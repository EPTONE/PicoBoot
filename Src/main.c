#include <pico/stdlib.h>
#include "ConfigParser/config_parser.h"
#include "Filesystem/filesystem.h"

int main(void) {
  stdio_init_all();
  filesystem_init();
  conf_parse("/sd/.sys/boot.conf");
}
