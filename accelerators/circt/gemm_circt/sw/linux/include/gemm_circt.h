// Copyright (c) 2011-2021 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#ifndef _GEMM_CIRCT_H_
#define _GEMM_CIRCT_H_

#ifdef __KERNEL__
#include <linux/ioctl.h>
#include <linux/types.h>
#else
#include <sys/ioctl.h>
#include <stdint.h>
#ifndef __user
#define __user
#endif
#endif /* __KERNEL__ */

#include <esp.h>
#include <esp_accelerator.h>

struct gemm_circt_access {
	struct esp_access esp;
	/* <<--regs-->> */
	unsigned batch_size;
	unsigned cols1;
	unsigned cols2;
	unsigned rows1;
	unsigned src_offset;
	unsigned dst_offset;
};

#define GEMM_CIRCT_IOC_ACCESS	_IOW ('S', 0, struct gemm_circt_access)

#endif /* _GEMM_CIRCT_H_ */
