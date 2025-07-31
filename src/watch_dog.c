/*******************************************************************************
* File name: watch_dog.c
* Description: Implements the private core functionality for the
*              Watchdog service. This includes IPC, signal handling, process
*              re-execution, and synchronization using semaphores.
*              The Watchdog monitors and restarts processes in case of failure.
* Owner: Ofir Nahshoni
* Review: Ofir1
* Review status: APPROVED (01.01.25)
*******************************************************************************/


#define _POSIX_C_SOURCE (200809L)   /* remove setenv warning */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>               /* waitpid */
#include <assert.h>                 /* assert */
#include <string.h>                 /* strcmp */
#include <fcntl.h>                  /* O_CREAT */
#include <stdio.h>                  /* fprintf */
#include <signal.h>                 /* sigaction */
#include <stdatomic.h>              /* atomic_uint */
#include <stdlib.h>                 /* setenv */

#include "watch_dog.h"


/*-----------------------------------macros-----------------------------------*/
#define LOGFILE_PATH ("./log.txt")
#define WD_ENV_VAR_NAME ("WD_PID")
#define EXEC_WD_PATH ("./wd_exec.out")


/*---------------------------static global variables--------------------------*/
static atomic_uint flag_stop = 0;
static atomic_uint signal_counter = 0;
static params_obj_t g_params = { 0 };
static char buffer_interval[STR_SIZE];
static char buffer_threshold[STR_SIZE];


/*------------------------------static functions------------------------------*/
static int ResetWatchDog();
static int ResetUser();
static int ResetIsolated();
static int TaskToExecute(void* args);
static void PulseSignal(int signum);
static void StopSignal(int signum);
static int InitSignalsDispositions(struct sigaction* act);
static int CreateWatchDog(params_obj_t* params);


/*----------------------static functions implementations----------------------*/
static int ResetWatchDog()
{
    sem_t* sem;
    pid_t fork_pid;
    char buffer[STR_SIZE];

    kill(g_params.pid_other, SIGUSR2);
    waitpid(g_params.pid_other, NULL, 0);

    fork_pid = fork();

    if (-1 == fork_pid)
    {
#ifndef NDEBUG
    AppendText("Re-execution fork of WD process failed\n");
#endif
        return 1;
    }

    else if (0 == fork_pid)
    {
        if (0 != ExecWatchDog())
        {
            return 1;
        }
        FreeAllocatedResources();
    }

    g_params.pid_other = fork_pid;
    g_params.is_user = 1;

    sprintf(buffer, "%d", fork_pid);
    setenv(WD_ENV_VAR_NAME, buffer, 1);

    sem = sem_open(SEM_NAME, O_CREAT, (S_IRUSR | S_IWUSR), 0);
    if (SEM_FAILED == sem)
    {
#ifndef NDEBUG
    AppendText("sem_open failed - ResetIsolated\n");
#endif
        return 1;
    }

    sem_wait(sem);
    sem_close(sem);

    return 0;
}

static int ResetUser()
{
    size_t i = 0;

    for (i = 0; i < (size_t)(g_params.argc - ADDITIONAL_ARGS + 1); ++i)
    {
        g_params.argv_wd[i] = g_params.argv_wd[i + 3];
    }
    g_params.argv_wd[i] = NULL;

    if (-1 == execvp(g_params.argv_wd[0], g_params.argv_wd))
    {
#ifndef NDEBUG
    AppendText("Re-execution of User process failed\n");
#endif
        return 1;
    }

    return 0;
}

static int ResetIsolated()
{
    atomic_store(&signal_counter, 0);

    if (g_params.is_user)
    {
        if (0 != ResetWatchDog())
        {
#ifndef NDEBUG
    FreeAllocatedResources();
#endif
            return 1;
        }
    }
    else
    {
        if (0 != ResetUser())
        {
#ifndef NDEBUG
    FreeAllocatedResources();
#endif
            return 1;
        }
    }

    return 0;
}

static int TaskToExecute(void* args)
{
    char log_buffer[STR_SIZE];

    UNUSED(args);

    if (1 == atomic_load(&flag_stop))
    {
        HeapSchedulerDestroy(g_params.sched);
        return 0;
    }

#ifndef NDEBUG
    sprintf(log_buffer, "Counter: %u (pid = %d)\n", signal_counter, getpid());
    AppendText(log_buffer);
#endif

    kill(g_params.pid_other, SIGUSR1);

#ifndef NDEBUG
    sprintf(log_buffer, "sent signal %d (SIGUSR1) to pid=%d\n", SIGUSR1,
            g_params.pid_other);
    AppendText(log_buffer);
#endif

    atomic_fetch_add(&signal_counter, 1);

    if (atomic_load(&signal_counter) > g_params.threshold)
    {
        HeapSchedulerStop(g_params.sched);
#ifndef NDEBUG
    sprintf(log_buffer, "sent signal %d (SIGUSR2) pid=%d -> pid=%d\n", SIGUSR2,
            getpid(), g_params.pid_other);
    AppendText(log_buffer);
#endif
    }

    return 0;
}

static void PulseSignal(int signum)
{
    UNUSED(signum);
    atomic_store(&signal_counter, 0);
}

static void StopSignal(int signum)
{
    UNUSED(signum);
    atomic_store(&flag_stop, 1);
}

static int InitSignalsDispositions(struct sigaction* act)
{
    act->sa_handler = PulseSignal;

    if (-1 == sigaction(SIGUSR1, act, NULL))
    {
        return 1;
    }

    act->sa_handler = StopSignal;

    if (-1 == sigaction(SIGUSR2, act, NULL))
    {
        return 1;
    }

    return 0;
}

static int CreateWatchDog(params_obj_t* params)
{
    struct sigaction s_act = { 0 };

    if (0 != InitSignalsDispositions(&s_act))
    {
#ifndef NDEBUG
    AppendText("setenv of WD_ENV_VAR_NAME failed\n");
#endif
        return 1;
    }

    if (!params->is_user)
    {
        g_params = *params;
    }
    else
    {
        g_params.pid_other = params->pid_other;
        g_params.is_user = params->is_user;
    }

    g_params.sched = HeapSchedulerCreate();

    if (!g_params.sched)
    {
        return 1;
    }

    HeapSchedulerAdd(g_params.sched, TaskToExecute, &g_params,
                        g_params.interval);

    return 0;
}


/*-------------------------API functions implementations----------------------*/
void AppendText(const char* str_input)
{
	FILE* fp = fopen(LOGFILE_PATH, "a");
	fputs(str_input, fp);
	fclose(fp);
}

int InitParams(size_t threshold, size_t interval, int argc, char** argv)
{
    size_t i = 0;

    g_params.argv = argv;
    g_params.interval = interval;
    g_params.threshold = threshold;
    g_params.argc = argc + ADDITIONAL_ARGS;
    g_params.argv_wd = (char**)malloc((g_params.argc) * sizeof(char*));

    if (!g_params.argv_wd)
    {
        return 1;
    }

    g_params.argv_wd[i++] = EXEC_WD_PATH;

    sprintf(buffer_interval, "%lu", interval);
    g_params.argv_wd[i++] = buffer_interval;

    sprintf(buffer_threshold, "%lu", threshold);
    g_params.argv_wd[i++] = buffer_threshold;

    for (; i < (size_t)(g_params.argc - 1); ++i)
    {
        g_params.argv_wd[i] = argv[i - ADDITIONAL_ARGS + 1];
    }

    g_params.argv_wd[i] = NULL;

    return 0;
}

int ExecWatchDog()
{
    char buffer[STR_SIZE];

    sprintf(buffer, "%d", getpid());
    if (-1 == setenv(WD_ENV_VAR_NAME, buffer, 1))
    {
#ifndef NDEBUG
    AppendText("setenv of WD_ENV_VAR_NAME failed\n");
#endif
        FreeAllocatedResources();
        return 1;
    }

    if (-1 == execvp(EXEC_WD_PATH, g_params.argv_wd))
    {
#ifndef NDEBUG
    AppendText("Initial execution of WD process failed\n");
#endif
        FreeAllocatedResources();
        return 1;
    }

    FreeAllocatedResources();
    return 1;
}

int RunWatchDog(params_obj_t* params)
{
    sem_t* sem = NULL;

    if (0 != CreateWatchDog(params))
    {
        return 1;
    }

    sem = sem_open(SEM_NAME, O_CREAT, (S_IRUSR | S_IWUSR), 0);

    if (SEM_FAILED == sem)
    {
        HeapSchedulerDestroy(g_params.sched);
        return 1;
    }

    sem_post(sem);

    while (STOPPED == HeapSchedulerRun(g_params.sched))
    {
        if (0 != ResetIsolated())
        {
            return 1;
        }
    }

    sem_close(sem);
    return 0;
}

void FreeAllocatedResources()
{
    free(g_params.argv_wd);
    sem_unlink(SEM_NAME);
}
