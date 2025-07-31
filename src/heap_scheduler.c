/* heap_scheduler.c */

#include <assert.h>			    /* assert */
#include <unistd.h>			    /* sleep */
#include <stdlib.h>			    /* free */
#include <time.h>			    /* time_t, time */

#include "task.h"			    /* task functions */
#include "heap_p_queue.h"       /* heap_pq_t */
#include "heap_scheduler.h"


typedef enum signal
{
	STOP = 0,
	ERR = 1,
	DESTROY = 2,
	CONTINUE = 3
} signal_t;

struct heap_scheduler
{
    heap_pq_t* heap_pq;
    status_t status;
    signal_t signal;
};


/*------------------------------static functions------------------------------*/
static int CompareFunc(const void* data, const void* param);
static int IsMatch(const void* task, const void* uid_to_compare);
static void SleepUntilTaskExecution(heap_scheduler_t* scheduler);
static void EventLoopHandler(heap_scheduler_t* scheduler);
static status_t SignalHandler(heap_scheduler_t* scheduler);


/*------------------------static functions implementations--------------------*/
static int CompareFunc(const void* data, const void* param)
{
	return (*((time_t*)data) - *((time_t*)param));
}

static int IsMatch(const void* task, const void* uid_to_compare)
{
	uid_t task_uid = TaskGetUID((task_t*)task);

	return UIDIsSame(task_uid, *((uid_t*)uid_to_compare));
}

static void SleepUntilTaskExecution(heap_scheduler_t* scheduler)
{
	time_t time_to_wait = 0;
	time_t next_time = 0;
	task_t* task_to_run = NULL;

	task_to_run = HeapPQPeek(scheduler->heap_pq);
	next_time = (time_t)TaskGetScheduledTime(task_to_run);
	time_to_wait = (time(NULL) <= next_time) ? next_time - time(NULL) : 0;

	sleep(time_to_wait);
}

static void EventLoopHandler(heap_scheduler_t* scheduler)
{
	int run_result = 0;
	task_t* task_to_run = NULL;

	assert(scheduler);

	task_to_run = HeapPQDequeue(scheduler->heap_pq);

	run_result = TaskRun(task_to_run);

	if (0 == run_result)
	{
		if (0 != HeapPQEnqueue(scheduler->heap_pq, task_to_run))
		{
			TaskDestroy(task_to_run);
			scheduler->signal = ERR;
		}
	}

	else	/* params = 0 */
	{
		TaskDestroy(task_to_run);
	}
}

static status_t SignalHandler(heap_scheduler_t* scheduler)
{
	switch (scheduler->signal)
	{
		case DESTROY:
			scheduler->status = DESTROYED;
			HeapSchedulerDestroy(scheduler);
			return DESTROYED;

		case STOP:
			scheduler->status = STOPPED;
			break;

		case ERR:
			scheduler->status = ERROR;
			break;

		default:
			scheduler->status = SUCCESS;
			break;
	}

	return scheduler->status;
}

heap_scheduler_t* HeapSchedulerCreate()
{
	heap_scheduler_t* scheduler = (heap_scheduler_t*)malloc(
                                    sizeof(heap_scheduler_t));

	if (NULL == scheduler)
	{
		return NULL;
	}

	scheduler->heap_pq = HeapPQCreate(CompareFunc);

	if (NULL == scheduler->heap_pq)
	{
		free(scheduler);
		return NULL;
	}

	scheduler->status = SUCCESS;
	scheduler->signal = CONTINUE;

	return scheduler;
}

void HeapSchedulerDestroy(heap_scheduler_t* scheduler)
{
	assert(scheduler);

	if (RUNNING == scheduler->status)
	{
		scheduler->signal = DESTROY;
		return;
	}

	HeapSchedulerClear(scheduler);
	HeapPQDestroy(scheduler->heap_pq);
	free(scheduler);
}

uid_t HeapSchedulerAdd(heap_scheduler_t* scheduler,
					   int (*action_func)(void* params),
					   void* params,
					   size_t interval_sec)
{
	int result_enqueue = 0;
	task_t* task_to_add = NULL;

	assert(scheduler);
	assert(action_func);

	task_to_add = TaskCreate(action_func, params, interval_sec);

	if (NULL == task_to_add)
	{
		return bad_uid;
	}

	result_enqueue = HeapPQEnqueue(scheduler->heap_pq, task_to_add);

	if (0 != result_enqueue)
	{
		TaskDestroy(task_to_add);
		return bad_uid;
	}

	return TaskGetUID(task_to_add);
}

int HeapSchedulerRemove(heap_scheduler_t* scheduler, uid_t identifier)
{
	task_t* task_to_remove = NULL;

	assert(scheduler);

	task_to_remove = HeapPQErase(scheduler->heap_pq, IsMatch, &identifier);

	if (NULL == task_to_remove)
	{
		return 1;
	}

	TaskDestroy(task_to_remove);
	return 0;
}

status_t HeapSchedulerRun(heap_scheduler_t* scheduler)
{
	assert(scheduler);

	if (RUNNING == scheduler->status)
	{
		return RUNNING;
	}

	scheduler->status = RUNNING;
	scheduler->signal = CONTINUE;

	/* "Event" loop - running */
	while ((CONTINUE == scheduler->signal) &&
            (!HeapPQIsEmpty(scheduler->heap_pq)))
	{
		SleepUntilTaskExecution(scheduler);

		EventLoopHandler(scheduler);
	}

	return SignalHandler(scheduler);
}

void HeapSchedulerStop(heap_scheduler_t* scheduler)
{
	assert(scheduler);

	if (DESTROY != scheduler->signal)
	{
		scheduler->signal = STOP;
	}
}

size_t HeapSchedulerSize(const heap_scheduler_t* scheduler)
{
	assert(scheduler);

	return HeapPQSize(scheduler->heap_pq);
}

int HeapSchedulerIsEmpty(const heap_scheduler_t* scheduler)
{
	assert(scheduler);

	return HeapPQIsEmpty(scheduler->heap_pq);
}

void HeapSchedulerClear(heap_scheduler_t* scheduler)
{
	assert(scheduler);

	while (!HeapSchedulerIsEmpty(scheduler))
	{
		TaskDestroy(HeapPQDequeue(scheduler->heap_pq));
	}
}
