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
)

target_link_libraries( pico PRIVATE -Wl,--print-memory-usage

  pico_stdlib
  hardware_spi
  filesystem_fat
  filesystem_vfs
  blockdevice_sd
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
#pico_set_linker_script(pico ${CMAKE_SOURCE_DIR}/memmap.ld)
