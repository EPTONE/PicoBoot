#### change all file operations to ff.h

we should focus on making the loader work first, and then move onto the configeration
for some weird fucking reason everything in the loader wasn't working, where talking checking if
it's the same all the way down to not executing the program fuck knows whats going on but if should work
fine. Sigh.

# DO EXTRA CHECKS FOR READ ERRORS

there seems to be a problem where just sometimes it has a tough time reading
we should impilment a function that will retry once one of these occur to give it
another chance before it explodes.
