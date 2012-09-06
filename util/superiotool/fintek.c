/*
 * This file is part of the superiotool project.
 *
 * Copyright (C) 2006 coresystems GmbH <info@coresystems.de>
 * Copyright (C) 2007-2008 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "superiotool.h"

#define DEVICE_ID_BYTE1_REG	0x20
#define DEVICE_ID_BYTE2_REG	0x21

#define VENDOR_ID_BYTE1_REG	0x23
#define VENDOR_ID_BYTE2_REG	0x24

#define FINTEK_VENDOR_ID	0x3419

static const struct superio_registers reg_table[] = {
	{0x0106, "F71862FG / F71863FG", {	/* Same ID? Datasheet typo? */
		/* We assume reserved bits are read as 0. */
		{NOLDN, NULL,
			{0x20,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,
			 0x2b,0x2c,0x2d,EOT},
			{0x06,0x01,0x19,0x34,0x00,0x00,MISC,0x00,0x00,0x00,
			 0x00,0x00,0x08,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf2,0xf4,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x03,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x03,0x42,EOT}},
		{0x4, "Hardware monitor",
			{0x30,0x60,0x61,0x70,EOT},
			{0x01,0x02,0x95,0x00,EOT}},
		{0x5, "Keyboard",
			{0x30,0x60,0x61,0x70,0x72,EOT},
			{0x01,0x00,0x60,0x00,0x00,EOT}},
		{0x6, "GPIO",
			{0xf0,0xf1,0xf2,0xf3,0xe0,0xe1,0xe2,0xe3,0xd0,0xd1,
			 0xd2,0xd3,0xc0,0xc1,0xc2,0xc3,0xb0,0xb1,0xb2,0xb3,
			 EOT},
			{0x00,0x0f,NANA,0x00,0x00,0xff,NANA,0x00,0x00,0xff,
			 NANA,0x00,0x00,0x0f,NANA,0x00,0x00,0x3f,NANA,0x00,
			 EOT}},
		{0x7, "VID",
			{0x30,0x60,0x61, 0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,
			 0xf7,EOT},
			{0x00,0x00,0x00, 0x00,0x00,MISC,0x00,NANA,0x00,0x00,
			 0x00,EOT}},
		{0x8, "SPI",
			{0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xfa,
			 0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0x10,0x04,0x01,NANA,0x00,0x00,0x00,NANA,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,EOT}},
		{0xa, "PME, ACPI",
			{0x30,0xf0,0xf1,0xf4,0xf5,0xf7,EOT},
			{0x00,0x00,NANA,0x06,0x1c,0x01,EOT}},
		{EOT}}},
	{0x2307, "F71889", {
		/* We assume reserved bits are read as 0. */
		{NOLDN, NULL,
			{0x20,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x2a,0x2b,
			 0x2c,0x2d,EOT},
			{0x07,0x23,0x19,0x34,0x00,0x00,0x00,0x00,0xf0,0x30,
			 0x00,0x08,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf2,0xf4,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x03,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x04,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x03,0x42,EOT}},
		{0x4, "Hardware monitor",
			{0x30,0x60,0x61,0x70,EOT},
			{0x01,0x02,0x95,0x00,EOT}},
		{0x5, "Keyboard",
			{0x30,0x60,0x61,0x70,0x72,0xfe,EOT},
			{0x01,0x00,0x60,0x01,0x0c,0x81,EOT}},
		{0x6, "GPIO",
			{0x80,0x81,0x82,0x83,0x90,0x91,0x92,0x93,0xa0,0xa1,
			 0xa2,0xa3,0xb0,0xb1,0xb2,0xc0,0xc1,0xc2,0xc3,0xd0,
			 0xd1,0xd2,0xd3,0xe0,0xe1,0xe2,0xe3,0xf0,0xf1,0xf2,
			 0xf3,0xfe,0xff,EOT},
			{0x00,0xff,NANA,0x00,0x00,0xff,NANA,0x00,0x00,0x1f,
			 NANA,0x00,0x00,0xff,NANA,0x00,0xff,NANA,0x00,0x00,
			 0xff,NANA,0x00,0x00,0x7f,NANA,0x00,0x00,0x7f,NANA,
			 0x00,0x00,0x00,EOT}},
		{0x7, "VID",
			{0x30,0x60,0x61,EOT},
			{0x00,0x00,0x00,EOT}},
		{0x8, "SPI",
			{0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xfa,
			 0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0x00,RSVD,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,EOT}},
		{0xa, "PME, ACPI",
			{0x30,0xf0,0xf1,0xf4,0xf5,0xf6,EOT},
			{0x00,0x00,0x00,0x26,0x1c,0x07,EOT}},
		{0xb, "VREF",
			{0xf0,0xf1,0xf2,0xf3,0xff,EOT},
			{0x64,0x64,0x64,0x00,0x00,EOT}},
		{EOT}}},
	{0x4103, "F71872F/FG / F71806F/FG", {	/* Same ID? Datasheet typo? */
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
			 0x29,0x2a,0x2b,0x2c,0x2d,EOT},
			{0x03,0x41,RSVD,0x19,0x34,0x00,0x00,MISC,0x66,
			 0x80,0x00,0x00,0x00,0x04,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf2,0xf4,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x03,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x04,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x03,0x42,EOT}},
		{0x4, "Hardware monitor",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x02,0x95,0x00,EOT}},
		{0x5, "Keyboard", /* Only documented on F71872F/FG. */
			{0x30,0x60,0x61,0x70,0x72,0xf0,0xf1,EOT},
			{0x01,0x00,0x60,0x00,0x00,0x83,0x00,EOT}},
		{0x6, "GPIO",
			{0x70,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
			 0xe9,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
			 EOT},
			{0x00,0x00,0x00,NANA,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x7f,0x00,0x7f,NANA,0x00,0xff,NANA,0x00,0x03,NANA,
			 EOT}},
		{0x7, "VID",
			{0x30,0x60,0x61,EOT},
			{0x00,0x00,0x00,EOT}},
		{0xa, "PME, ACPI",
			{0x30,0xf0,0xf1,0xf4,0xf5,EOT},
			{0x00,0x00,0x61,0x06,0x3c,EOT}},
		{EOT}}},
	{0x4105, "F71882FG/F71883FG", {		/* Same ID? Datasheet typo? */
		/* We assume reserved bits are read as 0. */
		{NOLDN, NULL,
			{0x20,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,
			 0x2b,0x2c,0x2d,EOT},
			{0x05,0x41,0x19,0x34,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x08,0x08,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf2,0xf4,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x03,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x04,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x03,0x42,EOT}},
		{0x4, "Hardware monitor",
			{0x30,0x60,0x61,0x70,EOT},
			{0x01,0x02,0x95,0x00,EOT}},
		{0x5, "Keyboard",
			{0x30,0x60,0x61,0x70,0x72,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x00,0x83,EOT}},
		{0x6, "GPIO",
			{0x70,0xe0,0xe1,0xe2,0xe3,0xd0,0xd1,0xd2,0xd3,0xc0,
			 0xc1,0xc2,0xc3,0xb0,0xb1,0xb2,0xb3,0xf0,0xf1,0xf2,
			 0xf3,EOT},
			{0x00,0x00,0xff,NANA,0x00,0x00,0xff,NANA,0x00,0x00,
			 0x0f,NANA,0x00,0x00,0x0f,NANA,0x00,0x00,0xff,NANA,
			 0x00,EOT}},
		{0x7, "VID",
			{0x30,0x60,0x61,EOT},
			{0x00,0x00,0x00,EOT}},
		{0x7, "SPI",
			{0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xfa,
			 0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0x10,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,EOT}},
		{0xa, "PME, ACPI",
			{0x30,0xf0,0xf1,0xf4,0xf5,EOT},
			{0x00,0x00,0x01,0x06,0x1c,EOT}},
		{EOT}}},
	{0x0604, "F71805F/FG", {
		/* We assume reserved bits are read as 0. */
		{NOLDN, NULL,
			{0x20,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,EOT},
			{0x04,0x06,0x19,0x34,0x00,0x00,0x3f,0x08,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf2,0xf4,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x03,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x04,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x03,0x42,EOT}},
		{0x4, "Hardware monitor",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x02,0x95,0x00,EOT}},
		{0x6, "GPIO",
			{0x70,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
			 0xe9,0xf0,0xf1,0xf3,0xf4,EOT},
			{0x00,0x00,0x00,NANA,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,NANA,0x00,NANA,EOT}},
		{0xa, "PME",
			{0x30,0xf0,0xf1,EOT},
			{0x00,0x00,0x00,EOT}},
		{EOT}}},
	{0x0581, "F8000", {	/* Fintek/ASUS F8000 */
		{EOT}}},
	{0x0802, "F81216D/DG", {
		{NOLDN, NULL,
			{0x25,0x2f,EOT},
			{0x00,RSVD,EOT}},
		{0x0, "UART1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{NANA,NANA,NANA,NANA,0x00,0x40,EOT}},
		{0x1, "UART2",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{NANA,NANA,NANA,NANA,0x00,EOT}},
		{0x2, "UART3",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{NANA,NANA,NANA,NANA,0x00,EOT}},
		{0x3, "UART4",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{NANA,NANA,NANA,NANA,0x00,EOT}},
		{0x8, "WDT",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x00,NANA,NANA,NANA,NANA,NANA,EOT}},
		{EOT}}},
	{0x1602, "F81216AD", {
		{NOLDN, NULL,
			{0x25,0x27,EOT},
			{0x00,NANA,EOT}},
		{0x0, "UART1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf4,0xf5,EOT},
			{NANA,NANA,NANA,NANA,0x00,0x40,0x00,0x00,EOT}},
		{0x1, "UART2",
			{0x30,0x60,0x61,0x70,0xf0,0xf4,0xf5,EOT},
			{NANA,NANA,NANA,NANA,0x00,0x00,0x00,EOT}},
		{0x2, "UART3",
			{0x30,0x60,0x61,0x70,0xf0,0xf4,0xf5,EOT},
			{NANA,NANA,NANA,NANA,0x00,0x00,0x00,EOT}},
		{0x3, "UART4",
			{0x30,0x60,0x61,0x70,0xf0,0xf4,0xf5,EOT},
			{NANA,NANA,NANA,NANA,0x00,0x00,0x00,EOT}},
		{0x8, "WDT",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x00,NANA,NANA,NANA,NANA,NANA,EOT}},
		{EOT}}},
	{0x0407, "F81865F/F-I", {
		{NOLDN, NULL,
			{0x02,0x07,0x20,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a-1,0x2a-2,0x2b,0x2c,0x2d,EOT},
			{NANA,0x00,0x07,0x04,0x19,0x34,NANA,NANA,NANA,0x00,0x00,0x00,0x00,0x1f,0x00,0x08,EOT}},
		{0x00, "FDC",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf2,0xf4,EOT},
			{NANA,0x03,0xf0,NANA,NANA,NANA,NANA,NANA,EOT}},
		{0x03, "LPT",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{NANA,0x03,0x78,NANA,NANA,NANA,EOT}},
		{0x04, "HWMON",
			{0x30,0x60,0x61,0x70,EOT},
			{NANA,0x02,0x95,NANA,EOT}},
		{0x05, "KBC",
			{0x30,0x60,0x61,0x70,0x72,0xfe,0xf0,EOT},
			{NANA,0x00,0x60,NANA,NANA,NANA,0x71,EOT}},
		{0x06, "GPIO",
			{0x30,0x60,0x61,0x70,0xf1,0xf2,0xf3,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xe0,0xe1,0xe2,0xe3,0xef,0xd0,0xd1,0xd2,0xd3,0xc0,0xc1,0xc2,0xc3,0xb0,0xb1,0xb2,0xb3,0xa0,0xa1,0xa2,0xa3,0x90,0x91,0x92,0x93,EOT},
			{NANA,0x00,0x60,NANA,0x00,NANA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,NANA,0x00,NANA,0x00,0xff,NANA,0x00,0x00,0xff,NANA,0x00,0x00,0xff,NANA,0x00,0x00,0xff,NANA,0x00,NANA,NANA,NANA,NANA,EOT}},
		{0x07, "WDT",
			{0x30,0x60,0x61,0xf5,0xf6,0xfa,EOT},
			{NANA,0x00,0x00,0x00,0x00,NANA,EOT}},
		{0x08, "SPI",
			{0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0x10,0x04,NANA,NANA,0x00,0x00,NANA,NANA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "PME & ACPI",
			{0x30,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,EOT},
			{NANA,NANA,NANA,NANA,NANA,0x06,NANA,0x00,EOT}},
		{0x0b, "RTC",
			{0x30,0x60,0x61,0x70,EOT},
			{NANA,0x00,0x00,NANA,EOT}},
		{0x10, "UART1",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf4,0xf5,EOT},
			{NANA,0x03,0xf8,NANA,NANA,NANA,0x00,0x00,EOT}},
		{0x11, "UART2",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf4,0xf5,EOT},
			{NANA,0x02,0xf8,NANA,NANA,NANA,0x00,0x00,EOT}},
		{0x12, "UART3",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf4,0xf5,EOT},
			{NANA,0x03,0xe8,NANA,NANA,NANA,0x00,0x00,EOT}},
		{0x13, "UART4",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf4,0xf5,EOT},
			{NANA,0x02,0xe8,NANA,NANA,NANA,0x00,0x00,EOT}},
		{0x14, "UART5",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf4,0xf5,EOT},
			{NANA,0x00,0x00,NANA,NANA,NANA,0x00,0x00,EOT}},
		{0x15, "UART6",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf4,0xf5,EOT},
			{NANA,0x00,0x00,NANA,NANA,NANA,0x00,0x00,EOT}},
		{EOT}}},
	{EOT}
};

void probe_idregs_fintek(uint16_t port)
{
	uint16_t vid, did;

	probing_for("Fintek", "", port);

	enter_conf_mode_winbond_fintek_ite_8787(port);

	did = regval(port, DEVICE_ID_BYTE1_REG);
	did |= (regval(port, DEVICE_ID_BYTE2_REG) << 8);

	vid = regval(port, VENDOR_ID_BYTE1_REG);
	vid |= (regval(port, VENDOR_ID_BYTE2_REG) << 8);

	if (vid != FINTEK_VENDOR_ID || superio_unknown(reg_table, did)) {
		if (verbose)
			printf(NOTFOUND "vid=0x%04x, id=0x%04x\n", vid, did);
		exit_conf_mode_winbond_fintek_ite_8787(port);
		return;
	}

	printf("Found Fintek %s (vid=0x%04x, id=0x%04x) at 0x%x\n",
	       get_superio_name(reg_table, did), vid, did, port);
	chip_found = 1;

	dump_superio("Fintek", reg_table, port, did, LDN_SEL);

	exit_conf_mode_winbond_fintek_ite_8787(port);
}


void probe_idregs_fintek_alternative(uint16_t port)
{
	uint16_t vid, did;

	probing_for("Fintek", "", port);

	enter_conf_mode_fintek_7777(port);

	did = regval(port, DEVICE_ID_BYTE1_REG);
	did |= (regval(port, DEVICE_ID_BYTE2_REG) << 8);

	vid = regval(port, VENDOR_ID_BYTE1_REG);
	vid |= (regval(port, VENDOR_ID_BYTE2_REG) << 8);

	if (vid != FINTEK_VENDOR_ID || superio_unknown(reg_table, did)) {
		if (verbose)
			printf(NOTFOUND "vid=0x%04x, id=0x%04x\n", vid, did);
		exit_conf_mode_fintek_7777(port);
		return;
	}

	printf("Found Fintek %s (vid=0x%04x, id=0x%04x) at 0x%x\n",
	       get_superio_name(reg_table, did), vid, did, port);
	chip_found = 1;

	dump_superio("Fintek", reg_table, port, did, LDN_SEL);

	exit_conf_mode_fintek_7777(port);
}

void print_fintek_chips(void)
{
	print_vendor_chips("Fintek", reg_table);
}
