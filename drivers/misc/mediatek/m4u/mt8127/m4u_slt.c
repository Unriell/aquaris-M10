/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/xlog.h>

#include "m4u.h"
#include "m4u_priv.h"
#include "ddp_slt.h"

#define SRC_BUF_ADDR data_argb_64x64

unsigned int gSrcVaBuf;
unsigned int gSrcMvaBuf;

unsigned int gDstVaBuf;
unsigned int gDstMvaBuf;

#define M4U_ASSERT(x) do {	\
	if (!(x))	\
		pr_err("M4Uassert fail, file:%s, line:%d", __FILE__, __LINE__);	\
} while (0)

#define M4UMSG(string, args...)	pr_debug("M4U"string, ##args)

static int slt_result;		/* 1 success,0 Error! */

static int m4u_read_state(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len = 0;

	M4UMSG("m4u binning slt result %d\n", slt_result);
	len += sprintf(buf + len, "%d", slt_result);
	return len;
}

void m4u_debug_init(void)
{
	/* register cat /proc/m4u_slt */
	create_proc_read_entry("m4u_slt_result", 0, NULL, m4u_read_state, NULL);
}

int m4u_test_main(void)
{
	int ret;
	unsigned int i;

	/* allocate memory */
	gSrcVaBuf = vmalloc(64 * 64 * 4);
	if (!gSrcVaBuf) {
		M4UMSG("vmalloc failed.\n");
		return -1;
	}

	gDstVaBuf = vmalloc(64 * 64 * 4);
	if (!gDstVaBuf) {
		M4UMSG("vmalloc failed.\n");
		return -1;
	}
	/* config port */
	M4U_PORT_STRUCT M4uPort;

	M4uPort.ePortID = DISP_OVL_0;
	M4uPort.Virtuality = 1;
	M4uPort.Security = 0;
	M4uPort.Distance = 1;
	M4uPort.Direction = 0;
	m4u_config_port(&M4uPort);

	M4uPort.ePortID = DISP_WDMA;
	M4uPort.Virtuality = 1;
	M4uPort.Security = 0;
	M4uPort.Distance = 1;
	M4uPort.Direction = 0;
	m4u_config_port(&M4uPort);

	/* allocate mva */
	ret = m4u_alloc_mva(DISP_OVL_0, gSrcVaBuf, 64 * 64 * 4, 0, 0, &(gSrcMvaBuf));
	M4U_ASSERT(0 == ret);
	m4u_insert_seq_range(DISP_OVL_0, gSrcMvaBuf, gSrcMvaBuf + 64 * 64 * 4 - 1,
			     SEQ_RANGE_LOW_PRIORITY, 1);

	M4UMSG("src module=DISP_OVL_0, va=0x%x, mva=0x%x.\n", gSrcVaBuf, gSrcMvaBuf);

	ret = m4u_alloc_mva(DISP_WDMA, gDstVaBuf, 64 * 64 * 4, 0, 0, &(gDstMvaBuf));
	M4U_ASSERT(0 == ret);
	m4u_insert_seq_range(DISP_OVL_0, gDstMvaBuf, gDstMvaBuf + 64 * 64 * 4 - 1,
			     SEQ_RANGE_LOW_PRIORITY, 1);

	M4UMSG("dst module=DISP_WDMA, va=0x%x, mva=0x%x.\n", gDstVaBuf, gDstMvaBuf);

	/* ddp test */
	memset(gSrcVaBuf, 0, 64 * 64 * 4);
	memcpy(gSrcVaBuf, SRC_BUF_ADDR, 64 * 64 * 4);

	memset(gDstVaBuf, 0, 64 * 64 * 4);

	m4u_dma_cache_maint(DISP_OVL_0, gSrcVaBuf, 64 * 64 * 4, M4U_CACHE_FLUSH_BEFORE_HW_READ_MEM);

	m4u_dma_cache_maint(DISP_WDMA, gDstVaBuf, 64 * 64 * 4, M4U_CACHE_FLUSH_BEFORE_HW_READ_MEM);

	slt_result = m4u_ddp_test(gSrcMvaBuf, gDstVaBuf, gDstMvaBuf);
	if (slt_result == 0)
		M4UMSG("case 1 result: success\n");
	else {
		M4UMSG("case 1 result: fail\n");
		M4U_ASSERT(0);
	}

	/* free va & mva */
	/*m4u_invalid_seq_range(DISP_OVL_0, gSrcMvaBuf, gSrcMvaBuf + 64 * 64 * 4 - 1);
	   m4u_invalid_seq_range(DISP_WDMA, gDstMvaBuf, gDstMvaBuf + 64 * 64 * 4 - 1);

	   m4u_dealloc_mva(DISP_OVL_0, gSrcVaBuf, 64 * 64 * 4, gSrcMvaBuf);
	   m4u_dealloc_mva(DISP_WDMA, gDstVaBuf, 64 * 64 * 4, gDstMvaBuf); */

	vfree(gSrcVaBuf);
	vfree(gDstVaBuf);

	/*
	   M4uPort.ePortID = DISP_OVL_0;
	   M4uPort.Virtuality = 0;
	   M4uPort.Security = 0;
	   M4uPort.Distance = 1;
	   M4uPort.Direction = 0;
	   m4u_config_port(&M4uPort);

	   M4uPort.ePortID = DISP_WDMA;
	   M4uPort.Virtuality = 0;
	   M4uPort.Security = 0;
	   M4uPort.Distance = 1;
	   M4uPort.Direction = 0;
	   m4u_config_port(&M4uPort); */

	m4u_debug_init();

	m4u_larb_backup_sec(0);
	m4u_larb_restore_sec(0);
	return 0;

}
