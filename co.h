typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

#define STACK_SIZE 4096
#define MAX_TASK_NUM 16

typedef struct ctx_s
{
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t edx;
    uint32_t ecx;
    uint32_t ebx;
    uint32_t eax;
    void *eip; // pushed by call
} ctx_t;

typedef struct task_s
{
    uint8_t stack[STACK_SIZE];
    ctx_t *pctx;
} task_t;

extern void myswitch(ctx_t **ptr_where_to_save_currrent_ctx, ctx_t *new_ctx);
