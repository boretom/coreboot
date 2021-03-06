/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * Parameters to initialise the DDR3 memory on the Beaglebone Black
 * Taken and adapted from U-Boot.
 */

#ifndef __MAINBOARD_TI_BEAGLEBONE_DDR3_H__
#define __MAINBOARD_TI_BEAGLEBONE_DDR3_H__

/* Micron MT41K256M16HA-125E */
#define MT41K256M16HA125E_EMIF_READ_LATENCY 0x100007
#define MT41K256M16HA125E_EMIF_TIM1 0x0AAAD4DB
#define MT41K256M16HA125E_EMIF_TIM2 0x266B7FDA
#define MT41K256M16HA125E_EMIF_TIM3 0x501F867F
#define MT41K256M16HA125E_EMIF_SDCFG 0x61C05332
#define MT41K256M16HA125E_EMIF_SDREF 0xC30
#define MT41K256M16HA125E_ZQ_CFG 0x50074BE4
#define MT41K256M16HA125E_RATIO 0x80
#define MT41K256M16HA125E_INVERT_CLKOUT 0x0
#define MT41K256M16HA125E_RD_DQS 0x38
#define MT41K256M16HA125E_WR_DQS 0x44
#define MT41K256M16HA125E_PHY_WR_DATA 0x7D
#define MT41K256M16HA125E_PHY_FIFO_WE 0x94
#define MT41K256M16HA125E_IOCTRL_VALUE 0x18B

#define EMIF_OCP_CONFIG_BEAGLEBONE_BLACK 0x00141414

#endif
