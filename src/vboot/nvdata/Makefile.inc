##
## Copyright 2019 Google Inc.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##

depthcharge-$(CONFIG_NVDATA_CMOS) += cmos.c
depthcharge-$(CONFIG_NVDATA_DISK) += disk.c
depthcharge-$(CONFIG_NVDATA_FAKE) += fake.c
depthcharge-$(CONFIG_NVDATA_FLASH) += flash.c