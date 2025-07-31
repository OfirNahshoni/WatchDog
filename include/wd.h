/*******************************************************************************
*   File name: wd.h
*   Description:
*   The `wd.h` file provides the API for initializing and stopping a watchdog
*   process that monitors a critical user process, ensuring its uninterrupted
*   execution. It includes function declarations for starting and stopping the
*   watchdog, along with the necessary status codes.
*******************************************************************************/


#ifndef __WD_H__
#define __WD_H__

#include <stddef.h>     /* size_t */

typedef enum wd_status
{
    WD_SUCCESS = 0,
    WD_FAILURE = 1,
    WD_NUM_OF_STATUS
} wd_status_t;


/**
*   @desc:              Initializes and starts the Watchdog service by creating
*                       a separate process that monitors the calling process.
*   @params:            @threshold: Number of missed SIGUSR1 signals before
*                       the Watchdog takes recovery action.
*                       @interval: Interval (in seconds) between signals sent
*                       by the Watchdog process.
*                       @argc: Number of command-line arguments for the process.
*                       @argv: Command-line arguments.
*   @return:            WD_SUCCESS on successful launch, WD_FAILURE on failure.
*   @error:             If the semaphore or thread creation fails, the function
*                       returns a failure status.
*/
wd_status_t WDStart(size_t threshold, size_t interval, int argc, char** argv);


/**
*   @desc:              Stops the Watchdog process and releases all allocated
*                       resources. Also signals the monitored process to stop.
*   @params:            None.
*   @return:            None.
*   @error:             Undefined behavior if called without a running watchdog.
*/
void WDStop(void);

#endif  /* __WD_H__ */
