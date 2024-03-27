///
/// \file LMS7002M_impl.h
///
/// Implementation struct for the LMS7002M C driver.
///
/// \copyright
/// Copyright (c) 2014-2015 Fairwaves, Inc.
/// Copyright (c) 2014-2015 Rice University
/// SPDX-License-Identifier: Apache-2.0
/// http://www.apache.org/licenses/LICENSE-2.0
///

#pragma once
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <LMS7002M/LMS7002M.h>
#include "dtoa.h"

/*!
 * Implementation of the LMS7002M data structure.
 * This is an opaque struct not available to the public API.
 */
struct LMS7002M_struct
{
    LMS7002M_spi_transact_t spi_transact;
    void *spi_transact_handle;

    //register shadows per channel (actual data)
    LMS7002M_regs_t _regs[2];

    //active register shadow ptr based on MIMO channel setting
    LMS7002M_regs_t *regs;

    volatile double cgen_freq; //!< last written CGEN frequency in Hz
    volatile double sxr_freq; //!< last written RX frequency in Hz
    volatile double sxt_freq; //!< last written TX frequency in Hz

    volatile double cgen_fref; //!< last written CGEN ref frequency in Hz
    volatile double sxr_fref; //!< last written RX ref frequency in Hz
    volatile double sxt_fref; //!< last written TX ref frequency in Hz
};
