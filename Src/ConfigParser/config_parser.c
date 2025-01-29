/* code base */
#include "config_parser.h"
#include "../Loader/loader.h"

/* standered c libary */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//TODO: also still need to make a config defualt that is automatally generated upon bootloader init
//      of course only if it already dosn't detect one.

/* pico-sdk */
#include <pico/stdlib.h>

char *app_path;

void conf_act() { // setting values and then acting upon them need to be two
                                      // seprate things or else we'll be executeing load_app
                                      // and taking control away from the bootloader before
                                      // the bootloader even has the time to make the changes
  load_app(app_path);
}

void conf_set(const char *sysval, char *usrval) {
  
  if(strcmp(sysval, "app_path.") == 0) { // the period does not need to be added, in the conf file
                                         // this is just random data thats getting thrown into the buffer
                                         // that for some reason produces a period at the end of the array.
    app_path = usrval;                   // TLDR: don't fucking change this as much as your OCD tells you to.
  }
  else if(strcmp(sysval, "app_path.") == 0) {

    app_path = "/sd/pico.bin";
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
  volatile uint32_t sysiter = 0; // system iteration
  volatile uint32_t usriter = 0; // user iteration
  
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

      sysiter = 0;
      usriter = 0;

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

    /* system value buffer assienment*/
    if(sve == false && buff[0] != 0) {

      if(sysiter >= sizeof(sysval)) {
        sysval = realloc(sysval, sizeof(sysval) + 10);
      }

      printf("%s%s%s\n", buff, "letter is being appended to sysval", sysval);
      printf("%s%ld\n", "sysval increment value", sysiter);
      sysval[sysiter] = buff[0];
      
      sysiter++; 
    }
    
    /* user value buffer assienment*/
    else if(sve == true && buff[0] != 0) {
     
      if(usriter >= sizeof(usrval)) {
        usrval = realloc(usrval, sizeof(usrval) + 10);
      }
  
      printf("%s%s%s\n", buff, " letter is being appended to usrval ", usrval);
      printf("%s%ld\n", "usrval increment value: ", usriter);
      usrval[usriter] = buff[0];
      
      usriter++;
    }
  } while(lettersread != 0);

  free(sysval);
  sysval = 0;
  free(usrval);
  usrval = 0;
  fclose(conffp);

  conf_act();
}
