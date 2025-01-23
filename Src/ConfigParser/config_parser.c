#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "config_parser.h"
#include <pico/stdlib.h>
#include <stdlib.h>
#include "../Loader/loader.h"

void conf_set(const char *sysval, const char *usrval) {
  
  if(strcmp(sysval, "app_path.") == 0) { 

    load_app(usrval); 
  }
  else if(strcmp(sysval, "app_path.") == 0) {

    load_app("/sd/pico.bin");
  }
}

void conf_parse(const char *conf_path) {
  
  FILE *conffp = fopen(conf_path, "r");
  if (conffp == NULL) {
    printf("%s\n", "failed to open file");
  }
  
  /* buffers */
  char *sysval = malloc(sizeof(char)*10); // system value
  char *usrval = malloc(sizeof(char)*10); // user value
  char buff[1];

  uint32_t lettersread = 0;

  /* iterations */
  volatile uint32_t sysint = 0; // system iteration
  volatile uint32_t usrint = 0; // user iteration
  
  /* system value end */
  volatile bool sve = false;

  do {

    lettersread = fread(buff, 1, 1, conffp);

    /* input sanatizing and comment checking */
    if(buff[0] == '=') {
      sve = true;
      buff[0] = 0;
    }
    else if(buff[0] == ' ') {
      buff[0] = 0;
    }
    
    if (buff[0] == '#') {
      
      while(buff[0] != '\n') {
        fread(buff, 1, 1, conffp);
      }
    }

    /* reset all vars, and clear all buffers */
    if(buff[0] == '\n') {

      conf_set(sysval, usrval);
      
      sve = false;

      sysint = 0;
      usrint = 0;

      printf("%s\n", sysval);
      printf("%s\n", usrval);

      for(uint32_t sysclr = 0; sysclr <= sizeof(sysval); sysclr++) {
        sysval[sysclr] = 0;
      }

      for(uint32_t usrclr = 0; usrclr <= sizeof(usrval); usrclr++) {
        usrval[usrclr] = 0;
      }

      buff[0] = 0;
    }

    /* system value buffer */
    if(sve == false && buff[0] != 0) {

      if(sysint >= sizeof(sysval)) {
        sysval = realloc(sysval, sizeof(sysval) + 10);
      }

      printf("%s%s%s\n", sysval, "letter is being appended to sysval", buff);
      printf("%s%ld\n", "sysval increment value", sysint);
      sysval[sysint] = buff[0];
      
      sysint++; 
    }
    /* user value buffer */
    else if(sve == true && buff[0] != 0) {
     
      if(usrint >= sizeof(usrval)) {
        usrval = realloc(usrval, sizeof(usrval) + 10);
      }
  
      printf("%s%s%s\n", usrval, " letter is being appended to usrval ", buff);
      printf("%s%ld\n", "usrval increment value: ", usrint);
      usrval[usrint] = buff[0];
      
      usrint++;
    }
  } while(lettersread != 0);

  free(sysval);
  sysval = 0;
  free(usrval);
  usrval = 0;
  fclose(conffp);
}
