/*
 * Copyright (C) STMicroelectronics 2009
 * Copyright (C) ST-Ericsson SA 2010
 *
 * Author: Kumar Sanghvi <kumar.sanghvi@stericsson.com>
 * Author: Sundar Iyer <sundar.iyer@stericsson.com>
 *
 * License Terms: GNU General Public License v2
 *
 * PRCM Unit registers
 */
#ifndef __DB8500_PRCMU_REGS_H
#define __DB8500_PRCMU_REGS_H

#include <linux/bitops.h>
#include <mach/hardware.h>

#define BITS(_start, _end) ((BIT(_end) - BIT(_start)) + BIT(_end))

#define PRCM_ARM_PLLDIVPS 0x118
#define PRCM_ARM_PLLDIVPS_ARM_BRM_RATE	BITS(0, 5)
#define PRCM_ARM_PLLDIVPS_MAX_MASK	0xF

#define PRCM_PLLARM_LOCKP 0x0A8
#define PRCM_PLLARM_LOCKP_PRCM_PLLARM_LOCKP3 BIT(1)

#define PRCM_ARM_CHGCLKREQ 0x114
#define PRCM_ARM_CHGCLKREQ_PRCM_ARM_CHGCLKREQ BIT(0)

#define PRCM_PLLARM_ENABLE 0x98
#define PRCM_PLLARM_ENABLE_PRCM_PLLARM_ENABLE	BIT(0)
#define PRCM_PLLARM_ENABLE_PRCM_PLLARM_COUNTON	BIT(8)

#define PRCM_ARMCLKFIX_MGT	0x0
#define PRCM_A9_RESETN_CLR	0x1f4
#define PRCM_A9_RESETN_SET	0x1f0
#define PRCM_ARM_LS_CLAMP	0x30C
#define PRCM_SRAM_A9		0x308

/* ARM WFI Standby signal register */
#define PRCM_ARM_WFI_STANDBY	0x130
#define PRCM_IOCR		0x310
#define PRCM_IOCR_IOFORCE BIT(0)

/* CPU mailbox registers */
#define PRCM_MBOX_CPU_VAL 0x0FC
#define PRCM_MBOX_CPU_SET 0x100

/* Dual A9 core interrupt management unit registers */
#define PRCM_A9_MASK_REQ 0x328
#define PRCM_A9_MASK_REQ_PRCM_A9_MASK_REQ BIT(0)

#define PRCM_A9_MASK_ACK	0x32C
#define PRCM_ARMITMSK31TO0	0x11C
#define PRCM_ARMITMSK63TO32	0x120
#define PRCM_ARMITMSK95TO64	0x124
#define PRCM_ARMITMSK127TO96	0x128
#define PRCM_POWER_STATE_VAL	0x25C
#define PRCM_ARMITVAL31TO0	0x260
#define PRCM_ARMITVAL63TO32	0x264
#define PRCM_ARMITVAL95TO64	0x268
#define PRCM_ARMITVAL127TO96	0x26C

#define PRCM_HOSTACCESS_REQ 0x334
#define PRCM_HOSTACCESS_REQ_HOSTACCESS_REQ BIT(0)

#define PRCM_ARM_IT1_CLR 0x48C
#define PRCM_ARM_IT1_VAL 0x494

#define PRCM_ITSTATUS0		0x148
#define PRCM_ITSTATUS1		0x150
#define PRCM_ITSTATUS2		0x158
#define PRCM_ITSTATUS3		0x160
#define PRCM_ITSTATUS4		0x168
#define PRCM_ITSTATUS5		0x484
#define PRCM_ITCLEAR5		0x488
#define PRCM_ARMIT_MASKXP70_IT	0x1018

/* System reset register */
#define PRCM_APE_SOFTRST 0x228

/* Level shifter and clamp control registers */
#define PRCM_MMIP_LS_CLAMP_SET 0x420
#define PRCM_MMIP_LS_CLAMP_CLR 0x424

/* PRCMU HW semaphore */
#define PRCM_SEM 0x400
#define PRCM_SEM_PRCM_SEM BIT(0)

/* PRCMU clock/PLL/reset registers */
#define PRCM_PLLDSI_FREQ	0x500
#define PRCM_PLLDSI_ENABLE	0x504
#define PRCM_PLLDSI_LOCKP	0x508
#define PRCM_DSI_PLLOUT_SEL	0x530
#define PRCM_DSITVCLK_DIV	0x52C
#define PRCM_APE_RESETN_SET	0x1E4
#define PRCM_APE_RESETN_CLR	0x1E8

#define PRCM_TCR		0x1C8
#define PRCM_TCR_TENSEL_MASK	BITS(0, 7)
#define PRCM_TCR_STOP_TIMERS	BIT(16)
#define PRCM_TCR_DOZE_MODE	BIT(17)

#define PRCM_CLKOCR			0x1CC
#define PRCM_CLKOCR_CLKODIV0_SHIFT	0
#define PRCM_CLKOCR_CLKODIV0_MASK	BITS(0, 5)
#define PRCM_CLKOCR_CLKOSEL0_SHIFT	6
#define PRCM_CLKOCR_CLKOSEL0_MASK	BITS(6, 8)
#define PRCM_CLKOCR_CLKODIV1_SHIFT	16
#define PRCM_CLKOCR_CLKODIV1_MASK	BITS(16, 21)
#define PRCM_CLKOCR_CLKOSEL1_SHIFT	22
#define PRCM_CLKOCR_CLKOSEL1_MASK	BITS(22, 24)
#define PRCM_CLKOCR_CLK1TYPE		BIT(28)

#define PRCM_SGACLK_MGT		0x014
#define PRCM_UARTCLK_MGT	0x018
#define PRCM_MSP02CLK_MGT	0x01C
#define PRCM_MSP1CLK_MGT	0x288
#define PRCM_I2CCLK_MGT		0x020
#define PRCM_SDMMCCLK_MGT	0x024
#define PRCM_SLIMCLK_MGT	0x028
#define PRCM_PER1CLK_MGT	0x02C
#define PRCM_PER2CLK_MGT	0x030
#define PRCM_PER3CLK_MGT	0x034
#define PRCM_PER5CLK_MGT	0x038
#define PRCM_PER6CLK_MGT	0x03C
#define PRCM_PER7CLK_MGT	0x040
#define PRCM_LCDCLK_MGT		0x044
#define PRCM_BMLCLK_MGT		0x04C
#define PRCM_HSITXCLK_MGT	0x050
#define PRCM_HSIRXCLK_MGT	0x054
#define PRCM_HDMICLK_MGT	0x058
#define PRCM_APEATCLK_MGT	0x05C
#define PRCM_APETRACECLK_MGT	0x060
#define PRCM_MCDECLK_MGT	0x064
#define PRCM_IPI2CCLK_MGT	0x068
#define PRCM_DSIALTCLK_MGT	0x06C
#define PRCM_DMACLK_MGT		0x074
#define PRCM_B2R2CLK_MGT	0x078
#define PRCM_TVCLK_MGT		0x07C
#define PRCM_UNIPROCLK_MGT	0x278
#define PRCM_SSPCLK_MGT		0x280
#define PRCM_RNGCLK_MGT		0x284
#define PRCM_UICCCLK_MGT	0x27C

#define PRCM_CLK_MGT_CLKPLLDIV_MASK	BITS(0, 4)
#define PRCM_CLK_MGT_CLKPLLSW_MASK	BITS(5, 7)
#define PRCM_CLK_MGT_CLKEN		BIT(8)

/* ePOD and memory power signal control registers */
#define PRCM_EPOD_C_SET		0x410
#define PRCM_SRAM_LS_SLEEP	0x304

/* Debug power control unit registers */
#define PRCM_POWER_STATE_SET 0x254

/* Miscellaneous unit registers */
#define PRCM_DSI_SW_RESET 0x324
#define PRCM_GPIOCR		0x138

/* GPIOCR register */
#define PRCM_GPIOCR_SPI2_SELECT BIT(23)

#define PRCM_DDR_SUBSYS_APE_MINBW  0x438

#endif /* __DB8500_PRCMU_REGS_H */
