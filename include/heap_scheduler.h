/* heap_scheduler.h */

#ifndef __HEAP_SCHEDULER_H__
#define __HEAP_SCHEDULER_H__

#include <stddef.h>     /* size_t */

#include "uid.h"   		/* uid_t */

typedef struct heap_scheduler heap_scheduler_t;

typedef enum status
{
    SUCCESS = 0,
    STOPPED = 1,
    ERROR = 2,
    DESTROYED = 3,
    RUNNING = 4		/* to avoid re-run when is already running */
} status_t;

/*
*   @desc:          Allocates new scheduler. must be destroyed with
*				@SchedulerDestroy
*   @params: 		None
*   @return value:  Pointer to the new scheduler
*   @error: 		Returns NULL if allocation fails
*   @time complex: 	O(malloc) for both AC/WC
*   @space complex: O(malloc) for both AC/WC
*/
heap_scheduler_t* HeapSchedulerCreate(void);

/*
*   @desc:          Destroys and frees @scheduler. In the event the scheduler is
*				still running it will signal to the scheduler to destroy
*				itself, after the current task is done running and will
*				return DESTROYED on	@SchedulerRun return value
*   @params: 		@scheduler: pre allocated scheduler
*   @return value:  None
*   @error: 		Undefined behavior if @scheduler is not valid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
void HeapSchedulerDestroy(heap_scheduler_t* heap_scheduler);

/*
*   @desc:          Adds a new task to @scheduler that will perform @action_func
*				with @params as params to @action_func and @interval_in_sec
*				which will say the amount of time between each invocation of
*				@action_func should pass
*   @params: 		@scheduler: pre allocated scheduler
*				@action_func: user function that the task will preform it
*				will return 0 if it should repeat or non zero value to
*                   indicate it shouldn't repeat no more.
*				@params: user pointer to additional data the user might want
*				to send to the function.
*				@interval_sec: the amount of seconds that should pass
*				between each invocation of @action_func
*   @return value:  Returns the unique uid of the newly added task.
*   @error: 		In the event that this function failed to add
				a new task it will return @bad_uid
				that is defined externally.
*				Undefined behavior if @scheduler is not valid or
*				@action_func is not valid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
uid_t HeapSchedulerAdd(heap_scheduler_t* heap_scheduler,
                            int (*action_func)(void* params),
                            void* params,
                            size_t interval_sec);

/*
*   @desc:          Removes a task from @scheduler identified by @identifier
*				In the event that a task requests to remove itself during
*				it's own run the task will not be found. use	the action
*				func return value to remove a running task from @scheduler
*   @params: 		@scheduler: pre allocated scheduler
*				@identifier: identifier to search for task to remove
*   @return value:  zero if found and removed the task and
*				nonzero if failed to find the task
*   @error: 		Undefined behavior if @scheduler is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int HeapSchedulerRemove(heap_scheduler_t* heap_scheduler, uid_t identifier);

/*
*   @desc:          Starts running @scheduler or if already running will return
*				@RUNNING status code
*   @params: 		@scheduler: pre allocated scheduler
*   @return value:  status code of the running process:
*				@SUCCESS means it ended successfully without getting called
*				to stop
*				@STOPPED means a function called for the scheduler to stop
*				@ERROR means an error accored mid running of the scheduler
*				@DESTROYED means a function called for destroy during the
*				running of the scheduler
*   @error: 		Undefined behavior if @scheduler is not valid
*   @time complex: 	O(n * m) for both AC/WC
*   @space complex: O(m) for both AC/WC
*/
status_t HeapSchedulerRun(heap_scheduler_t* heap_scheduler);

/*
*   @desc:          Sends a signal to the scheduler to stop @scheduler
*   @params: 		@scheduler: pre allocated scheduler
*   @return value:  None
*   @error: 		Undefined behavior if @scheduler is invalid
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
void HeapSchedulerStop(heap_scheduler_t* heap_scheduler);

/*
*   @desc:          Counts the amount of tasks currently in @scheduler
*   @params: 		@scheduler: pre allocated scheduler
*   @return value:  Returns the count of tasks in @scheduler
*   @error: 		Undefined behavior if @scheduler is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
size_t HeapSchedulerSize(const heap_scheduler_t* heap_scheduler);

/*
*   @desc:          Checks whether @scheduler is empty
*   @params: 		@scheduler: pre allocated scheduler
*   @return value:  Returns 1 if the scheduler doesn't have tasks and
*				0 otherwise
*   @error: 		Undefined behavior if @scheduler is invalid
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int HeapSchedulerIsEmpty(const heap_scheduler_t* heap_scheduler);

/*
*   @desc:          Removes all tasks from @scheduler. If it is called from
*				a task it will remove all the tasks besides the caller
*   @params: 		@scheduler: pre allocated scheduler
*   @return value:  None
*   @error: 		Undefined behavior if scheduler is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
void HeapSchedulerClear(heap_scheduler_t* heap_scheduler);

#endif /* __HEAP_SCHEDULER_H__ */
