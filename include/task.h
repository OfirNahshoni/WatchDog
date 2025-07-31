/* task.h */

#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>  	 	/* size_t */

#include "uid.h"			/* uid_t */

typedef struct task task_t;

/*
*   @desc:          Allocates new task must be destroyed with @TaskDestroy
*   @params: 		@action_func: the function the task will call when it runs
*				will return 0 if it should repeat using the interval or
*				non zero value to indicate it shouldn't repeat no more.
*				@params: user params to send into @action_func
*				@interval_sec: the amount of time that should pass between
*				each invocation of @action_func
*   @return value:  Pointer to the new task
*   @error: 		Returns NULL if the allocation fails
*   @time complex: 	O(malloc) for both AC/WC
*   @space complex: O(malloc) for both AC/WC
*/
task_t* TaskCreate(int (*action_func)(void* params), void* params,
			    size_t interval_sec);

/*
*   @desc:          Frees allocated task which was created using @TaskCreate
*   @params: 		@task: pre allocated task
*   @return value:  None
*   @error: 		None
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
void TaskDestroy(task_t* task);

/*
*   @desc:          Runs the action of @task and updates the next time this
*				task should operate
*   @params: 		@task: pre allocated task
*   @return value:  Returns the @task's action return value which was described
*				@TaskCreate
*   @error: 		Undefined behavior if @task is invalid
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int TaskRun(task_t* task);

/*
*   @desc:          Returns the @task's unique identifier for identification
*   @params: 		@task: pre allocated task
*   @return value:  Returns the task's unique identifier
*   @error: 		Undefined behavior if @task is invalid
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
uid_t TaskGetUID(const task_t* task);

/*
*   @desc:          Returns @task's next scheduled run time
*   @params: 		@task: pre allocated task
*   @return value:  Returns the task's next scheduled run time
*   @error: 		Undefined behavior if @task is invalid
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
size_t TaskGetScheduledTime(const task_t* task);

/*
*   @desc:          Returns if @task1 and @task2 are the same task
*   @params: 		@task: pre allocated task
*   @return value:  Returns 1 if @task1 and @task2 are the same and 0 otherwise
*   @error: 		Undefined behavior if @task1 or @task2 is invalid
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int TaskIsEqual(const task_t* task1, const task_t* task2);

#endif /* __TASK_H__ */
