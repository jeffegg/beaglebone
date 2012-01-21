/*

  Broadcom B43 wireless driver
  IEEE 802.11n 2059 radio device data tables

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.

*/

#include "b43.h"
#include "radio_2059.h"

#define RADIOREGS(r00, r01, r02, r03, r04, r05, r06, r07, r08, r09, \
		  r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, \
		  r20, r21, r22, r23, r24, r25, r26, r27, r28) \
	.radio_syn16			= r00,	\
	.radio_syn17			= r01,	\
	.radio_syn22			= r02,	\
	.radio_syn25			= r03,	\
	.radio_syn27			= r04,	\
	.radio_syn28			= r05,	\
	.radio_syn29			= r06,	\
	.radio_syn2c			= r07,	\
	.radio_syn2d			= r08,	\
	.radio_syn37			= r09,	\
	.radio_syn41			= r10,	\
	.radio_syn43			= r11,	\
	.radio_syn47			= r12,	\
	.radio_syn4a			= r13,	\
	.radio_syn58			= r14,	\
	.radio_syn5a			= r15,	\
	.radio_syn6a			= r16,	\
	.radio_syn6d			= r17,	\
	.radio_syn6e			= r18,	\
	.radio_syn92			= r19,	\
	.radio_syn98			= r20,	\
	.radio_rxtx4a			= r21,	\
	.radio_rxtx58			= r22,	\
	.radio_rxtx5a			= r23,	\
	.radio_rxtx6a			= r24,	\
	.radio_rxtx6d			= r25,	\
	.radio_rxtx6e			= r26,	\
	.radio_rxtx92			= r27,	\
	.radio_rxtx98			= r28

#define PHYREGS(r0, r1, r2, r3, r4, r5)	\
	.phy_regs.bw1	= r0,	\
	.phy_regs.bw2	= r1,	\
	.phy_regs.bw3	= r2,	\
	.phy_regs.bw4	= r3,	\
	.phy_regs.bw5	= r4,	\
	.phy_regs.bw6	= r5

static const struct b43_phy_ht_channeltab_e_radio2059 b43_phy_ht_channeltab_radio2059[] = {
  {	.freq			= 2412,
	RADIOREGS(0x48, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x6c,
		  0x09, 0x0f, 0x0a, 0x00, 0x0a, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03c9, 0x03c5, 0x03c1, 0x043a, 0x043f, 0x0443),
  },
  {	.freq			= 2417,
	RADIOREGS(0x4b, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x71,
		  0x09, 0x0f, 0x0a, 0x00, 0x0a, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03cb, 0x03c7, 0x03c3, 0x0438, 0x043d, 0x0441),
  },
  {	.freq			= 2422,
	RADIOREGS(0x4e, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x76,
		  0x09, 0x0f, 0x09, 0x00, 0x09, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03cd, 0x03c9, 0x03c5, 0x0436, 0x043a, 0x043f),
  },
  {	.freq			= 2427,
	RADIOREGS(0x52, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x7b,
		  0x09, 0x0f, 0x09, 0x00, 0x09, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03cf, 0x03cb, 0x03c7, 0x0434, 0x0438, 0x043d),
  },
  {	.freq			= 2432,
	RADIOREGS(0x55, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x80,
		  0x09, 0x0f, 0x08, 0x00, 0x08, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03d1, 0x03cd, 0x03c9, 0x0431, 0x0436, 0x043a),
  },
  {	.freq			= 2437,
	RADIOREGS(0x58, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x85,
		  0x09, 0x0f, 0x08, 0x00, 0x08, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03d3, 0x03cf, 0x03cb, 0x042f, 0x0434, 0x0438),
  },
  {	.freq			= 2442,
	RADIOREGS(0x5c, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x8a,
		  0x09, 0x0f, 0x07, 0x00, 0x07, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03d5, 0x03d1, 0x03cd, 0x042d, 0x0431, 0x0436),
  },
  {	.freq			= 2447,
	RADIOREGS(0x5f, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x8f,
		  0x09, 0x0f, 0x07, 0x00, 0x07, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03d7, 0x03d3, 0x03cf, 0x042b, 0x042f, 0x0434),
  },
  {	.freq			= 2452,
	RADIOREGS(0x62, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x94,
		  0x09, 0x0f, 0x07, 0x00, 0x07, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03d9, 0x03d5, 0x03d1, 0x0429, 0x042d, 0x0431),
  },
  {	.freq			= 2457,
	RADIOREGS(0x66, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x99,
		  0x09, 0x0f, 0x06, 0x00, 0x06, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03db, 0x03d7, 0x03d3, 0x0427, 0x042b, 0x042f),
  },
  {	.freq			= 2462,
	RADIOREGS(0x69, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0x9e,
		  0x09, 0x0f, 0x06, 0x00, 0x06, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03dd, 0x03d9, 0x03d5, 0x0424, 0x0429, 0x042d),
  },
  {	.freq			= 2467,
	RADIOREGS(0x6c, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0xa3,
		  0x09, 0x0f, 0x05, 0x00, 0x05, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03df, 0x03db, 0x03d7, 0x0422, 0x0427, 0x042b),
  },
  {	.freq			= 2472,
	RADIOREGS(0x70, 0x16, 0x30, 0x1b, 0x0a, 0x0a, 0x30, 0xa8,
		  0x09, 0x0f, 0x05, 0x00, 0x05, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x61, 0x03,
		  0x00, 0x00, 0x00, 0xf0, 0x00),
	PHYREGS(0x03e1, 0x03dd, 0x03d9, 0x0420, 0x0424, 0x0429),
  },
};

const struct b43_phy_ht_channeltab_e_radio2059
*b43_phy_ht_get_channeltab_e_r2059(struct b43_wldev *dev, u16 freq)
{
	const struct b43_phy_ht_channeltab_e_radio2059 *e;
	unsigned int i;

	e = b43_phy_ht_channeltab_radio2059;
	for (i = 0; i < ARRAY_SIZE(b43_phy_ht_channeltab_radio2059); i++, e++) {
		if (e->freq == freq)
			return e;
	}

	return NULL;
}
