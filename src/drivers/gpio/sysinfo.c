/*
 * Copyright (c) 2012 The Chromium OS Authors.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <libpayload.h>

#include "drivers/gpio/sysinfo.h"

struct cb_gpio *sysinfo_lookup_gpio(const char *name)
{
	for (int i = 0; i < lib_sysinfo.num_gpios; i++) {
		if (!strncmp((char *)lib_sysinfo.gpios[i].name, name,
				CB_GPIO_MAX_NAME_LENGTH))
			return &lib_sysinfo.gpios[i];
	}

	printf("Failed to find gpio %s\n", name);
	return NULL;
}
