
/* Microcode patches for the CPM as supplied by Motorola.
 * This is the one for IIC/SPI.  There is a newer one that
 * also relocates SMC2, but this would require additional changes
 * to uart.c, so I am holding off on that for a moment.
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/mpc8xx.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/8xx_immap.h>
#include <asm/cpm.h>
#include <asm/cpm1.h>

/*
 * I2C/SPI relocation patch arrays.
 */

#ifdef CONFIG_I2C_SPI_UCODE_PATCH

static uint patch_2000[] __initdata = {
	0x7FFFEFD9,
	0x3FFD0000,
	0x7FFB49F7,
	0x7FF90000,
	0x5FEFADF7,
	0x5F89ADF7,
	0x5FEFAFF7,
	0x5F89AFF7,
	0x3A9CFBC8,
	0xE7C0EDF0,
	0x77C1E1BB,
	0xF4DC7F1D,
	0xABAD932F,
	0x4E08FDCF,
	0x6E0FAFF8,
	0x7CCF76CF,
	0xFD1FF9CF,
	0xABF88DC6,
	0xAB5679F7,
	0xB0937383,
	0xDFCE79F7,
	0xB091E6BB,
	0xE5BBE74F,
	0xB3FA6F0F,
	0x6FFB76CE,
	0xEE0DF9CF,
	0x2BFBEFEF,
	0xCFEEF9CF,
	0x76CEAD24,
	0x90B2DF9A,
	0x7FDDD0BF,
	0x4BF847FD,
	0x7CCF76CE,
	0xCFEF7E1F,
	0x7F1D7DFD,
	0xF0B6EF71,
	0x7FC177C1,
	0xFBC86079,
	0xE722FBC8,
	0x5FFFDFFF,
	0x5FB2FFFB,
	0xFBC8F3C8,
	0x94A67F01,
	0x7F1D5F39,
	0xAFE85F5E,
	0xFFDFDF96,
	0xCB9FAF7D,
	0x5FC1AFED,
	0x8C1C5FC1,
	0xAFDD5FC3,
	0xDF9A7EFD,
	0xB0B25FB2,
	0xFFFEABAD,
	0x5FB2FFFE,
	0x5FCE600B,
	0xE6BB600B,
	0x5FCEDFC6,
	0x27FBEFDF,
	0x5FC8CFDE,
	0x3A9CE7C0,
	0xEDF0F3C8,
	0x7F0154CD,
	0x7F1D2D3D,
	0x363A7570,
	0x7E0AF1CE,
	0x37EF2E68,
	0x7FEE10EC,
	0xADF8EFDE,
	0xCFEAE52F,
	0x7D0FE12B,
	0xF1CE5F65,
	0x7E0A4DF8,
	0xCFEA5F72,
	0x7D0BEFEE,
	0xCFEA5F74,
	0xE522EFDE,
	0x5F74CFDA,
	0x0B627385,
	0xDF627E0A,
	0x30D8145B,
	0xBFFFF3C8,
	0x5FFFDFFF,
	0xA7F85F5E,
	0xBFFE7F7D,
	0x10D31450,
	0x5F36BFFF,
	0xAF785F5E,
	0xBFFDA7F8,
	0x5F36BFFE,
	0x77FD30C0,
	0x4E08FDCF,
	0xE5FF6E0F,
	0xAFF87E1F,
	0x7E0FFD1F,
	0xF1CF5F1B,
	0xABF80D5E,
	0x5F5EFFEF,
	0x79F730A2,
	0xAFDD5F34,
	0x47F85F34,
	0xAFED7FDD,
	0x50B24978,
	0x47FD7F1D,
	0x7DFD70AD,
	0xEF717EC1,
	0x6BA47F01,
	0x2D267EFD,
	0x30DE5F5E,
	0xFFFD5F5E,
	0xFFEF5F5E,
	0xFFDF0CA0,
	0xAFED0A9E,
	0xAFDD0C3A,
	0x5F3AAFBD,
	0x7FBDB082,
	0x5F8247F8
};

static uint patch_2f00[] __initdata = {
	0x3E303430,
	0x34343737,
	0xABF7BF9B,
	0x994B4FBD,
	0xBD599493,
	0x349FFF37,
	0xFB9B177D,
	0xD9936956,
	0xBBFDD697,
	0xBDD2FD11,
	0x31DB9BB3,
	0x63139637,
	0x93733693,
	0x193137F7,
	0x331737AF,
	0x7BB9B999,
	0xBB197957,
	0x7FDFD3D5,
	0x73B773F7,
	0x37933B99,
	0x1D115316,
	0x99315315,
	0x31694BF4,
	0xFBDBD359,
	0x31497353,
	0x76956D69,
	0x7B9D9693,
	0x13131979,
	0x79376935
};
#endif

/*
 * I2C/SPI/SMC1 relocation patch arrays.
 */

#ifdef CONFIG_I2C_SPI_SMC1_UCODE_PATCH

static uint patch_2000[] __initdata = {
	0x3fff0000,
	0x3ffd0000,
	0x3ffb0000,
	0x3ff90000,
	0x5f13eff8,
	0x5eb5eff8,
	0x5f88adf7,
	0x5fefadf7,
	0x3a9cfbc8,
	0x77cae1bb,
	0xf4de7fad,
	0xabae9330,
	0x4e08fdcf,
	0x6e0faff8,
	0x7ccf76cf,
	0xfdaff9cf,
	0xabf88dc8,
	0xab5879f7,
	0xb0925d8d,
	0xdfd079f7,
	0xb090e6bb,
	0xe5bbe74f,
	0x9e046f0f,
	0x6ffb76ce,
	0xee0cf9cf,
	0x2bfbefef,
	0xcfeef9cf,
	0x76cead23,
	0x90b3df99,
	0x7fddd0c1,
	0x4bf847fd,
	0x7ccf76ce,
	0xcfef77ca,
	0x7eaf7fad,
	0x7dfdf0b7,
	0xef7a7fca,
	0x77cafbc8,
	0x6079e722,
	0xfbc85fff,
	0xdfff5fb3,
	0xfffbfbc8,
	0xf3c894a5,
	0xe7c9edf9,
	0x7f9a7fad,
	0x5f36afe8,
	0x5f5bffdf,
	0xdf95cb9e,
	0xaf7d5fc3,
	0xafed8c1b,
	0x5fc3afdd,
	0x5fc5df99,
	0x7efdb0b3,
	0x5fb3fffe,
	0xabae5fb3,
	0xfffe5fd0,
	0x600be6bb,
	0x600b5fd0,
	0xdfc827fb,
	0xefdf5fca,
	0xcfde3a9c,
	0xe7c9edf9,
	0xf3c87f9e,
	0x54ca7fed,
	0x2d3a3637,
	0x756f7e9a,
	0xf1ce37ef,
	0x2e677fee,
	0x10ebadf8,
	0xefdecfea,
	0xe52f7d9f,
	0xe12bf1ce,
	0x5f647e9a,
	0x4df8cfea,
	0x5f717d9b,
	0xefeecfea,
	0x5f73e522,
	0xefde5f73,
	0xcfda0b61,
	0x5d8fdf61,
	0xe7c9edf9,
	0x7e9a30d5,
	0x1458bfff,
	0xf3c85fff,
	0xdfffa7f8,
	0x5f5bbffe,
	0x7f7d10d0,
	0x144d5f33,
	0xbfffaf78,
	0x5f5bbffd,
	0xa7f85f33,
	0xbffe77fd,
	0x30bd4e08,
	0xfdcfe5ff,
	0x6e0faff8,
	0x7eef7e9f,
	0xfdeff1cf,
	0x5f17abf8,
	0x0d5b5f5b,
	0xffef79f7,
	0x309eafdd,
	0x5f3147f8,
	0x5f31afed,
	0x7fdd50af,
	0x497847fd,
	0x7f9e7fed,
	0x7dfd70a9,
	0xef7e7ece,
	0x6ba07f9e,
	0x2d227efd,
	0x30db5f5b,
	0xfffd5f5b,
	0xffef5f5b,
	0xffdf0c9c,
	0xafed0a9a,
	0xafdd0c37,
	0x5f37afbd,
	0x7fbdb081,
	0x5f8147f8,
	0x3a11e710,
	0xedf0ccdd,
	0xf3186d0a,
	0x7f0e5f06,
	0x7fedbb38,
	0x3afe7468,
	0x7fedf4fc,
	0x8ffbb951,
	0xb85f77fd,
	0xb0df5ddd,
	0xdefe7fed,
	0x90e1e74d,
	0x6f0dcbf7,
	0xe7decfed,
	0xcb74cfed,
	0xcfeddf6d,
	0x91714f74,
	0x5dd2deef,
	0x9e04e7df,
	0xefbb6ffb,
	0xe7ef7f0e,
	0x9e097fed,
	0xebdbeffa,
	0xeb54affb,
	0x7fea90d7,
	0x7e0cf0c3,
	0xbffff318,
	0x5fffdfff,
	0xac59efea,
	0x7fce1ee5,
	0xe2ff5ee1,
	0xaffbe2ff,
	0x5ee3affb,
	0xf9cc7d0f,
	0xaef8770f,
	0x7d0fb0c6,
	0xeffbbfff,
	0xcfef5ede,
	0x7d0fbfff,
	0x5ede4cf8,
	0x7fddd0bf,
	0x49f847fd,
	0x7efdf0bb,
	0x7fedfffd,
	0x7dfdf0b7,
	0xef7e7e1e,
	0x5ede7f0e,
	0x3a11e710,
	0xedf0ccab,
	0xfb18ad2e,
	0x1ea9bbb8,
	0x74283b7e,
	0x73c2e4bb,
	0x2ada4fb8,
	0xdc21e4bb,
	0xb2a1ffbf,
	0x5e2c43f8,
	0xfc87e1bb,
	0xe74ffd91,
	0x6f0f4fe8,
	0xc7ba32e2,
	0xf396efeb,
	0x600b4f78,
	0xe5bb760b,
	0x53acaef8,
	0x4ef88b0e,
	0xcfef9e09,
	0xabf8751f,
	0xefef5bac,
	0x741f4fe8,
	0x751e760d,
	0x7fdbf081,
	0x741cafce,
	0xefcc7fce,
	0x751e70ac,
	0x741ce7bb,
	0x3372cfed,
	0xafdbefeb,
	0xe5bb760b,
	0x53f2aef8,
	0xafe8e7eb,
	0x4bf8771e,
	0x7e247fed,
	0x4fcbe2cc,
	0x7fbc30a9,
	0x7b0f7a0f,
	0x34d577fd,
	0x308b5db7,
	0xde553e5f,
	0xaf78741f,
	0x741f30f0,
	0xcfef5e2c,
	0x741f3eac,
	0xafb8771e,
	0x5e677fed,
	0x0bd3e2cc,
	0x741ccfec,
	0xe5ca53cd,
	0x6fcb4f74,
	0x5dadde4b,
	0x2ab63d38,
	0x4bb3de30,
	0x751f741c,
	0x6c42effa,
	0xefea7fce,
	0x6ffc30be,
	0xefec3fca,
	0x30b3de2e,
	0xadf85d9e,
	0xaf7daefd,
	0x5d9ede2e,
	0x5d9eafdd,
	0x761f10ac,
	0x1da07efd,
	0x30adfffe,
	0x4908fb18,
	0x5fffdfff,
	0xafbb709b,
	0x4ef85e67,
	0xadf814ad,
	0x7a0f70ad,
	0xcfef50ad,
	0x7a0fde30,
	0x5da0afed,
	0x3c12780f,
	0xefef780f,
	0xefef790f,
	0xa7f85e0f,
	0xffef790f,
	0xefef790f,
	0x14adde2e,
	0x5d9eadfd,
	0x5e2dfffb,
	0xe79addfd,
	0xeff96079,
	0x607ae79a,
	0xddfceff9,
	0x60795dff,
	0x607acfef,
	0xefefefdf,
	0xefbfef7f,
	0xeeffedff,
	0xebffe7ff,
	0xafefafdf,
	0xafbfaf7f,
	0xaeffadff,
	0xabffa7ff,
	0x6fef6fdf,
	0x6fbf6f7f,
	0x6eff6dff,
	0x6bff67ff,
	0x2fef2fdf,
	0x2fbf2f7f,
	0x2eff2dff,
	0x2bff27ff,
	0x4e08fd1f,
	0xe5ff6e0f,
	0xaff87eef,
	0x7e0ffdef,
	0xf11f6079,
	0xabf8f542,
	0x7e0af11c,
	0x37cfae3a,
	0x7fec90be,
	0xadf8efdc,
	0xcfeae52f,
	0x7d0fe12b,
	0xf11c6079,
	0x7e0a4df8,
	0xcfea5dc4,
	0x7d0befec,
	0xcfea5dc6,
	0xe522efdc,
	0x5dc6cfda,
	0x4e08fd1f,
	0x6e0faff8,
	0x7c1f761f,
	0xfdeff91f,
	0x6079abf8,
	0x761cee24,
	0xf91f2bfb,
	0xefefcfec,
	0xf91f6079,
	0x761c27fb,
	0xefdf5da7,
	0xcfdc7fdd,
	0xd09c4bf8,
	0x47fd7c1f,
	0x761ccfcf,
	0x7eef7fed,
	0x7dfdf093,
	0xef7e7f1e,
	0x771efb18,
	0x6079e722,
	0xe6bbe5bb,
	0xae0ae5bb,
	0x600bae85,
	0xe2bbe2bb,
	0xe2bbe2bb,
	0xaf02e2bb,
	0xe2bb2ff9,
	0x6079e2bb
};

static uint patch_2f00[] __initdata = {
	0x30303030,
	0x3e3e3434,
	0xabbf9b99,
	0x4b4fbdbd,
	0x59949334,
	0x9fff37fb,
	0x9b177dd9,
	0x936956bb,
	0xfbdd697b,
	0xdd2fd113,
	0x1db9f7bb,
	0x36313963,
	0x79373369,
	0x3193137f,
	0x7331737a,
	0xf7bb9b99,
	0x9bb19795,
	0x77fdfd3d,
	0x573b773f,
	0x737933f7,
	0xb991d115,
	0x31699315,
	0x31531694,
	0xbf4fbdbd,
	0x35931497,
	0x35376956,
	0xbd697b9d,
	0x96931313,
	0x19797937,
	0x6935af78,
	0xb9b3baa3,
	0xb8788683,
	0x368f78f7,
	0x87778733,
	0x3ffffb3b,
	0x8e8f78b8,
	0x1d118e13,
	0xf3ff3f8b,
	0x6bd8e173,
	0xd1366856,
	0x68d1687b,
	0x3daf78b8,
	0x3a3a3f87,
	0x8f81378f,
	0xf876f887,
	0x77fd8778,
	0x737de8d6,
	0xbbf8bfff,
	0xd8df87f7,
	0xfd876f7b,
	0x8bfff8bd,
	0x8683387d,
	0xb873d87b,
	0x3b8fd7f8,
	0xf7338883,
	0xbb8ee1f8,
	0xef837377,
	0x3337b836,
	0x817d11f8,
	0x7378b878,
	0xd3368b7d,
	0xed731b7d,
	0x833731f3,
	0xf22f3f23
};

static uint patch_2e00[] __initdata = {
	0x27eeeeee,
	0xeeeeeeee,
	0xeeeeeeee,
	0xeeeeeeee,
	0xee4bf4fb,
	0xdbd259bb,
	0x1979577f,
	0xdfd2d573,
	0xb773f737,
	0x4b4fbdbd,
	0x25b9b177,
	0xd2d17376,
	0x956bbfdd,
	0x697bdd2f,
	0xff9f79ff,
	0xff9ff22f
};
#endif

/*
 *  USB SOF patch arrays.
 */

#ifdef CONFIG_USB_SOF_UCODE_PATCH

static uint patch_2000[] __initdata = {
	0x7fff0000,
	0x7ffd0000,
	0x7ffb0000,
	0x49f7ba5b,
	0xba383ffb,
	0xf9b8b46d,
	0xe5ab4e07,
	0xaf77bffe,
	0x3f7bbf79,
	0xba5bba38,
	0xe7676076,
	0x60750000
};

static uint patch_2f00[] __initdata = {
	0x3030304c,
	0xcab9e441,
	0xa1aaf220
};
#endif

void __init cpm_load_patch(cpm8xx_t *cp)
{
	volatile uint		*dp;		/* Dual-ported RAM. */
	volatile cpm8xx_t	*commproc;
#if defined(CONFIG_I2C_SPI_UCODE_PATCH) || \
    defined(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)
	volatile iic_t		*iip;
	volatile struct spi_pram *spp;
#ifdef CONFIG_I2C_SPI_SMC1_UCODE_PATCH
	volatile smc_uart_t	*smp;
#endif
#endif
	int	i;

	commproc = cp;

#ifdef CONFIG_USB_SOF_UCODE_PATCH
	commproc->cp_rccr = 0;

	dp = (uint *)(commproc->cp_dpmem);
	for (i=0; i<(sizeof(patch_2000)/4); i++)
		*dp++ = patch_2000[i];

	dp = (uint *)&(commproc->cp_dpmem[0x0f00]);
	for (i=0; i<(sizeof(patch_2f00)/4); i++)
		*dp++ = patch_2f00[i];

	commproc->cp_rccr = 0x0009;

	printk("USB SOF microcode patch installed\n");
#endif /* CONFIG_USB_SOF_UCODE_PATCH */

#if defined(CONFIG_I2C_SPI_UCODE_PATCH) || \
    defined(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)

	commproc->cp_rccr = 0;

	dp = (uint *)(commproc->cp_dpmem);
	for (i=0; i<(sizeof(patch_2000)/4); i++)
		*dp++ = patch_2000[i];

	dp = (uint *)&(commproc->cp_dpmem[0x0f00]);
	for (i=0; i<(sizeof(patch_2f00)/4); i++)
		*dp++ = patch_2f00[i];

	iip = (iic_t *)&commproc->cp_dparam[PROFF_IIC];
# define RPBASE 0x0500
	iip->iic_rpbase = RPBASE;

	/* Put SPI above the IIC, also 32-byte aligned.
	*/
	i = (RPBASE + sizeof(iic_t) + 31) & ~31;
	spp = (struct spi_pram *)&commproc->cp_dparam[PROFF_SPI];
	spp->rpbase = i;

# if defined(CONFIG_I2C_SPI_UCODE_PATCH)
	commproc->cp_cpmcr1 = 0x802a;
	commproc->cp_cpmcr2 = 0x8028;
	commproc->cp_cpmcr3 = 0x802e;
	commproc->cp_cpmcr4 = 0x802c;
	commproc->cp_rccr = 1;

	printk("I2C/SPI microcode patch installed.\n");
# endif /* CONFIG_I2C_SPI_UCODE_PATCH */

# if defined(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)

	dp = (uint *)&(commproc->cp_dpmem[0x0e00]);
	for (i=0; i<(sizeof(patch_2e00)/4); i++)
		*dp++ = patch_2e00[i];

	commproc->cp_cpmcr1 = 0x8080;
	commproc->cp_cpmcr2 = 0x808a;
	commproc->cp_cpmcr3 = 0x8028;
	commproc->cp_cpmcr4 = 0x802a;
	commproc->cp_rccr = 3;

	smp = (smc_uart_t *)&commproc->cp_dparam[PROFF_SMC1];
	smp->smc_rpbase = 0x1FC0;

	printk("I2C/SPI/SMC1 microcode patch installed.\n");
# endif /* CONFIG_I2C_SPI_SMC1_UCODE_PATCH) */

#endif /* some variation of the I2C/SPI patch was selected */
}

/*
 *  Take this entire routine out, since no one calls it and its
 * logic is suspect.
 */

#if 0
void
verify_patch(volatile immap_t *immr)
{
	volatile uint		*dp;
	volatile cpm8xx_t	*commproc;
	int i;

	commproc = (cpm8xx_t *)&immr->im_cpm;

	printk("cp_rccr %x\n", commproc->cp_rccr);
	commproc->cp_rccr = 0;

	dp = (uint *)(commproc->cp_dpmem);
	for (i=0; i<(sizeof(patch_2000)/4); i++)
		if (*dp++ != patch_2000[i]) {
			printk("patch_2000 bad at %d\n", i);
			dp--;
			printk("found 0x%X, wanted 0x%X\n", *dp, patch_2000[i]);
			break;
		}

	dp = (uint *)&(commproc->cp_dpmem[0x0f00]);
	for (i=0; i<(sizeof(patch_2f00)/4); i++)
		if (*dp++ != patch_2f00[i]) {
			printk("patch_2f00 bad at %d\n", i);
			dp--;
			printk("found 0x%X, wanted 0x%X\n", *dp, patch_2f00[i]);
			break;
		}

	commproc->cp_rccr = 0x0009;
}
#endif
