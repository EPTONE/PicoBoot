# CONFIGERING THE SOFTWARE

there should be a folder labeled Conf in the root of this repo, this folder contains a header file with the pins
and spi identifers change these to however you have set up your current wiring to the sdcard. Next head to the main.c
located in the Src fold in the root of the repo, you'll see a function called `load_app();` change the file in the double quotes
to yourfile.bin

> [!NOTE]
> the current setup will change in the future to favor a config file which is currently temperally disabled due to a recent
> change in project dependancies this will reduce the need for recompulation allowing one to focus on there applications,
> the .conf file will also allow for other configeration options to be broken out in the event that they are required.

# BUILDING THE SOFTWARE

Your first going to want to clone the [pico-sdk](https://github.com/raspberrypi/pico-sdk.git) into an easy to remeber directory
using `git clone https://your.url.here`.

> [!TIP]
> you can add can `--recursive` to get all the pico-sdk submodules for you to use in other projects.

We'll then want to let your shell now where it is with `export your/directory/here`, you can also add this to you
.bashrc to make availble everytime you start up a terminal.

Now your going to want to `git clone --recursive https://this/repo` onto your system, now `cd` into the directory
and type `mkdir build && cd build`. You'll then want to type `cmake ..` this will generate the makefile as well as some
other things if there are any error see to it they are resolved, and then type `make`, and the bootloader should begin compiling.

> [!TIP]
> you can add a `-j your cpu's core count` to `make` to decrease compile time for the project.

Now put the pico into DFU mode, and then plug it into your computor, it then should pop up as a usb mass storage,
now just drag and drop or use the `cp` command to flash the pico, if everything has done correctly it should be working.

# BUILDING APPLICATIONS

## TEMPLATE

The template repo is already pre set up with all the things required to prepare you for application development
for the bootloader if can be found [here](https://github.com/EPTONE/PicoBoot-AppTemplate.git) it should work
without any extra configeration

## FROM SCRATCH

Theres a folder in the root of the repository that is called External this is a folder that contains a linker script
called [memmap_app_flash.ld](External/memmap_app_flash.ld), you will then want to take this file and add it into your own
project and then add `pico_set_linker_script(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/path/to/linker)`.

> [!NOTE]
> this is also all you just need when porting programs just be sure that you have enough space on flash to add the app.

# CREADIT

I'd like to thank [Hiroyuki Oyama](https://github.com/oyama) for posting his works with his own [bootloader](https://github.com/oyama/pico-sdcard-boot.git)
as this enabled me to both learn and continue this prusuit in constructing this bootloader, you should also check out the filesystem he uses it was a little
overpowered for my use case but for those who want to take a gander I'll leave it in the repo [here](./Deps/), you can also find it [here](https://github.com/oyama/pico-vfs.git).
