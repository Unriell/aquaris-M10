#ifndef _G3DKMD_DEBUGFS_HELPER_H_
#define _G3DKMD_DEBUGFS_HELPER_H_

#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include "g3dbase_common_define.h"
#include "yl_define.h"

#include "g3dkmd_define.h"



/* DEBUGFS_PRINT_LOG_OR_SEQ()
 * Description:
 *   a print wrapper, which is used to direct data to seq_file or printk
 *   directly
 *
 * Inputs:
 *   seqfile: output file descripter, to prink if NULL.
 *   fmf    : format string when print
 *   args   : the following args like printk()
 */

#define DEBUGFS_PRINT_LOG_OR_SEQ(seq_file, fmt, args...) \
    do { \
        if (seq_file) { \
            seq_printf(seq_file, fmt, ##args); \
        } else { \
            printk(fmt, ##args); \
        } \
    } while(0);


#define DECLARE_G3D_DEBUGFS_OPS(_fops, _ops_read, _ops_write)           \
static int _fops##_my_seq_open(struct inode *inode, struct file *file)  \
{                                                                       \
    return single_open(file, _ops_read, NULL);                          \
}                                                                       \
                                                                        \
static struct file_operations _fops                     =               \
{                                                                       \
    .owner   = THIS_MODULE,                                             \
    .write   = _ops_write,                                              \
    .open    = _fops##_my_seq_open,                                     \
    .read    = seq_read,                                                \
    .llseek  = seq_lseek,                                               \
    .release = seq_release,                                             \
};



/* _g3dKmdCreateDebugfsFileNode()
 * Description:
 *   Use to create a seq file; this can be used in all debugfs sub-system.
 *
 * Inputs:
 *   name           : folder name
 *   parrentNode    : the node of parrent folder
 *   *func          : the print function using seq_file.
 */
void _g3dKmdCreateDebugfsFileNode(const char * name, struct dentry *parentNode,
     int (*func)(struct seq_file*, void*));

#endif //_G3DKMD_DEBUGFS_HELPER_H_
