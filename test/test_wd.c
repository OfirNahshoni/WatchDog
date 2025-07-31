/*
* File name: test_wd.c
* Description: Entry point for testing the Watchdog API. This program simulates
*              a critical user process monitored by the Watchdog. It tests the
*              start, stop, and functionality of the Watchdog service.
*/

#include <stdio.h>          /* fprintf */

#include "wd.h"

#define INTERVAL (3)
#define THRESHOLD (4)

static void CriticalCodeForWatchDogSecure(char** argv, int argc);
static void PrintArgv(char** argv, int argc);

int main(int argc, char* argv[])
{
    CriticalCodeForWatchDogSecure(argv, argc);
    PrintArgv(argv, argc);

    return 0;
}

static void CriticalCodeForWatchDogSecure(char** argv, int argc)
{
    size_t i = 0;
    wd_status_t status;

    status = WDStart(THRESHOLD, INTERVAL, argc, argv);

    for (; i < 10000000; ++i)
    {
        fprintf(stdout, "%lu\n", i);
    }

    if (WD_SUCCESS == status)
    {
        fprintf(stdout, "Success\n");
    }
    else
    {
        fprintf(stdout, "Failure\n");
    }

    WDStop();
}

static void PrintArgv(char** argv, int argc)
{
    size_t i = 0;

    fprintf(stdout, "Original command-line arguments:\n");

    for (; i < (size_t)argc; ++i)
    {
        fprintf(stdout, "argv[%lu] = %s\n", i, argv[i]);
    }
}
