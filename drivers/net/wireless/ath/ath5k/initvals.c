/*
 * Initial register settings functions
 *
 * Copyright (c) 2004-2007 Reyk Floeter <reyk@openbsd.org>
 * Copyright (c) 2006-2009 Nick Kossifidis <mickflemm@gmail.com>
 * Copyright (c) 2007-2008 Jiri Slaby <jirislaby@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "ath5k.h"
#include "reg.h"
#include "debug.h"
#include "base.h"

/*
 * Mode-independent initial register writes
 */

struct ath5k_ini {
	u16	ini_register;
	u32	ini_value;

	enum {
		AR5K_INI_WRITE = 0,	/* Default */
		AR5K_INI_READ = 1,	/* Cleared on read */
	} ini_mode;
};

/*
 * Mode specific initial register values
 */

struct ath5k_ini_mode {
	u16	mode_register;
	u32	mode_value[3];
};

/* Initial register settings for AR5210 */
static const struct ath5k_ini ar5210_ini[] = {
	/* PCU and MAC registers */
	{ AR5K_NOQCU_TXDP0,	0 },
	{ AR5K_NOQCU_TXDP1,	0 },
	{ AR5K_RXDP,		0 },
	{ AR5K_CR,		0 },
	{ AR5K_ISR,		0, AR5K_INI_READ },
	{ AR5K_IMR,		0 },
	{ AR5K_IER,		AR5K_IER_DISABLE },
	{ AR5K_BSR,		0, AR5K_INI_READ },
	{ AR5K_TXCFG,		AR5K_DMASIZE_128B },
	{ AR5K_RXCFG,		AR5K_DMASIZE_128B },
	{ AR5K_CFG,		AR5K_INIT_CFG },
	{ AR5K_TOPS,		8 },
	{ AR5K_RXNOFRM,		8 },
	{ AR5K_RPGTO,		0 },
	{ AR5K_TXNOFRM,		0 },
	{ AR5K_SFR,		0 },
	{ AR5K_MIBC,		0 },
	{ AR5K_MISC,		0 },
	{ AR5K_RX_FILTER_5210,	0 },
	{ AR5K_MCAST_FILTER0_5210, 0 },
	{ AR5K_MCAST_FILTER1_5210, 0 },
	{ AR5K_TX_MASK0,	0 },
	{ AR5K_TX_MASK1,	0 },
	{ AR5K_CLR_TMASK,	0 },
	{ AR5K_TRIG_LVL,	AR5K_TUNE_MIN_TX_FIFO_THRES },
	{ AR5K_DIAG_SW_5210,	0 },
	{ AR5K_RSSI_THR,	AR5K_TUNE_RSSI_THRES },
	{ AR5K_TSF_L32_5210,	0 },
	{ AR5K_TIMER0_5210,	0 },
	{ AR5K_TIMER1_5210,	0xffffffff },
	{ AR5K_TIMER2_5210,	0xffffffff },
	{ AR5K_TIMER3_5210,	1 },
	{ AR5K_CFP_DUR_5210,	0 },
	{ AR5K_CFP_PERIOD_5210,	0 },
	/* PHY registers */
	{ AR5K_PHY(0),	0x00000047 },
	{ AR5K_PHY_AGC,	0x00000000 },
	{ AR5K_PHY(3),	0x09848ea6 },
	{ AR5K_PHY(4),	0x3d32e000 },
	{ AR5K_PHY(5),	0x0000076b },
	{ AR5K_PHY_ACT,	AR5K_PHY_ACT_DISABLE },
	{ AR5K_PHY(8),	0x02020200 },
	{ AR5K_PHY(9),	0x00000e0e },
	{ AR5K_PHY(10),	0x0a020201 },
	{ AR5K_PHY(11),	0x00036ffc },
	{ AR5K_PHY(12),	0x00000000 },
	{ AR5K_PHY(13),	0x00000e0e },
	{ AR5K_PHY(14),	0x00000007 },
	{ AR5K_PHY(15),	0x00020100 },
	{ AR5K_PHY(16),	0x89630000 },
	{ AR5K_PHY(17),	0x1372169c },
	{ AR5K_PHY(18),	0x0018b633 },
	{ AR5K_PHY(19),	0x1284613c },
	{ AR5K_PHY(20),	0x0de8b8e0 },
	{ AR5K_PHY(21),	0x00074859 },
	{ AR5K_PHY(22),	0x7e80beba },
	{ AR5K_PHY(23),	0x313a665e },
	{ AR5K_PHY_AGCCTL, 0x00001d08 },
	{ AR5K_PHY(25),	0x0001ce00 },
	{ AR5K_PHY(26),	0x409a4190 },
	{ AR5K_PHY(28),	0x0000000f },
	{ AR5K_PHY(29),	0x00000080 },
	{ AR5K_PHY(30),	0x00000004 },
	{ AR5K_PHY(31),	0x00000018 },	/* 0x987c */
	{ AR5K_PHY(64),	0x00000000 },	/* 0x9900 */
	{ AR5K_PHY(65),	0x00000000 },
	{ AR5K_PHY(66),	0x00000000 },
	{ AR5K_PHY(67),	0x00800000 },
	{ AR5K_PHY(68),	0x00000003 },
	/* BB gain table (64bytes) */
	{ AR5K_BB_GAIN(0), 0x00000000 },
	{ AR5K_BB_GAIN(1), 0x00000020 },
	{ AR5K_BB_GAIN(2), 0x00000010 },
	{ AR5K_BB_GAIN(3), 0x00000030 },
	{ AR5K_BB_GAIN(4), 0x00000008 },
	{ AR5K_BB_GAIN(5), 0x00000028 },
	{ AR5K_BB_GAIN(6), 0x00000028 },
	{ AR5K_BB_GAIN(7), 0x00000004 },
	{ AR5K_BB_GAIN(8), 0x00000024 },
	{ AR5K_BB_GAIN(9), 0x00000014 },
	{ AR5K_BB_GAIN(10), 0x00000034 },
	{ AR5K_BB_GAIN(11), 0x0000000c },
	{ AR5K_BB_GAIN(12), 0x0000002c },
	{ AR5K_BB_GAIN(13), 0x00000002 },
	{ AR5K_BB_GAIN(14), 0x00000022 },
	{ AR5K_BB_GAIN(15), 0x00000012 },
	{ AR5K_BB_GAIN(16), 0x00000032 },
	{ AR5K_BB_GAIN(17), 0x0000000a },
	{ AR5K_BB_GAIN(18), 0x0000002a },
	{ AR5K_BB_GAIN(19), 0x00000001 },
	{ AR5K_BB_GAIN(20), 0x00000021 },
	{ AR5K_BB_GAIN(21), 0x00000011 },
	{ AR5K_BB_GAIN(22), 0x00000031 },
	{ AR5K_BB_GAIN(23), 0x00000009 },
	{ AR5K_BB_GAIN(24), 0x00000029 },
	{ AR5K_BB_GAIN(25), 0x00000005 },
	{ AR5K_BB_GAIN(26), 0x00000025 },
	{ AR5K_BB_GAIN(27), 0x00000015 },
	{ AR5K_BB_GAIN(28), 0x00000035 },
	{ AR5K_BB_GAIN(29), 0x0000000d },
	{ AR5K_BB_GAIN(30), 0x0000002d },
	{ AR5K_BB_GAIN(31), 0x00000003 },
	{ AR5K_BB_GAIN(32), 0x00000023 },
	{ AR5K_BB_GAIN(33), 0x00000013 },
	{ AR5K_BB_GAIN(34), 0x00000033 },
	{ AR5K_BB_GAIN(35), 0x0000000b },
	{ AR5K_BB_GAIN(36), 0x0000002b },
	{ AR5K_BB_GAIN(37), 0x00000007 },
	{ AR5K_BB_GAIN(38), 0x00000027 },
	{ AR5K_BB_GAIN(39), 0x00000017 },
	{ AR5K_BB_GAIN(40), 0x00000037 },
	{ AR5K_BB_GAIN(41), 0x0000000f },
	{ AR5K_BB_GAIN(42), 0x0000002f },
	{ AR5K_BB_GAIN(43), 0x0000002f },
	{ AR5K_BB_GAIN(44), 0x0000002f },
	{ AR5K_BB_GAIN(45), 0x0000002f },
	{ AR5K_BB_GAIN(46), 0x0000002f },
	{ AR5K_BB_GAIN(47), 0x0000002f },
	{ AR5K_BB_GAIN(48), 0x0000002f },
	{ AR5K_BB_GAIN(49), 0x0000002f },
	{ AR5K_BB_GAIN(50), 0x0000002f },
	{ AR5K_BB_GAIN(51), 0x0000002f },
	{ AR5K_BB_GAIN(52), 0x0000002f },
	{ AR5K_BB_GAIN(53), 0x0000002f },
	{ AR5K_BB_GAIN(54), 0x0000002f },
	{ AR5K_BB_GAIN(55), 0x0000002f },
	{ AR5K_BB_GAIN(56), 0x0000002f },
	{ AR5K_BB_GAIN(57), 0x0000002f },
	{ AR5K_BB_GAIN(58), 0x0000002f },
	{ AR5K_BB_GAIN(59), 0x0000002f },
	{ AR5K_BB_GAIN(60), 0x0000002f },
	{ AR5K_BB_GAIN(61), 0x0000002f },
	{ AR5K_BB_GAIN(62), 0x0000002f },
	{ AR5K_BB_GAIN(63), 0x0000002f },
	/* 5110 RF gain table (64btes) */
	{ AR5K_RF_GAIN(0), 0x0000001d },
	{ AR5K_RF_GAIN(1), 0x0000005d },
	{ AR5K_RF_GAIN(2), 0x0000009d },
	{ AR5K_RF_GAIN(3), 0x000000dd },
	{ AR5K_RF_GAIN(4), 0x0000011d },
	{ AR5K_RF_GAIN(5), 0x00000021 },
	{ AR5K_RF_GAIN(6), 0x00000061 },
	{ AR5K_RF_GAIN(7), 0x000000a1 },
	{ AR5K_RF_GAIN(8), 0x000000e1 },
	{ AR5K_RF_GAIN(9), 0x00000031 },
	{ AR5K_RF_GAIN(10), 0x00000071 },
	{ AR5K_RF_GAIN(11), 0x000000b1 },
	{ AR5K_RF_GAIN(12), 0x0000001c },
	{ AR5K_RF_GAIN(13), 0x0000005c },
	{ AR5K_RF_GAIN(14), 0x00000029 },
	{ AR5K_RF_GAIN(15), 0x00000069 },
	{ AR5K_RF_GAIN(16), 0x000000a9 },
	{ AR5K_RF_GAIN(17), 0x00000020 },
	{ AR5K_RF_GAIN(18), 0x00000019 },
	{ AR5K_RF_GAIN(19), 0x00000059 },
	{ AR5K_RF_GAIN(20), 0x00000099 },
	{ AR5K_RF_GAIN(21), 0x00000030 },
	{ AR5K_RF_GAIN(22), 0x00000005 },
	{ AR5K_RF_GAIN(23), 0x00000025 },
	{ AR5K_RF_GAIN(24), 0x00000065 },
	{ AR5K_RF_GAIN(25), 0x000000a5 },
	{ AR5K_RF_GAIN(26), 0x00000028 },
	{ AR5K_RF_GAIN(27), 0x00000068 },
	{ AR5K_RF_GAIN(28), 0x0000001f },
	{ AR5K_RF_GAIN(29), 0x0000001e },
	{ AR5K_RF_GAIN(30), 0x00000018 },
	{ AR5K_RF_GAIN(31), 0x00000058 },
	{ AR5K_RF_GAIN(32), 0x00000098 },
	{ AR5K_RF_GAIN(33), 0x00000003 },
	{ AR5K_RF_GAIN(34), 0x00000004 },
	{ AR5K_RF_GAIN(35), 0x00000044 },
	{ AR5K_RF_GAIN(36), 0x00000084 },
	{ AR5K_RF_GAIN(37), 0x00000013 },
	{ AR5K_RF_GAIN(38), 0x00000012 },
	{ AR5K_RF_GAIN(39), 0x00000052 },
	{ AR5K_RF_GAIN(40), 0x00000092 },
	{ AR5K_RF_GAIN(41), 0x000000d2 },
	{ AR5K_RF_GAIN(42), 0x0000002b },
	{ AR5K_RF_GAIN(43), 0x0000002a },
	{ AR5K_RF_GAIN(44), 0x0000006a },
	{ AR5K_RF_GAIN(45), 0x000000aa },
	{ AR5K_RF_GAIN(46), 0x0000001b },
	{ AR5K_RF_GAIN(47), 0x0000001a },
	{ AR5K_RF_GAIN(48), 0x0000005a },
	{ AR5K_RF_GAIN(49), 0x0000009a },
	{ AR5K_RF_GAIN(50), 0x000000da },
	{ AR5K_RF_GAIN(51), 0x00000006 },
	{ AR5K_RF_GAIN(52), 0x00000006 },
	{ AR5K_RF_GAIN(53), 0x00000006 },
	{ AR5K_RF_GAIN(54), 0x00000006 },
	{ AR5K_RF_GAIN(55), 0x00000006 },
	{ AR5K_RF_GAIN(56), 0x00000006 },
	{ AR5K_RF_GAIN(57), 0x00000006 },
	{ AR5K_RF_GAIN(58), 0x00000006 },
	{ AR5K_RF_GAIN(59), 0x00000006 },
	{ AR5K_RF_GAIN(60), 0x00000006 },
	{ AR5K_RF_GAIN(61), 0x00000006 },
	{ AR5K_RF_GAIN(62), 0x00000006 },
	{ AR5K_RF_GAIN(63), 0x00000006 },
	/* PHY activation */
	{ AR5K_PHY(53), 0x00000020 },
	{ AR5K_PHY(51), 0x00000004 },
	{ AR5K_PHY(50), 0x00060106 },
	{ AR5K_PHY(39), 0x0000006d },
	{ AR5K_PHY(48), 0x00000000 },
	{ AR5K_PHY(52), 0x00000014 },
	{ AR5K_PHY_ACT, AR5K_PHY_ACT_ENABLE },
};

/* Initial register settings for AR5211 */
static const struct ath5k_ini ar5211_ini[] = {
	{ AR5K_RXDP,		0x00000000 },
	{ AR5K_RTSD0,		0x84849c9c },
	{ AR5K_RTSD1,		0x7c7c7c7c },
	{ AR5K_RXCFG,		0x00000005 },
	{ AR5K_MIBC,		0x00000000 },
	{ AR5K_TOPS,		0x00000008 },
	{ AR5K_RXNOFRM,		0x00000008 },
	{ AR5K_TXNOFRM,		0x00000010 },
	{ AR5K_RPGTO,		0x00000000 },
	{ AR5K_RFCNT,		0x0000001f },
	{ AR5K_QUEUE_TXDP(0),	0x00000000 },
	{ AR5K_QUEUE_TXDP(1),	0x00000000 },
	{ AR5K_QUEUE_TXDP(2),	0x00000000 },
	{ AR5K_QUEUE_TXDP(3),	0x00000000 },
	{ AR5K_QUEUE_TXDP(4),	0x00000000 },
	{ AR5K_QUEUE_TXDP(5),	0x00000000 },
	{ AR5K_QUEUE_TXDP(6),	0x00000000 },
	{ AR5K_QUEUE_TXDP(7),	0x00000000 },
	{ AR5K_QUEUE_TXDP(8),	0x00000000 },
	{ AR5K_QUEUE_TXDP(9),	0x00000000 },
	{ AR5K_DCU_FP,		0x00000000 },
	{ AR5K_STA_ID1,		0x00000000 },
	{ AR5K_BSS_ID0,		0x00000000 },
	{ AR5K_BSS_ID1,		0x00000000 },
	{ AR5K_RSSI_THR,	0x00000000 },
	{ AR5K_CFP_PERIOD_5211,	0x00000000 },
	{ AR5K_TIMER0_5211,	0x00000030 },
	{ AR5K_TIMER1_5211,	0x0007ffff },
	{ AR5K_TIMER2_5211,	0x01ffffff },
	{ AR5K_TIMER3_5211,	0x00000031 },
	{ AR5K_CFP_DUR_5211,	0x00000000 },
	{ AR5K_RX_FILTER_5211,	0x00000000 },
	{ AR5K_MCAST_FILTER0_5211, 0x00000000 },
	{ AR5K_MCAST_FILTER1_5211, 0x00000002 },
	{ AR5K_DIAG_SW_5211,	0x00000000 },
	{ AR5K_ADDAC_TEST,	0x00000000 },
	{ AR5K_DEFAULT_ANTENNA,	0x00000000 },
	/* PHY registers */
	{ AR5K_PHY_AGC,	0x00000000 },
	{ AR5K_PHY(3),	0x2d849093 },
	{ AR5K_PHY(4),	0x7d32e000 },
	{ AR5K_PHY(5),	0x00000f6b },
	{ AR5K_PHY_ACT,	0x00000000 },
	{ AR5K_PHY(11),	0x00026ffe },
	{ AR5K_PHY(12),	0x00000000 },
	{ AR5K_PHY(15),	0x00020100 },
	{ AR5K_PHY(16),	0x206a017a },
	{ AR5K_PHY(19),	0x1284613c },
	{ AR5K_PHY(21),	0x00000859 },
	{ AR5K_PHY(26),	0x409a4190 },	/* 0x9868 */
	{ AR5K_PHY(27),	0x050cb081 },
	{ AR5K_PHY(28),	0x0000000f },
	{ AR5K_PHY(29),	0x00000080 },
	{ AR5K_PHY(30),	0x0000000c },
	{ AR5K_PHY(64),	0x00000000 },
	{ AR5K_PHY(65),	0x00000000 },
	{ AR5K_PHY(66),	0x00000000 },
	{ AR5K_PHY(67),	0x00800000 },
	{ AR5K_PHY(68),	0x00000001 },
	{ AR5K_PHY(71),	0x0000092a },
	{ AR5K_PHY_IQ,	0x00000000 },
	{ AR5K_PHY(73),	0x00058a05 },
	{ AR5K_PHY(74),	0x00000001 },
	{ AR5K_PHY(75),	0x00000000 },
	{ AR5K_PHY_PAPD_PROBE, 0x00000000 },
	{ AR5K_PHY(77),	0x00000000 },	/* 0x9934 */
	{ AR5K_PHY(78),	0x00000000 },	/* 0x9938 */
	{ AR5K_PHY(79),	0x0000003f },	/* 0x993c */
	{ AR5K_PHY(80),	0x00000004 },
	{ AR5K_PHY(82),	0x00000000 },
	{ AR5K_PHY(83),	0x00000000 },
	{ AR5K_PHY(84),	0x00000000 },
	{ AR5K_PHY_RADAR, 0x5d50f14c },
	{ AR5K_PHY(86),	0x00000018 },
	{ AR5K_PHY(87),	0x004b6a8e },
	/* Initial Power table (32bytes)
	 * common on all cards/modes.
	 * Note: Table is rewritten during
	 * txpower setup later using calibration
	 * data etc. so next write is non-common */
	{ AR5K_PHY_PCDAC_TXPOWER(1), 0x06ff05ff },
	{ AR5K_PHY_PCDAC_TXPOWER(2), 0x07ff07ff },
	{ AR5K_PHY_PCDAC_TXPOWER(3), 0x08ff08ff },
	{ AR5K_PHY_PCDAC_TXPOWER(4), 0x09ff09ff },
	{ AR5K_PHY_PCDAC_TXPOWER(5), 0x0aff0aff },
	{ AR5K_PHY_PCDAC_TXPOWER(6), 0x0bff0bff },
	{ AR5K_PHY_PCDAC_TXPOWER(7), 0x0cff0cff },
	{ AR5K_PHY_PCDAC_TXPOWER(8), 0x0dff0dff },
	{ AR5K_PHY_PCDAC_TXPOWER(9), 0x0fff0eff },
	{ AR5K_PHY_PCDAC_TXPOWER(10), 0x12ff12ff },
	{ AR5K_PHY_PCDAC_TXPOWER(11), 0x14ff13ff },
	{ AR5K_PHY_PCDAC_TXPOWER(12), 0x16ff15ff },
	{ AR5K_PHY_PCDAC_TXPOWER(13), 0x19ff17ff },
	{ AR5K_PHY_PCDAC_TXPOWER(14), 0x1bff1aff },
	{ AR5K_PHY_PCDAC_TXPOWER(15), 0x1eff1dff },
	{ AR5K_PHY_PCDAC_TXPOWER(16), 0x23ff20ff },
	{ AR5K_PHY_PCDAC_TXPOWER(17), 0x27ff25ff },
	{ AR5K_PHY_PCDAC_TXPOWER(18), 0x2cff29ff },
	{ AR5K_PHY_PCDAC_TXPOWER(19), 0x31ff2fff },
	{ AR5K_PHY_PCDAC_TXPOWER(20), 0x37ff34ff },
	{ AR5K_PHY_PCDAC_TXPOWER(21), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(22), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(23), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(24), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(25), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(26), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(27), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(28), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(29), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(30), 0x3aff3aff },
	{ AR5K_PHY_PCDAC_TXPOWER(31), 0x3aff3aff },
	{ AR5K_PHY_CCKTXCTL, 0x00000000 },
	{ AR5K_PHY(642), 0x503e4646 },
	{ AR5K_PHY_GAIN_2GHZ, 0x6480416c },
	{ AR5K_PHY(644), 0x0199a003 },
	{ AR5K_PHY(645), 0x044cd610 },
	{ AR5K_PHY(646), 0x13800040 },
	{ AR5K_PHY(647), 0x1be00060 },
	{ AR5K_PHY(648), 0x0c53800a },
	{ AR5K_PHY(649), 0x0014df3b },
	{ AR5K_PHY(650), 0x000001b5 },
	{ AR5K_PHY(651), 0x00000020 },
};

/* Initial mode-specific settings for AR5211
 * 5211 supports OFDM-only g (draft g) but we
 * need to test it !
 */
static const struct ath5k_ini_mode ar5211_ini_mode[] = {
	{ AR5K_TXCFG,
	/*	A/XR          B           G       */
	   { 0x00000015, 0x0000001d, 0x00000015 } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(0),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(1),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(2),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(3),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(4),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(5),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(6),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(7),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(8),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(9),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_DCU_GBL_IFS_SLOT,
	   { 0x00000168, 0x000001b8, 0x00000168 } },
	{ AR5K_DCU_GBL_IFS_SIFS,
	   { 0x00000230, 0x000000b0, 0x00000230 } },
	{ AR5K_DCU_GBL_IFS_EIFS,
	   { 0x00000d98, 0x00001f48, 0x00000d98 } },
	{ AR5K_DCU_GBL_IFS_MISC,
	   { 0x0000a0e0, 0x00005880, 0x0000a0e0 } },
	{ AR5K_TIME_OUT,
	   { 0x04000400, 0x20003000, 0x04000400 } },
	{ AR5K_USEC_5211,
	   { 0x0e8d8fa7, 0x01608f95, 0x0e8d8fa7 } },
	{ AR5K_PHY(8),
	   { 0x02020200, 0x02010200, 0x02020200 } },
	{ AR5K_PHY_RF_CTL2,
	   { 0x00000e0e, 0x00000707, 0x00000e0e } },
	{ AR5K_PHY_RF_CTL3,
	   { 0x0a020001, 0x05010000, 0x0a020001 } },
	{ AR5K_PHY_RF_CTL4,
	   { 0x00000e0e, 0x00000e0e, 0x00000e0e } },
	{ AR5K_PHY_PA_CTL,
	   { 0x00000007, 0x0000000b, 0x0000000b } },
	{ AR5K_PHY_SETTLING,
	   { 0x1372169c, 0x137216a8, 0x1372169c } },
	{ AR5K_PHY_GAIN,
	   { 0x0018ba67, 0x0018ba69, 0x0018ba69 } },
	{ AR5K_PHY_DESIRED_SIZE,
	   { 0x0c28b4e0, 0x0c28b4e0, 0x0c28b4e0 } },
	{ AR5K_PHY_SIG,
	   { 0x7e800d2e, 0x7ec00d2e, 0x7e800d2e } },
	{ AR5K_PHY_AGCCOARSE,
	   { 0x31375d5e, 0x313a5d5e, 0x31375d5e } },
	{ AR5K_PHY_AGCCTL,
	   { 0x0000bd10, 0x0000bd38, 0x0000bd10 } },
	{ AR5K_PHY_NF,
	   { 0x0001ce00, 0x0001ce00, 0x0001ce00 } },
	{ AR5K_PHY_RX_DELAY,
	   { 0x00002710, 0x0000157c, 0x00002710 } },
	{ AR5K_PHY(70),
	   { 0x00000190, 0x00000084, 0x00000190 } },
	{ AR5K_PHY_FRAME_CTL_5211,
	   { 0x6fe01020, 0x6fe00920, 0x6fe01020 } },
	{ AR5K_PHY_PCDAC_TXPOWER_BASE,
	   { 0x05ff14ff, 0x05ff14ff, 0x05ff19ff } },
	{ AR5K_RF_BUFFER_CONTROL_4,
	   { 0x00000010, 0x00000010, 0x00000010 } },
};

/* Initial register settings for AR5212 */
static const struct ath5k_ini ar5212_ini_common_start[] = {
	{ AR5K_RXDP,		0x00000000 },
	{ AR5K_RXCFG,		0x00000005 },
	{ AR5K_MIBC,		0x00000000 },
	{ AR5K_TOPS,		0x00000008 },
	{ AR5K_RXNOFRM,		0x00000008 },
	{ AR5K_TXNOFRM,		0x00000010 },
	{ AR5K_RPGTO,		0x00000000 },
	{ AR5K_RFCNT,		0x0000001f },
	{ AR5K_QUEUE_TXDP(0),	0x00000000 },
	{ AR5K_QUEUE_TXDP(1),	0x00000000 },
	{ AR5K_QUEUE_TXDP(2),	0x00000000 },
	{ AR5K_QUEUE_TXDP(3),	0x00000000 },
	{ AR5K_QUEUE_TXDP(4),	0x00000000 },
	{ AR5K_QUEUE_TXDP(5),	0x00000000 },
	{ AR5K_QUEUE_TXDP(6),	0x00000000 },
	{ AR5K_QUEUE_TXDP(7),	0x00000000 },
	{ AR5K_QUEUE_TXDP(8),	0x00000000 },
	{ AR5K_QUEUE_TXDP(9),	0x00000000 },
	{ AR5K_DCU_FP,		0x00000000 },
	{ AR5K_DCU_TXP,		0x00000000 },
	/* Tx filter table 0 (32 entries) */
	{ AR5K_DCU_TX_FILTER_0(0),  0x00000000 }, /* DCU 0 */
	{ AR5K_DCU_TX_FILTER_0(1),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(2),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(3),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(4),  0x00000000 }, /* DCU 1 */
	{ AR5K_DCU_TX_FILTER_0(5),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(6),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(7),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(8),  0x00000000 }, /* DCU 2 */
	{ AR5K_DCU_TX_FILTER_0(9),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(10), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(11), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(12), 0x00000000 }, /* DCU 3 */
	{ AR5K_DCU_TX_FILTER_0(13), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(14), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(15), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(16), 0x00000000 }, /* DCU 4 */
	{ AR5K_DCU_TX_FILTER_0(17), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(18), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(19), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(20), 0x00000000 }, /* DCU 5 */
	{ AR5K_DCU_TX_FILTER_0(21), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(22), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(23), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(24), 0x00000000 }, /* DCU 6 */
	{ AR5K_DCU_TX_FILTER_0(25), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(26), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(27), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(28), 0x00000000 }, /* DCU 7 */
	{ AR5K_DCU_TX_FILTER_0(29), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(30), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_0(31), 0x00000000 },
	/* Tx filter table 1 (16 entries) */
	{ AR5K_DCU_TX_FILTER_1(0),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(1),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(2),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(3),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(4),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(5),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(6),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(7),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(8),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(9),  0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(10), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(11), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(12), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(13), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(14), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_1(15), 0x00000000 },
	{ AR5K_DCU_TX_FILTER_CLR, 0x00000000 },
	{ AR5K_DCU_TX_FILTER_SET, 0x00000000 },
	{ AR5K_STA_ID1,		0x00000000 },
	{ AR5K_BSS_ID0,		0x00000000 },
	{ AR5K_BSS_ID1,		0x00000000 },
	{ AR5K_BEACON_5211,	0x00000000 },
	{ AR5K_CFP_PERIOD_5211, 0x00000000 },
	{ AR5K_TIMER0_5211,	0x00000030 },
	{ AR5K_TIMER1_5211,	0x0007ffff },
	{ AR5K_TIMER2_5211,	0x01ffffff },
	{ AR5K_TIMER3_5211,	0x00000031 },
	{ AR5K_CFP_DUR_5211,	0x00000000 },
	{ AR5K_RX_FILTER_5211,	0x00000000 },
	{ AR5K_DIAG_SW_5211,	0x00000000 },
	{ AR5K_ADDAC_TEST,	0x00000000 },
	{ AR5K_DEFAULT_ANTENNA,	0x00000000 },
	{ AR5K_FRAME_CTL_QOSM,	0x000fc78f },
	{ AR5K_XRMODE,		0x2a82301a },
	{ AR5K_XRDELAY,		0x05dc01e0 },
	{ AR5K_XRTIMEOUT,	0x1f402710 },
	{ AR5K_XRCHIRP,		0x01f40000 },
	{ AR5K_XRSTOMP,		0x00001e1c },
	{ AR5K_SLEEP0,		0x0002aaaa },
	{ AR5K_SLEEP1,		0x02005555 },
	{ AR5K_SLEEP2,		0x00000000 },
	{ AR_BSSMSKL,		0xffffffff },
	{ AR_BSSMSKU,		0x0000ffff },
	{ AR5K_TXPC,		0x00000000 },
	{ AR5K_PROFCNT_TX,	0x00000000 },
	{ AR5K_PROFCNT_RX,	0x00000000 },
	{ AR5K_PROFCNT_RXCLR,	0x00000000 },
	{ AR5K_PROFCNT_CYCLE,	0x00000000 },
	{ AR5K_QUIET_CTL1,	0x00000088 },
	/* Initial rate duration table (32 entries )*/
	{ AR5K_RATE_DUR(0),	0x00000000 },
	{ AR5K_RATE_DUR(1),	0x0000008c },
	{ AR5K_RATE_DUR(2),	0x000000e4 },
	{ AR5K_RATE_DUR(3),	0x000002d5 },
	{ AR5K_RATE_DUR(4),	0x00000000 },
	{ AR5K_RATE_DUR(5),	0x00000000 },
	{ AR5K_RATE_DUR(6),	0x000000a0 },
	{ AR5K_RATE_DUR(7),	0x000001c9 },
	{ AR5K_RATE_DUR(8),	0x0000002c },
	{ AR5K_RATE_DUR(9),	0x0000002c },
	{ AR5K_RATE_DUR(10),	0x00000030 },
	{ AR5K_RATE_DUR(11),	0x0000003c },
	{ AR5K_RATE_DUR(12),	0x0000002c },
	{ AR5K_RATE_DUR(13),	0x0000002c },
	{ AR5K_RATE_DUR(14),	0x00000030 },
	{ AR5K_RATE_DUR(15),	0x0000003c },
	{ AR5K_RATE_DUR(16),	0x00000000 },
	{ AR5K_RATE_DUR(17),	0x00000000 },
	{ AR5K_RATE_DUR(18),	0x00000000 },
	{ AR5K_RATE_DUR(19),	0x00000000 },
	{ AR5K_RATE_DUR(20),	0x00000000 },
	{ AR5K_RATE_DUR(21),	0x00000000 },
	{ AR5K_RATE_DUR(22),	0x00000000 },
	{ AR5K_RATE_DUR(23),	0x00000000 },
	{ AR5K_RATE_DUR(24),	0x000000d5 },
	{ AR5K_RATE_DUR(25),	0x000000df },
	{ AR5K_RATE_DUR(26),	0x00000102 },
	{ AR5K_RATE_DUR(27),	0x0000013a },
	{ AR5K_RATE_DUR(28),	0x00000075 },
	{ AR5K_RATE_DUR(29),	0x0000007f },
	{ AR5K_RATE_DUR(30),	0x000000a2 },
	{ AR5K_RATE_DUR(31),	0x00000000 },
	{ AR5K_QUIET_CTL2,	0x00010002 },
	{ AR5K_TSF_PARM,	0x00000001 },
	{ AR5K_QOS_NOACK,	0x000000c0 },
	{ AR5K_PHY_ERR_FIL,	0x00000000 },
	{ AR5K_XRLAT_TX,	0x00000168 },
	{ AR5K_ACKSIFS,		0x00000000 },
	/* Rate -> db table
	 * notice ...03<-02<-01<-00 ! */
	{ AR5K_RATE2DB(0),	0x03020100 },
	{ AR5K_RATE2DB(1),	0x07060504 },
	{ AR5K_RATE2DB(2),	0x0b0a0908 },
	{ AR5K_RATE2DB(3),	0x0f0e0d0c },
	{ AR5K_RATE2DB(4),	0x13121110 },
	{ AR5K_RATE2DB(5),	0x17161514 },
	{ AR5K_RATE2DB(6),	0x1b1a1918 },
	{ AR5K_RATE2DB(7),	0x1f1e1d1c },
	/* Db -> Rate table */
	{ AR5K_DB2RATE(0),	0x03020100 },
	{ AR5K_DB2RATE(1),	0x07060504 },
	{ AR5K_DB2RATE(2),	0x0b0a0908 },
	{ AR5K_DB2RATE(3),	0x0f0e0d0c },
	{ AR5K_DB2RATE(4),	0x13121110 },
	{ AR5K_DB2RATE(5),	0x17161514 },
	{ AR5K_DB2RATE(6),	0x1b1a1918 },
	{ AR5K_DB2RATE(7),	0x1f1e1d1c },
	/* PHY registers (Common settings
	 * for all chips/modes) */
	{ AR5K_PHY(3),		0xad848e19 },
	{ AR5K_PHY(4),		0x7d28e000 },
	{ AR5K_PHY_TIMING_3,	0x9c0a9f6b },
	{ AR5K_PHY_ACT,		0x00000000 },
	{ AR5K_PHY(16),		0x206a017a },
	{ AR5K_PHY(21),		0x00000859 },
	{ AR5K_PHY_BIN_MASK_1,	0x00000000 },
	{ AR5K_PHY_BIN_MASK_2,	0x00000000 },
	{ AR5K_PHY_BIN_MASK_3,	0x00000000 },
	{ AR5K_PHY_BIN_MASK_CTL, 0x00800000 },
	{ AR5K_PHY_ANT_CTL,	0x00000001 },
	/*{ AR5K_PHY(71), 0x0000092a },*/ /* Old value */
	{ AR5K_PHY_MAX_RX_LEN,	0x00000c80 },
	{ AR5K_PHY_IQ,		0x05100000 },
	{ AR5K_PHY_WARM_RESET,	0x00000001 },
	{ AR5K_PHY_CTL,		0x00000004 },
	{ AR5K_PHY_TXPOWER_RATE1, 0x1e1f2022 },
	{ AR5K_PHY_TXPOWER_RATE2, 0x0a0b0c0d },
	{ AR5K_PHY_TXPOWER_RATE_MAX, 0x0000003f },
	{ AR5K_PHY(82),		0x9280b212 },
	{ AR5K_PHY_RADAR,	0x5d50e188 },
	/*{ AR5K_PHY(86), 0x000000ff },*/
	{ AR5K_PHY(87),		0x004b6a8e },
	{ AR5K_PHY_NFTHRES,	0x000003ce },
	{ AR5K_PHY_RESTART,	0x192fb515 },
	{ AR5K_PHY(94),		0x00000001 },
	{ AR5K_PHY_RFBUS_REQ,	0x00000000 },
	/*{ AR5K_PHY(644), 0x0080a333 },*/ /* Old value */
	/*{ AR5K_PHY(645), 0x00206c10 },*/ /* Old value */
	{ AR5K_PHY(644),	0x00806333 },
	{ AR5K_PHY(645),	0x00106c10 },
	{ AR5K_PHY(646),	0x009c4060 },
	/* { AR5K_PHY(647), 0x1483800a }, */
	/* { AR5K_PHY(648), 0x01831061 }, */ /* Old value */
	{ AR5K_PHY(648),	0x018830c6 },
	{ AR5K_PHY(649),	0x00000400 },
	/*{ AR5K_PHY(650), 0x000001b5 },*/
	{ AR5K_PHY(651),	0x00000000 },
	{ AR5K_PHY_TXPOWER_RATE3, 0x20202020 },
	{ AR5K_PHY_TXPOWER_RATE4, 0x20202020 },
	/*{ AR5K_PHY(655), 0x13c889af },*/
	{ AR5K_PHY(656),	0x38490a20 },
	{ AR5K_PHY(657),	0x00007bb6 },
	{ AR5K_PHY(658),	0x0fff3ffc },
};

/* Initial mode-specific settings for AR5212 (Written before ar5212_ini) */
static const struct ath5k_ini_mode ar5212_ini_mode_start[] = {
	{ AR5K_QUEUE_DFS_LOCAL_IFS(0),
	/*	A/XR          B           G       */
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(1),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(2),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(3),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(4),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(5),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(6),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(7),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(8),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_QUEUE_DFS_LOCAL_IFS(9),
	   { 0x002ffc0f, 0x002ffc1f, 0x002ffc0f } },
	{ AR5K_DCU_GBL_IFS_SIFS,
	   { 0x00000230, 0x000000b0, 0x00000160 } },
	{ AR5K_DCU_GBL_IFS_SLOT,
	   { 0x00000168, 0x000001b8, 0x0000018c } },
	{ AR5K_DCU_GBL_IFS_EIFS,
	   { 0x00000e60, 0x00001f1c, 0x00003e38 } },
	{ AR5K_DCU_GBL_IFS_MISC,
	   { 0x0000a0e0, 0x00005880, 0x0000b0e0 } },
	{ AR5K_TIME_OUT,
	   { 0x03e803e8, 0x04200420, 0x08400840 } },
	{ AR5K_PHY(8),
	   { 0x02020200, 0x02010200, 0x02020200 } },
	{ AR5K_PHY_RF_CTL2,
	   { 0x00000e0e, 0x00000707, 0x00000e0e } },
	{ AR5K_PHY_SETTLING,
	   { 0x1372161c, 0x13721722, 0x137216a2 } },
	{ AR5K_PHY_AGCCTL,
	   { 0x00009d10, 0x00009d18, 0x00009d18 } },
	{ AR5K_PHY_NF,
	   { 0x0001ce00, 0x0001ce00, 0x0001ce00 } },
	{ AR5K_PHY_WEAK_OFDM_HIGH_THR,
	   { 0x409a4190, 0x409a4190, 0x409a4190 } },
	{ AR5K_PHY(70),
	   { 0x000001b8, 0x00000084, 0x00000108 } },
	{ AR5K_PHY_OFDM_SELFCORR,
	   { 0x10058a05, 0x10058a05, 0x10058a05 } },
	{ 0xa230,
	   { 0x00000000, 0x00000000, 0x00000108 } },
};

/* Initial mode-specific settings for AR5212 + RF5111 (Written after ar5212_ini) */
static const struct ath5k_ini_mode rf5111_ini_mode_end[] = {
	{ AR5K_TXCFG,
	/*	A/XR          B           G       */
	   { 0x00008015, 0x00008015, 0x00008015 } },
	{ AR5K_USEC_5211,
	   { 0x128d8fa7, 0x04e00f95, 0x12e00fab } },
	{ AR5K_PHY_RF_CTL3,
	   { 0x0a020001, 0x05010100, 0x0a020001 } },
	{ AR5K_PHY_RF_CTL4,
	   { 0x00000e0e, 0x00000e0e, 0x00000e0e } },
	{ AR5K_PHY_PA_CTL,
	   { 0x00000007, 0x0000000b, 0x0000000b } },
	{ AR5K_PHY_GAIN,
	   { 0x0018da5a, 0x0018ca69, 0x0018ca69 } },
	{ AR5K_PHY_DESIRED_SIZE,
	   { 0x0de8b4e0, 0x0de8b4e0, 0x0de8b4e0 } },
	{ AR5K_PHY_SIG,
	   { 0x7e800d2e, 0x7ee84d2e, 0x7ee84d2e } },
	{ AR5K_PHY_AGCCOARSE,
	   { 0x3137665e, 0x3137665e, 0x3137665e } },
	{ AR5K_PHY_WEAK_OFDM_LOW_THR,
	   { 0x050cb081, 0x050cb081, 0x050cb080 } },
	{ AR5K_PHY_RX_DELAY,
	   { 0x00002710, 0x0000157c, 0x00002af8 } },
	{ AR5K_PHY_FRAME_CTL_5211,
	   { 0xf7b81020, 0xf7b80d20, 0xf7b81020 } },
	{ AR5K_PHY_GAIN_2GHZ,
	   { 0x642c416a, 0x6440416a, 0x6440416a } },
	{ AR5K_PHY_CCK_RX_CTL_4,
	   { 0x1883800a, 0x1873800a, 0x1883800a } },
};

static const struct ath5k_ini rf5111_ini_common_end[] = {
	{ AR5K_DCU_FP,		0x00000000 },
	{ AR5K_PHY_AGC,		0x00000000 },
	{ AR5K_PHY_ADC_CTL,	0x00022ffe },
	{ 0x983c,		0x00020100 },
	{ AR5K_PHY_GAIN_OFFSET,	0x1284613c },
	{ AR5K_PHY_PAPD_PROBE,	0x00004883 },
	{ 0x9940,		0x00000004 },
	{ 0x9958,		0x000000ff },
	{ 0x9974,		0x00000000 },
	{ AR5K_PHY_SPENDING,	0x00000018 },
	{ AR5K_PHY_CCKTXCTL,	0x00000000 },
	{ AR5K_PHY_CCK_CROSSCORR, 0xd03e6788 },
	{ AR5K_PHY_DAG_CCK_CTL,	0x000001b5 },
	{ 0xa23c,		0x13c889af },
};

/* Initial mode-specific settings for AR5212 + RF5112 (Written after ar5212_ini) */
static const struct ath5k_ini_mode rf5112_ini_mode_end[] = {
	{ AR5K_TXCFG,
	/*	A/XR          B           G       */
	   { 0x00008015, 0x00008015, 0x00008015 } },
	{ AR5K_USEC_5211,
	   { 0x128d93a7, 0x04e01395, 0x12e013ab } },
	{ AR5K_PHY_RF_CTL3,
	   { 0x0a020001, 0x05020100, 0x0a020001 } },
	{ AR5K_PHY_RF_CTL4,
	   { 0x00000e0e, 0x00000e0e, 0x00000e0e } },
	{ AR5K_PHY_PA_CTL,
	   { 0x00000007, 0x0000000b, 0x0000000b } },
	{ AR5K_PHY_GAIN,
	   { 0x0018da6d, 0x0018ca75, 0x0018ca75 } },
	{ AR5K_PHY_DESIRED_SIZE,
	   { 0x0de8b4e0, 0x0de8b4e0, 0x0de8b4e0 } },
	{ AR5K_PHY_SIG,
	   { 0x7e800d2e, 0x7ee80d2e, 0x7ee80d2e } },
	{ AR5K_PHY_AGCCOARSE,
	   { 0x3137665e, 0x3137665e, 0x3137665e } },
	{ AR5K_PHY_WEAK_OFDM_LOW_THR,
	   { 0x050cb081, 0x050cb081, 0x050cb081 } },
	{ AR5K_PHY_RX_DELAY,
	   { 0x000007d0, 0x0000044c, 0x00000898 } },
	{ AR5K_PHY_FRAME_CTL_5211,
	   { 0xf7b81020, 0xf7b80d10, 0xf7b81010 } },
	{ AR5K_PHY_CCKTXCTL,
	   { 0x00000000, 0x00000008, 0x00000008 } },
	{ AR5K_PHY_CCK_CROSSCORR,
	   { 0xd6be6788, 0xd03e6788, 0xd03e6788 } },
	{ AR5K_PHY_GAIN_2GHZ,
	   { 0x642c0140, 0x6442c160, 0x6442c160 } },
	{ AR5K_PHY_CCK_RX_CTL_4,
	   { 0x1883800a, 0x1873800a, 0x1883800a } },
};

static const struct ath5k_ini rf5112_ini_common_end[] = {
	{ AR5K_DCU_FP,		0x00000000 },
	{ AR5K_PHY_AGC,		0x00000000 },
	{ AR5K_PHY_ADC_CTL,	0x00022ffe },
	{ 0x983c,		0x00020100 },
	{ AR5K_PHY_GAIN_OFFSET,	0x1284613c },
	{ AR5K_PHY_PAPD_PROBE,	0x00004882 },
	{ 0x9940,		0x00000004 },
	{ 0x9958,		0x000000ff },
	{ 0x9974,		0x00000000 },
	{ AR5K_PHY_DAG_CCK_CTL,	0x000001b5 },
	{ 0xa23c,		0x13c889af },
};

/* Initial mode-specific settings for RF5413/5414 (Written after ar5212_ini) */
static const struct ath5k_ini_mode rf5413_ini_mode_end[] = {
	{ AR5K_TXCFG,
	/*	A/XR          B           G       */
	   { 0x00000015, 0x00000015, 0x00000015 } },
	{ AR5K_USEC_5211,
	   { 0x128d93a7, 0x04e01395, 0x12e013ab } },
	{ AR5K_PHY_RF_CTL3,
	   { 0x0a020001, 0x05020100, 0x0a020001 } },
	{ AR5K_PHY_RF_CTL4,
	   { 0x00000e0e, 0x00000e0e, 0x00000e0e } },
	{ AR5K_PHY_PA_CTL,
	   { 0x00000007, 0x0000000b, 0x0000000b } },
	{ AR5K_PHY_GAIN,
	   { 0x0018fa61, 0x001a1a63, 0x001a1a63 } },
	{ AR5K_PHY_DESIRED_SIZE,
	   { 0x0c98b4e0, 0x0c98b0da, 0x0c98b0da } },
	{ AR5K_PHY_SIG,
	   { 0x7ec80d2e, 0x7ec80d2e, 0x7ec80d2e } },
	{ AR5K_PHY_AGCCOARSE,
	   { 0x3139605e, 0x3139605e, 0x3139605e } },
	{ AR5K_PHY_WEAK_OFDM_LOW_THR,
	   { 0x050cb081, 0x050cb081, 0x050cb081 } },
	{ AR5K_PHY_RX_DELAY,
	   { 0x000007d0, 0x0000044c, 0x00000898 } },
	{ AR5K_PHY_FRAME_CTL_5211,
	   { 0xf7b81000, 0xf7b80d00, 0xf7b81000 } },
	{ AR5K_PHY_CCKTXCTL,
	   { 0x00000000, 0x00000000, 0x00000000 } },
	{ AR5K_PHY_CCK_CROSSCORR,
	   { 0xd6be6788, 0xd03e6788, 0xd03e6788 } },
	{ AR5K_PHY_GAIN_2GHZ,
	   { 0x002ec1e0, 0x002ac120, 0x002ac120 } },
	{ AR5K_PHY_CCK_RX_CTL_4,
	   { 0x1883800a, 0x1863800a, 0x1883800a } },
	{ 0xa300,
	   { 0x18010000, 0x18010000, 0x18010000 } },
	{ 0xa304,
	   { 0x30032602, 0x30032602, 0x30032602 } },
	{ 0xa308,
	   { 0x48073e06, 0x48073e06, 0x48073e06 } },
	{ 0xa30c,
	   { 0x560b4c0a, 0x560b4c0a, 0x560b4c0a } },
	{ 0xa310,
	   { 0x641a600f, 0x641a600f, 0x641a600f } },
	{ 0xa314,
	   { 0x784f6e1b, 0x784f6e1b, 0x784f6e1b } },
	{ 0xa318,
	   { 0x868f7c5a, 0x868f7c5a, 0x868f7c5a } },
	{ 0xa31c,
	   { 0x90cf865b, 0x8ecf865b, 0x8ecf865b } },
	{ 0xa320,
	   { 0x9d4f970f, 0x9b4f970f, 0x9b4f970f } },
	{ 0xa324,
	   { 0xa7cfa38f, 0xa3cf9f8f, 0xa3cf9f8f } },
	{ 0xa328,
	   { 0xb55faf1f, 0xb35faf1f, 0xb35faf1f } },
	{ 0xa32c,
	   { 0xbddfb99f, 0xbbdfb99f, 0xbbdfb99f } },
	{ 0xa330,
	   { 0xcb7fc53f, 0xcb7fc73f, 0xcb7fc73f } },
	{ 0xa334,
	   { 0xd5ffd1bf, 0xd3ffd1bf, 0xd3ffd1bf } },
};

static const struct ath5k_ini rf5413_ini_common_end[] = {
	{ AR5K_DCU_FP,		0x000003e0 },
	{ AR5K_5414_CBCFG,	0x00000010 },
	{ AR5K_SEQ_MASK,	0x0000000f },
	{ 0x809c,		0x00000000 },
	{ 0x80a0,		0x00000000 },
	{ AR5K_MIC_QOS_CTL,	0x00000000 },
	{ AR5K_MIC_QOS_SEL,	0x00000000 },
	{ AR5K_MISC_MODE,	0x00000000 },
	{ AR5K_OFDM_FIL_CNT,	0x00000000 },
	{ AR5K_CCK_FIL_CNT,	0x00000000 },
	{ AR5K_PHYERR_CNT1,	0x00000000 },
	{ AR5K_PHYERR_CNT1_MASK, 0x00000000 },
	{ AR5K_PHYERR_CNT2,	0x00000000 },
	{ AR5K_PHYERR_CNT2_MASK, 0x00000000 },
	{ AR5K_TSF_THRES,	0x00000000 },
	{ 0x8140,		0x800003f9 },
	{ 0x8144,		0x00000000 },
	{ AR5K_PHY_AGC,		0x00000000 },
	{ AR5K_PHY_ADC_CTL,	0x0000a000 },
	{ 0x983c,		0x00200400 },
	{ AR5K_PHY_GAIN_OFFSET, 0x1284233c },
	{ AR5K_PHY_SCR,		0x0000001f },
	{ AR5K_PHY_SLMT,	0x00000080 },
	{ AR5K_PHY_SCAL,	0x0000000e },
	{ 0x9958,		0x00081fff },
	{ AR5K_PHY_TIMING_7,	0x00000000 },
	{ AR5K_PHY_TIMING_8,	0x02800000 },
	{ AR5K_PHY_TIMING_11,	0x00000000 },
	{ AR5K_PHY_HEAVY_CLIP_ENABLE, 0x00000000 },
	{ 0x99e4,		0xaaaaaaaa },
	{ 0x99e8,		0x3c466478 },
	{ 0x99ec,		0x000000aa },
	{ AR5K_PHY_SCLOCK,	0x0000000c },
	{ AR5K_PHY_SDELAY,	0x000000ff },
	{ AR5K_PHY_SPENDING,	0x00000014 },
	{ AR5K_PHY_DAG_CCK_CTL, 0x000009b5 },
	{ 0xa23c,		0x93c889af },
	{ AR5K_PHY_FAST_ADC,	0x00000001 },
	{ 0xa250,		0x0000a000 },
	{ AR5K_PHY_BLUETOOTH,	0x00000000 },
	{ AR5K_PHY_TPC_RG1,	0x0cc75380 },
	{ 0xa25c,		0x0f0f0f01 },
	{ 0xa260,		0x5f690f01 },
	{ 0xa264,		0x00418a11 },
	{ 0xa268,		0x00000000 },
	{ AR5K_PHY_TPC_RG5,	0x0c30c16a },
	{ 0xa270, 0x00820820 },
	{ 0xa274, 0x081b7caa },
	{ 0xa278, 0x1ce739ce },
	{ 0xa27c, 0x051701ce },
	{ 0xa338, 0x00000000 },
	{ 0xa33c, 0x00000000 },
	{ 0xa340, 0x00000000 },
	{ 0xa344, 0x00000000 },
	{ 0xa348, 0x3fffffff },
	{ 0xa34c, 0x3fffffff },
	{ 0xa350, 0x3fffffff },
	{ 0xa354, 0x0003ffff },
	{ 0xa358, 0x79a8aa1f },
	{ 0xa35c, 0x066c420f },
	{ 0xa360, 0x0f282207 },
	{ 0xa364, 0x17601685 },
	{ 0xa368, 0x1f801104 },
	{ 0xa36c, 0x37a00c03 },
	{ 0xa370, 0x3fc40883 },
	{ 0xa374, 0x57c00803 },
	{ 0xa378, 0x5fd80682 },
	{ 0xa37c, 0x7fe00482 },
	{ 0xa380, 0x7f3c7bba },
	{ 0xa384, 0xf3307ff0 },
};

/* Initial mode-specific settings for RF2413/2414 (Written after ar5212_ini) */
/* XXX: a mode ? */
static const struct ath5k_ini_mode rf2413_ini_mode_end[] = {
	{ AR5K_TXCFG,
	/*	A/XR          B           G       */
	   { 0x00000015, 0x00000015, 0x00000015 } },
	{ AR5K_USEC_5211,
	   { 0x128d93a7, 0x04e01395, 0x12e013ab } },
	{ AR5K_PHY_RF_CTL3,
	   { 0x0a020001, 0x05020000, 0x0a020001 } },
	{ AR5K_PHY_RF_CTL4,
	   { 0x00000e00, 0x00000e00, 0x00000e00 } },
	{ AR5K_PHY_PA_CTL,
	   { 0x00000002, 0x0000000a, 0x0000000a } },
	{ AR5K_PHY_GAIN,
	   { 0x0018da6d, 0x001a6a64, 0x001a6a64 } },
	{ AR5K_PHY_DESIRED_SIZE,
	   { 0x0de8b4e0, 0x0de8b0da, 0x0c98b0da } },
	{ AR5K_PHY_SIG,
	   { 0x7e800d2e, 0x7ee80d2e, 0x7ec80d2e } },
	{ AR5K_PHY_AGCCOARSE,
	   { 0x3137665e, 0x3137665e, 0x3139605e } },
	{ AR5K_PHY_WEAK_OFDM_LOW_THR,
	   { 0x050cb081, 0x050cb081, 0x050cb081 } },
	{ AR5K_PHY_RX_DELAY,
	   { 0x000007d0, 0x0000044c, 0x00000898 } },
	{ AR5K_PHY_FRAME_CTL_5211,
	   { 0xf7b81000, 0xf7b80d00, 0xf7b81000 } },
	{ AR5K_PHY_CCKTXCTL,
	   { 0x00000000, 0x00000000, 0x00000000 } },
	{ AR5K_PHY_CCK_CROSSCORR,
	   { 0xd6be6788, 0xd03e6788, 0xd03e6788 } },
	{ AR5K_PHY_GAIN_2GHZ,
	   { 0x002c0140, 0x0042c140, 0x0042c140 } },
	{ AR5K_PHY_CCK_RX_CTL_4,
	   { 0x1883800a, 0x1863800a, 0x1883800a } },
};

static const struct ath5k_ini rf2413_ini_common_end[] = {
	{ AR5K_DCU_FP,		0x000003e0 },
	{ AR5K_SEQ_MASK,	0x0000000f },
	{ AR5K_MIC_QOS_CTL,	0x00000000 },
	{ AR5K_MIC_QOS_SEL,	0x00000000 },
	{ AR5K_MISC_MODE,	0x00000000 },
	{ AR5K_OFDM_FIL_CNT,	0x00000000 },
	{ AR5K_CCK_FIL_CNT,	0x00000000 },
	{ AR5K_PHYERR_CNT1,	0x00000000 },
	{ AR5K_PHYERR_CNT1_MASK, 0x00000000 },
	{ AR5K_PHYERR_CNT2,	0x00000000 },
	{ AR5K_PHYERR_CNT2_MASK, 0x00000000 },
	{ AR5K_TSF_THRES,	0x00000000 },
	{ 0x8140,		0x800000a8 },
	{ 0x8144,		0x00000000 },
	{ AR5K_PHY_AGC,		0x00000000 },
	{ AR5K_PHY_ADC_CTL,	0x0000a000 },
	{ 0x983c,		0x00200400 },
	{ AR5K_PHY_GAIN_OFFSET,	0x1284233c },
	{ AR5K_PHY_SCR,		0x0000001f },
	{ AR5K_PHY_SLMT,	0x00000080 },
	{ AR5K_PHY_SCAL,	0x0000000e },
	{ 0x9958,		0x000000ff },
	{ AR5K_PHY_TIMING_7,	0x00000000 },
	{ AR5K_PHY_TIMING_8,	0x02800000 },
	{ AR5K_PHY_TIMING_11,	0x00000000 },
	{ AR5K_PHY_HEAVY_CLIP_ENABLE, 0x00000000 },
	{ 0x99e4,		0xaaaaaaaa },
	{ 0x99e8,		0x3c466478 },
	{ 0x99ec,		0x000000aa },
	{ AR5K_PHY_SCLOCK,	0x0000000c },
	{ AR5K_PHY_SDELAY,	0x000000ff },
	{ AR5K_PHY_SPENDING,	0x00000014 },
	{ AR5K_PHY_DAG_CCK_CTL,	0x000009b5 },
	{ 0xa23c,		0x93c889af },
	{ AR5K_PHY_FAST_ADC,	0x00000001 },
	{ 0xa250,		0x0000a000 },
	{ AR5K_PHY_BLUETOOTH,	0x00000000 },
	{ AR5K_PHY_TPC_RG1,	0x0cc75380 },
	{ 0xa25c,		0x0f0f0f01 },
	{ 0xa260,		0x5f690f01 },
	{ 0xa264,		0x00418a11 },
	{ 0xa268,		0x00000000 },
	{ AR5K_PHY_TPC_RG5,	0x0c30c16a },
	{ 0xa270, 0x00820820 },
	{ 0xa274, 0x001b7caa },
	{ 0xa278, 0x1ce739ce },
	{ 0xa27c, 0x051701ce },
	{ 0xa300, 0x18010000 },
	{ 0xa304, 0x30032602 },
	{ 0xa308, 0x48073e06 },
	{ 0xa30c, 0x560b4c0a },
	{ 0xa310, 0x641a600f },
	{ 0xa314, 0x784f6e1b },
	{ 0xa318, 0x868f7c5a },
	{ 0xa31c, 0x8ecf865b },
	{ 0xa320, 0x9d4f970f },
	{ 0xa324, 0xa5cfa18f },
	{ 0xa328, 0xb55faf1f },
	{ 0xa32c, 0xbddfb99f },
	{ 0xa330, 0xcd7fc73f },
	{ 0xa334, 0xd5ffd1bf },
	{ 0xa338, 0x00000000 },
	{ 0xa33c, 0x00000000 },
	{ 0xa340, 0x00000000 },
	{ 0xa344, 0x00000000 },
	{ 0xa348, 0x3fffffff },
	{ 0xa34c, 0x3fffffff },
	{ 0xa350, 0x3fffffff },
	{ 0xa354, 0x0003ffff },
	{ 0xa358, 0x79a8aa1f },
	{ 0xa35c, 0x066c420f },
	{ 0xa360, 0x0f282207 },
	{ 0xa364, 0x17601685 },
	{ 0xa368, 0x1f801104 },
	{ 0xa36c, 0x37a00c03 },
	{ 0xa370, 0x3fc40883 },
	{ 0xa374, 0x57c00803 },
	{ 0xa378, 0x5fd80682 },
	{ 0xa37c, 0x7fe00482 },
	{ 0xa380, 0x7f3c7bba },
	{ 0xa384, 0xf3307ff0 },
};

/* Initial mode-specific settings for RF2425 (Written after ar5212_ini) */
/* XXX: a mode ? */
static const struct ath5k_ini_mode rf2425_ini_mode_end[] = {
	{ AR5K_TXCFG,
	/*	A/XR          B           G       */
	   { 0x00000015, 0x00000015, 0x00000015 } },
	{ AR5K_USEC_5211,
	   { 0x128d93a7, 0x04e01395, 0x12e013ab } },
	{ AR5K_PHY_RF_CTL3,
	   { 0x0a020001, 0x05020100, 0x0a020001 } },
	{ AR5K_PHY_RF_CTL4,
	   { 0x00000e0e, 0x00000e0e, 0x00000e0e } },
	{ AR5K_PHY_PA_CTL,
	   { 0x00000003, 0x0000000b, 0x0000000b } },
	{ AR5K_PHY_SETTLING,
	   { 0x1372161c, 0x13721722, 0x13721422 } },
	{ AR5K_PHY_GAIN,
	   { 0x0018fa61, 0x00199a65, 0x00199a65 } },
	{ AR5K_PHY_DESIRED_SIZE,
	   { 0x0c98b4e0, 0x0c98b0da, 0x0c98b0da } },
	{ AR5K_PHY_SIG,
	   { 0x7ec80d2e, 0x7ec80d2e, 0x7ec80d2e } },
	{ AR5K_PHY_AGCCOARSE,
	   { 0x3139605e, 0x3139605e, 0x3139605e } },
	{ AR5K_PHY_WEAK_OFDM_LOW_THR,
	   { 0x050cb081, 0x050cb081, 0x050cb081 } },
	{ AR5K_PHY_RX_DELAY,
	   { 0x000007d0, 0x0000044c, 0x00000898 } },
	{ AR5K_PHY_FRAME_CTL_5211,
	   { 0xf7b81000, 0xf7b80d00, 0xf7b81000 } },
	{ AR5K_PHY_CCKTXCTL,
	   { 0x00000000, 0x00000000, 0x00000000 } },
	{ AR5K_PHY_CCK_CROSSCORR,
	   { 0xd6be6788, 0xd03e6788, 0xd03e6788 } },
	{ AR5K_PHY_GAIN_2GHZ,
	   { 0x00000140, 0x0052c140, 0x0052c140 } },
	{ AR5K_PHY_CCK_RX_CTL_4,
	   { 0x1883800a, 0x1863800a, 0x1883800a } },
	{ 0xa324,
	   { 0xa7cfa7cf, 0xa7cfa7cf, 0xa7cfa7cf } },
	{ 0xa328,
	   { 0xa7cfa7cf, 0xa7cfa7cf, 0xa7cfa7cf } },
	{ 0xa32c,
	   { 0xa7cfa7cf, 0xa7cfa7cf, 0xa7cfa7cf } },
	{ 0xa330,
	   { 0xa7cfa7cf, 0xa7cfa7cf, 0xa7cfa7cf } },
	{ 0xa334,
	   { 0xa7cfa7cf, 0xa7cfa7cf, 0xa7cfa7cf } },
};

static const struct ath5k_ini rf2425_ini_common_end[] = {
	{ AR5K_DCU_FP,		0x000003e0 },
	{ AR5K_SEQ_MASK,	0x0000000f },
	{ 0x809c,		0x00000000 },
	{ 0x80a0,		0x00000000 },
	{ AR5K_MIC_QOS_CTL,	0x00000000 },
	{ AR5K_MIC_QOS_SEL,	0x00000000 },
	{ AR5K_MISC_MODE,	0x00000000 },
	{ AR5K_OFDM_FIL_CNT,	0x00000000 },
	{ AR5K_CCK_FIL_CNT,	0x00000000 },
	{ AR5K_PHYERR_CNT1,	0x00000000 },
	{ AR5K_PHYERR_CNT1_MASK, 0x00000000 },
	{ AR5K_PHYERR_CNT2,	0x00000000 },
	{ AR5K_PHYERR_CNT2_MASK, 0x00000000 },
	{ AR5K_TSF_THRES,	0x00000000 },
	{ 0x8140,		0x800003f9 },
	{ 0x8144,		0x00000000 },
	{ AR5K_PHY_AGC,		0x00000000 },
	{ AR5K_PHY_ADC_CTL,	0x0000a000 },
	{ 0x983c,		0x00200400 },
	{ AR5K_PHY_GAIN_OFFSET, 0x1284233c },
	{ AR5K_PHY_SCR,		0x0000001f },
	{ AR5K_PHY_SLMT,	0x00000080 },
	{ AR5K_PHY_SCAL,	0x0000000e },
	{ 0x9958,		0x00081fff },
	{ AR5K_PHY_TIMING_7,	0x00000000 },
	{ AR5K_PHY_TIMING_8,	0x02800000 },
	{ AR5K_PHY_TIMING_11,	0x00000000 },
	{ 0x99dc,		0xfebadbe8 },
	{ AR5K_PHY_HEAVY_CLIP_ENABLE, 0x00000000 },
	{ 0x99e4,		0xaaaaaaaa },
	{ 0x99e8,		0x3c466478 },
	{ 0x99ec,		0x000000aa },
	{ AR5K_PHY_SCLOCK,	0x0000000c },
	{ AR5K_PHY_SDELAY,	0x000000ff },
	{ AR5K_PHY_SPENDING,	0x00000014 },
	{ AR5K_PHY_DAG_CCK_CTL,	0x000009b5 },
	{ AR5K_PHY_TXPOWER_RATE3, 0x20202020 },
	{ AR5K_PHY_TXPOWER_RATE4, 0x20202020 },
	{ 0xa23c,		0x93c889af },
	{ AR5K_PHY_FAST_ADC,	0x00000001 },
	{ 0xa250,		0x0000a000 },
	{ AR5K_PHY_BLUETOOTH,	0x00000000 },
	{ AR5K_PHY_TPC_RG1,	0x0cc75380 },
	{ 0xa25c,		0x0f0f0f01 },
	{ 0xa260,		0x5f690f01 },
	{ 0xa264,		0x00418a11 },
	{ 0xa268,		0x00000000 },
	{ AR5K_PHY_TPC_RG5,	0x0c30c166 },
	{ 0xa270, 0x00820820 },
	{ 0xa274, 0x081a3caa },
	{ 0xa278, 0x1ce739ce },
	{ 0xa27c, 0x051701ce },
	{ 0xa300, 0x16010000 },
	{ 0xa304, 0x2c032402 },
	{ 0xa308, 0x48433e42 },
	{ 0xa30c, 0x5a0f500b },
	{ 0xa310, 0x6c4b624a },
	{ 0xa314, 0x7e8b748a },
	{ 0xa318, 0x96cf8ccb },
	{ 0xa31c, 0xa34f9d0f },
	{ 0xa320, 0xa7cfa58f },
	{ 0xa348, 0x3fffffff },
	{ 0xa34c, 0x3fffffff },
	{ 0xa350, 0x3fffffff },
	{ 0xa354, 0x0003ffff },
	{ 0xa358, 0x79a8aa1f },
	{ 0xa35c, 0x066c420f },
	{ 0xa360, 0x0f282207 },
	{ 0xa364, 0x17601685 },
	{ 0xa368, 0x1f801104 },
	{ 0xa36c, 0x37a00c03 },
	{ 0xa370, 0x3fc40883 },
	{ 0xa374, 0x57c00803 },
	{ 0xa378, 0x5fd80682 },
	{ 0xa37c, 0x7fe00482 },
	{ 0xa380, 0x7f3c7bba },
	{ 0xa384, 0xf3307ff0 },
};

/*
 * Initial BaseBand Gain settings for RF5111/5112 (AR5210 comes with
 * RF5110 only so initial BB Gain settings are included in AR5K_AR5210_INI)
 */

/* RF5111 Initial BaseBand Gain settings */
static const struct ath5k_ini rf5111_ini_bbgain[] = {
	{ AR5K_BB_GAIN(0), 0x00000000 },
	{ AR5K_BB_GAIN(1), 0x00000020 },
	{ AR5K_BB_GAIN(2), 0x00000010 },
	{ AR5K_BB_GAIN(3), 0x00000030 },
	{ AR5K_BB_GAIN(4), 0x00000008 },
	{ AR5K_BB_GAIN(5), 0x00000028 },
	{ AR5K_BB_GAIN(6), 0x00000004 },
	{ AR5K_BB_GAIN(7), 0x00000024 },
	{ AR5K_BB_GAIN(8), 0x00000014 },
	{ AR5K_BB_GAIN(9), 0x00000034 },
	{ AR5K_BB_GAIN(10), 0x0000000c },
	{ AR5K_BB_GAIN(11), 0x0000002c },
	{ AR5K_BB_GAIN(12), 0x00000002 },
	{ AR5K_BB_GAIN(13), 0x00000022 },
	{ AR5K_BB_GAIN(14), 0x00000012 },
	{ AR5K_BB_GAIN(15), 0x00000032 },
	{ AR5K_BB_GAIN(16), 0x0000000a },
	{ AR5K_BB_GAIN(17), 0x0000002a },
	{ AR5K_BB_GAIN(18), 0x00000006 },
	{ AR5K_BB_GAIN(19), 0x00000026 },
	{ AR5K_BB_GAIN(20), 0x00000016 },
	{ AR5K_BB_GAIN(21), 0x00000036 },
	{ AR5K_BB_GAIN(22), 0x0000000e },
	{ AR5K_BB_GAIN(23), 0x0000002e },
	{ AR5K_BB_GAIN(24), 0x00000001 },
	{ AR5K_BB_GAIN(25), 0x00000021 },
	{ AR5K_BB_GAIN(26), 0x00000011 },
	{ AR5K_BB_GAIN(27), 0x00000031 },
	{ AR5K_BB_GAIN(28), 0x00000009 },
	{ AR5K_BB_GAIN(29), 0x00000029 },
	{ AR5K_BB_GAIN(30), 0x00000005 },
	{ AR5K_BB_GAIN(31), 0x00000025 },
	{ AR5K_BB_GAIN(32), 0x00000015 },
	{ AR5K_BB_GAIN(33), 0x00000035 },
	{ AR5K_BB_GAIN(34), 0x0000000d },
	{ AR5K_BB_GAIN(35), 0x0000002d },
	{ AR5K_BB_GAIN(36), 0x00000003 },
	{ AR5K_BB_GAIN(37), 0x00000023 },
	{ AR5K_BB_GAIN(38), 0x00000013 },
	{ AR5K_BB_GAIN(39), 0x00000033 },
	{ AR5K_BB_GAIN(40), 0x0000000b },
	{ AR5K_BB_GAIN(41), 0x0000002b },
	{ AR5K_BB_GAIN(42), 0x0000002b },
	{ AR5K_BB_GAIN(43), 0x0000002b },
	{ AR5K_BB_GAIN(44), 0x0000002b },
	{ AR5K_BB_GAIN(45), 0x0000002b },
	{ AR5K_BB_GAIN(46), 0x0000002b },
	{ AR5K_BB_GAIN(47), 0x0000002b },
	{ AR5K_BB_GAIN(48), 0x0000002b },
	{ AR5K_BB_GAIN(49), 0x0000002b },
	{ AR5K_BB_GAIN(50), 0x0000002b },
	{ AR5K_BB_GAIN(51), 0x0000002b },
	{ AR5K_BB_GAIN(52), 0x0000002b },
	{ AR5K_BB_GAIN(53), 0x0000002b },
	{ AR5K_BB_GAIN(54), 0x0000002b },
	{ AR5K_BB_GAIN(55), 0x0000002b },
	{ AR5K_BB_GAIN(56), 0x0000002b },
	{ AR5K_BB_GAIN(57), 0x0000002b },
	{ AR5K_BB_GAIN(58), 0x0000002b },
	{ AR5K_BB_GAIN(59), 0x0000002b },
	{ AR5K_BB_GAIN(60), 0x0000002b },
	{ AR5K_BB_GAIN(61), 0x0000002b },
	{ AR5K_BB_GAIN(62), 0x00000002 },
	{ AR5K_BB_GAIN(63), 0x00000016 },
};

/* RF5112 Initial BaseBand Gain settings (Same for RF5413/5414+) */
static const struct ath5k_ini rf5112_ini_bbgain[] = {
	{ AR5K_BB_GAIN(0), 0x00000000 },
	{ AR5K_BB_GAIN(1), 0x00000001 },
	{ AR5K_BB_GAIN(2), 0x00000002 },
	{ AR5K_BB_GAIN(3), 0x00000003 },
	{ AR5K_BB_GAIN(4), 0x00000004 },
	{ AR5K_BB_GAIN(5), 0x00000005 },
	{ AR5K_BB_GAIN(6), 0x00000008 },
	{ AR5K_BB_GAIN(7), 0x00000009 },
	{ AR5K_BB_GAIN(8), 0x0000000a },
	{ AR5K_BB_GAIN(9), 0x0000000b },
	{ AR5K_BB_GAIN(10), 0x0000000c },
	{ AR5K_BB_GAIN(11), 0x0000000d },
	{ AR5K_BB_GAIN(12), 0x00000010 },
	{ AR5K_BB_GAIN(13), 0x00000011 },
	{ AR5K_BB_GAIN(14), 0x00000012 },
	{ AR5K_BB_GAIN(15), 0x00000013 },
	{ AR5K_BB_GAIN(16), 0x00000014 },
	{ AR5K_BB_GAIN(17), 0x00000015 },
	{ AR5K_BB_GAIN(18), 0x00000018 },
	{ AR5K_BB_GAIN(19), 0x00000019 },
	{ AR5K_BB_GAIN(20), 0x0000001a },
	{ AR5K_BB_GAIN(21), 0x0000001b },
	{ AR5K_BB_GAIN(22), 0x0000001c },
	{ AR5K_BB_GAIN(23), 0x0000001d },
	{ AR5K_BB_GAIN(24), 0x00000020 },
	{ AR5K_BB_GAIN(25), 0x00000021 },
	{ AR5K_BB_GAIN(26), 0x00000022 },
	{ AR5K_BB_GAIN(27), 0x00000023 },
	{ AR5K_BB_GAIN(28), 0x00000024 },
	{ AR5K_BB_GAIN(29), 0x00000025 },
	{ AR5K_BB_GAIN(30), 0x00000028 },
	{ AR5K_BB_GAIN(31), 0x00000029 },
	{ AR5K_BB_GAIN(32), 0x0000002a },
	{ AR5K_BB_GAIN(33), 0x0000002b },
	{ AR5K_BB_GAIN(34), 0x0000002c },
	{ AR5K_BB_GAIN(35), 0x0000002d },
	{ AR5K_BB_GAIN(36), 0x00000030 },
	{ AR5K_BB_GAIN(37), 0x00000031 },
	{ AR5K_BB_GAIN(38), 0x00000032 },
	{ AR5K_BB_GAIN(39), 0x00000033 },
	{ AR5K_BB_GAIN(40), 0x00000034 },
	{ AR5K_BB_GAIN(41), 0x00000035 },
	{ AR5K_BB_GAIN(42), 0x00000035 },
	{ AR5K_BB_GAIN(43), 0x00000035 },
	{ AR5K_BB_GAIN(44), 0x00000035 },
	{ AR5K_BB_GAIN(45), 0x00000035 },
	{ AR5K_BB_GAIN(46), 0x00000035 },
	{ AR5K_BB_GAIN(47), 0x00000035 },
	{ AR5K_BB_GAIN(48), 0x00000035 },
	{ AR5K_BB_GAIN(49), 0x00000035 },
	{ AR5K_BB_GAIN(50), 0x00000035 },
	{ AR5K_BB_GAIN(51), 0x00000035 },
	{ AR5K_BB_GAIN(52), 0x00000035 },
	{ AR5K_BB_GAIN(53), 0x00000035 },
	{ AR5K_BB_GAIN(54), 0x00000035 },
	{ AR5K_BB_GAIN(55), 0x00000035 },
	{ AR5K_BB_GAIN(56), 0x00000035 },
	{ AR5K_BB_GAIN(57), 0x00000035 },
	{ AR5K_BB_GAIN(58), 0x00000035 },
	{ AR5K_BB_GAIN(59), 0x00000035 },
	{ AR5K_BB_GAIN(60), 0x00000035 },
	{ AR5K_BB_GAIN(61), 0x00000035 },
	{ AR5K_BB_GAIN(62), 0x00000010 },
	{ AR5K_BB_GAIN(63), 0x0000001a },
};


/*
 * Write initial register dump
 */
static void ath5k_hw_ini_registers(struct ath5k_hw *ah, unsigned int size,
		const struct ath5k_ini *ini_regs, bool skip_pcu)
{
	unsigned int i;

	/* Write initial registers */
	for (i = 0; i < size; i++) {
		/* Skip PCU registers if
		 * requested */
		if (skip_pcu &&
				ini_regs[i].ini_register >= AR5K_PCU_MIN &&
				ini_regs[i].ini_register <= AR5K_PCU_MAX)
			continue;

		switch (ini_regs[i].ini_mode) {
		case AR5K_INI_READ:
			/* Cleared on read */
			ath5k_hw_reg_read(ah, ini_regs[i].ini_register);
			break;
		case AR5K_INI_WRITE:
		default:
			AR5K_REG_WAIT(i);
			ath5k_hw_reg_write(ah, ini_regs[i].ini_value,
					ini_regs[i].ini_register);
		}
	}
}

static void ath5k_hw_ini_mode_registers(struct ath5k_hw *ah,
		unsigned int size, const struct ath5k_ini_mode *ini_mode,
		u8 mode)
{
	unsigned int i;

	for (i = 0; i < size; i++) {
		AR5K_REG_WAIT(i);
		ath5k_hw_reg_write(ah, ini_mode[i].mode_value[mode],
			(u32)ini_mode[i].mode_register);
	}

}

int ath5k_hw_write_initvals(struct ath5k_hw *ah, u8 mode, bool skip_pcu)
{
	/*
	 * Write initial register settings
	 */

	/* For AR5212 and compatible */
	if (ah->ah_version == AR5K_AR5212) {

		/* First set of mode-specific settings */
		ath5k_hw_ini_mode_registers(ah,
			ARRAY_SIZE(ar5212_ini_mode_start),
			ar5212_ini_mode_start, mode);

		/*
		 * Write initial settings common for all modes
		 */
		ath5k_hw_ini_registers(ah, ARRAY_SIZE(ar5212_ini_common_start),
				ar5212_ini_common_start, skip_pcu);

		/* Second set of mode-specific settings */
		switch (ah->ah_radio) {
		case AR5K_RF5111:

			ath5k_hw_ini_mode_registers(ah,
					ARRAY_SIZE(rf5111_ini_mode_end),
					rf5111_ini_mode_end, mode);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5111_ini_common_end),
					rf5111_ini_common_end, skip_pcu);

			/* Baseband gain table */
			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5111_ini_bbgain),
					rf5111_ini_bbgain, skip_pcu);

			break;
		case AR5K_RF5112:

			ath5k_hw_ini_mode_registers(ah,
					ARRAY_SIZE(rf5112_ini_mode_end),
					rf5112_ini_mode_end, mode);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5112_ini_common_end),
					rf5112_ini_common_end, skip_pcu);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5112_ini_bbgain),
					rf5112_ini_bbgain, skip_pcu);

			break;
		case AR5K_RF5413:

			ath5k_hw_ini_mode_registers(ah,
					ARRAY_SIZE(rf5413_ini_mode_end),
					rf5413_ini_mode_end, mode);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5413_ini_common_end),
					rf5413_ini_common_end, skip_pcu);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5112_ini_bbgain),
					rf5112_ini_bbgain, skip_pcu);

			break;
		case AR5K_RF2316:
		case AR5K_RF2413:

			ath5k_hw_ini_mode_registers(ah,
					ARRAY_SIZE(rf2413_ini_mode_end),
					rf2413_ini_mode_end, mode);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf2413_ini_common_end),
					rf2413_ini_common_end, skip_pcu);

			/* Override settings from rf2413_ini_common_end */
			if (ah->ah_radio == AR5K_RF2316) {
				ath5k_hw_reg_write(ah, 0x00004000,
							AR5K_PHY_AGC);
				ath5k_hw_reg_write(ah, 0x081b7caa,
							0xa274);
			}

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5112_ini_bbgain),
					rf5112_ini_bbgain, skip_pcu);
			break;
		case AR5K_RF2317:

			ath5k_hw_ini_mode_registers(ah,
					ARRAY_SIZE(rf2413_ini_mode_end),
					rf2413_ini_mode_end, mode);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf2425_ini_common_end),
					rf2425_ini_common_end, skip_pcu);

			/* Override settings from rf2413_ini_mode_end */
			ath5k_hw_reg_write(ah, 0x00180a65, AR5K_PHY_GAIN);

			/* Override settings from rf2413_ini_common_end */
			ath5k_hw_reg_write(ah, 0x00004000, AR5K_PHY_AGC);
			AR5K_REG_WRITE_BITS(ah, AR5K_PHY_TPC_RG5,
				AR5K_PHY_TPC_RG5_PD_GAIN_OVERLAP, 0xa);
			ath5k_hw_reg_write(ah, 0x800000a8, 0x8140);
			ath5k_hw_reg_write(ah, 0x000000ff, 0x9958);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5112_ini_bbgain),
					rf5112_ini_bbgain, skip_pcu);
			break;
		case AR5K_RF2425:

			ath5k_hw_ini_mode_registers(ah,
					ARRAY_SIZE(rf2425_ini_mode_end),
					rf2425_ini_mode_end, mode);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf2425_ini_common_end),
					rf2425_ini_common_end, skip_pcu);

			ath5k_hw_ini_registers(ah,
					ARRAY_SIZE(rf5112_ini_bbgain),
					rf5112_ini_bbgain, skip_pcu);
			break;
		default:
			return -EINVAL;

		}

	/* For AR5211 */
	} else if (ah->ah_version == AR5K_AR5211) {

		/* AR5K_MODE_11B */
		if (mode > 2) {
			ATH5K_ERR(ah,
				"unsupported channel mode: %d\n", mode);
			return -EINVAL;
		}

		/* Mode-specific settings */
		ath5k_hw_ini_mode_registers(ah, ARRAY_SIZE(ar5211_ini_mode),
				ar5211_ini_mode, mode);

		/*
		 * Write initial settings common for all modes
		 */
		ath5k_hw_ini_registers(ah, ARRAY_SIZE(ar5211_ini),
				ar5211_ini, skip_pcu);

		/* AR5211 only comes with 5111 */

		/* Baseband gain table */
		ath5k_hw_ini_registers(ah, ARRAY_SIZE(rf5111_ini_bbgain),
				rf5111_ini_bbgain, skip_pcu);
	/* For AR5210 (for mode settings check out ath5k_hw_reset_tx_queue) */
	} else if (ah->ah_version == AR5K_AR5210) {
		ath5k_hw_ini_registers(ah, ARRAY_SIZE(ar5210_ini),
				ar5210_ini, skip_pcu);
	}

	return 0;
}
