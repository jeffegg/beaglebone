/*
 * miscellaneous helper functions
 *
 * Copyright (c) Clemens Ladisch <clemens@ladisch.de>
 * Licensed under the terms of the GNU General Public License, version 2.
 */

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/firewire.h>
#include <linux/module.h>
#include "lib.h"

#define ERROR_RETRY_DELAY_MS	5

/**
 * rcode_string - convert a firewire result code to a string
 * @rcode: the result
 */
const char *rcode_string(unsigned int rcode)
{
	static const char *const names[] = {
		[RCODE_COMPLETE]	= "complete",
		[RCODE_CONFLICT_ERROR]	= "conflict error",
		[RCODE_DATA_ERROR]	= "data error",
		[RCODE_TYPE_ERROR]	= "type error",
		[RCODE_ADDRESS_ERROR]	= "address error",
		[RCODE_SEND_ERROR]	= "send error",
		[RCODE_CANCELLED]	= "cancelled",
		[RCODE_BUSY]		= "busy",
		[RCODE_GENERATION]	= "generation",
		[RCODE_NO_ACK]		= "no ack",
	};

	if (rcode < ARRAY_SIZE(names) && names[rcode])
		return names[rcode];
	else
		return "unknown";
}
EXPORT_SYMBOL(rcode_string);

/**
 * snd_fw_transaction - send a request and wait for its completion
 * @unit: the driver's unit on the target device
 * @tcode: the transaction code
 * @offset: the address in the target's address space
 * @buffer: input/output data
 * @length: length of @buffer
 *
 * Submits an asynchronous request to the target device, and waits for the
 * response.  The node ID and the current generation are derived from @unit.
 * On a bus reset or an error, the transaction is retried a few times.
 * Returns zero on success, or a negative error code.
 */
int snd_fw_transaction(struct fw_unit *unit, int tcode,
		       u64 offset, void *buffer, size_t length)
{
	struct fw_device *device = fw_parent_device(unit);
	int generation, rcode, tries = 0;

	for (;;) {
		generation = device->generation;
		smp_rmb(); /* node_id vs. generation */
		rcode = fw_run_transaction(device->card, tcode,
					   device->node_id, generation,
					   device->max_speed, offset,
					   buffer, length);

		if (rcode == RCODE_COMPLETE)
			return 0;

		if (rcode_is_permanent_error(rcode) || ++tries >= 3) {
			dev_err(&unit->device, "transaction failed: %s\n",
				rcode_string(rcode));
			return -EIO;
		}

		msleep(ERROR_RETRY_DELAY_MS);
	}
}
EXPORT_SYMBOL(snd_fw_transaction);

MODULE_DESCRIPTION("FireWire audio helper functions");
MODULE_AUTHOR("Clemens Ladisch <clemens@ladisch.de>");
MODULE_LICENSE("GPL v2");
