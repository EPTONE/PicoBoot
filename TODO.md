#### UPDATE debugging prints

we need to do this because in cgdb we can't get past the filesystem init for some fucking reason
we can then focus on the ff.h

#### change all file operations to ff.h

we should focus on making the loader work first, and then move onto the configeration
for some weird fucking reason everything in the loader wasn't working, where talking checking if
it's the same all the way down to not executing the program fuck knows whats going on but if should work
fine. Sigh.
