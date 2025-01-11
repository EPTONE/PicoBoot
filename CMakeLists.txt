cmake_minimum_required(VERSION 3.31.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(PICO_SDK_PATH "~/Programs/DOWNLOADED/Libs/PicoSDKs/pico-sdk")
set(FAMILY rp2040)

# set(PICO_NO_FLASH TRUE) # this will also be add to app template or else it throws an error

include(pico_sdk_import.cmake)
add_subdirectory(dep/pico-vfs)

project(debug C CXX ASM)

pico_sdk_init()

add_executable( ${CMAKE_PROJECT_NAME}

  Src/main.c
  
  Src/ASM/exec.h
  Src/ASM/exec.s

  Src/Linker/linkerscript.h
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

# pico_set_binary_type(pico no_flash)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

# pico_set_linker_script(pico ${CMAKE_SOURCE_DIR}/External/memmap_app.ld) # this will be added to app template, pico no flash must be true for it to change the addresses

pico_set_linker_script(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Src/Linker/memmap_bootloader.ld)
