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

#ifndef __EXTD_DRV_LOG_H__
#define __EXTD_DRV_LOG_H__
#include "ddp_debug.h"

/* HDMI log functions*/
#define HDMI_LOG(fmt, arg...) \
	do { \
		if (hdmi_log_on) \
			pr_debug("[EXTD-HDMI]:"fmt, ##arg); \
	} while (0)

#define HDMI_FUNC()		pr_debug("[EXTD-HDMI]:%s\n", __func__)
#define HDMI_ERR(fmt, arg...)	pr_err("[EXTD-HDMI]:"fmt, ##arg)



/*external display fence log functions*/
#define EXT_FENCE_FUNC() pr_warn("[EXTD][fence] %s\n", __func__)
#define EXTD_FENCE_LOG(fmt, arg...)	pr_warn("[EXTD][fence]:"fmt, ##arg)
#define EXTD_FENCE_ERR(fmt, arg...) pr_err("fence error(%d):"fmt, __LINE__, ##arg)

#define EXTD_FENCE_LOG_D(fmt, arg...) pr_debug("fence"fmt, ##arg)

#define EXTD_FENCE_LOG_D_IF(con, fmt, arg...) \
			do { \
				if (con) \
					pr_debug("fence"fmt, ##arg); \
			} while (0)

/*external display lcm log functions*/
#define EXT_LCM_LOG(fmt, arg...)	pr_warn("[EXTD][LCM]:"fmt, ##arg)
#define EXT_LCM_ERR(fmt, arg...)	pr_err("[EXTD][LCM]:"fmt, ##arg)
#define EXT_LCM_FUNC()	pr_warn("[EXTD][LCM] %s\n", __func__)

/*external display log functions*/
#define EXT_DISP_DBG(string, args...)  \
	do {	\
		if (g_mobilelog) \
			pr_debug("[EXTD]:"string, ##args); \
	} while (0)

#define EXT_DISP_LOG(fmt, arg...)	pr_debug("[EXTD]:"fmt, ##arg)
#define EXT_DISP_ERR(fmt, arg...)	pr_err("[EXTD]:"fmt, ##arg)
#define EXT_DISP_FUNC()  \
	do {	\
		if (g_mobilelog) \
			pr_debug("[EXTD]:%s\n", __func__); \
	} while (0)


#endif
