MEMORY {

  FLASH(rx) : ORIGIN = 0x10000000 + 256k, LENGTH = 2048k - 256k
  BOOTLOADER(rwx) : ORIGIN = 0x20000000, LENGTH = 10k
  APP(rwx) : ORIGIN = 0x20000000 + 10k, LENGTH = 256k - 10k
  SCRATCH_X(rwx) : ORIGIN = 0x20040000, LENGTH = 4k
  SCRATCH_Y(rwx) : ORIGIN = 0x20041000, LENGTH = 4k
}

__BOOTLOADER_START__ = ORIGIN(BOOTLOADER);
__BOOTLOADER_SIZE__ = LENGTH(BOOTLOADER);
__APP_START__ = ORIGIN(APP);
__APP_SIZE__ = LENGTH(APP);

ENTRY(_entry_point)

/*SECTIONS {
  .text : {
    KEEP (*(.vectors))
  /*  KEEP(*(.binary_info_header))
    __binary_info_header_end = .;
    KEEP (*(reset))
    .ALIGN(4);

  } > FLASH

}
*/
