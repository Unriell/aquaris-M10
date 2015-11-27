#ifndef _G3DKMD_MMCQ_H_
#define _G3DKMD_MMCQ_H_

// define
#define MMCQ_INSTRUCTION_SIZE   8
#define MMCQ_DEFAULT_MASK       0xFFFFFFFF

typedef void* MMCQ_HANDLE;

#define MMCQ_TOKEN_VALUE_TRUE       1
#define MMCQ_TOKEN_VALUE_FALSE      0

typedef enum
{
    MMCQ_SUBSYS_GPU         = 0,
    MMCQ_SUBSYS_MMCE        = 14
}MMCQ_SUBSYS;

#define MMCQ_TOKEN_SHIFT        0x180
#define MMCQ_TOKEN_FRAME_FLUSH  (MMCQ_TOKEN_SHIFT + 0x2)

typedef enum
{
    MMCQ_TO_UPDATE_NO_UPDATE    = 0,
    MMCQ_TO_UPDATE_UPDATE       = 1
}MMCQ_TO_UPDATE;

typedef enum
{
    MMCQ_UPDATE_OP_ASSIGN   = 0,
    MMCQ_UPDATE_OP_INC      = 1,
    MMCQ_UPDATE_OP_DEC      = 2
}MMCQ_UPDATE_OP;

typedef enum
{
    MMCQ_TO_WAIT_NO_WAIT    = 0,
    MMCQ_TO_WAIT_WAIT       = 1,
}MMCQ_TO_WAIT;

typedef enum
{
    MMCQ_WAIT_OP_EQ         = 0,    // equal
    MMCQ_WAIT_OP_GTE        = 1,    // greater than or equal
    MMCQ_WAIT_OP_LTE        = 2     // less than or equal
}MMCQ_WAIT_OP;

typedef enum
{
    MMCQ_JUMP_MODE_RELATIVE = 0,
    MMCQ_JUMP_MODE_ABS      = 1
}MMCQ_JUMP_MODE;

typedef enum
{
    MMCQ_SUSPENDABLE        = 0,
    MMCQ_NON_SUSPENDABLE    = 1
}MMCQ_IS_SUSPENDABLE;

typedef enum
{
    MMCQ_MARKER_EOC,
    MMCQ_MARKER_PREFETCH_EN
}MMCQ_MARKER;

typedef enum
{
    MMCQ_STATE_STOP,
    MMCQ_STATE_RUN,
    MMCQ_STATE_PAUSE,
    MMCQ_STATE_UNKNOWN
} MMCQ_STATE;

typedef enum
{
    MMCQ_ADDR_BASE0,
    MMCQ_ADDR_WRITE_PTR
} ADDR_TYPE;

MMCQ_HANDLE g3dKmdMmcqCreate(void);
void g3dKmdMmcqRelease(MMCQ_HANDLE handle);
void g3dKmdMmcqReset(MMCQ_HANDLE handle);

void g3dKmdMmcqSetThread(MMCQ_HANDLE mmcq_handle, unsigned int thd_handle);
void g3dKmdMmcqWaitIdle(MMCQ_HANDLE mmcq_handle);

void g3dKmdMmcqResume(MMCQ_HANDLE handle);
void g3dKmdMmcqSuspend(MMCQ_HANDLE handle);

int g3dKmdMmcqWaitSize(MMCQ_HANDLE handle, unsigned int size);

void g3dKmdMmcqWrite(MMCQ_HANDLE handle, MMCQ_SUBSYS subsys, unsigned int reg_addr, unsigned int value, unsigned int mask);
void g3dKmdMmcqRead(MMCQ_HANDLE handle, MMCQ_SUBSYS subsys, unsigned int reg_addr);
void g3dKmdMmcqPoll(MMCQ_HANDLE handle, MMCQ_SUBSYS subsys, unsigned int reg_addr, unsigned int value, unsigned int mask);
void g3dKmdMmcqWaitForEvent(MMCQ_HANDLE handle, unsigned int token, unsigned int wait_value);
void g3dKmdMmcqWaitAndClear(MMCQ_HANDLE handle, unsigned int token, unsigned int wait_value, unsigned int clear_value);
void g3dKmdMmcqUpdateToken(MMCQ_HANDLE handle, unsigned int token, unsigned int set_value);
void g3dKmdMmcqJump(MMCQ_HANDLE handle, MMCQ_JUMP_MODE mode, unsigned int addr);
void g3dKmdMmcqFlush(MMCQ_HANDLE handle);
void g3dKmdMmcqAddMarker(MMCQ_HANDLE handle, MMCQ_MARKER marker);
void g3dKmdMmcqJumpToNextInst(MMCQ_HANDLE handle);
void g3dKmdMmcqUpdateBuffAddr(MMCQ_HANDLE handle);
void g3dKmdMmcqSetIsSuspendable(MMCQ_HANDLE handle, MMCQ_IS_SUSPENDABLE is_suspendable);

void g3dKmdMmcqSetPC(MMCQ_HANDLE handle, unsigned int pc);
unsigned int g3dKmdMmcqGetPC(MMCQ_HANDLE handle);

MMCQ_STATE g3dKmdMmcqGetState(MMCQ_HANDLE handle);

unsigned int g3dKmdMmcqGetAddr(MMCQ_HANDLE handle, ADDR_TYPE type);
void g3dKmdMmcqSetJumpTarget(MMCQ_HANDLE handle, unsigned int target);

    #ifdef PATTERN_DUMP
void g3dKmdMmcqDumpBegin(MMCQ_HANDLE handle);
void _g3dKmdMmcqDumpBuf(void* fp, MMCQ_HANDLE handle);
void g3dKmdMmcqDumpEnd(MMCQ_HANDLE handle, unsigned int dumpCount);

void g3dKmdRiuGceDump(MMCQ_HANDLE handle, unsigned int dumpCount);
void g3dKmdParseRiuGce(void* fp, MMCQ_HANDLE handle);
    #endif

#endif

