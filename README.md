# HOW TO USE

## BUILDING THE SOFTWARE

You'll want to clone the the [pico-sdk](https://github.com/raspberrypi/pico-sdk.git) first as this will contain many neccesry functions that are used in the software,
use these commands to download the sdk, and then export it to the enviroment so it can be seen by cmake. (you can also add the export command to your .bashrc)
    
```
git clone https://github.com/raspberrypi/pico-sdk.git
export PICO_SDK_PATH=path/to/dir/
```
    
next you'll want to clone this repository, and it's depenacy [pico-vfs](https://github.com/oyama/pico-vfs.git) use these commands to do so. 
(The recursive command will automaticlly clone the depenacy). I would also like to recommend, to take a look at the Conf folder, and edit the values
or look at the values in there, and make sure they corralate with your SD Cards wiring, also the name of the application will also be important to 
makeing the bootloader load the program. (This configuation set up will be changed in the future in the place of a configuation file .txt file located directly on the SD card
this will prevent having to constanly recompile and reflash the program onto the pico, and may help in other aspects of the bootloader)
    
```
git clone --recursive https://github.com/EPTONE/PicoBoot.git
cd PicoBoot
mkdir build
cd build
cmake .. && make
```

I recommend that you add the `-j` following the number of cores on your computer, as this will greatly increase compile time.
Now press and hold the BOOTSEL button on the pico, while doing so plug it into your computer and waiting until it mounts
and then drag and drop the .uf2 file or 'cp PicoBoot.uf2 /path/to/pico' and as long as all your cableing is correct, and I've done
everything right on my end then everything should work.

## BUILDING APPLICATIONS

In the external file in the root of the repo there is some premade linker scripts, 
there is also a premade template repo with everything already set up, and ready to be tried out [PicoBoot-AppTemplate](https://github.com/EPTONE/PicoBoot-AppTemplate.git).

you'll also need the pico-sdk as well, this will have many of the basic functions you'll want to have
when using the device [pico-sdk](https://github.com/raspberrypi/pico-sdk.git). You'll have to tell cmake where it is
with these commands.

```
git clone https://github.com/raspberrypi/pico-sdk.git
export PICO_SDK_PATH=path/to/dir/
```

You'll then want to clone the template, and if your itching to just see it work build it and copy the .bin file to you guessed it the SD card

```
git clone https://github.com/EPTONE/PicoBoot-AppTemplate.git
cd PicoBoot-AppTemplate
mkdir build
cd build
cmake .. && make
```

and again I recommend you add the `-j` at the end of cmake with the number of CPU possed by your CPU as this will make building way faster.
Now if everythings wired up correctly or I don't go stupied then your pico should be flashing or what ever your currently having it do.
