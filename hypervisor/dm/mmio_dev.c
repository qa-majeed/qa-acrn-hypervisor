/*
 * Copyright (C) 2020 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <util.h>
#include <acrn_hv_defs.h>
#include <asm/pgtable.h>
#include <asm/guest/vm.h>
#include <asm/guest/ept.h>

int32_t assign_mmio_dev(struct acrn_vm *vm, const struct acrn_mmiodev *mmiodev)
{
	int32_t ret = -EINVAL;

	if (mem_aligned_check(mmiodev->user_vm_pa, PAGE_SIZE) &&
			mem_aligned_check(mmiodev->service_vm_pa, PAGE_SIZE) &&
			mem_aligned_check(mmiodev->size, PAGE_SIZE)) {
		ept_add_mr(vm, (uint64_t *)vm->arch_vm.nworld_eptp, mmiodev->service_vm_pa,
				is_sos_vm(vm) ? mmiodev->service_vm_pa: mmiodev->user_vm_pa,
				mmiodev->size, EPT_RWX | EPT_UNCACHED);
		ret = 0;
	}

	return ret;
}

int32_t deassign_mmio_dev(struct acrn_vm *vm, const struct acrn_mmiodev *mmiodev)
{
	int32_t ret = -EINVAL;

	if (mem_aligned_check(mmiodev->user_vm_pa, PAGE_SIZE) &&
			mem_aligned_check(mmiodev->service_vm_pa, PAGE_SIZE) &&
			mem_aligned_check(mmiodev->size, PAGE_SIZE)) {
		ept_del_mr(vm, (uint64_t *)vm->arch_vm.nworld_eptp,
				is_sos_vm(vm) ? mmiodev->service_vm_pa: mmiodev->user_vm_pa, mmiodev->size);
		ret = 0;
	}

	return ret;
}