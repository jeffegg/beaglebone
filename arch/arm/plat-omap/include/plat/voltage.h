/*
 * OMAP Voltage Management Routines
 *
 * Author: Thara Gopinath	<thara@ti.com>
 *
 * Copyright (C) 2009 Texas Instruments, Inc.
 * Thara Gopinath <thara@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_OMAP_VOLTAGE_H
#define __ARCH_ARM_OMAP_VOLTAGE_H

#include <linux/err.h>

#include <plat/vc.h>
#include <plat/vp.h>

struct powerdomain;

/* XXX document */
#define VOLTSCALE_VPFORCEUPDATE		1
#define VOLTSCALE_VCBYPASS		2

/*
 * OMAP3 GENERIC setup times. Revisit to see if these needs to be
 * passed from board or PMIC file
 */
#define OMAP3_CLKSETUP		0xff
#define OMAP3_VOLTOFFSET	0xff
#define OMAP3_VOLTSETUP2	0xff

struct omap_vdd_info;

/**
 * struct omap_vfsm_instance - per-voltage manager FSM register/bitfield
 * data
 * @voltsetup_mask: SETUP_TIME* bitmask in the PRM_VOLTSETUP* register
 * @voltsetup_reg: register offset of PRM_VOLTSETUP from PRM base
 *
 * XXX What about VOLTOFFSET/VOLTCTRL?
 */
struct omap_vfsm_instance {
	u32 voltsetup_mask;
	u8 voltsetup_reg;
};

/**
 * struct voltagedomain - omap voltage domain global structure.
 * @name: Name of the voltage domain which can be used as a unique identifier.
 * @scalable: Whether or not this voltage domain is scalable
 * @node: list_head linking all voltage domains
 * @pwrdm_list: list_head linking all powerdomains in this voltagedomain
 * @vc: pointer to VC channel associated with this voltagedomain
 * @vp: pointer to VP associated with this voltagedomain
 * @read: read a VC/VP register
 * @write: write a VC/VP register
 * @read: read-modify-write a VC/VP register
 * @sys_clk: system clock name/frequency, used for various timing calculations
 * @scale: function used to scale the voltage of the voltagedomain
 * @nominal_volt: current nominal voltage for this voltage domain
 * @volt_data: voltage table having the distinct voltages supported
 *             by the domain and other associated per voltage data.
 */
struct voltagedomain {
	char *name;
	bool scalable;
	struct list_head node;
	struct list_head pwrdm_list;
	struct omap_vc_channel *vc;
	const struct omap_vfsm_instance *vfsm;
	struct omap_vp_instance *vp;
	struct omap_voltdm_pmic *pmic;

	/* VC/VP register access functions: SoC specific */
	u32 (*read) (u8 offset);
	void (*write) (u32 val, u8 offset);
	u32 (*rmw)(u32 mask, u32 bits, u8 offset);

	union {
		const char *name;
		u32 rate;
	} sys_clk;

	int (*scale) (struct voltagedomain *voltdm,
		      unsigned long target_volt);

	u32 nominal_volt;
	struct omap_volt_data *volt_data;
	struct omap_vdd_info *vdd;
	struct dentry *debug_dir;

	bool use_regulator;
	struct regulator *regulator;
	int (*regulator_init) (struct voltagedomain *voltdm);
};

/**
 * struct omap_volt_data - Omap voltage specific data.
 * @voltage_nominal:	The possible voltage value in uV
 * @sr_efuse_offs:	The offset of the efuse register(from system
 *			control module base address) from where to read
 *			the n-target value for the smartreflex module.
 * @sr_errminlimit:	Error min limit value for smartreflex. This value
 *			differs at differnet opp and thus is linked
 *			with voltage.
 * @vp_errorgain:	Error gain value for the voltage processor. This
 *			field also differs according to the voltage/opp.
 */
struct omap_volt_data {
	u32	volt_nominal;
	u32	sr_efuse_offs;
	u8	sr_errminlimit;
	u8	vp_errgain;
};

/**
 * struct omap_voltdm_pmic - PMIC specific data required by voltage driver.
 * @slew_rate:	PMIC slew rate (in uv/us)
 * @step_size:	PMIC voltage step size (in uv)
 * @i2c_slave_addr: I2C slave address of PMIC
 * @volt_reg_addr: voltage configuration register address
 * @cmd_reg_addr: command (on, on-LP, ret, off) configuration register address
 * @i2c_high_speed: whether VC uses I2C high-speed mode to PMIC
 * @i2c_mcode: master code value for I2C high-speed preamble transmission
 * @vsel_to_uv:	PMIC API to convert vsel value to actual voltage in uV.
 * @uv_to_vsel:	PMIC API to convert voltage in uV to vsel value.
 */
struct omap_voltdm_pmic {
	int slew_rate;
	int step_size;
	u32 on_volt;
	u32 onlp_volt;
	u32 ret_volt;
	u32 off_volt;
	u16 volt_setup_time;
	u16 i2c_slave_addr;
	u16 volt_reg_addr;
	u16 cmd_reg_addr;
	u8 vp_erroroffset;
	u8 vp_vstepmin;
	u8 vp_vstepmax;
	u8 vp_vddmin;
	u8 vp_vddmax;
	u8 vp_timeout_us;
	bool i2c_high_speed;
	u8 i2c_mcode;
	unsigned long (*vsel_to_uv) (const u8 vsel);
	u8 (*uv_to_vsel) (unsigned long uV);
};

/**
 * struct omap_vdd_dep_volt - Map table for voltage dependencies
 * @main_vdd_volt	: The main vdd voltage
 * @dep_vdd_volt	: The voltage at which the dependent vdd should be
 *			  when the main vdd is at <main_vdd_volt> voltage
 *
 * Table containing the parent vdd voltage and the dependent vdd voltage
 * corresponding to it.
 */
struct omap_vdd_dep_volt {
	u32 main_vdd_volt;
	u32 dep_vdd_volt;
};

/**
 * struct omap_vdd_dep_info -  Dependent vdd info
 * @name		: Dependent vdd name
 * @_dep_voltdm		: internal structure meant to prevent multiple lookups
 * @dep_table		: Table containing the dependent vdd voltage
 *			  corresponding to every main vdd voltage.
 * @nr_dep_entries	: number of dependency voltage entries
 */
struct omap_vdd_dep_info {
	char *name;
	struct voltagedomain *_dep_voltdm;
	struct omap_vdd_dep_volt *dep_table;
	int nr_dep_entries;
};

/**
 * omap_vdd_info - Per Voltage Domain info
 *
 * @volt_data		: voltage table having the distinct voltages supported
 *			  by the domain and other associated per voltage data.
 * @dep_vdd_info	: Array ending with a 0 terminator for dependency
 *			  voltage information.
 */
struct omap_vdd_info {
	struct omap_volt_data *volt_data;
	struct omap_vdd_dep_info *dep_vdd_info;
};

void omap_voltage_get_volttable(struct voltagedomain *voltdm,
		struct omap_volt_data **volt_data);
struct omap_volt_data *omap_voltage_get_voltdata(struct voltagedomain *voltdm,
		unsigned long volt);
int omap_voltage_register_pmic(struct voltagedomain *voltdm,
			       struct omap_voltdm_pmic *pmic);
void omap_change_voltscale_method(struct voltagedomain *voltdm,
		int voltscale_method);
int omap_voltage_late_init(void);

extern void omap2xxx_voltagedomains_init(void);
extern void omap3xxx_voltagedomains_init(void);
extern void am33xx_voltagedomains_init(void);
extern void omap44xx_voltagedomains_init(void);

struct voltagedomain *voltdm_lookup(const char *name);
void voltdm_init(struct voltagedomain **voltdm_list);
int voltdm_add_pwrdm(struct voltagedomain *voltdm, struct powerdomain *pwrdm);
int voltdm_for_each(int (*fn)(struct voltagedomain *voltdm, void *user),
		    void *user);
int voltdm_for_each_pwrdm(struct voltagedomain *voltdm,
			  int (*fn)(struct voltagedomain *voltdm,
				    struct powerdomain *pwrdm));
int voltdm_scale(struct voltagedomain *voltdm, unsigned long target_volt);
void voltdm_reset(struct voltagedomain *voltdm);
unsigned long voltdm_get_voltage(struct voltagedomain *voltdm);
#endif
