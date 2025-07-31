/* heap_p_queue.c */

#include "heap_p_queue.h"

#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */

#include "heap.h"           /* heap_t */
#include "heap_p_queue.h"

struct heap_pq
{
    heap_t* heap;
};

heap_pq_t* HeapPQCreate(int (*compare_func)(const void*, const void*))
{
    heap_pq_t* heap_pq = NULL;

    assert(compare_func);

    heap_pq = (heap_pq_t*)malloc(sizeof(heap_pq_t));

    if (NULL == heap_pq)
    {
        return NULL;
    }

    heap_pq->heap = HeapCreate(compare_func);

    if (NULL == heap_pq->heap)
    {
        free(heap_pq);
        return NULL;
    }

    return heap_pq;
}

void HeapPQDestroy(heap_pq_t* heap_pq)
{
    assert(heap_pq);

    HeapDestroy(heap_pq->heap);
    free(heap_pq);
}

int HeapPQEnqueue(heap_pq_t* heap_pq, void* data)
{
    assert(heap_pq);

    return HeapPush(heap_pq->heap, data);
}

void* HeapPQDequeue(heap_pq_t* heap_pq)
{
    void* removed_data = NULL;

    assert(heap_pq);
    assert(!HeapPQIsEmpty(heap_pq));

    removed_data = HeapPeek(heap_pq->heap);

    return (!HeapPop(heap_pq->heap) ? removed_data : NULL);
}

void* HeapPQPeek(const heap_pq_t* heap_pq)
{
    assert(heap_pq);
    assert(!HeapPQIsEmpty(heap_pq));

    return HeapPeek(heap_pq->heap);
}

int HeapPQIsEmpty(const heap_pq_t* heap_pq)
{
    assert(heap_pq);

    return HeapIsEmpty(heap_pq->heap);
}

size_t HeapPQSize(const heap_pq_t* heap_pq)
{
    assert(heap_pq);

    return HeapSize(heap_pq->heap);
}

void HeapPQClear(heap_pq_t* heap_pq)
{
    assert(heap_pq);

    while (!HeapIsEmpty(heap_pq->heap))
    {
        /* ignore return value */
        HeapPop(heap_pq->heap);
    }
}

void* HeapPQErase(heap_pq_t* heap_pq,
                    int (*is_match)(const void* data, const void* param),
                    const void* param)
{
    assert(heap_pq);
    assert(is_match);

    return HeapRemove(heap_pq->heap, (void*)param, is_match);
}
