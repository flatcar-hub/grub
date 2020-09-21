/* linux.h - ARM linux specific definitions */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2013  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRUB_ARM_LINUX_HEADER
#define GRUB_ARM_LINUX_HEADER 1

#include "system.h"

#define GRUB_LINUX_ARM_MAGIC_SIGNATURE 0x016f2818

struct linux_arm_kernel_header {
  grub_uint32_t code0;
  grub_uint32_t reserved1[8];
  grub_uint32_t magic;
  grub_uint32_t start; /* _start */
  grub_uint32_t end;   /* _edata */
  grub_uint32_t reserved2[4];
  grub_uint32_t hdr_offset;
};

#if defined GRUB_MACHINE_UBOOT
# include <grub/uboot/uboot.h>
# define LINUX_ADDRESS        (start_of_ram + 0x8000)
# define LINUX_INITRD_ADDRESS (start_of_ram + 0x02000000)
# define LINUX_FDT_ADDRESS    (LINUX_INITRD_ADDRESS - 0x10000)
# define grub_arm_firmware_get_boot_data grub_uboot_get_boot_data
# define grub_arm_firmware_get_machine_type grub_uboot_get_machine_type
#elif defined GRUB_MACHINE_EFI
# include <grub/efi/efi.h>
# include <grub/machine/loader.h>
/* On UEFI platforms - load the images at the lowest available address not
   less than *_PHYS_OFFSET from the first available memory location. */
# define LINUX_PHYS_OFFSET        (0x00008000)
# define LINUX_INITRD_PHYS_OFFSET (LINUX_PHYS_OFFSET + 0x02000000)
# define LINUX_FDT_PHYS_OFFSET    (LINUX_INITRD_PHYS_OFFSET - 0x10000)
# define grub_arm_firmware_get_boot_data (grub_addr_t)grub_efi_get_firmware_fdt
static inline grub_uint32_t
grub_arm_firmware_get_machine_type (void)
{
  return GRUB_ARM_MACHINE_TYPE_FDT;
}
#elif defined (GRUB_MACHINE_COREBOOT)
#include <grub/fdtbus.h>
#include <grub/machine/kernel.h>
# define LINUX_ADDRESS        (start_of_ram + 0x8000)
# define LINUX_INITRD_ADDRESS (start_of_ram + 0x02000000)
# define LINUX_FDT_ADDRESS    (LINUX_INITRD_ADDRESS - 0x10000)
static inline const void *
grub_arm_firmware_get_boot_data (void)
{
  return grub_fdtbus_get_fdt ();
}
static inline grub_uint32_t
grub_arm_firmware_get_machine_type (void)
{
  return GRUB_ARM_MACHINE_TYPE_FDT;
}
#endif

#define FDT_ADDITIONAL_ENTRIES_SIZE	0x300

#endif /* ! GRUB_ARM_LINUX_HEADER */
