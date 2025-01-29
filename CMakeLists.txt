cmake_minimum_required(VERSION 3.31.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(FAMILY rp2040)

include(pico_sdk_import.cmake)
add_subdirectory(dep/pico-vfs)

project(PicoBoot C CXX ASM)

pico_sdk_init()

add_executable( ${CMAKE_PROJECT_NAME}

  Src/main.c

  Src/Loader/ASM/exec.s
  Src/Loader/ASM/exec.h 
  Src/Loader/loader.c
  Src/Loader/loader.h

  Src/Filesystem/filesystem.c
  Src/Filesystem/filesystem.h

  Src/Err/err.c
  Src/Err/err.h

  Src/Linker/linkerscript.h

  Conf/conf.h

  Src/ConfigParser/config_parser.c
  Src/ConfigParser/config_parser.h # not yet still need to brainstorm this
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -Wl,--print-memory-usage

  pico_stdlib
  pico_stdio

  hardware_spi
  hardware_flash
  hardware_sync

  filesystem_fat
  filesystem_vfs
  blockdevice_sd
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
pico_set_linker_script(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Src/Linker/memmap_bootloader.ld)
