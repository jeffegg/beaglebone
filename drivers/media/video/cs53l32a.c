/*
 * cs53l32a (Adaptec AVC-2010 and AVC-2410) i2c ivtv driver.
 * Copyright (C) 2005  Martin Vaughan
 *
 * Audio source switching for Adaptec AVC-2410 added by Trev Jackson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/i2c.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-ctrls.h>

MODULE_DESCRIPTION("i2c device driver for cs53l32a Audio ADC");
MODULE_AUTHOR("Martin Vaughan");
MODULE_LICENSE("GPL");

static int debug;

module_param(debug, bool, 0644);

MODULE_PARM_DESC(debug, "Debugging messages, 0=Off (default), 1=On");


struct cs53l32a_state {
	struct v4l2_subdev sd;
	struct v4l2_ctrl_handler hdl;
};

static inline struct cs53l32a_state *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct cs53l32a_state, sd);
}

static inline struct v4l2_subdev *to_sd(struct v4l2_ctrl *ctrl)
{
	return &container_of(ctrl->handler, struct cs53l32a_state, hdl)->sd;
}

/* ----------------------------------------------------------------------- */

static int cs53l32a_write(struct v4l2_subdev *sd, u8 reg, u8 value)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	return i2c_smbus_write_byte_data(client, reg, value);
}

static int cs53l32a_read(struct v4l2_subdev *sd, u8 reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	return i2c_smbus_read_byte_data(client, reg);
}

static int cs53l32a_s_routing(struct v4l2_subdev *sd,
			      u32 input, u32 output, u32 config)
{
	/* There are 2 physical inputs, but the second input can be
	   placed in two modes, the first mode bypasses the PGA (gain),
	   the second goes through the PGA. Hence there are three
	   possible inputs to choose from. */
	if (input > 2) {
		v4l2_err(sd, "Invalid input %d.\n", input);
		return -EINVAL;
	}
	cs53l32a_write(sd, 0x01, 0x01 + (input << 4));
	return 0;
}

static int cs53l32a_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_subdev *sd = to_sd(ctrl);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		cs53l32a_write(sd, 0x03, ctrl->val ? 0xf0 : 0x30);
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		cs53l32a_write(sd, 0x04, (u8)ctrl->val);
		cs53l32a_write(sd, 0x05, (u8)ctrl->val);
		return 0;
	}
	return -EINVAL;
}

static int cs53l32a_g_chip_ident(struct v4l2_subdev *sd, struct v4l2_dbg_chip_ident *chip)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	return v4l2_chip_ident_i2c_client(client,
			chip, V4L2_IDENT_CS53l32A, 0);
}

static int cs53l32a_log_status(struct v4l2_subdev *sd)
{
	struct cs53l32a_state *state = to_state(sd);
	u8 v = cs53l32a_read(sd, 0x01);

	v4l2_info(sd, "Input:  %d\n", (v >> 4) & 3);
	v4l2_ctrl_handler_log_status(&state->hdl, sd->name);
	return 0;
}

/* ----------------------------------------------------------------------- */

static const struct v4l2_ctrl_ops cs53l32a_ctrl_ops = {
	.s_ctrl = cs53l32a_s_ctrl,
};

static const struct v4l2_subdev_core_ops cs53l32a_core_ops = {
	.log_status = cs53l32a_log_status,
	.g_chip_ident = cs53l32a_g_chip_ident,
	.g_ext_ctrls = v4l2_subdev_g_ext_ctrls,
	.try_ext_ctrls = v4l2_subdev_try_ext_ctrls,
	.s_ext_ctrls = v4l2_subdev_s_ext_ctrls,
	.g_ctrl = v4l2_subdev_g_ctrl,
	.s_ctrl = v4l2_subdev_s_ctrl,
	.queryctrl = v4l2_subdev_queryctrl,
	.querymenu = v4l2_subdev_querymenu,
};

static const struct v4l2_subdev_audio_ops cs53l32a_audio_ops = {
	.s_routing = cs53l32a_s_routing,
};

static const struct v4l2_subdev_ops cs53l32a_ops = {
	.core = &cs53l32a_core_ops,
	.audio = &cs53l32a_audio_ops,
};

/* ----------------------------------------------------------------------- */

/* i2c implementation */

/*
 * Generic i2c probe
 * concerning the addresses: i2c wants 7 bit (without the r/w bit), so '>>1'
 */

static int cs53l32a_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	struct cs53l32a_state *state;
	struct v4l2_subdev *sd;
	int i;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;

	if (!id)
		strlcpy(client->name, "cs53l32a", sizeof(client->name));

	v4l_info(client, "chip found @ 0x%x (%s)\n",
			client->addr << 1, client->adapter->name);

	state = kzalloc(sizeof(struct cs53l32a_state), GFP_KERNEL);
	if (state == NULL)
		return -ENOMEM;
	sd = &state->sd;
	v4l2_i2c_subdev_init(sd, client, &cs53l32a_ops);

	for (i = 1; i <= 7; i++) {
		u8 v = cs53l32a_read(sd, i);

		v4l2_dbg(1, debug, sd, "Read Reg %d %02x\n", i, v);
	}

	v4l2_ctrl_handler_init(&state->hdl, 2);
	v4l2_ctrl_new_std(&state->hdl, &cs53l32a_ctrl_ops,
			V4L2_CID_AUDIO_VOLUME, -96, 12, 1, 0);
	v4l2_ctrl_new_std(&state->hdl, &cs53l32a_ctrl_ops,
			V4L2_CID_AUDIO_MUTE, 0, 1, 1, 0);
	sd->ctrl_handler = &state->hdl;
	if (state->hdl.error) {
		int err = state->hdl.error;

		v4l2_ctrl_handler_free(&state->hdl);
		kfree(state);
		return err;
	}

	/* Set cs53l32a internal register for Adaptec 2010/2410 setup */

	cs53l32a_write(sd, 0x01, 0x21);
	cs53l32a_write(sd, 0x02, 0x29);
	cs53l32a_write(sd, 0x03, 0x30);
	cs53l32a_write(sd, 0x04, 0x00);
	cs53l32a_write(sd, 0x05, 0x00);
	cs53l32a_write(sd, 0x06, 0x00);
	cs53l32a_write(sd, 0x07, 0x00);

	/* Display results, should be 0x21,0x29,0x30,0x00,0x00,0x00,0x00 */

	for (i = 1; i <= 7; i++) {
		u8 v = cs53l32a_read(sd, i);

		v4l2_dbg(1, debug, sd, "Read Reg %d %02x\n", i, v);
	}
	return 0;
}

static int cs53l32a_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct cs53l32a_state *state = to_state(sd);

	v4l2_device_unregister_subdev(sd);
	v4l2_ctrl_handler_free(&state->hdl);
	kfree(state);
	return 0;
}

static const struct i2c_device_id cs53l32a_id[] = {
	{ "cs53l32a", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, cs53l32a_id);

static struct i2c_driver cs53l32a_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "cs53l32a",
	},
	.probe		= cs53l32a_probe,
	.remove		= cs53l32a_remove,
	.id_table	= cs53l32a_id,
};

static __init int init_cs53l32a(void)
{
	return i2c_add_driver(&cs53l32a_driver);
}

static __exit void exit_cs53l32a(void)
{
	i2c_del_driver(&cs53l32a_driver);
}

module_init(init_cs53l32a);
module_exit(exit_cs53l32a);
