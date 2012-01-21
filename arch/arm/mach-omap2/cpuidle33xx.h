/*
 * TI AM33XX cpuidle platform support
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _AM33XX_CPUIDLE_H
#define _AM33XX_CPUIDLE_H

struct am33xx_cpuidle_config {
	u32 ddr2_pdown;
	void __iomem *emif_base;
};

#endif
