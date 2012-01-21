/* arch/arm/mach-s3c2410/include/mach/dma.h
 *
 * Copyright (C) 2003-2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Samsung S3C24XX DMA support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/* DMA Register definitions */

#define S3C2410_DMA_DISRC		(0x00)
#define S3C2410_DMA_DISRCC		(0x04)
#define S3C2410_DMA_DIDST		(0x08)
#define S3C2410_DMA_DIDSTC		(0x0C)
#define S3C2410_DMA_DCON		(0x10)
#define S3C2410_DMA_DSTAT		(0x14)
#define S3C2410_DMA_DCSRC		(0x18)
#define S3C2410_DMA_DCDST		(0x1C)
#define S3C2410_DMA_DMASKTRIG		(0x20)
#define S3C2412_DMA_DMAREQSEL		(0x24)
#define S3C2443_DMA_DMAREQSEL		(0x24)

#define S3C2410_DISRCC_INC		(1<<0)
#define S3C2410_DISRCC_APB		(1<<1)

#define S3C2410_DMASKTRIG_STOP		(1<<2)
#define S3C2410_DMASKTRIG_ON		(1<<1)
#define S3C2410_DMASKTRIG_SWTRIG	(1<<0)

#define S3C2410_DCON_DEMAND		(0<<31)
#define S3C2410_DCON_HANDSHAKE		(1<<31)
#define S3C2410_DCON_SYNC_PCLK		(0<<30)
#define S3C2410_DCON_SYNC_HCLK		(1<<30)

#define S3C2410_DCON_INTREQ		(1<<29)

#define S3C2410_DCON_CH0_XDREQ0		(0<<24)
#define S3C2410_DCON_CH0_UART0		(1<<24)
#define S3C2410_DCON_CH0_SDI		(2<<24)
#define S3C2410_DCON_CH0_TIMER		(3<<24)
#define S3C2410_DCON_CH0_USBEP1		(4<<24)

#define S3C2410_DCON_CH1_XDREQ1		(0<<24)
#define S3C2410_DCON_CH1_UART1		(1<<24)
#define S3C2410_DCON_CH1_I2SSDI		(2<<24)
#define S3C2410_DCON_CH1_SPI		(3<<24)
#define S3C2410_DCON_CH1_USBEP2		(4<<24)

#define S3C2410_DCON_CH2_I2SSDO		(0<<24)
#define S3C2410_DCON_CH2_I2SSDI		(1<<24)
#define S3C2410_DCON_CH2_SDI		(2<<24)
#define S3C2410_DCON_CH2_TIMER		(3<<24)
#define S3C2410_DCON_CH2_USBEP3		(4<<24)

#define S3C2410_DCON_CH3_UART2		(0<<24)
#define S3C2410_DCON_CH3_SDI		(1<<24)
#define S3C2410_DCON_CH3_SPI		(2<<24)
#define S3C2410_DCON_CH3_TIMER		(3<<24)
#define S3C2410_DCON_CH3_USBEP4		(4<<24)

#define S3C2410_DCON_SRCSHIFT		(24)
#define S3C2410_DCON_SRCMASK		(7<<24)

#define S3C2410_DCON_BYTE		(0<<20)
#define S3C2410_DCON_HALFWORD		(1<<20)
#define S3C2410_DCON_WORD		(2<<20)

#define S3C2410_DCON_AUTORELOAD		(0<<22)
#define S3C2410_DCON_NORELOAD		(1<<22)
#define S3C2410_DCON_HWTRIG		(1<<23)

#ifdef CONFIG_CPU_S3C2440
#define S3C2440_DIDSTC_CHKINT		(1<<2)

#define S3C2440_DCON_CH0_I2SSDO		(5<<24)
#define S3C2440_DCON_CH0_PCMIN		(6<<24)

#define S3C2440_DCON_CH1_PCMOUT		(5<<24)
#define S3C2440_DCON_CH1_SDI		(6<<24)

#define S3C2440_DCON_CH2_PCMIN		(5<<24)
#define S3C2440_DCON_CH2_MICIN		(6<<24)

#define S3C2440_DCON_CH3_MICIN		(5<<24)
#define S3C2440_DCON_CH3_PCMOUT		(6<<24)
#endif

#ifdef CONFIG_CPU_S3C2412

#define S3C2412_DMAREQSEL_SRC(x)	((x)<<1)

#define S3C2412_DMAREQSEL_HW		(1)

#define S3C2412_DMAREQSEL_SPI0TX	S3C2412_DMAREQSEL_SRC(0)
#define S3C2412_DMAREQSEL_SPI0RX	S3C2412_DMAREQSEL_SRC(1)
#define S3C2412_DMAREQSEL_SPI1TX	S3C2412_DMAREQSEL_SRC(2)
#define S3C2412_DMAREQSEL_SPI1RX	S3C2412_DMAREQSEL_SRC(3)
#define S3C2412_DMAREQSEL_I2STX		S3C2412_DMAREQSEL_SRC(4)
#define S3C2412_DMAREQSEL_I2SRX		S3C2412_DMAREQSEL_SRC(5)
#define S3C2412_DMAREQSEL_TIMER		S3C2412_DMAREQSEL_SRC(9)
#define S3C2412_DMAREQSEL_SDI		S3C2412_DMAREQSEL_SRC(10)
#define S3C2412_DMAREQSEL_USBEP1	S3C2412_DMAREQSEL_SRC(13)
#define S3C2412_DMAREQSEL_USBEP2	S3C2412_DMAREQSEL_SRC(14)
#define S3C2412_DMAREQSEL_USBEP3	S3C2412_DMAREQSEL_SRC(15)
#define S3C2412_DMAREQSEL_USBEP4	S3C2412_DMAREQSEL_SRC(16)
#define S3C2412_DMAREQSEL_XDREQ0	S3C2412_DMAREQSEL_SRC(17)
#define S3C2412_DMAREQSEL_XDREQ1	S3C2412_DMAREQSEL_SRC(18)
#define S3C2412_DMAREQSEL_UART0_0	S3C2412_DMAREQSEL_SRC(19)
#define S3C2412_DMAREQSEL_UART0_1	S3C2412_DMAREQSEL_SRC(20)
#define S3C2412_DMAREQSEL_UART1_0	S3C2412_DMAREQSEL_SRC(21)
#define S3C2412_DMAREQSEL_UART1_1	S3C2412_DMAREQSEL_SRC(22)
#define S3C2412_DMAREQSEL_UART2_0	S3C2412_DMAREQSEL_SRC(23)
#define S3C2412_DMAREQSEL_UART2_1	S3C2412_DMAREQSEL_SRC(24)

#endif

#define S3C2443_DMAREQSEL_SRC(x)	((x)<<1)

#define S3C2443_DMAREQSEL_HW		(1)

#define S3C2443_DMAREQSEL_SPI0TX	S3C2443_DMAREQSEL_SRC(0)
#define S3C2443_DMAREQSEL_SPI0RX	S3C2443_DMAREQSEL_SRC(1)
#define S3C2443_DMAREQSEL_SPI1TX	S3C2443_DMAREQSEL_SRC(2)
#define S3C2443_DMAREQSEL_SPI1RX	S3C2443_DMAREQSEL_SRC(3)
#define S3C2443_DMAREQSEL_I2STX		S3C2443_DMAREQSEL_SRC(4)
#define S3C2443_DMAREQSEL_I2SRX		S3C2443_DMAREQSEL_SRC(5)
#define S3C2443_DMAREQSEL_TIMER		S3C2443_DMAREQSEL_SRC(9)
#define S3C2443_DMAREQSEL_SDI		S3C2443_DMAREQSEL_SRC(10)
#define S3C2443_DMAREQSEL_XDREQ0	S3C2443_DMAREQSEL_SRC(17)
#define S3C2443_DMAREQSEL_XDREQ1	S3C2443_DMAREQSEL_SRC(18)
#define S3C2443_DMAREQSEL_UART0_0	S3C2443_DMAREQSEL_SRC(19)
#define S3C2443_DMAREQSEL_UART0_1	S3C2443_DMAREQSEL_SRC(20)
#define S3C2443_DMAREQSEL_UART1_0	S3C2443_DMAREQSEL_SRC(21)
#define S3C2443_DMAREQSEL_UART1_1	S3C2443_DMAREQSEL_SRC(22)
#define S3C2443_DMAREQSEL_UART2_0	S3C2443_DMAREQSEL_SRC(23)
#define S3C2443_DMAREQSEL_UART2_1	S3C2443_DMAREQSEL_SRC(24)
#define S3C2443_DMAREQSEL_UART3_0	S3C2443_DMAREQSEL_SRC(25)
#define S3C2443_DMAREQSEL_UART3_1	S3C2443_DMAREQSEL_SRC(26)
#define S3C2443_DMAREQSEL_PCMOUT	S3C2443_DMAREQSEL_SRC(27)
#define S3C2443_DMAREQSEL_PCMIN 	S3C2443_DMAREQSEL_SRC(28)
#define S3C2443_DMAREQSEL_MICIN		S3C2443_DMAREQSEL_SRC(29)
