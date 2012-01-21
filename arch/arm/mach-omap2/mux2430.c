/*
 * Copyright (C) 2010 Nokia
 * Copyright (C) 2010 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>

#include "mux.h"

#ifdef CONFIG_OMAP_MUX

#define _OMAP2430_MUXENTRY(M0, g, m0, m1, m2, m3, m4, m5, m6, m7)		\
{									\
	.reg_offset	= (OMAP2430_CONTROL_PADCONF_##M0##_OFFSET),	\
	.gpio		= (g),						\
	.muxnames	= { m0, m1, m2, m3, m4, m5, m6, m7 },		\
}

#else

#define _OMAP2430_MUXENTRY(M0, g, m0, m1, m2, m3, m4, m5, m6, m7)		\
{									\
	.reg_offset	= (OMAP2430_CONTROL_PADCONF_##M0##_OFFSET),	\
	.gpio		= (g),						\
}

#endif

#define _OMAP2430_BALLENTRY(M0, bb, bt)					\
{									\
	.reg_offset	= (OMAP2430_CONTROL_PADCONF_##M0##_OFFSET),	\
	.balls		= { bb, bt },					\
}

/*
 * Superset of all mux modes for omap2430
 */
static struct omap_mux __initdata omap2430_muxmodes[] = {
	_OMAP2430_MUXENTRY(CAM_D0, 133,
		"cam_d0", "hw_dbg0", "sti_dout", "gpio_133",
		NULL, NULL, "etk_d2", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D10, 146,
		"cam_d10", NULL, NULL, "gpio_146",
		NULL, NULL, "etk_d12", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D11, 145,
		"cam_d11", NULL, NULL, "gpio_145",
		NULL, NULL, "etk_d13", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D1, 132,
		"cam_d1", "hw_dbg1", "sti_din", "gpio_132",
		NULL, NULL, "etk_d3", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D2, 129,
		"cam_d2", "hw_dbg2", "mcbsp1_clkx", "gpio_129",
		NULL, NULL, "etk_d4", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D3, 128,
		"cam_d3", "hw_dbg3", "mcbsp1_dr", "gpio_128",
		NULL, NULL, "etk_d5", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D4, 143,
		"cam_d4", "hw_dbg4", "mcbsp1_fsr", "gpio_143",
		NULL, NULL, "etk_d6", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D5, 112,
		"cam_d5", "hw_dbg5", "mcbsp1_clkr", "gpio_112",
		NULL, NULL, "etk_d7", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D6, 137,
		"cam_d6", "hw_dbg6", NULL, "gpio_137",
		NULL, NULL, "etk_d8", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D7, 136,
		"cam_d7", "hw_dbg7", NULL, "gpio_136",
		NULL, NULL, "etk_d9", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D8, 135,
		"cam_d8", "hw_dbg8", NULL, "gpio_135",
		NULL, NULL, "etk_d10", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_D9, 134,
		"cam_d9", "hw_dbg9", NULL, "gpio_134",
		NULL, NULL, "etk_d11", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_HS, 11,
		"cam_hs", "hw_dbg10", "mcbsp1_dx", "gpio_11",
		NULL, NULL, "etk_d1", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_LCLK, 0,
		"cam_lclk", NULL, "mcbsp_clks", NULL,
		NULL, NULL, "etk_c1", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_VS, 12,
		"cam_vs", "hw_dbg11", "mcbsp1_fsx", "gpio_12",
		NULL, NULL, "etk_d0", "safe_mode"),
	_OMAP2430_MUXENTRY(CAM_XCLK, 0,
		"cam_xclk", NULL, "sti_clk", NULL,
		NULL, NULL, "etk_c2", NULL),
	_OMAP2430_MUXENTRY(DSS_ACBIAS, 48,
		"dss_acbias", NULL, "mcbsp2_fsx", "gpio_48",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA0, 40,
		"dss_data0", "uart1_cts", NULL, "gpio_40",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA10, 128,
		"dss_data10", "sdi_data1n", NULL, "gpio_128",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA11, 129,
		"dss_data11", "sdi_data1p", NULL, "gpio_129",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA12, 130,
		"dss_data12", "sdi_data2n", NULL, "gpio_130",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA13, 131,
		"dss_data13", "sdi_data2p", NULL, "gpio_131",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA14, 132,
		"dss_data14", "sdi_data3n", NULL, "gpio_132",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA15, 133,
		"dss_data15", "sdi_data3p", NULL, "gpio_133",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA16, 46,
		"dss_data16", NULL, NULL, "gpio_46",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA17, 47,
		"dss_data17", NULL, NULL, "gpio_47",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA1, 41,
		"dss_data1", "uart1_rts", NULL, "gpio_41",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA2, 42,
		"dss_data2", "uart1_tx", NULL, "gpio_42",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA3, 43,
		"dss_data3", "uart1_rx", NULL, "gpio_43",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA4, 44,
		"dss_data4", "uart3_rx_irrx", NULL, "gpio_44",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA5, 45,
		"dss_data5", "uart3_tx_irtx", NULL, "gpio_45",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA6, 144,
		"dss_data6", NULL, NULL, "gpio_144",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA7, 147,
		"dss_data7", NULL, NULL, "gpio_147",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA8, 38,
		"dss_data8", NULL, NULL, "gpio_38",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_DATA9, 39,
		"dss_data9", NULL, NULL, "gpio_39",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(DSS_HSYNC, 110,
		"dss_hsync", NULL, NULL, "gpio_110",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_113, 113,
		"gpio_113", "mcbsp2_clkx", NULL, "gpio_113",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_114, 114,
		"gpio_114", "mcbsp2_fsx", NULL, "gpio_114",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_115, 115,
		"gpio_115", "mcbsp2_dr", NULL, "gpio_115",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_116, 116,
		"gpio_116", "mcbsp2_dx", NULL, "gpio_116",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_128, 128,
		"gpio_128", NULL, "sti_din", "gpio_128",
		NULL, "sys_boot0", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_129, 129,
		"gpio_129", NULL, "sti_dout", "gpio_129",
		NULL, "sys_boot1", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_130, 130,
		"gpio_130", NULL, NULL, "gpio_130",
		"jtag_emu2", "sys_boot2", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_131, 131,
		"gpio_131", NULL, NULL, "gpio_131",
		"jtag_emu3", "sys_boot3", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_132, 132,
		"gpio_132", NULL, NULL, "gpio_132",
		NULL, "sys_boot4", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_133, 133,
		"gpio_133", NULL, NULL, "gpio_133",
		NULL, "sys_boot5", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_134, 134,
		"gpio_134", "ccp_datn", NULL, "gpio_134",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_135, 135,
		"gpio_135", "ccp_datp", NULL, "gpio_135",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_136, 136,
		"gpio_136", "ccp_clkn", NULL, "gpio_136",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_137, 137,
		"gpio_137", "ccp_clkp", NULL, "gpio_137",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_138, 138,
		"gpio_138", "spi3_clk", NULL, "gpio_138",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_139, 139,
		"gpio_139", "spi3_cs0", "sys_ndmareq3", "gpio_139",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_140, 140,
		"gpio_140", "spi3_simo", "sys_ndmareq4", "gpio_140",
		NULL, NULL, "etk_d14", "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_141, 141,
		"gpio_141", "spi3_somi", NULL, "gpio_141",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_142, 142,
		"gpio_142", "spi3_cs1", "sys_ndmareq2", "gpio_142",
		NULL, NULL, "etk_d15", "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_148, 148,
		"gpio_148", "mcbsp5_fsx", NULL, "gpio_148",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_149, 149,
		"gpio_149", "mcbsp5_dx", NULL, "gpio_149",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_150, 150,
		"gpio_150", "mcbsp5_dr", NULL, "gpio_150",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_151, 151,
		"gpio_151", "sys_pwrok", NULL, "gpio_151",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_152, 152,
		"gpio_152", "uart1_cts", "sys_ndmareq1", "gpio_152",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_153, 153,
		"gpio_153", "uart1_rx", "sys_ndmareq0", "gpio_153",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_154, 154,
		"gpio_154", "mcbsp5_clkx", NULL, "gpio_154",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_63, 63,
		"gpio_63", "mcbsp4_clkx", NULL, "gpio_63",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_78, 78,
		"gpio_78", NULL, "uart2_rts", "gpio_78",
		"uart3_rts_sd", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_79, 79,
		"gpio_79", "secure_indicator", "uart2_tx", "gpio_79",
		"uart3_tx_irtx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_7, 7,
		"gpio_7", NULL, "uart2_cts", "gpio_7",
		"uart3_cts_rctx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPIO_80, 80,
		"gpio_80", NULL, "uart2_rx", "gpio_80",
		"uart3_rx_irrx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A10, 3,
		"gpmc_a10", NULL, "sys_ndmareq0", "gpio_3",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A1, 31,
		"gpmc_a1", NULL, NULL, "gpio_31",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A2, 30,
		"gpmc_a2", NULL, NULL, "gpio_30",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A3, 29,
		"gpmc_a3", NULL, NULL, "gpio_29",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A4, 49,
		"gpmc_a4", NULL, NULL, "gpio_49",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A5, 53,
		"gpmc_a5", NULL, NULL, "gpio_53",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A6, 52,
		"gpmc_a6", NULL, NULL, "gpio_52",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A7, 6,
		"gpmc_a7", NULL, NULL, "gpio_6",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A8, 5,
		"gpmc_a8", NULL, NULL, "gpio_5",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_A9, 4,
		"gpmc_a9", NULL, "sys_ndmareq1", "gpio_4",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_CLK, 21,
		"gpmc_clk", NULL, NULL, "gpio_21",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D10, 18,
		"gpmc_d10", NULL, NULL, "gpio_18",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D11, 57,
		"gpmc_d11", NULL, NULL, "gpio_57",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D12, 77,
		"gpmc_d12", NULL, NULL, "gpio_77",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D13, 76,
		"gpmc_d13", NULL, NULL, "gpio_76",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D14, 55,
		"gpmc_d14", NULL, NULL, "gpio_55",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D15, 54,
		"gpmc_d15", NULL, NULL, "gpio_54",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D8, 20,
		"gpmc_d8", NULL, NULL, "gpio_20",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_D9, 19,
		"gpmc_d9", NULL, NULL, "gpio_19",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS1, 22,
		"gpmc_ncs1", NULL, NULL, "gpio_22",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS2, 23,
		"gpmc_ncs2", NULL, NULL, "gpio_23",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS3, 24,
		"gpmc_ncs3", "gpmc_io_dir", NULL, "gpio_24",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS4, 25,
		"gpmc_ncs4", NULL, NULL, "gpio_25",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS5, 26,
		"gpmc_ncs5", NULL, NULL, "gpio_26",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS6, 27,
		"gpmc_ncs6", NULL, NULL, "gpio_27",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_NCS7, 28,
		"gpmc_ncs7", "gpmc_io_dir", NULL, "gpio_28",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_WAIT1, 33,
		"gpmc_wait1", NULL, NULL, "gpio_33",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_WAIT2, 34,
		"gpmc_wait2", NULL, NULL, "gpio_34",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(GPMC_WAIT3, 35,
		"gpmc_wait3", NULL, NULL, "gpio_35",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(HDQ_SIO, 101,
		"hdq_sio", "usb2_tllse0", "sys_altclk", "gpio_101",
		"uart3_rx_irrx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(I2C1_SCL, 50,
		"i2c1_scl", NULL, NULL, "gpio_50",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(I2C1_SDA, 51,
		"i2c1_sda", NULL, NULL, "gpio_51",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(I2C2_SCL, 99,
		"i2c2_scl", NULL, NULL, "gpio_99",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(I2C2_SDA, 100,
		"i2c2_sda", NULL, NULL, "gpio_100",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(JTAG_EMU0, 127,
		"jtag_emu0", "secure_indicator", NULL, "gpio_127",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(JTAG_EMU1, 126,
		"jtag_emu1", NULL, NULL, "gpio_126",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP1_CLKR, 92,
		"mcbsp1_clkr", "ssi2_dat_tx", NULL, "gpio_92",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP1_CLKX, 98,
		"mcbsp1_clkx", "ssi2_wake", NULL, "gpio_98",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP1_DR, 95,
		"mcbsp1_dr", "ssi2_dat_rx", NULL, "gpio_95",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP1_DX, 94,
		"mcbsp1_dx", "ssi2_rdy_tx", NULL, "gpio_94",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP1_FSR, 93,
		"mcbsp1_fsr", "ssi2_flag_tx", NULL, "gpio_93",
		"spi2_cs1", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP1_FSX, 97,
		"mcbsp1_fsx", "ssi2_rdy_rx", NULL, "gpio_97",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP2_CLKX, 147,
		"mcbsp2_clkx", "sdi_clkp", "dss_data23", "gpio_147",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP2_DR, 144,
		"mcbsp2_dr", "sdi_clkn", "dss_data22", "gpio_144",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP3_CLKX, 71,
		"mcbsp3_clkx", NULL, NULL, "gpio_71",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP3_DR, 73,
		"mcbsp3_dr", NULL, NULL, "gpio_73",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP3_DX, 74,
		"mcbsp3_dx", NULL, "sti_clk", "gpio_74",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP3_FSX, 72,
		"mcbsp3_fsx", NULL, NULL, "gpio_72",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(MCBSP_CLKS, 96,
		"mcbsp_clks", "ssi2_flag_rx", NULL, "gpio_96",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC1_CLKO, 0,
		"sdmmc1_clko", "ms_clko", NULL, NULL,
		NULL, "hw_dbg9", "hw_dbg3", "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC1_CMD, 0,
		"sdmmc1_cmd", "ms_bs", NULL, NULL,
		NULL, "hw_dbg8", "hw_dbg2", "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC1_DAT0, 0,
		"sdmmc1_dat0", "ms_dat0", NULL, NULL,
		NULL, "hw_dbg7", "hw_dbg1", "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC1_DAT1, 75,
		"sdmmc1_dat1", "ms_dat1", NULL, "gpio_75",
		NULL, "hw_dbg6", "hw_dbg0", "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC1_DAT2, 0,
		"sdmmc1_dat2", "ms_dat2", NULL, NULL,
		NULL, "hw_dbg5", "hw_dbg10", "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC1_DAT3, 0,
		"sdmmc1_dat3", "ms_dat3", NULL, NULL,
		NULL, "hw_dbg4", "hw_dbg11", "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC2_CLKO, 13,
		"sdmmc2_clko", NULL, NULL, "gpio_13",
		NULL, "spi3_clk", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC2_CMD, 15,
		"sdmmc2_cmd", "usb2_rcv", NULL, "gpio_15",
		NULL, "spi3_simo", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC2_DAT0, 16,
		"sdmmc2_dat0", "usb2_tllse0", NULL, "gpio_16",
		NULL, "spi3_somi", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC2_DAT1, 58,
		"sdmmc2_dat1", "usb2_txen", NULL, "gpio_58",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC2_DAT2, 17,
		"sdmmc2_dat2", "usb2_dat", NULL, "gpio_17",
		NULL, "spi3_cs1", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDMMC2_DAT3, 14,
		"sdmmc2_dat3", "usb2_se0", NULL, "gpio_14",
		NULL, "spi3_cs0", NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDRC_A12, 2,
		"sdrc_a12", NULL, NULL, "gpio_2",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDRC_A13, 1,
		"sdrc_a13", NULL, NULL, "gpio_1",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDRC_A14, 0,
		"sdrc_a14", NULL, NULL, "gpio_0",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDRC_CKE1, 36,
		"sdrc_cke1", NULL, NULL, "gpio_36",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SDRC_NCS1, 37,
		"sdrc_ncs1", NULL, NULL, "gpio_37",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_CLK, 81,
		"spi1_clk", NULL, NULL, "gpio_81",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_CS0, 84,
		"spi1_cs0", NULL, NULL, "gpio_84",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_CS1, 85,
		"spi1_cs1", NULL, NULL, "gpio_85",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_CS2, 86,
		"spi1_cs2", NULL, NULL, "gpio_86",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_CS3, 87,
		"spi1_cs3", "spi2_cs1", NULL, "gpio_87",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_SIMO, 82,
		"spi1_simo", NULL, NULL, "gpio_82",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI1_SOMI, 83,
		"spi1_somi", NULL, NULL, "gpio_83",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI2_CLK, 88,
		"spi2_clk", "gpt9_pwm_evt", NULL, "gpio_88",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI2_CS0, 91,
		"spi2_cs0", "gpt12_pwm_evt", NULL, "gpio_91",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI2_SIMO, 89,
		"spi2_simo", "gpt10_pwm_evt", NULL, "gpio_89",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SPI2_SOMI, 90,
		"spi2_somi", "gpt11_pwm_evt", NULL, "gpio_90",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_DAT_RX, 62,
		"ssi1_dat_rx", "uart1_rx", "usb1_dat", "gpio_62",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_DAT_TX, 59,
		"ssi1_dat_tx", "uart1_tx", "usb1_se0", "gpio_59",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_FLAG_RX, 64,
		"ssi1_flag_rx", "mcbsp4_dr", NULL, "gpio_64",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_FLAG_TX, 60,
		"ssi1_flag_tx", "uart1_rts", "usb1_rcv", "gpio_60",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_RDY_RX, 65,
		"ssi1_rdy_rx", "mcbsp4_dx", NULL, "gpio_65",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_RDY_TX, 61,
		"ssi1_rdy_tx", "uart1_cts", "usb1_txen", "gpio_61",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SSI1_WAKE, 66,
		"ssi1_wake", "mcbsp4_fsx", NULL, "gpio_66",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SYS_CLKOUT, 111,
		"sys_clkout", NULL, NULL, "gpio_111",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SYS_DRM_MSECURE, 118,
		"sys_drm_msecure", NULL, "sys_ndmareq6", "gpio_118",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SYS_NIRQ0, 56,
		"sys_nirq0", NULL, NULL, "gpio_56",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(SYS_NIRQ1, 125,
		"sys_nirq1", NULL, "sys_ndmareq5", "gpio_125",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART1_CTS, 32,
		"uart1_cts", "sdi_vsync", "dss_data18", "gpio_32",
		"mcbsp5_clkx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART1_RTS, 8,
		"uart1_rts", "sdi_hsync", "dss_data19", "gpio_8",
		"mcbsp5_fsx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART1_RX, 10,
		"uart1_rx", "sdi_stp", "dss_data21", "gpio_10",
		"mcbsp5_dr", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART1_TX, 9,
		"uart1_tx", "sdi_den", "dss_data20", "gpio_9",
		"mcbsp5_dx", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART2_CTS, 67,
		"uart2_cts", "usb1_rcv", "gpt9_pwm_evt", "gpio_67",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART2_RTS, 68,
		"uart2_rts", "usb1_txen", "gpt10_pwm_evt", "gpio_68",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART2_RX, 70,
		"uart2_rx", "usb1_dat", "gpt12_pwm_evt", "gpio_70",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART2_TX, 69,
		"uart2_tx", "usb1_se0", "gpt11_pwm_evt", "gpio_69",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART3_CTS_RCTX, 102,
		"uart3_cts_rctx", "uart3_rx_irrx", NULL, "gpio_102",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART3_RTS_SD, 103,
		"uart3_rts_sd", "uart3_tx_irtx", NULL, "gpio_103",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART3_RX_IRRX, 105,
		"uart3_rx_irrx", NULL, NULL, "gpio_105",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(UART3_TX_IRTX, 104,
		"uart3_tx_irtx", "uart3_cts_rctx", NULL, "gpio_104",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_CLK, 120,
		"usb0hs_clk", NULL, NULL, "gpio_120",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA0, 0,
		"usb0hs_data0", "uart3_tx_irtx", NULL, NULL,
		"usb0_txen", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA1, 0,
		"usb0hs_data1", "uart3_rx_irrx", NULL, NULL,
		"usb0_dat", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA2, 0,
		"usb0hs_data2", "uart3_rts_sd", NULL, NULL,
		"usb0_se0", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA3, 106,
		"usb0hs_data3", NULL, "uart3_cts_rctx", "gpio_106",
		"usb0_puen", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA4, 107,
		"usb0hs_data4", "mcbsp2_dr", NULL, "gpio_107",
		"usb0_vp", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA5, 108,
		"usb0hs_data5", "mcbsp2_dx", NULL, "gpio_108",
		"usb0_vm", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA6, 109,
		"usb0hs_data6", "mcbsp2_fsx", NULL, "gpio_109",
		"usb0_rcv", NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DATA7, 124,
		"usb0hs_data7", "mcbsp2_clkx", NULL, "gpio_124",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_DIR, 121,
		"usb0hs_dir", NULL, NULL, "gpio_121",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_NXT, 123,
		"usb0hs_nxt", NULL, NULL, "gpio_123",
		NULL, NULL, NULL, "safe_mode"),
	_OMAP2430_MUXENTRY(USB0HS_STP, 122,
		"usb0hs_stp", NULL, NULL, "gpio_122",
		NULL, NULL, NULL, "safe_mode"),
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};

/*
 * Balls for POP package
 * 447-pin s-PBGA Package, 0.00mm Ball Pitch (Bottom)
 */
#ifdef CONFIG_DEBUG_FS
static struct omap_ball __initdata omap2430_pop_ball[] = {
	_OMAP2430_BALLENTRY(CAM_D0, "t8", NULL),
	_OMAP2430_BALLENTRY(CAM_D1, "t4", NULL),
	_OMAP2430_BALLENTRY(CAM_D10, "r4", NULL),
	_OMAP2430_BALLENTRY(CAM_D11, "w3", NULL),
	_OMAP2430_BALLENTRY(CAM_D2, "r2", NULL),
	_OMAP2430_BALLENTRY(CAM_D3, "u3", NULL),
	_OMAP2430_BALLENTRY(CAM_D4, "u2", NULL),
	_OMAP2430_BALLENTRY(CAM_D5, "v1", NULL),
	_OMAP2430_BALLENTRY(CAM_D6, "t3", NULL),
	_OMAP2430_BALLENTRY(CAM_D7, "r3", NULL),
	_OMAP2430_BALLENTRY(CAM_D8, "u7", NULL),
	_OMAP2430_BALLENTRY(CAM_D9, "t7", NULL),
	_OMAP2430_BALLENTRY(CAM_HS, "p2", NULL),
	_OMAP2430_BALLENTRY(CAM_LCLK, "r7", NULL),
	_OMAP2430_BALLENTRY(CAM_VS, "n2", NULL),
	_OMAP2430_BALLENTRY(CAM_XCLK, "p3", NULL),
	_OMAP2430_BALLENTRY(DSS_ACBIAS, "y3", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA0, "v8", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA1, "w1", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA10, "k25", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA11, "j25", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA12, "k24", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA13, "j24", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA14, "h25", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA15, "g25", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA16, "ac3", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA17, "y7", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA2, "u8", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA3, "u4", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA4, "v3", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA5, "aa4", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA6, "w8", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA7, "y1", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA8, "aa2", NULL),
	_OMAP2430_BALLENTRY(DSS_DATA9, "ab4", NULL),
	_OMAP2430_BALLENTRY(DSS_HSYNC, "v2", NULL),
	_OMAP2430_BALLENTRY(GPIO_113, "ad16", NULL),
	_OMAP2430_BALLENTRY(GPIO_114, "ac10", NULL),
	_OMAP2430_BALLENTRY(GPIO_115, "ad13", NULL),
	_OMAP2430_BALLENTRY(GPIO_116, "ae15", NULL),
	_OMAP2430_BALLENTRY(GPIO_128, "p1", NULL),
	_OMAP2430_BALLENTRY(GPIO_129, "r1", NULL),
	_OMAP2430_BALLENTRY(GPIO_130, "p7", NULL),
	_OMAP2430_BALLENTRY(GPIO_131, "l8", NULL),
	_OMAP2430_BALLENTRY(GPIO_132, "w24", NULL),
	_OMAP2430_BALLENTRY(GPIO_133, "aa24", NULL),
	_OMAP2430_BALLENTRY(GPIO_134, "ae12", NULL),
	_OMAP2430_BALLENTRY(GPIO_135, "ae11", NULL),
	_OMAP2430_BALLENTRY(GPIO_136, "ad12", NULL),
	_OMAP2430_BALLENTRY(GPIO_137, "ad11", NULL),
	_OMAP2430_BALLENTRY(GPIO_138, "y12", NULL),
	_OMAP2430_BALLENTRY(GPIO_139, "ad17", NULL),
	_OMAP2430_BALLENTRY(GPIO_140, "l7", NULL),
	_OMAP2430_BALLENTRY(GPIO_141, "ac24", NULL),
	_OMAP2430_BALLENTRY(GPIO_142, "m3", NULL),
	_OMAP2430_BALLENTRY(GPIO_148, "af12", NULL),
	_OMAP2430_BALLENTRY(GPIO_149, "k7", NULL),
	_OMAP2430_BALLENTRY(GPIO_150, "m1", NULL),
	_OMAP2430_BALLENTRY(GPIO_151, "ad14", NULL),
	_OMAP2430_BALLENTRY(GPIO_152, "ad18", NULL),
	_OMAP2430_BALLENTRY(GPIO_153, "u24", NULL),
	_OMAP2430_BALLENTRY(GPIO_154, "ae16", NULL),
	_OMAP2430_BALLENTRY(GPIO_63, "n3", NULL),
	_OMAP2430_BALLENTRY(GPIO_7, "ac23", NULL),
	_OMAP2430_BALLENTRY(GPIO_78, "ad10", NULL),
	_OMAP2430_BALLENTRY(GPIO_79, "ae10", NULL),
	_OMAP2430_BALLENTRY(GPIO_80, "ae13", NULL),
	_OMAP2430_BALLENTRY(GPMC_A1, "a9", NULL),
	_OMAP2430_BALLENTRY(GPMC_A10, "g12", NULL),
	_OMAP2430_BALLENTRY(GPMC_A2, "b8", NULL),
	_OMAP2430_BALLENTRY(GPMC_A3, "g10", NULL),
	_OMAP2430_BALLENTRY(GPMC_A4, "g11", NULL),
	_OMAP2430_BALLENTRY(GPMC_A5, "a10", NULL),
	_OMAP2430_BALLENTRY(GPMC_A6, "g13", NULL),
	_OMAP2430_BALLENTRY(GPMC_A7, "a6", NULL),
	_OMAP2430_BALLENTRY(GPMC_A8, "h1", NULL),
	_OMAP2430_BALLENTRY(GPMC_A9, "c8", NULL),
	_OMAP2430_BALLENTRY(GPMC_CLK, "n1", "l1"),
	_OMAP2430_BALLENTRY(GPMC_D10, "d1", "n1"),
	_OMAP2430_BALLENTRY(GPMC_D11, "d2", "p2"),
	_OMAP2430_BALLENTRY(GPMC_D12, "e1", "p1"),
	_OMAP2430_BALLENTRY(GPMC_D13, "e3", "m1"),
	_OMAP2430_BALLENTRY(GPMC_D14, "c7", "j2"),
	_OMAP2430_BALLENTRY(GPMC_D15, "f3", "k2"),
	_OMAP2430_BALLENTRY(GPMC_D8, "e2", "r1"),
	_OMAP2430_BALLENTRY(GPMC_D9, "ab1", "t1"),
	_OMAP2430_BALLENTRY(GPMC_NCS1, "ac1", "w1"),
	_OMAP2430_BALLENTRY(GPMC_NCS2, "c6", NULL),
	_OMAP2430_BALLENTRY(GPMC_NCS3, "b9", NULL),
	_OMAP2430_BALLENTRY(GPMC_NCS4, "b4", NULL),
	_OMAP2430_BALLENTRY(GPMC_NCS5, "a4", NULL),
	_OMAP2430_BALLENTRY(GPMC_NCS6, "f1", NULL),
	_OMAP2430_BALLENTRY(GPMC_NCS7, "a7", NULL),
	_OMAP2430_BALLENTRY(GPMC_WAIT1, "j1", "y8"),
	_OMAP2430_BALLENTRY(GPMC_WAIT2, "b7", NULL),
	_OMAP2430_BALLENTRY(GPMC_WAIT3, "g14", NULL),
	_OMAP2430_BALLENTRY(HDQ_SIO, "h20", NULL),
	_OMAP2430_BALLENTRY(I2C1_SCL, "y17", NULL),
	_OMAP2430_BALLENTRY(I2C1_SDA, "ac19", NULL),
	_OMAP2430_BALLENTRY(I2C2_SCL, "n7", NULL),
	_OMAP2430_BALLENTRY(I2C2_SDA, "m4", NULL),
	_OMAP2430_BALLENTRY(JTAG_EMU0, "e25", NULL),
	_OMAP2430_BALLENTRY(JTAG_EMU1, "e24", NULL),
	_OMAP2430_BALLENTRY(MCBSP1_CLKR, "ab2", NULL),
	_OMAP2430_BALLENTRY(MCBSP1_CLKX, "y9", NULL),
	_OMAP2430_BALLENTRY(MCBSP1_DR, "af3", NULL),
	_OMAP2430_BALLENTRY(MCBSP1_DX, "aa1", NULL),
	_OMAP2430_BALLENTRY(MCBSP1_FSR, "ad5", NULL),
	_OMAP2430_BALLENTRY(MCBSP1_FSX, "ab3", NULL),
	_OMAP2430_BALLENTRY(MCBSP2_CLKX, "j26", NULL),
	_OMAP2430_BALLENTRY(MCBSP2_DR, "k26", NULL),
	_OMAP2430_BALLENTRY(MCBSP3_CLKX, "ac9", NULL),
	_OMAP2430_BALLENTRY(MCBSP3_DR, "ae2", NULL),
	_OMAP2430_BALLENTRY(MCBSP3_DX, "af4", NULL),
	_OMAP2430_BALLENTRY(MCBSP3_FSX, "ae4", NULL),
	_OMAP2430_BALLENTRY(MCBSP_CLKS, "ad6", NULL),
	_OMAP2430_BALLENTRY(SDMMC1_CLKO, "n23", NULL),
	_OMAP2430_BALLENTRY(SDMMC1_CMD, "l23", NULL),
	_OMAP2430_BALLENTRY(SDMMC1_DAT0, "m24", NULL),
	_OMAP2430_BALLENTRY(SDMMC1_DAT1, "p23", NULL),
	_OMAP2430_BALLENTRY(SDMMC1_DAT2, "t20", NULL),
	_OMAP2430_BALLENTRY(SDMMC1_DAT3, "r20", NULL),
	_OMAP2430_BALLENTRY(SDMMC2_CLKO, "v26", NULL),
	_OMAP2430_BALLENTRY(SDMMC2_CMD, "w20", NULL),
	_OMAP2430_BALLENTRY(SDMMC2_DAT0, "v23", NULL),
	_OMAP2430_BALLENTRY(SDMMC2_DAT1, "y24", NULL),
	_OMAP2430_BALLENTRY(SDMMC2_DAT2, "v25", NULL),
	_OMAP2430_BALLENTRY(SDMMC2_DAT3, "v24", NULL),
	_OMAP2430_BALLENTRY(SDRC_A12, "w26", "r21"),
	_OMAP2430_BALLENTRY(SDRC_A13, "af20", "aa15"),
	_OMAP2430_BALLENTRY(SDRC_A14, "af16", "y12"),
	_OMAP2430_BALLENTRY(SDRC_CKE1, "af15", "y13"),
	_OMAP2430_BALLENTRY(SDRC_NCS1, "aa25", "t20"),
	_OMAP2430_BALLENTRY(SPI1_CLK, "y18", NULL),
	_OMAP2430_BALLENTRY(SPI1_CS0, "u1", NULL),
	_OMAP2430_BALLENTRY(SPI1_CS1, "af19", NULL),
	_OMAP2430_BALLENTRY(SPI1_CS2, "ae19", NULL),
	_OMAP2430_BALLENTRY(SPI1_CS3, "h24", NULL),
	_OMAP2430_BALLENTRY(SPI1_SIMO, "ad15", NULL),
	_OMAP2430_BALLENTRY(SPI1_SOMI, "ae17", NULL),
	_OMAP2430_BALLENTRY(SPI2_CLK, "y20", NULL),
	_OMAP2430_BALLENTRY(SPI2_CS0, "y19", NULL),
	_OMAP2430_BALLENTRY(SPI2_SIMO, "ac20", NULL),
	_OMAP2430_BALLENTRY(SPI2_SOMI, "ad19", NULL),
	_OMAP2430_BALLENTRY(SSI1_DAT_RX, "aa26", NULL),
	_OMAP2430_BALLENTRY(SSI1_DAT_TX, "ad24", NULL),
	_OMAP2430_BALLENTRY(SSI1_FLAG_RX, "ad23", NULL),
	_OMAP2430_BALLENTRY(SSI1_FLAG_TX, "ab24", NULL),
	_OMAP2430_BALLENTRY(SSI1_RDY_RX, "ab25", NULL),
	_OMAP2430_BALLENTRY(SSI1_RDY_TX, "y25", NULL),
	_OMAP2430_BALLENTRY(SSI1_WAKE, "ac25", NULL),
	_OMAP2430_BALLENTRY(SYS_CLKOUT, "r25", NULL),
	_OMAP2430_BALLENTRY(SYS_DRM_MSECURE, "ae3", NULL),
	_OMAP2430_BALLENTRY(SYS_NIRQ0, "w25", NULL),
	_OMAP2430_BALLENTRY(SYS_NIRQ1, "ad21", NULL),
	_OMAP2430_BALLENTRY(UART1_CTS, "p24", NULL),
	_OMAP2430_BALLENTRY(UART1_RTS, "p25", NULL),
	_OMAP2430_BALLENTRY(UART1_RX, "n24", NULL),
	_OMAP2430_BALLENTRY(UART1_TX, "r24", NULL),
	_OMAP2430_BALLENTRY(UART2_CTS, "u25", NULL),
	_OMAP2430_BALLENTRY(UART2_RTS, "t23", NULL),
	_OMAP2430_BALLENTRY(UART2_RX, "t24", NULL),
	_OMAP2430_BALLENTRY(UART2_TX, "u20", NULL),
	_OMAP2430_BALLENTRY(UART3_CTS_RCTX, "m2", NULL),
	_OMAP2430_BALLENTRY(UART3_RTS_SD, "k2", NULL),
	_OMAP2430_BALLENTRY(UART3_RX_IRRX, "l3", NULL),
	_OMAP2430_BALLENTRY(UART3_TX_IRTX, "l2", NULL),
	_OMAP2430_BALLENTRY(USB0HS_CLK, "ae8", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA0, "ad4", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA1, "ae6", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA2, "af9", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA3, "ad9", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA4, "y11", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA5, "ad7", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA6, "ae7", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DATA7, "ac7", NULL),
	_OMAP2430_BALLENTRY(USB0HS_DIR, "ad8", NULL),
	_OMAP2430_BALLENTRY(USB0HS_NXT, "ae9", NULL),
	_OMAP2430_BALLENTRY(USB0HS_STP, "ae5", NULL),
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define omap2430_pop_ball	 NULL
#endif

int __init omap2430_mux_init(struct omap_board_mux *board_subset, int flags)
{
	struct omap_ball *package_balls = NULL;

	switch (flags & OMAP_PACKAGE_MASK) {
	case OMAP_PACKAGE_ZAC:
		package_balls = omap2430_pop_ball;
		break;
	default:
		pr_warning("%s: No ball data available for omap2420 package\n",
				__func__);
	}

	return omap_mux_init("core", OMAP_MUX_REG_8BIT | OMAP_MUX_GPIO_IN_MODE3,
			     OMAP2430_CONTROL_PADCONF_MUX_PBASE,
			     OMAP2430_CONTROL_PADCONF_MUX_SIZE,
			     omap2430_muxmodes, NULL, board_subset,
			     package_balls);
}
