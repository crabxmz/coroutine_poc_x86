#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "co.h"
#include <pthread.h>

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

void init_task_with_param(task_t *task_ptr, void *fn, void *arg)
{
    *(void **)(&task_ptr->stack[STACK_SIZE] - sizeof(void *)) = arg; // esp+4
    // esp+0 (ret addr)
    task_ptr->pctx = (ctx_t *)(&task_ptr->stack[STACK_SIZE] - sizeof(ctx_t) - sizeof(void *) * 2); // context
    task_ptr->pctx->eip = fn;
}

void push_task(task_t *task_ptr)
{
    q[tail++] = task_ptr;
    tail %= MAX_TASK_NUM;
}

void pop_task()
{
    head = (head + 1) % MAX_TASK_NUM;
}

task_t *get_task()
{
    return q[head];
}

void run_task(task_t *tk)
{
    cur_proc = tk;
    myswitch(&main_proc.pctx, cur_proc->pctx);
}

void myyield()
{
    myswitch(&cur_proc->pctx, main_proc.pctx); // save current task and yield to scheduler
}

void myexit()
{
    if (cur_proc == q[head])
    {
        pop_task();
    }
    myyield();
}

void t1()
{
    puts("hello scheduler");
    myexit();
}

void t2()
{
    puts("hello new task");
    myexit();
}

void t3(void *arg)
{
    printf("hello param: %p\n", arg);
    myyield(); // actively yield, wait for recovery
    printf("hello i survived\n");
    myexit();
}

void myschedule()
{
    while (1)
    {
        if (has_task())
        {
            sleep(1);
            run_task(get_task());
        }
        else
        {
            init_task(&tk1, t2);
            push_task(&tk1);
        }
    }
}

void *co_thread(void *arg)
{
    init_task(&main_proc, 0);
    init_task_with_param(&tk1, t3, (void *)0xdeadbeef);
    push_task(&tk1);
    myschedule();
    return 0;
}

int main()
{
    pthread_t myThread;
    int ret;
    ret = pthread_create(&myThread, 0, co_thread, 0);
    if (ret != 0)
    {
        puts("Error creating thread");
        return 1;
    }
    ret = pthread_join(myThread, 0);
    if (ret != 0)
    {
        puts("Error joining thread");
        return 1;
    }
    return 0;
}