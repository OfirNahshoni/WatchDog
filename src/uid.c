/******************************************************************************
* File name: ilrd_uid.c
 * Owner: Ofir Nahshoni
 * Review: Arthur
 * Review status: APPROVED
 ******************************************************************************/

#include <unistd.h>		/* getpid */
#include <stdio.h>      /* fprintf */
#include <pthread.h>    /* mutex_t */
#include <stdatomic.h>  /* fetch_and_add */
#include <string.h>		/* memcpy, memcmp */
#include <ifaddrs.h>	/* getifaddrs, freeifaddrs */

#include "uid.h"

const uid_t bad_uid = { 0 };

uid_t UIDCreate(void)
{
    uid_t uid;
    static atomic_int count = 0;
    struct ifaddrs* addr_struct = NULL;

    uid.time = time(NULL);
    uid.pid = getpid();

    if (0 != getifaddrs(&addr_struct))
    {
        return bad_uid;
    }

    memcpy(uid.ip, addr_struct->ifa_addr->sa_data, sizeof(uid.ip));

    uid.counter = atomic_fetch_add(&count, 1);

    freeifaddrs(addr_struct);

    return uid;
}

int UIDIsSame(uid_t uid1, uid_t uid2)
{
    return ((uid1.time == uid2.time) &&
           (uid1.counter == uid2.counter) &&
           (uid1.pid == uid2.pid) &&
           (0 == memcmp(uid1.ip, uid2.ip, sizeof(uid1.ip))));
}
