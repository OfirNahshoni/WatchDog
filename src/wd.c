/*******************************************************************************
* File name: wd.c
* Description: This file implements the public API of the Watchdog service.
*              It provides the implementation for starting and stopping the
*              Watchdog monitoring service, ensuring reliable process
*              monitoring using inter-process communication (IPC), threading,
*              and signals.
* Owner: Ofir Nahshoni
* Review: Ofir1
* Review status: APPROVED (01.01.25)
*******************************************************************************/


#define _POSIX_C_SOURCE (200809L)   /* remove setenv warning */

#include <fcntl.h>                  /* O_CREAT */
#include <sys/stat.h>               /* 0_* constants */
#include <unistd.h>                 /* fork, execv */
#include <pthread.h>                /* pid_t */
#include <assert.h>                 /* assert */
#include <string.h>                 /* strcpy */
#include <stdlib.h>                 /* setenv, getenv, malloc, atoi */
#include <stdio.h>                  /* sprintf */
#include <string.h>                 /* strcpy */
#include <signal.h>                 /* kill */
#include <ctype.h>                  /* isspace */

#include "watch_dog.h"              /* private library */
#include "wd.h"                     /* public library */


/*-----------------------------------macros-----------------------------------*/
#define ADDITIONAL_ARGS (4)
#define WD_ENV_VAR_NAME ("WD_PID")


/*------------------------------global variables------------------------------*/
static pthread_t wd_thread;
params_obj_t params = { 0 };


/*----------------------static functions implementations----------------------*/
static void* ThreadHandler(void* arg)
{
    UNUSED(arg);

    RunWatchDog(&params);

    return NULL;
}


/*-------------------------API functions implementations----------------------*/
wd_status_t WDStart(size_t threshold, size_t interval, int argc, char** argv)
{
    sem_t* sem;
    pid_t fork_pid;
    pthread_attr_t attr;
    char buffer[STR_SIZE];

    if (0 != InitParams(threshold, interval, argc, argv))
    {
#ifndef NDEBUG
    AppendText("allocation and extend of argv failed\n");
#endif
        return WD_FAILURE;
    }

    sem = sem_open(SEM_NAME, O_CREAT, (S_IRUSR | S_IWUSR), 0);

    if (SEM_FAILED == sem)
    {
#ifndef NDEBUG
    AppendText("open semaphore failed\n");
#endif
        return WD_FAILURE;
    }

    fork_pid = fork();

    if (-1 == fork_pid)
    {
#ifndef NDEBUG
    AppendText("Initial fork failed\n");
#endif
        return WD_FAILURE;
    }

    else if (0 == fork_pid)
    {
        if (0 != ExecWatchDog())
        {
            return WD_FAILURE;
        }
        FreeAllocatedResources();
    }

    else
    {
        params.pid_other = fork_pid;
        params.is_user = 1;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        sem_wait(sem);
        pthread_create(&wd_thread, &attr, ThreadHandler, NULL);
        sem_wait(sem);

        sprintf(buffer, "%d", fork_pid);
        if (-1 == setenv(WD_ENV_VAR_NAME, buffer, 1))
        {
#ifndef NDEBUG
    AppendText("setenv of WD_ENV_VAR_NAME failed\n");
#endif
            return WD_FAILURE;
        }
    }

    return WD_SUCCESS;
}

void WDStop(void)
{
    char log_buffer[STR_SIZE];
    char* pid_wd_as_str = getenv(WD_ENV_VAR_NAME);

#ifndef NDEBUG
    AppendText("In Stop function:\n");
    sprintf(log_buffer, "pid wd: %s\n", pid_wd_as_str);
    AppendText(log_buffer);
#endif

    kill((pid_t)atoi(pid_wd_as_str), SIGUSR2);
    raise(SIGUSR2);

    FreeAllocatedResources();
    pthread_join(wd_thread, NULL);
}
