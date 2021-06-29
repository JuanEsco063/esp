// Copyright (c) 2011-2021 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#ifndef __ESP_CFG_000_H__
#define __ESP_CFG_000_H__

#include "libesp.h"
#include "gemm_circt.h"

typedef int32_t token_t;

/* <<--params-def-->> */
#define BATCH_SIZE 4
#define COLS1 16
#define COLS2 16
#define ROWS1 16

/* <<--params-->> */
const int32_t batch_size = BATCH_SIZE;
const int32_t cols1 = COLS1;
const int32_t cols2 = COLS2;
const int32_t rows1 = ROWS1;

#define NACC 1

struct gemm_circt_access gemm_cfg_000[] = {
	{
		/* <<--descriptor-->> */
		.batch_size = BATCH_SIZE,
		.cols1 = COLS1,
		.cols2 = COLS2,
		.rows1 = ROWS1,
		.src_offset = 0,
		.dst_offset = 0,
		.esp.coherence = ACC_COH_NONE,
		.esp.p2p_store = 0,
		.esp.p2p_nsrcs = 0,
		.esp.p2p_srcs = {"", "", "", ""},
	}
};

esp_thread_info_t cfg_000[] = {
	{
		.run = true,
		.devname = "gemm_circt.0",
		.ioctl_req = GEMM_CIRCT_IOC_ACCESS,
		.esp_desc = &(gemm_cfg_000[0].esp),
	}
};

#endif /* __ESP_CFG_000_H__ */
