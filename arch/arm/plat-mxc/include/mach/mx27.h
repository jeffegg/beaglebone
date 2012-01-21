/*
 * Copyright 2004-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 *
 * This contains i.MX27-specific hardware definitions. For those
 * hardware pieces that are common between i.MX21 and i.MX27, have a
 * look at mx2x.h.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef __MACH_MX27_H__
#define __MACH_MX27_H__

#ifndef __ASSEMBLER__
#include <linux/io.h>
#endif

#define MX27_AIPI_BASE_ADDR		0x10000000
#define MX27_AIPI_SIZE			SZ_1M
#define MX27_DMA_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x01000)
#define MX27_WDOG_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x02000)
#define MX27_GPT1_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x03000)
#define MX27_GPT2_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x04000)
#define MX27_GPT3_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x05000)
#define MX27_PWM_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x06000)
#define MX27_RTC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x07000)
#define MX27_KPP_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x08000)
#define MX27_OWIRE_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x09000)
#define MX27_UART1_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x0a000)
#define MX27_UART2_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x0b000)
#define MX27_UART3_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x0c000)
#define MX27_UART4_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x0d000)
#define MX27_CSPI1_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x0e000)
#define MX27_CSPI2_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x0f000)
#define MX27_SSI1_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x10000)
#define MX27_SSI2_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x11000)
#define MX27_I2C1_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x12000)
#define MX27_SDHC1_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x13000)
#define MX27_SDHC2_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x14000)
#define MX27_GPIO_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x15000)
#define MX27_GPIO1_BASE_ADDR			(MX27_GPIO_BASE_ADDR + 0x000)
#define MX27_GPIO2_BASE_ADDR			(MX27_GPIO_BASE_ADDR + 0x100)
#define MX27_GPIO3_BASE_ADDR			(MX27_GPIO_BASE_ADDR + 0x200)
#define MX27_GPIO4_BASE_ADDR			(MX27_GPIO_BASE_ADDR + 0x300)
#define MX27_GPIO5_BASE_ADDR			(MX27_GPIO_BASE_ADDR + 0x400)
#define MX27_GPIO6_BASE_ADDR			(MX27_GPIO_BASE_ADDR + 0x500)
#define MX27_AUDMUX_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x16000)
#define MX27_CSPI3_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x17000)
#define MX27_MSHC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x18000)
#define MX27_GPT4_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x19000)
#define MX27_GPT5_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x1a000)
#define MX27_UART5_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x1b000)
#define MX27_UART6_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x1c000)
#define MX27_I2C2_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x1d000)
#define MX27_SDHC3_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x1e000)
#define MX27_GPT6_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x1f000)
#define MX27_LCDC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x21000)
#define MX27_SLCDC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x22000)
#define MX27_VPU_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x23000)
#define MX27_USB_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x24000)
#define MX27_USB_OTG_BASE_ADDR			(MX27_USB_BASE_ADDR + 0x0000)
#define MX27_USB_HS1_BASE_ADDR			(MX27_USB_BASE_ADDR + 0x0200)
#define MX27_USB_HS2_BASE_ADDR			(MX27_USB_BASE_ADDR + 0x0400)
#define MX27_SAHARA_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x25000)
#define MX27_EMMAPP_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x26000)
#define MX27_EMMAPRP_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x26400)
#define MX27_CCM_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x27000)
#define MX27_SYSCTRL_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x27800)
#define MX27_IIM_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x28000)
#define MX27_RTIC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x2a000)
#define MX27_FEC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x2b000)
#define MX27_SCC_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x2c000)
#define MX27_ETB_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x3b000)
#define MX27_ETB_RAM_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x3c000)
#define MX27_JAM_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x3e000)
#define MX27_MAX_BASE_ADDR			(MX27_AIPI_BASE_ADDR + 0x3f000)

#define MX27_AVIC_BASE_ADDR		0x10040000

/* ROM patch */
#define MX27_ROMP_BASE_ADDR		0x10041000

#define MX27_SAHB1_BASE_ADDR		0x80000000
#define MX27_SAHB1_SIZE			SZ_1M
#define MX27_CSI_BASE_ADDR			(MX27_SAHB1_BASE_ADDR + 0x0000)
#define MX27_ATA_BASE_ADDR			(MX27_SAHB1_BASE_ADDR + 0x1000)

/* Memory regions and CS */
#define MX27_SDRAM_BASE_ADDR		0xa0000000
#define MX27_CSD1_BASE_ADDR		0xb0000000

#define MX27_CS0_BASE_ADDR		0xc0000000
#define MX27_CS1_BASE_ADDR		0xc8000000
#define MX27_CS2_BASE_ADDR		0xd0000000
#define MX27_CS3_BASE_ADDR		0xd2000000
#define MX27_CS4_BASE_ADDR		0xd4000000
#define MX27_CS5_BASE_ADDR		0xd6000000

/* NAND, SDRAM, WEIM, M3IF, EMI controllers */
#define MX27_X_MEMC_BASE_ADDR		0xd8000000
#define MX27_X_MEMC_SIZE		SZ_1M
#define MX27_NFC_BASE_ADDR			(MX27_X_MEMC_BASE_ADDR)
#define MX27_SDRAMC_BASE_ADDR			(MX27_X_MEMC_BASE_ADDR + 0x1000)
#define MX27_WEIM_BASE_ADDR			(MX27_X_MEMC_BASE_ADDR + 0x2000)
#define MX27_M3IF_BASE_ADDR			(MX27_X_MEMC_BASE_ADDR + 0x3000)
#define MX27_PCMCIA_CTL_BASE_ADDR		(MX27_X_MEMC_BASE_ADDR + 0x4000)

#define MX27_WEIM_CSCRx_BASE_ADDR(cs)	(MX27_WEIM_BASE_ADDR + (cs) * 0x10)
#define MX27_WEIM_CSCRxU(cs)			(MX27_WEIM_CSCRx_BASE_ADDR(cs))
#define MX27_WEIM_CSCRxL(cs)			(MX27_WEIM_CSCRx_BASE_ADDR(cs) + 0x4)
#define MX27_WEIM_CSCRxA(cs)			(MX27_WEIM_CSCRx_BASE_ADDR(cs) + 0x8)

#define MX27_PCMCIA_MEM_BASE_ADDR	0xdc000000

/* IRAM */
#define MX27_IRAM_BASE_ADDR		0xffff4c00	/* internal ram */

#define MX27_IO_P2V(x)			IMX_IO_P2V(x)
#define MX27_IO_ADDRESS(x)		IOMEM(MX27_IO_P2V(x))

#ifndef __ASSEMBLER__
static inline void mx27_setup_weimcs(size_t cs,
		unsigned upper, unsigned lower, unsigned addional)
{
	__raw_writel(upper, MX27_IO_ADDRESS(MX27_WEIM_CSCRxU(cs)));
	__raw_writel(lower, MX27_IO_ADDRESS(MX27_WEIM_CSCRxL(cs)));
	__raw_writel(addional, MX27_IO_ADDRESS(MX27_WEIM_CSCRxA(cs)));
}
#endif

/* fixed interrupt numbers */
#define MX27_INT_I2C2		1
#define MX27_INT_GPT6		2
#define MX27_INT_GPT5		3
#define MX27_INT_GPT4		4
#define MX27_INT_RTIC		5
#define MX27_INT_CSPI3		6
#define MX27_INT_SDHC		7
#define MX27_INT_GPIO		8
#define MX27_INT_SDHC3		9
#define MX27_INT_SDHC2		10
#define MX27_INT_SDHC1		11
#define MX27_INT_I2C1		12
#define MX27_INT_SSI2		13
#define MX27_INT_SSI1		14
#define MX27_INT_CSPI2		15
#define MX27_INT_CSPI1		16
#define MX27_INT_UART4		17
#define MX27_INT_UART3		18
#define MX27_INT_UART2		19
#define MX27_INT_UART1		20
#define MX27_INT_KPP		21
#define MX27_INT_RTC		22
#define MX27_INT_PWM		23
#define MX27_INT_GPT3		24
#define MX27_INT_GPT2		25
#define MX27_INT_GPT1		26
#define MX27_INT_WDOG		27
#define MX27_INT_PCMCIA		28
#define MX27_INT_NFC		29
#define MX27_INT_ATA		30
#define MX27_INT_CSI		31
#define MX27_INT_DMACH0		32
#define MX27_INT_DMACH1		33
#define MX27_INT_DMACH2		34
#define MX27_INT_DMACH3		35
#define MX27_INT_DMACH4		36
#define MX27_INT_DMACH5		37
#define MX27_INT_DMACH6		38
#define MX27_INT_DMACH7		39
#define MX27_INT_DMACH8		40
#define MX27_INT_DMACH9		41
#define MX27_INT_DMACH10	42
#define MX27_INT_DMACH11	43
#define MX27_INT_DMACH12	44
#define MX27_INT_DMACH13	45
#define MX27_INT_DMACH14	46
#define MX27_INT_DMACH15	47
#define MX27_INT_UART6		48
#define MX27_INT_UART5		49
#define MX27_INT_FEC		50
#define MX27_INT_EMMAPRP	51
#define MX27_INT_EMMAPP		52
#define MX27_INT_VPU		53
#define MX27_INT_USB_HS1	54
#define MX27_INT_USB_HS2	55
#define MX27_INT_USB_OTG	56
#define MX27_INT_SCC_SMN	57
#define MX27_INT_SCC_SCM	58
#define MX27_INT_SAHARA		59
#define MX27_INT_SLCDC		60
#define MX27_INT_LCDC		61
#define MX27_INT_IIM		62
#define MX27_INT_CCM		63

/* fixed DMA request numbers */
#define MX27_DMA_REQ_CSPI3_RX	1
#define MX27_DMA_REQ_CSPI3_TX	2
#define MX27_DMA_REQ_EXT	3
#define MX27_DMA_REQ_MSHC	4
#define MX27_DMA_REQ_SDHC2	6
#define MX27_DMA_REQ_SDHC1	7
#define MX27_DMA_REQ_SSI2_RX0	8
#define MX27_DMA_REQ_SSI2_TX0	9
#define MX27_DMA_REQ_SSI2_RX1	10
#define MX27_DMA_REQ_SSI2_TX1	11
#define MX27_DMA_REQ_SSI1_RX0	12
#define MX27_DMA_REQ_SSI1_TX0	13
#define MX27_DMA_REQ_SSI1_RX1	14
#define MX27_DMA_REQ_SSI1_TX1	15
#define MX27_DMA_REQ_CSPI2_RX	16
#define MX27_DMA_REQ_CSPI2_TX	17
#define MX27_DMA_REQ_CSPI1_RX	18
#define MX27_DMA_REQ_CSPI1_TX	19
#define MX27_DMA_REQ_UART4_RX	20
#define MX27_DMA_REQ_UART4_TX	21
#define MX27_DMA_REQ_UART3_RX	22
#define MX27_DMA_REQ_UART3_TX	23
#define MX27_DMA_REQ_UART2_RX	24
#define MX27_DMA_REQ_UART2_TX	25
#define MX27_DMA_REQ_UART1_RX	26
#define MX27_DMA_REQ_UART1_TX	27
#define MX27_DMA_REQ_ATA_TX	28
#define MX27_DMA_REQ_ATA_RCV	29
#define MX27_DMA_REQ_CSI_STAT	30
#define MX27_DMA_REQ_CSI_RX	31
#define MX27_DMA_REQ_UART5_TX	32
#define MX27_DMA_REQ_UART5_RX	33
#define MX27_DMA_REQ_UART6_TX	34
#define MX27_DMA_REQ_UART6_RX	35
#define MX27_DMA_REQ_SDHC3	36
#define MX27_DMA_REQ_NFC	37

#ifndef __ASSEMBLY__
extern int mx27_revision(void);
#endif

#endif /* ifndef __MACH_MX27_H__ */
