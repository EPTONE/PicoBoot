cmake_minimum_required(VERSION 3.31.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(PICO_SDK_PATH "~/Programs/DOWNLOADED/Libs/PicoSDKs/pico-sdk")
set(FAMILY rp2040)

include(pico_sdk_import.cmake)
add_subdirectory(dep/pico-vfs)

project(pico C CXX ASM)

pico_sdk_init()

add_executable( pico

  Src/main.c
  
  Src/ASM/loader.h
  Src/ASM/loader.s

  Src/Linker/linkerscript.h
)

target_link_libraries(pico PRIVATE -Wl,--print-memory-usage

  pico_stdlib
  hardware_spi
  filesystem_fat
  filesystem_vfs
  blockdevice_sd
)

# pico_set_binary_type(pico no_flash)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

# pico_set_linker_script(pico ${CMAKE_SOURCE_DIR}/External/memmap_app.ld) # this will be added to app template
# set(PICO_NO_FLASH TRUE) # this will also be add to app template or else it throws an error

pico_set_linker_script(pico ${CMAKE_SOURCE_DIR}/Src/Linker/memmap_bootloader.ld)
