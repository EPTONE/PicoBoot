#include "stdio.h"
#include "stdlib.h"

FILE *conf_fp;
char *conf_buffer;

int parseconf() { // assentment of values

  fread(conf_buffer, 1, 1, conf_fp);

  return 0;
}

int loadconf(char *mntpoint) { // assenment of file
  
  conf_fp = fopen(mntpoint, "r");
  if (conf_fp == 0) {
    return -1;    
  }

  return 0;
}
