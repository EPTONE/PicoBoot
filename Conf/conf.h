#ifndef CONF_H
#define CONF_H

#include "tf_card.h"
#include <hardware/spi.h>

/* SD Card Pin Defines */
#define SPI1 spi1
#define SD_SO 12
#define SD_SI 11
#define SD_CLK 10
#define SD_CS 13

pico_fatfs_spi_config_t sdconf = {
    SPI1,
    CLK_SLOW_DEFAULT,
    CLK_FAST_DEFAULT,
    SD_SO,
    SD_CS,
    SD_CLK,
    SD_SI,
    true
};

#endif
