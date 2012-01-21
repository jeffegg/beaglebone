/*******************************************************************************

  Intel 10 Gigabit PCI Express Linux driver
  Copyright(c) 1999 - 2011 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  Linux NICS <linux.nics@intel.com>
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/


#include "ixgbe.h"
#include "ixgbe_type.h"
#include "ixgbe_dcb.h"
#include "ixgbe_dcb_82598.h"
#include "ixgbe_dcb_82599.h"

/**
 * ixgbe_ieee_credits - This calculates the ieee traffic class
 * credits from the configured bandwidth percentages. Credits
 * are the smallest unit programmable into the underlying
 * hardware. The IEEE 802.1Qaz specification do not use bandwidth
 * groups so this is much simplified from the CEE case.
 */
s32 ixgbe_ieee_credits(__u8 *bw, __u16 *refill, __u16 *max, int max_frame)
{
	int min_percent = 100;
	int min_credit, multiplier;
	int i;

	min_credit = ((max_frame / 2) + DCB_CREDIT_QUANTUM - 1) /
			DCB_CREDIT_QUANTUM;

	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		if (bw[i] < min_percent && bw[i])
			min_percent = bw[i];
	}

	multiplier = (min_credit / min_percent) + 1;

	/* Find out the hw credits for each TC */
	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		int val = min(bw[i] * multiplier, MAX_CREDIT_REFILL);

		if (val < min_credit)
			val = min_credit;
		refill[i] = val;

		max[i] = bw[i] ? (bw[i] * MAX_CREDIT)/100 : min_credit;
	}
	return 0;
}

/**
 * ixgbe_dcb_calculate_tc_credits - Calculates traffic class credits
 * @ixgbe_dcb_config: Struct containing DCB settings.
 * @direction: Configuring either Tx or Rx.
 *
 * This function calculates the credits allocated to each traffic class.
 * It should be called only after the rules are checked by
 * ixgbe_dcb_check_config().
 */
s32 ixgbe_dcb_calculate_tc_credits(struct ixgbe_hw *hw,
				   struct ixgbe_dcb_config *dcb_config,
				   int max_frame, u8 direction)
{
	struct tc_bw_alloc *p;
	int min_credit;
	int min_multiplier;
	int min_percent = 100;
	s32 ret_val = 0;
	/* Initialization values default for Tx settings */
	u32 credit_refill       = 0;
	u32 credit_max          = 0;
	u16 link_percentage     = 0;
	u8  bw_percent          = 0;
	u8  i;

	if (dcb_config == NULL) {
		ret_val = DCB_ERR_CONFIG;
		goto out;
	}

	min_credit = ((max_frame / 2) + DCB_CREDIT_QUANTUM - 1) /
			DCB_CREDIT_QUANTUM;

	/* Find smallest link percentage */
	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		p = &dcb_config->tc_config[i].path[direction];
		bw_percent = dcb_config->bw_percentage[direction][p->bwg_id];
		link_percentage = p->bwg_percent;

		link_percentage = (link_percentage * bw_percent) / 100;

		if (link_percentage && link_percentage < min_percent)
			min_percent = link_percentage;
	}

	/*
	 * The ratio between traffic classes will control the bandwidth
	 * percentages seen on the wire. To calculate this ratio we use
	 * a multiplier. It is required that the refill credits must be
	 * larger than the max frame size so here we find the smallest
	 * multiplier that will allow all bandwidth percentages to be
	 * greater than the max frame size.
	 */
	min_multiplier = (min_credit / min_percent) + 1;

	/* Find out the link percentage for each TC first */
	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		p = &dcb_config->tc_config[i].path[direction];
		bw_percent = dcb_config->bw_percentage[direction][p->bwg_id];

		link_percentage = p->bwg_percent;
		/* Must be careful of integer division for very small nums */
		link_percentage = (link_percentage * bw_percent) / 100;
		if (p->bwg_percent > 0 && link_percentage == 0)
			link_percentage = 1;

		/* Save link_percentage for reference */
		p->link_percent = (u8)link_percentage;

		/* Calculate credit refill ratio using multiplier */
		credit_refill = min(link_percentage * min_multiplier,
				    MAX_CREDIT_REFILL);
		p->data_credits_refill = (u16)credit_refill;

		/* Calculate maximum credit for the TC */
		credit_max = (link_percentage * MAX_CREDIT) / 100;

		/*
		 * Adjustment based on rule checking, if the percentage
		 * of a TC is too small, the maximum credit may not be
		 * enough to send out a jumbo frame in data plane arbitration.
		 */
		if (credit_max && (credit_max < min_credit))
			credit_max = min_credit;

		if (direction == DCB_TX_CONFIG) {
			/*
			 * Adjustment based on rule checking, if the
			 * percentage of a TC is too small, the maximum
			 * credit may not be enough to send out a TSO
			 * packet in descriptor plane arbitration.
			 */
			if ((hw->mac.type == ixgbe_mac_82598EB) &&
			    credit_max &&
			    (credit_max < MINIMUM_CREDIT_FOR_TSO))
				credit_max = MINIMUM_CREDIT_FOR_TSO;

			dcb_config->tc_config[i].desc_credits_max =
				(u16)credit_max;
		}

		p->data_credits_max = (u16)credit_max;
	}

out:
	return ret_val;
}

void ixgbe_dcb_unpack_pfc(struct ixgbe_dcb_config *cfg, u8 *pfc_en)
{
	int i;

	*pfc_en = 0;
	for (i = 0; i < MAX_TRAFFIC_CLASS; i++)
		*pfc_en |= (cfg->tc_config[i].dcb_pfc & 0xF) << i;
}

void ixgbe_dcb_unpack_refill(struct ixgbe_dcb_config *cfg, int direction,
			     u16 *refill)
{
	struct tc_bw_alloc *p;
	int i;

	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		p = &cfg->tc_config[i].path[direction];
		refill[i] = p->data_credits_refill;
	}
}

void ixgbe_dcb_unpack_max(struct ixgbe_dcb_config *cfg, u16 *max)
{
	int i;

	for (i = 0; i < MAX_TRAFFIC_CLASS; i++)
		max[i] = cfg->tc_config[i].desc_credits_max;
}

void ixgbe_dcb_unpack_bwgid(struct ixgbe_dcb_config *cfg, int direction,
			    u8 *bwgid)
{
	struct tc_bw_alloc *p;
	int i;

	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		p = &cfg->tc_config[i].path[direction];
		bwgid[i] = p->bwg_id;
	}
}

void ixgbe_dcb_unpack_prio(struct ixgbe_dcb_config *cfg, int direction,
			    u8 *ptype)
{
	struct tc_bw_alloc *p;
	int i;

	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		p = &cfg->tc_config[i].path[direction];
		ptype[i] = p->prio_type;
	}
}

/**
 * ixgbe_dcb_hw_config - Config and enable DCB
 * @hw: pointer to hardware structure
 * @dcb_config: pointer to ixgbe_dcb_config structure
 *
 * Configure dcb settings and enable dcb mode.
 */
s32 ixgbe_dcb_hw_config(struct ixgbe_hw *hw,
                        struct ixgbe_dcb_config *dcb_config)
{
	s32 ret = 0;
	u8 pfc_en;
	u8 ptype[MAX_TRAFFIC_CLASS];
	u8 bwgid[MAX_TRAFFIC_CLASS];
	u16 refill[MAX_TRAFFIC_CLASS];
	u16 max[MAX_TRAFFIC_CLASS];
	/* CEE does not define a priority to tc mapping so map 1:1 */
	u8 prio_tc[MAX_TRAFFIC_CLASS] = {0, 1, 2, 3, 4, 5, 6, 7};

	/* Unpack CEE standard containers */
	ixgbe_dcb_unpack_pfc(dcb_config, &pfc_en);
	ixgbe_dcb_unpack_refill(dcb_config, DCB_TX_CONFIG, refill);
	ixgbe_dcb_unpack_max(dcb_config, max);
	ixgbe_dcb_unpack_bwgid(dcb_config, DCB_TX_CONFIG, bwgid);
	ixgbe_dcb_unpack_prio(dcb_config, DCB_TX_CONFIG, ptype);

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		ret = ixgbe_dcb_hw_config_82598(hw, pfc_en, refill, max,
						bwgid, ptype);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		ret = ixgbe_dcb_hw_config_82599(hw, pfc_en, refill, max,
						bwgid, ptype, prio_tc);
		break;
	default:
		break;
	}
	return ret;
}

/* Helper routines to abstract HW specifics from DCB netlink ops */
s32 ixgbe_dcb_hw_pfc_config(struct ixgbe_hw *hw, u8 pfc_en)
{
	int ret = -EINVAL;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		ret = ixgbe_dcb_config_pfc_82598(hw, pfc_en);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		ret = ixgbe_dcb_config_pfc_82599(hw, pfc_en);
		break;
	default:
		break;
	}
	return ret;
}

s32 ixgbe_dcb_hw_ets_config(struct ixgbe_hw *hw,
			    u16 *refill, u16 *max, u8 *bwg_id,
			    u8 *prio_type, u8 *prio_tc)
{
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		ixgbe_dcb_config_rx_arbiter_82598(hw, refill, max,
							prio_type);
		ixgbe_dcb_config_tx_desc_arbiter_82598(hw, refill, max,
							     bwg_id, prio_type);
		ixgbe_dcb_config_tx_data_arbiter_82598(hw, refill, max,
							     bwg_id, prio_type);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		ixgbe_dcb_config_rx_arbiter_82599(hw, refill, max,
						  bwg_id, prio_type, prio_tc);
		ixgbe_dcb_config_tx_desc_arbiter_82599(hw, refill, max,
						       bwg_id, prio_type);
		ixgbe_dcb_config_tx_data_arbiter_82599(hw, refill, max, bwg_id,
						       prio_type, prio_tc);
		break;
	default:
		break;
	}
	return 0;
}
