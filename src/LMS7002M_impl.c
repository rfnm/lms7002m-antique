///
/// \file LMS7002M_impl.c
///
/// Implementation details for the LMS7002M C driver.
/// This file defines the LMS7002M_t structure,
/// and the SPI transaction helper functions.
///
/// \copyright
/// Copyright (c) 2016-2017 Skylark Wireless
/// Copyright (c) 2014-2015 Fairwaves, Inc.
/// Copyright (c) 2014-2015 Rice University
/// SPDX-License-Identifier: Apache-2.0
/// http://www.apache.org/licenses/LICENSE-2.0
///

//#include <stdlib.h>
//#include <ctype.h>
//#include <string.h>
#include "LMS7002M_impl.h"
#include <LMS7002M/LMS7002M_logger.h>

/***********************************************************************
 * Create/destroy implementations
 **********************************************************************/
LMS7002M_t *LMS7002M_create(LMS7002M_spi_transact_t transact, void *handle)
{
    LMS7002M_t *self = (LMS7002M_t *) kmalloc(sizeof(LMS7002M_t), GFP_KERNEL);
    if (self == NULL) return NULL;
    self->spi_transact = transact;
    self->spi_transact_handle = handle;
    LMS7002M_regs_init(&self->_regs[0]);
    LMS7002M_regs_init(&self->_regs[1]);
    self->regs = self->_regs;
    self->cgen_freq = 0.0;
    self->sxr_freq = 0.0;
    self->sxt_freq = 0.0;
    self->cgen_fref = 0.0;
    self->sxr_fref = 0.0;
    self->sxt_fref = 0.0;
    return self;
}

void LMS7002M_destroy(LMS7002M_t *self)
{
    kfree(self);
}

/***********************************************************************
 * Helper calls to format SPI transactions
 **********************************************************************/
void LMS7002M_spi_write(LMS7002M_t *self, const int addr, const int value)
{
    uint32_t data = (((uint32_t)1) << 31) | (((uint32_t)addr) << 16) | (value & 0xffff);
    self->spi_transact(self->spi_transact_handle, data, false/*no readback*/);
}

int LMS7002M_spi_read(LMS7002M_t *self, const int addr)
{
    uint32_t data = (((uint32_t)addr) << 16);
    return self->spi_transact(self->spi_transact_handle, data, true/*readback*/) & 0xffff;
}

void LMS7002M_regs_spi_write(LMS7002M_t *self, const int addr)
{
    int value = LMS7002M_regs_get(self->regs, addr);
    LMS7002M_spi_write(self, addr, value);

    //for CHAB mode: duplicate to the CHB register shadow
    if (self->regs->reg_0x0020_mac == REG_0X0020_MAC_CHAB)
    {
        LMS7002M_regs_set(&self->_regs[1], addr, value);
    }
}

void LMS7002M_regs_spi_read(LMS7002M_t *self, const int addr)
{
    LMS7002M_regs_set(self->regs, addr, LMS7002M_spi_read(self, addr));
}

LMS7002M_regs_t *LMS7002M_regs(LMS7002M_t *self)
{
    return self->regs;
}

void LMS7002M_regs_to_rfic(LMS7002M_t *self)
{
    LMS7002M_set_mac_ch(self, LMS_CHA);
    for (const int *addrp = LMS7002M_regs_addrs(); *addrp != 0; addrp++)
    {
        LMS7002M_regs_spi_write(self, *addrp);
    }

    LMS7002M_set_mac_ch(self, LMS_CHB);
    for (const int *addrp = LMS7002M_regs_addrs(); *addrp != 0; addrp++)
    {
        //ignore registers that do not depend on MAC
        if (*addrp < 0x0100) continue;

        LMS7002M_regs_spi_write(self, *addrp);
    }

    //ensure that we cache the chips actual version register
    //used in the codebase to handle hardware rev differences
    LMS7002M_regs_spi_read(self, 0x002f);
}

void LMS7002M_rfic_to_regs(LMS7002M_t *self)
{
    LMS7002M_set_mac_ch(self, LMS_CHA);
    for (const int *addrp = LMS7002M_regs_addrs(); *addrp != 0; addrp++)
    {
        LMS7002M_regs_spi_read(self, *addrp);
    }

    LMS7002M_set_mac_ch(self, LMS_CHB);
    for (const int *addrp = LMS7002M_regs_addrs(); *addrp != 0; addrp++)
    {
        LMS7002M_regs_spi_read(self, *addrp);
    }
}

