/*
 * Coldfire generic GPIO support
 *
 * (C) Copyright 2009, Steven King <sfking@fdwdc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/mcfgpio.h>

static struct mcf_gpio_chip mcf_gpio_chips[] = {
	{
		.gpio_chip			= {
			.label			= "PIRQ",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value,
			.base			= 1,
			.ngpio			= 7,
		},
		.pddr				= (void __iomem *) MCFEPORT_EPDDR,
		.podr				= (void __iomem *) MCFEPORT_EPDR,
		.ppdr				= (void __iomem *) MCFEPORT_EPPDR,
	},
	{
		.gpio_chip			= {
			.label			= "ADDR",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 13,
			.ngpio			= 3,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_ADDR,
		.podr				= (void __iomem *) MCFGPIO_PODR_ADDR,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_ADDR,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_ADDR,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_ADDR,
	},
	{
		.gpio_chip			= {
			.label			= "DATAH",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 16,
			.ngpio			= 8,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_DATAH,
		.podr				= (void __iomem *) MCFGPIO_PODR_DATAH,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_DATAH,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_DATAH,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_DATAH,
	},
	{
		.gpio_chip			= {
			.label			= "DATAL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 24,
			.ngpio			= 8,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_DATAL,
		.podr				= (void __iomem *) MCFGPIO_PODR_DATAL,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_DATAL,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_DATAL,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_DATAL,
	},
	{
		.gpio_chip			= {
			.label			= "BUSCTL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 32,
			.ngpio			= 8,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_BUSCTL,
		.podr				= (void __iomem *) MCFGPIO_PODR_BUSCTL,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_BUSCTL,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_BUSCTL,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_BUSCTL,
	},
	{
		.gpio_chip			= {
			.label			= "BS",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 40,
			.ngpio			= 4,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_BS,
		.podr				= (void __iomem *) MCFGPIO_PODR_BS,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_BS,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_BS,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_BS,
	},
	{
		.gpio_chip			= {
			.label			= "CS",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 49,
			.ngpio			= 7,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_CS,
		.podr				= (void __iomem *) MCFGPIO_PODR_CS,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_CS,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_CS,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_CS,
	},
	{
		.gpio_chip			= {
			.label			= "SDRAM",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 56,
			.ngpio			= 6,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_SDRAM,
		.podr				= (void __iomem *) MCFGPIO_PODR_SDRAM,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_SDRAM,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_SDRAM,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_SDRAM,
	},
	{
		.gpio_chip			= {
			.label			= "FECI2C",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 64,
			.ngpio			= 4,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_FECI2C,
		.podr				= (void __iomem *) MCFGPIO_PODR_FECI2C,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_FECI2C,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_FECI2C,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_FECI2C,
	},
	{
		.gpio_chip			= {
			.label			= "UARTH",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 72,
			.ngpio			= 2,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_UARTH,
		.podr				= (void __iomem *) MCFGPIO_PODR_UARTH,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_UARTH,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_UARTH,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_UARTH,
	},
	{
		.gpio_chip			= {
			.label			= "UARTL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 80,
			.ngpio			= 8,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_UARTL,
		.podr				= (void __iomem *) MCFGPIO_PODR_UARTL,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_UARTL,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_UARTL,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_UARTL,
	},
	{
		.gpio_chip			= {
			.label			= "QSPI",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 88,
			.ngpio			= 5,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_QSPI,
		.podr				= (void __iomem *) MCFGPIO_PODR_QSPI,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_QSPI,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_QSPI,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_QSPI,
	},
	{
		.gpio_chip			= {
			.label			= "TIMER",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 96,
			.ngpio			= 8,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_TIMER,
		.podr				= (void __iomem *) MCFGPIO_PODR_TIMER,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_TIMER,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_TIMER,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_TIMER,
	},
	{
		.gpio_chip			= {
			.label			= "ETPU",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 104,
			.ngpio			= 3,
		},
		.pddr				= (void __iomem *) MCFGPIO_PDDR_ETPU,
		.podr				= (void __iomem *) MCFGPIO_PODR_ETPU,
		.ppdr				= (void __iomem *) MCFGPIO_PPDSDR_ETPU,
		.setr				= (void __iomem *) MCFGPIO_PPDSDR_ETPU,
		.clrr				= (void __iomem *) MCFGPIO_PCLRR_ETPU,
	},
};

static int __init mcf_gpio_init(void)
{
	unsigned i = 0;
	while (i < ARRAY_SIZE(mcf_gpio_chips))
		(void)gpiochip_add((struct gpio_chip *)&mcf_gpio_chips[i++]);
	return 0;
}

core_initcall(mcf_gpio_init);
