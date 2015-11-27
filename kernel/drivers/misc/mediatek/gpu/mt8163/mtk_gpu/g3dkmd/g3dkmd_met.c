#if defined(linux) && !defined(linux_user_mode)
#if !defined(MET_USER_EVENT_SUPPORT)

#include <linux/kernel.h>
#include "g3dbase_common_define.h"
#include "yl_define.h"
#include "g3dkmd_define.h"
#include "g3dkmd_met.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0) && defined(G3DKMD_MET_PER_CPU_VARIABLE)
DEFINE_PER_CPU(char[MET_STRBUF_SIZE], g3dkmd_met_strbuf_nmi);
DEFINE_PER_CPU(char[MET_STRBUF_SIZE], g3dkmd_met_strbuf_irq);
DEFINE_PER_CPU(char[MET_STRBUF_SIZE], g3dkmd_met_strbuf_sirq);
DEFINE_PER_CPU(char[MET_STRBUF_SIZE], g3dkmd_met_strbuf);
#endif

int tracing_mark_write(int type, unsigned int class_id,
                  const char *name, unsigned int value)
{
    switch (type) {
    case TYPE_START:
        MET_PRINTK("B|%d|%s\n", class_id, name);
        break;
    case TYPE_END:
        MET_PRINTK("E|%s\n", name);
        break;
    case TYPE_ONESHOT:
        MET_PRINTK("C|%d|%s|%d\n", class_id, name, value);
        break;
    case TYPE_ASYNC_START:
        MET_PRINTK("S|%d|%s|%d\n", class_id, name, value);
        break;
    case TYPE_ASYNC_END:
        MET_PRINTK("F|%d|%s|%d\n", class_id, name, value);
        break;
    default:
        return -1;
    }
    return 0;
}

#endif /* !MET_USER_EVENT_SUPPORT */
#endif
