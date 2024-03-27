///
/// \file spidev_interface.h
///
/// A SPI transaction interface through linux spidev.
///
/// \copyright
/// Copyright (c) 2014-2015 Fairwaves, Inc.
/// Copyright (c) 2014-2015 Rice University
/// SPDX-License-Identifier: Apache-2.0
/// http://www.apache.org/licenses/LICENSE-2.0
///

#pragma once
#include <stdint.h>
#include <stdbool.h>

/*!
 * Initialize a spidev interface given the device node path.
 * The result is the handle to pass into LMS7002M_create().
 * \param path the device node path (Ex /dev/spiXXXX)
 * \return the transaction handle or NULL for error
 */
static inline void *spidev_interface_open(const char *path);

/*!
 * Close an open spidev interface using the handle.
 */
static inline void spidev_interface_close(void *handle);

/*!
 * The SPI transaction implementation - pass this to LMS7002M_create().
 */
static inline uint32_t spidev_interface_transact(void *handle, const uint32_t data, const bool readback);

/***********************************************************************
 * Implementation details below
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

static inline void *spidev_interface_open(const char *path)
{
    int fd = open(path, O_RDWR);
    if (fd < 0)
    {
        perror("open spidev");
        return NULL;
    }
    int *handle = (int *)malloc(sizeof(int));
    *handle = fd;




    uint32_t mode;

    // mode |= SPI_LOOP; mode |= SPI_CPHA; mode |= SPI_CPOL; mode |= SPI_LSB_FIRST; mode |= SPI_CS_HIGH; mode |= SPI_3WIRE; mode |= SPI_NO_CS; mode |= SPI_READY;
    
    mode = 0;
    //mode |= SPI_CPHA;

    if (ioctl(fd, SPI_IOC_WR_MODE32 , &mode)<0) {
        perror("can't set spi mode");
        return handle;
    }

    mode = 0;

    if (ioctl(fd, SPI_IOC_RD_MODE32, &mode) < 0)
        {
        perror("SPI rd_mode");
        return handle;
    }

    printf("spi mode is %08x\n", mode);


    uint8_t bits_per_word;
    bits_per_word = 32;

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word)) {
        perror("can't set bits per word");
    }

    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits_per_word)) {
        perror("can't get bits per word");
    }
    
    printf("spi bits per word is %d\n", bits_per_word);



    
    return handle;
}

static inline void spidev_interface_close(void *handle)
{
    int *fd = (int *)handle;
    close(*fd);
    free(fd);
}

static inline uint32_t spidev_interface_transact(void *handle, const uint32_t data, const bool readback)
{
    int *fd = (int *)handle;

    //transaction data structures
    struct spi_ioc_transfer xfer[1];
    unsigned char txbuf[4];
    unsigned char rxbuf[4];

    //setup transaction
    memset(xfer, 0, sizeof(xfer));
    xfer[0].tx_buf = (unsigned long)txbuf;
    //only specify rx buffer when readback is specified
    if (readback) xfer[0].rx_buf = (unsigned long)rxbuf;
    xfer[0].len = 4; //bytes

    //load tx data
    txbuf[3] = (data >> 24);
    txbuf[2] = (data >> 16);
    txbuf[1] = (data >> 8);
    txbuf[0] = (data >> 0);

    long status = ioctl(*fd, SPI_IOC_MESSAGE(1), xfer);
    if (status < 0)
    {
        perror("SPI_IOC_MESSAGE");
    }

    //load rx data
    return \
        (((uint32_t)rxbuf[3]) << 24) |
        (((uint32_t)rxbuf[2]) << 16) |
        (((uint32_t)rxbuf[1]) << 8) |
        (((uint32_t)rxbuf[0]) << 0);
}
