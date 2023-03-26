#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "co.h"

task_t *q[MAX_TASK_NUM];
int head = 0, tail = 0;

task_t main_proc;
task_t tk1;
task_t *cur_proc;

int has_task()
{
    return head != tail;
}

void init_task(task_t *task_ptr, void *fn)
{
    task_ptr->pctx = (ctx_t *)(&task_ptr->stack[STACK_SIZE] - sizeof(ctx_t));
    task_ptr->pctx->eip = fn;
}

void create_task(task_t *task_ptr)
{
    q[tail++] = task_ptr;
    tail %= MAX_TASK_NUM;
}

void load_task()
{
    cur_proc = q[head++];
    head %= MAX_TASK_NUM;
}

void t1()
{
    puts("hello scheduler");
    myswitch(&cur_proc->pctx, main_proc.pctx); // task yield to scheduler
}

void t2()
{
    puts("hello task2");
    myswitch(&cur_proc->pctx, main_proc.pctx); // task yield to scheduler
}

void myschedule()
{
    while (1)
    {
        if (has_task())
        {
            sleep(1);
            load_task();
            myswitch(&main_proc.pctx, cur_proc->pctx);
        }
        else
        {
            init_task(&tk1, t2);
            create_task(&tk1);
        }
    }
}

int main()
{
    init_task(&main_proc, 0);

    init_task(&tk1, t1);

    create_task(&tk1);

    myschedule();
    return 0;
}