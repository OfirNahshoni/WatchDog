/******************************************************************************
* File name: task.c
 * Owner: Ofir Nahshoni
 * Review: Amit
 * Review status: APPROVED
 ******************************************************************************/

#include <assert.h>		/* assert */
#include <time.h>		/* time */
#include <stdlib.h>		/* malloc, free */

#include "task.h"

struct task
{
    uid_t uid;
    int (*action_func)(void* params);
    void* params;
    size_t interval_sec;
    time_t time_to_run;
};

task_t* TaskCreate(int (*action_func)(void* params), void* params,
                    size_t interval_sec)
{
    task_t* new_task = NULL;
    assert(action_func);

    new_task = (task_t*)malloc(sizeof(task_t));

    if (NULL == new_task)
    {
        return NULL;
    }

    new_task->uid = UIDCreate();

    if (UIDIsSame(bad_uid, new_task->uid))
    {
        free(new_task);
        return NULL;
    }

    new_task->action_func = action_func;
    new_task->params = params;
    new_task->interval_sec = interval_sec;
    new_task->time_to_run = time(NULL) + (time_t)interval_sec;

    return new_task;
}

void TaskDestroy(task_t* task)
{
    free(task);
}

int TaskRun(task_t* task)
{
    assert(task);

    task->time_to_run += (time_t)(task->interval_sec);

    return task->action_func(task->params);
}

uid_t TaskGetUID(const task_t* task)
{
    assert(task);

    return task->uid;
}

size_t TaskGetScheduledTime(const task_t* task)
{
    assert(task);

    return (size_t)(task->time_to_run);
}

int TaskIsEqual(const task_t* task1, const task_t* task2)
{
    assert(task1);
    assert(task2);

    return UIDIsSame(task1->uid, task2->uid);
}
