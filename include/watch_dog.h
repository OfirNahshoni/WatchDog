/*******************************************************************************
*   File name: watch_dog.h
*   Description:
*   Internal API for managing the Watchdog service. It provides
*   structures, constants, and function declarations necessary
*   for implementing the core functionalities of the Watchdog.
*******************************************************************************/


#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

#include <semaphore.h>                  /* sem_t */
#include <sys/types.h>                  /* size_t, pid_t */

#include "heap_scheduler.h"             /* heap_scheduler_t */

#define STR_SIZE (256)
#define ADDITIONAL_ARGS (4)
#define UNUSED(x) ((void)x)
#define SEM_NAME ("/wd_sem")
#define EXEC_WD_PATH ("./wd_exec.out")


typedef struct params_obj
{
    int argc;
    char** argv;
    char** argv_wd;
    int is_user;
    pid_t pid_other;
    size_t interval;
    size_t threshold;
    heap_scheduler_t* sched;
} params_obj_t;


/**
 * @desc:       Appends a given text string to a log file. Ensures thread-safe
 *              logging for debugging or monitoring purposes.
 * @params:     @str_input: A null-terminated string to be appended
 *              to the log file.
 * @return:     None.
 * @error:      If the log file cannot be opened, the error is ignored silently.
 * @time:       O(1) for AC/WC.
 * @space:      O(1) for AC/WC.
 */
void AppendText(const char* str_input);


/**
*   @desc:      Initializes parameters for the Watchdog process. Prepares
*               necessary structures and environment variables.
*   @params:    @threshold: Threshold value for missed signals.
*               @interval: Interval for signal transmission.
*               @argc: Number of arguments passed to the process.
*               @argv: Argument list for the process.
*   @return:    0 on success, non-zero on failure.
*   @error:     Undefined behavior if memory allocation fails.
*/
int InitParams(size_t threshold, size_t interval, int argc, char** argv);


/**
*   @desc:      Executes the Watchdog process by replacing the current process
*               image with the Watchdog executable.
*   @params:    None.
*   @return:    0 on success, non-zero on failure.
*   @error:     Undefined behavior if execution fails.
*/
int ExecWatchDog();


/**
*   @desc:      Runs the watchdog process to monitor a user process.
*   @params:    @params: Parameters for the watchdog process.
*   @return:    0 on success, non-zero on failure.
*   @error:     Undefined behavior if parameters are invalid.
*   @time:      O(1) for AC/WC.
*   @space:     O(1) for AC/WC.
*/
int RunWatchDog(params_obj_t* params);


/**
*   @desc:      Frees all dynamically allocated resources, including the
*               `argv_wd` array, the semaphore, and any other allocated memory
*               associated with the Watchdog process.
*   @params:    None.
*   @return:    None.
*   @error:     Undefined behavior if called without initialized resources.
*   @note:      Should be called when the Watchdog process is terminated.
*/
void FreeAllocatedResources();

#endif      /* __WATCH_DOG_H__ */
