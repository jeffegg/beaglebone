/*
 *
 * arch/arm/mach-u300/core.c
 *
 *
 * Copyright (C) 2007-2010 ST-Ericsson SA
 * License terms: GNU General Public License (GPL) version 2
 * Core platform support, IRQ handling and device definitions.
 * Author: Linus Walleij <linus.walleij@stericsson.com>
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/termios.h>
#include <linux/dmaengine.h>
#include <linux/amba/bus.h>
#include <linux/amba/serial.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/fsmc.h>

#include <asm/types.h>
#include <asm/setup.h>
#include <asm/memory.h>
#include <asm/hardware/vic.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/coh901318.h>
#include <mach/hardware.h>
#include <mach/syscon.h>
#include <mach/dma_channels.h>

#include "clock.h"
#include "mmc.h"
#include "spi.h"
#include "i2c.h"

/*
 * Static I/O mappings that are needed for booting the U300 platforms. The
 * only things we need are the areas where we find the timer, syscon and
 * intcon, since the remaining device drivers will map their own memory
 * physical to virtual as the need arise.
 */
static struct map_desc u300_io_desc[] __initdata = {
	{
		.virtual	= U300_SLOW_PER_VIRT_BASE,
		.pfn		= __phys_to_pfn(U300_SLOW_PER_PHYS_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= U300_AHB_PER_VIRT_BASE,
		.pfn		= __phys_to_pfn(U300_AHB_PER_PHYS_BASE),
		.length		= SZ_32K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= U300_FAST_PER_VIRT_BASE,
		.pfn		= __phys_to_pfn(U300_FAST_PER_PHYS_BASE),
		.length		= SZ_32K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= 0xffff2000, /* TCM memory */
		.pfn		= __phys_to_pfn(0xffff2000),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},

	/*
	 * This overlaps with the IRQ vectors etc at 0xffff0000, so these
	 * may have to be moved to 0x00000000 in order to use the ROM.
	 */
	/*
	{
		.virtual	= U300_BOOTROM_VIRT_BASE,
		.pfn		= __phys_to_pfn(U300_BOOTROM_PHYS_BASE),
		.length		= SZ_64K,
		.type		= MT_ROM,
	},
	*/
};

void __init u300_map_io(void)
{
	iotable_init(u300_io_desc, ARRAY_SIZE(u300_io_desc));
}

/*
 * Declaration of devices found on the U300 board and
 * their respective memory locations.
 */

static struct amba_pl011_data uart0_plat_data = {
#ifdef CONFIG_COH901318
	.dma_filter = coh901318_filter_id,
	.dma_rx_param = (void *) U300_DMA_UART0_RX,
	.dma_tx_param = (void *) U300_DMA_UART0_TX,
#endif
};

static struct amba_device uart0_device = {
	.dev = {
		.coherent_dma_mask = ~0,
		.init_name = "uart0", /* Slow device at 0x3000 offset */
		.platform_data = &uart0_plat_data,
	},
	.res = {
		.start = U300_UART0_BASE,
		.end   = U300_UART0_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	.irq = { IRQ_U300_UART0, NO_IRQ },
};

/* The U335 have an additional UART1 on the APP CPU */
#ifdef CONFIG_MACH_U300_BS335
static struct amba_pl011_data uart1_plat_data = {
#ifdef CONFIG_COH901318
	.dma_filter = coh901318_filter_id,
	.dma_rx_param = (void *) U300_DMA_UART1_RX,
	.dma_tx_param = (void *) U300_DMA_UART1_TX,
#endif
};

static struct amba_device uart1_device = {
	.dev = {
		.coherent_dma_mask = ~0,
		.init_name = "uart1", /* Fast device at 0x7000 offset */
		.platform_data = &uart1_plat_data,
	},
	.res = {
		.start = U300_UART1_BASE,
		.end   = U300_UART1_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	.irq = { IRQ_U300_UART1, NO_IRQ },
};
#endif

static struct amba_device pl172_device = {
	.dev = {
		.init_name = "pl172", /* AHB device at 0x4000 offset */
		.platform_data = NULL,
	},
	.res = {
		.start = U300_EMIF_CFG_BASE,
		.end   = U300_EMIF_CFG_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};


/*
 * Everything within this next ifdef deals with external devices connected to
 * the APP SPI bus.
 */
static struct amba_device pl022_device = {
	.dev = {
		.coherent_dma_mask = ~0,
		.init_name = "pl022", /* Fast device at 0x6000 offset */
	},
	.res = {
		.start = U300_SPI_BASE,
		.end   = U300_SPI_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	.irq = {IRQ_U300_SPI, NO_IRQ },
	/*
	 * This device has a DMA channel but the Linux driver does not use
	 * it currently.
	 */
};

static struct amba_device mmcsd_device = {
	.dev = {
		.init_name = "mmci", /* Fast device at 0x1000 offset */
		.platform_data = NULL, /* Added later */
	},
	.res = {
		.start = U300_MMCSD_BASE,
		.end   = U300_MMCSD_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	.irq = {IRQ_U300_MMCSD_MCIINTR0, IRQ_U300_MMCSD_MCIINTR1 },
	/*
	 * This device has a DMA channel but the Linux driver does not use
	 * it currently.
	 */
};

/*
 * The order of device declaration may be important, since some devices
 * have dependencies on other devices being initialized first.
 */
static struct amba_device *amba_devs[] __initdata = {
	&uart0_device,
#ifdef CONFIG_MACH_U300_BS335
	&uart1_device,
#endif
	&pl022_device,
	&pl172_device,
	&mmcsd_device,
};

/* Here follows a list of all hw resources that the platform devices
 * allocate. Note, clock dependencies are not included
 */

static struct resource gpio_resources[] = {
	{
		.start = U300_GPIO_BASE,
		.end   = (U300_GPIO_BASE + SZ_4K - 1),
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "gpio0",
		.start = IRQ_U300_GPIO_PORT0,
		.end   = IRQ_U300_GPIO_PORT0,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "gpio1",
		.start = IRQ_U300_GPIO_PORT1,
		.end   = IRQ_U300_GPIO_PORT1,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "gpio2",
		.start = IRQ_U300_GPIO_PORT2,
		.end   = IRQ_U300_GPIO_PORT2,
		.flags = IORESOURCE_IRQ,
	},
#ifdef U300_COH901571_3
	{
		.name  = "gpio3",
		.start = IRQ_U300_GPIO_PORT3,
		.end   = IRQ_U300_GPIO_PORT3,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "gpio4",
		.start = IRQ_U300_GPIO_PORT4,
		.end   = IRQ_U300_GPIO_PORT4,
		.flags = IORESOURCE_IRQ,
	},
#ifdef CONFIG_MACH_U300_BS335
	{
		.name  = "gpio5",
		.start = IRQ_U300_GPIO_PORT5,
		.end   = IRQ_U300_GPIO_PORT5,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "gpio6",
		.start = IRQ_U300_GPIO_PORT6,
		.end   = IRQ_U300_GPIO_PORT6,
		.flags = IORESOURCE_IRQ,
	},
#endif /* CONFIG_MACH_U300_BS335 */
#endif /* U300_COH901571_3 */
};

static struct resource keypad_resources[] = {
	{
		.start = U300_KEYPAD_BASE,
		.end   = U300_KEYPAD_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "coh901461-press",
		.start = IRQ_U300_KEYPAD_KEYBF,
		.end   = IRQ_U300_KEYPAD_KEYBF,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "coh901461-release",
		.start = IRQ_U300_KEYPAD_KEYBR,
		.end   = IRQ_U300_KEYPAD_KEYBR,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource rtc_resources[] = {
	{
		.start = U300_RTC_BASE,
		.end   = U300_RTC_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = IRQ_U300_RTC,
		.end   = IRQ_U300_RTC,
		.flags = IORESOURCE_IRQ,
	},
};

/*
 * Fsmc does have IRQs: #43 and #44 (NFIF and NFIF2)
 * but these are not yet used by the driver.
 */
static struct resource fsmc_resources[] = {
	{
		.name  = "nand_data",
		.start = U300_NAND_CS0_PHYS_BASE,
		.end   = U300_NAND_CS0_PHYS_BASE + SZ_16K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "fsmc_regs",
		.start = U300_NAND_IF_PHYS_BASE,
		.end   = U300_NAND_IF_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct resource i2c0_resources[] = {
	{
		.start = U300_I2C0_BASE,
		.end   = U300_I2C0_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = IRQ_U300_I2C0,
		.end   = IRQ_U300_I2C0,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource i2c1_resources[] = {
	{
		.start = U300_I2C1_BASE,
		.end   = U300_I2C1_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = IRQ_U300_I2C1,
		.end   = IRQ_U300_I2C1,
		.flags = IORESOURCE_IRQ,
	},

};

static struct resource wdog_resources[] = {
	{
		.start = U300_WDOG_BASE,
		.end   = U300_WDOG_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = IRQ_U300_WDOG,
		.end   = IRQ_U300_WDOG,
		.flags = IORESOURCE_IRQ,
	}
};

/* TODO: These should be protected by suitable #ifdef's */
static struct resource ave_resources[] = {
	{
		.name  = "AVE3e I/O Area",
		.start = U300_VIDEOENC_BASE,
		.end   = U300_VIDEOENC_BASE + SZ_512K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "AVE3e IRQ0",
		.start = IRQ_U300_VIDEO_ENC_0,
		.end   = IRQ_U300_VIDEO_ENC_0,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "AVE3e IRQ1",
		.start = IRQ_U300_VIDEO_ENC_1,
		.end   = IRQ_U300_VIDEO_ENC_1,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name  = "AVE3e Physmem Area",
		.start = 0, /* 0 will be remapped to reserved memory */
		.end   = SZ_1M - 1,
		.flags = IORESOURCE_MEM,
	},
	/*
	 * The AVE3e requires two regions of 256MB that it considers
	 * "invisible". The hardware will not be able to access these
	 * addresses, so they should never point to system RAM.
	 */
	{
		.name  = "AVE3e Reserved 0",
		.start = 0xd0000000,
		.end   = 0xd0000000 + SZ_256M - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "AVE3e Reserved 1",
		.start = 0xe0000000,
		.end   = 0xe0000000 + SZ_256M - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct resource dma_resource[] = {
	{
		.start = U300_DMAC_BASE,
		.end = U300_DMAC_BASE + PAGE_SIZE - 1,
		.flags =  IORESOURCE_MEM,
	},
	{
		.start = IRQ_U300_DMA,
		.end = IRQ_U300_DMA,
		.flags =  IORESOURCE_IRQ,
	}
};

#ifdef CONFIG_MACH_U300_BS335
/* points out all dma slave channels.
 * Syntax is [A1, B1, A2, B2, .... ,-1,-1]
 * Select all channels from A to B, end of list is marked with -1,-1
 */
static int dma_slave_channels[] = {
	U300_DMA_MSL_TX_0, U300_DMA_SPI_RX,
	U300_DMA_UART1_TX, U300_DMA_UART1_RX, -1, -1};

/* points out all dma memcpy channels. */
static int dma_memcpy_channels[] = {
	U300_DMA_GENERAL_PURPOSE_0, U300_DMA_GENERAL_PURPOSE_8, -1, -1};

#else /* CONFIG_MACH_U300_BS335 */

static int dma_slave_channels[] = {U300_DMA_MSL_TX_0, U300_DMA_SPI_RX, -1, -1};
static int dma_memcpy_channels[] = {
	U300_DMA_GENERAL_PURPOSE_0, U300_DMA_GENERAL_PURPOSE_10, -1, -1};

#endif

/** register dma for memory access
 *
 * active  1 means dma intends to access memory
 *         0 means dma wont access memory
 */
static void coh901318_access_memory_state(struct device *dev, bool active)
{
}

#define flags_memcpy_config (COH901318_CX_CFG_CH_DISABLE | \
			COH901318_CX_CFG_RM_MEMORY_TO_MEMORY | \
			COH901318_CX_CFG_LCR_DISABLE | \
			COH901318_CX_CFG_TC_IRQ_ENABLE | \
			COH901318_CX_CFG_BE_IRQ_ENABLE)
#define flags_memcpy_lli_chained (COH901318_CX_CTRL_TC_ENABLE | \
			COH901318_CX_CTRL_BURST_COUNT_32_BYTES | \
			COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS | \
			COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE | \
			COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS | \
			COH901318_CX_CTRL_DST_ADDR_INC_ENABLE | \
			COH901318_CX_CTRL_MASTER_MODE_M1RW | \
			COH901318_CX_CTRL_TCP_DISABLE | \
			COH901318_CX_CTRL_TC_IRQ_DISABLE | \
			COH901318_CX_CTRL_HSP_DISABLE | \
			COH901318_CX_CTRL_HSS_DISABLE | \
			COH901318_CX_CTRL_DDMA_LEGACY | \
			COH901318_CX_CTRL_PRDD_SOURCE)
#define flags_memcpy_lli (COH901318_CX_CTRL_TC_ENABLE | \
			COH901318_CX_CTRL_BURST_COUNT_32_BYTES | \
			COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS | \
			COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE | \
			COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS | \
			COH901318_CX_CTRL_DST_ADDR_INC_ENABLE | \
			COH901318_CX_CTRL_MASTER_MODE_M1RW | \
			COH901318_CX_CTRL_TCP_DISABLE | \
			COH901318_CX_CTRL_TC_IRQ_DISABLE | \
			COH901318_CX_CTRL_HSP_DISABLE | \
			COH901318_CX_CTRL_HSS_DISABLE | \
			COH901318_CX_CTRL_DDMA_LEGACY | \
			COH901318_CX_CTRL_PRDD_SOURCE)
#define flags_memcpy_lli_last (COH901318_CX_CTRL_TC_ENABLE | \
			COH901318_CX_CTRL_BURST_COUNT_32_BYTES | \
			COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS | \
			COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE | \
			COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS | \
			COH901318_CX_CTRL_DST_ADDR_INC_ENABLE | \
			COH901318_CX_CTRL_MASTER_MODE_M1RW | \
			COH901318_CX_CTRL_TCP_DISABLE | \
			COH901318_CX_CTRL_TC_IRQ_ENABLE | \
			COH901318_CX_CTRL_HSP_DISABLE | \
			COH901318_CX_CTRL_HSS_DISABLE | \
			COH901318_CX_CTRL_DDMA_LEGACY | \
			COH901318_CX_CTRL_PRDD_SOURCE)

const struct coh_dma_channel chan_config[U300_DMA_CHANNELS] = {
	{
		.number = U300_DMA_MSL_TX_0,
		.name = "MSL TX 0",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 0 * 0x40 + 0x20,
	},
	{
		.number = U300_DMA_MSL_TX_1,
		.name = "MSL TX 1",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 1 * 0x40 + 0x20,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
	},
	{
		.number = U300_DMA_MSL_TX_2,
		.name = "MSL TX 2",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 2 * 0x40 + 0x20,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.desc_nbr_max = 10,
	},
	{
		.number = U300_DMA_MSL_TX_3,
		.name = "MSL TX 3",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 3 * 0x40 + 0x20,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
	},
	{
		.number = U300_DMA_MSL_TX_4,
		.name = "MSL TX 4",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 4 * 0x40 + 0x20,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1R_M2W |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
	},
	{
		.number = U300_DMA_MSL_TX_5,
		.name = "MSL TX 5",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 5 * 0x40 + 0x20,
	},
	{
		.number = U300_DMA_MSL_TX_6,
		.name = "MSL TX 6",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 6 * 0x40 + 0x20,
	},
	{
		.number = U300_DMA_MSL_RX_0,
		.name = "MSL RX 0",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 0 * 0x40 + 0x220,
	},
	{
		.number = U300_DMA_MSL_RX_1,
		.name = "MSL RX 1",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 1 * 0x40 + 0x220,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_MSL_RX_2,
		.name = "MSL RX 2",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 2 * 0x40 + 0x220,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_MSL_RX_3,
		.name = "MSL RX 3",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 3 * 0x40 + 0x220,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_MSL_RX_4,
		.name = "MSL RX 4",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 4 * 0x40 + 0x220,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_MSL_RX_5,
		.name = "MSL RX 5",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 5 * 0x40 + 0x220,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_32_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M2R_M1W |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_DEMAND_DMA1 |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_MSL_RX_6,
		.name = "MSL RX 6",
		.priority_high = 0,
		.dev_addr = U300_MSL_BASE + 6 * 0x40 + 0x220,
	},
	/*
	 * Don't set up device address, burst count or size of src
	 * or dst bus for this peripheral - handled by PrimeCell
	 * DMA extension.
	 */
	{
		.number = U300_DMA_MMCSD_RX_TX,
		.name = "MMCSD RX TX",
		.priority_high = 0,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,

	},
	{
		.number = U300_DMA_MSPRO_TX,
		.name = "MSPRO TX",
		.priority_high = 0,
	},
	{
		.number = U300_DMA_MSPRO_RX,
		.name = "MSPRO RX",
		.priority_high = 0,
	},
	/*
	 * Don't set up device address, burst count or size of src
	 * or dst bus for this peripheral - handled by PrimeCell
	 * DMA extension.
	 */
	{
		.number = U300_DMA_UART0_TX,
		.name = "UART0 TX",
		.priority_high = 0,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
	},
	{
		.number = U300_DMA_UART0_RX,
		.name = "UART0 RX",
		.priority_high = 0,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
	},
	{
		.number = U300_DMA_APEX_TX,
		.name = "APEX TX",
		.priority_high = 0,
	},
	{
		.number = U300_DMA_APEX_RX,
		.name = "APEX RX",
		.priority_high = 0,
	},
	{
		.number = U300_DMA_PCM_I2S0_TX,
		.name = "PCM I2S0 TX",
		.priority_high = 1,
		.dev_addr = U300_PCM_I2S0_BASE + 0x14,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
	},
	{
		.number = U300_DMA_PCM_I2S0_RX,
		.name = "PCM I2S0 RX",
		.priority_high = 1,
		.dev_addr = U300_PCM_I2S0_BASE + 0x10,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_PCM_I2S1_TX,
		.name = "PCM I2S1 TX",
		.priority_high = 1,
		.dev_addr =  U300_PCM_I2S1_BASE + 0x14,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_SOURCE,
	},
	{
		.number = U300_DMA_PCM_I2S1_RX,
		.name = "PCM I2S1 RX",
		.priority_high = 1,
		.dev_addr = U300_PCM_I2S1_BASE + 0x10,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_DEST,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_BURST_COUNT_16_BYTES |
				COH901318_CX_CTRL_SRC_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_SRC_ADDR_INC_DISABLE |
				COH901318_CX_CTRL_DST_BUS_SIZE_32_BITS |
				COH901318_CX_CTRL_DST_ADDR_INC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_ENABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY |
				COH901318_CX_CTRL_PRDD_DEST,
	},
	{
		.number = U300_DMA_XGAM_CDI,
		.name = "XGAM CDI",
		.priority_high = 0,
	},
	{
		.number = U300_DMA_XGAM_PDI,
		.name = "XGAM PDI",
		.priority_high = 0,
	},
	/*
	 * Don't set up device address, burst count or size of src
	 * or dst bus for this peripheral - handled by PrimeCell
	 * DMA extension.
	 */
	{
		.number = U300_DMA_SPI_TX,
		.name = "SPI TX",
		.priority_high = 0,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
	},
	{
		.number = U300_DMA_SPI_RX,
		.name = "SPI RX",
		.priority_high = 0,
		.param.config = COH901318_CX_CFG_CH_DISABLE |
				COH901318_CX_CFG_LCR_DISABLE |
				COH901318_CX_CFG_TC_IRQ_ENABLE |
				COH901318_CX_CFG_BE_IRQ_ENABLE,
		.param.ctrl_lli_chained = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_DISABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,
		.param.ctrl_lli_last = 0 |
				COH901318_CX_CTRL_TC_ENABLE |
				COH901318_CX_CTRL_MASTER_MODE_M1RW |
				COH901318_CX_CTRL_TCP_DISABLE |
				COH901318_CX_CTRL_TC_IRQ_ENABLE |
				COH901318_CX_CTRL_HSP_ENABLE |
				COH901318_CX_CTRL_HSS_DISABLE |
				COH901318_CX_CTRL_DDMA_LEGACY,

	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_0,
		.name = "GENERAL 00",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_1,
		.name = "GENERAL 01",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_2,
		.name = "GENERAL 02",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_3,
		.name = "GENERAL 03",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_4,
		.name = "GENERAL 04",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_5,
		.name = "GENERAL 05",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_6,
		.name = "GENERAL 06",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_7,
		.name = "GENERAL 07",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_8,
		.name = "GENERAL 08",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
#ifdef CONFIG_MACH_U300_BS335
	{
		.number = U300_DMA_UART1_TX,
		.name = "UART1 TX",
		.priority_high = 0,
	},
	{
		.number = U300_DMA_UART1_RX,
		.name = "UART1 RX",
		.priority_high = 0,
	}
#else
	{
		.number = U300_DMA_GENERAL_PURPOSE_9,
		.name = "GENERAL 09",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	},
	{
		.number = U300_DMA_GENERAL_PURPOSE_10,
		.name = "GENERAL 10",
		.priority_high = 0,

		.param.config = flags_memcpy_config,
		.param.ctrl_lli_chained = flags_memcpy_lli_chained,
		.param.ctrl_lli = flags_memcpy_lli,
		.param.ctrl_lli_last = flags_memcpy_lli_last,
	}
#endif
};


static struct coh901318_platform coh901318_platform = {
	.chans_slave = dma_slave_channels,
	.chans_memcpy = dma_memcpy_channels,
	.access_memory_state = coh901318_access_memory_state,
	.chan_conf = chan_config,
	.max_channels = U300_DMA_CHANNELS,
};

static struct platform_device wdog_device = {
	.name = "coh901327_wdog",
	.id = -1,
	.num_resources = ARRAY_SIZE(wdog_resources),
	.resource = wdog_resources,
};

static struct platform_device i2c0_device = {
	.name = "stu300",
	.id = 0,
	.num_resources = ARRAY_SIZE(i2c0_resources),
	.resource = i2c0_resources,
};

static struct platform_device i2c1_device = {
	.name = "stu300",
	.id = 1,
	.num_resources = ARRAY_SIZE(i2c1_resources),
	.resource = i2c1_resources,
};

static struct platform_device gpio_device = {
	.name = "u300-gpio",
	.id = -1,
	.num_resources = ARRAY_SIZE(gpio_resources),
	.resource = gpio_resources,
};

static struct platform_device keypad_device = {
	.name = "keypad",
	.id = -1,
	.num_resources = ARRAY_SIZE(keypad_resources),
	.resource = keypad_resources,
};

static struct platform_device rtc_device = {
	.name = "rtc-coh901331",
	.id = -1,
	.num_resources = ARRAY_SIZE(rtc_resources),
	.resource = rtc_resources,
};

static struct mtd_partition u300_partitions[] = {
	{
		.name = "bootrecords",
		.offset = 0,
		.size = SZ_128K,
	},
	{
		.name = "free",
		.offset = SZ_128K,
		.size = 8064 * SZ_1K,
	},
	{
		.name = "platform",
		.offset = 8192 * SZ_1K,
		.size = 253952 * SZ_1K,
	},
};

static struct fsmc_nand_platform_data nand_platform_data = {
	.partitions = u300_partitions,
	.nr_partitions = ARRAY_SIZE(u300_partitions),
	.options = NAND_SKIP_BBTSCAN,
	.width = FSMC_NAND_BW8,
};

static struct platform_device nand_device = {
	.name = "fsmc-nand",
	.id = -1,
	.resource = fsmc_resources,
	.num_resources = ARRAY_SIZE(fsmc_resources),
	.dev = {
		.platform_data = &nand_platform_data,
	},
};

static struct platform_device ave_device = {
	.name = "video_enc",
	.id = -1,
	.num_resources = ARRAY_SIZE(ave_resources),
	.resource = ave_resources,
};

static struct platform_device dma_device = {
	.name		= "coh901318",
	.id		= -1,
	.resource	= dma_resource,
	.num_resources  = ARRAY_SIZE(dma_resource),
	.dev = {
		.platform_data = &coh901318_platform,
		.coherent_dma_mask = ~0,
	},
};

/*
 * Notice that AMBA devices are initialized before platform devices.
 *
 */
static struct platform_device *platform_devs[] __initdata = {
	&dma_device,
	&i2c0_device,
	&i2c1_device,
	&keypad_device,
	&rtc_device,
	&gpio_device,
	&nand_device,
	&wdog_device,
	&ave_device
};


/*
 * Interrupts: the U300 platforms have two pl190 ARM PrimeCells connected
 * together so some interrupts are connected to the first one and some
 * to the second one.
 */
void __init u300_init_irq(void)
{
	u32 mask[2] = {0, 0};
	struct clk *clk;
	int i;

	/* initialize clocking early, we want to clock the INTCON */
	u300_clock_init();

	/* Clock the interrupt controller */
	clk = clk_get_sys("intcon", NULL);
	BUG_ON(IS_ERR(clk));
	clk_enable(clk);

	for (i = 0; i < NR_IRQS; i++)
		set_bit(i, (unsigned long *) &mask[0]);
	vic_init((void __iomem *) U300_INTCON0_VBASE, 0, mask[0], mask[0]);
	vic_init((void __iomem *) U300_INTCON1_VBASE, 32, mask[1], mask[1]);
}


/*
 * U300 platforms peripheral handling
 */
struct db_chip {
	u16 chipid;
	const char *name;
};

/*
 * This is a list of the Digital Baseband chips used in the U300 platform.
 */
static struct db_chip db_chips[] __initdata = {
	{
		.chipid = 0xb800,
		.name = "DB3000",
	},
	{
		.chipid = 0xc000,
		.name = "DB3100",
	},
	{
		.chipid = 0xc800,
		.name = "DB3150",
	},
	{
		.chipid = 0xd800,
		.name = "DB3200",
	},
	{
		.chipid = 0xe000,
		.name = "DB3250",
	},
	{
		.chipid = 0xe800,
		.name = "DB3210",
	},
	{
		.chipid = 0xf000,
		.name = "DB3350 P1x",
	},
	{
		.chipid = 0xf100,
		.name = "DB3350 P2x",
	},
	{
		.chipid = 0x0000, /* List terminator */
		.name = NULL,
	}
};

static void __init u300_init_check_chip(void)
{

	u16 val;
	struct db_chip *chip;
	const char *chipname;
	const char unknown[] = "UNKNOWN";

	/* Read out and print chip ID */
	val = readw(U300_SYSCON_VBASE + U300_SYSCON_CIDR);
	/* This is in funky bigendian order... */
	val = (val & 0xFFU) << 8 | (val >> 8);
	chip = db_chips;
	chipname = unknown;

	for ( ; chip->chipid; chip++) {
		if (chip->chipid == (val & 0xFF00U)) {
			chipname = chip->name;
			break;
		}
	}
	printk(KERN_INFO "Initializing U300 system on %s baseband chip " \
	       "(chip ID 0x%04x)\n", chipname, val);

#ifdef CONFIG_MACH_U300_BS330
	if ((val & 0xFF00U) != 0xd800) {
		printk(KERN_ERR "Platform configured for BS330 " \
		       "with DB3200 but %s detected, expect problems!",
		       chipname);
	}
#endif
#ifdef CONFIG_MACH_U300_BS335
	if ((val & 0xFF00U) != 0xf000 && (val & 0xFF00U) != 0xf100) {
		printk(KERN_ERR "Platform configured for BS335 " \
		       " with DB3350 but %s detected, expect problems!",
		       chipname);
	}
#endif
#ifdef CONFIG_MACH_U300_BS365
	if ((val & 0xFF00U) != 0xe800) {
		printk(KERN_ERR "Platform configured for BS365 " \
		       "with DB3210 but %s detected, expect problems!",
		       chipname);
	}
#endif


}

/*
 * Some devices and their resources require reserved physical memory from
 * the end of the available RAM. This function traverses the list of devices
 * and assigns actual addresses to these.
 */
static void __init u300_assign_physmem(void)
{
	unsigned long curr_start = __pa(high_memory);
	int i, j;

	for (i = 0; i < ARRAY_SIZE(platform_devs); i++) {
		for (j = 0; j < platform_devs[i]->num_resources; j++) {
			struct resource *const res =
			  &platform_devs[i]->resource[j];

			if (IORESOURCE_MEM == res->flags &&
				     0 == res->start) {
				res->start  = curr_start;
				res->end   += curr_start;
				curr_start += resource_size(res);

				printk(KERN_INFO "core.c: Mapping RAM " \
				       "%#x-%#x to device %s:%s\n",
					res->start, res->end,
				       platform_devs[i]->name, res->name);
			}
		}
	}
}

void __init u300_init_devices(void)
{
	int i;
	u16 val;

	/* Check what platform we run and print some status information */
	u300_init_check_chip();

	/* Set system to run at PLL208, max performance, a known state. */
	val = readw(U300_SYSCON_VBASE + U300_SYSCON_CCR);
	val &= ~U300_SYSCON_CCR_CLKING_PERFORMANCE_MASK;
	writew(val, U300_SYSCON_VBASE + U300_SYSCON_CCR);
	/* Wait for the PLL208 to lock if not locked in yet */
	while (!(readw(U300_SYSCON_VBASE + U300_SYSCON_CSR) &
		 U300_SYSCON_CSR_PLL208_LOCK_IND));
	/* Initialize SPI device with some board specifics */
	u300_spi_init(&pl022_device);

	/* Register the AMBA devices in the AMBA bus abstraction layer */
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
		struct amba_device *d = amba_devs[i];
		amba_device_register(d, &iomem_resource);
	}

	u300_assign_physmem();

	/* Register subdevices on the I2C buses */
	u300_i2c_register_board_devices();

	/* Register the platform devices */
	platform_add_devices(platform_devs, ARRAY_SIZE(platform_devs));

	/* Register subdevices on the SPI bus */
	u300_spi_register_board_devices();

#ifndef CONFIG_MACH_U300_SEMI_IS_SHARED
	/*
	 * Enable SEMI self refresh. Self-refresh of the SDRAM is entered when
	 * both subsystems are requesting this mode.
	 * If we not share the Acc SDRAM, this is never the case. Therefore
	 * enable it here from the App side.
	 */
	val = readw(U300_SYSCON_VBASE + U300_SYSCON_SMCR) |
		U300_SYSCON_SMCR_SEMI_SREFREQ_ENABLE;
	writew(val, U300_SYSCON_VBASE + U300_SYSCON_SMCR);
#endif /* CONFIG_MACH_U300_SEMI_IS_SHARED */
}

static int core_module_init(void)
{
	/*
	 * This needs to be initialized later: it needs the input framework
	 * to be initialized first.
	 */
	return mmc_init(&mmcsd_device);
}
module_init(core_module_init);
