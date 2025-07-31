/******************************************************************************
 * File name: heap.c
 * Owner: Ofir Nahshoni
 * Review: Sahar
 * Review status: APPROVED
 ******************************************************************************/

#include <assert.h>         /* assert */
#include <sys/types.h>      /* ssize_t */
#include <stdlib.h>         /* malloc, free */

#include "dvector.h"        /* dvector_t */
#include "heap.h"

/*-----------------------------------macros-----------------------------------*/
#define VECTOR_CAPACITY 10
#define UNUSED(x) ((void)x)
#define PARENT_IDX(i) ((i - 1) / 2)
#define LEFT_CHILD_IDX(i) (2 * i + 1)
#define RIGHT_CHILD_IDX(i) (2 * i + 2)

/*-----------------------------typdefs & Structures---------------------------*/
struct heap
{
    compare_func_t compare_func;
    dvector_t* vector;
};

/*------------------------------static functions------------------------------*/
static void SwapElements(dvector_t* vector, size_t idx1, size_t idx2)
{
    void* data1 = NULL;
    void* data2 = NULL;

    DvectorGetElement(vector, idx1, &data1);
    DvectorGetElement(vector, idx2, &data2);

    DvectorSetElement(vector, idx1, &data2);
    DvectorSetElement(vector, idx2, &data1);
}

static void HeapifyUp(heap_t* heap, size_t idx)
{
    void* current = NULL;
    void* parent = NULL;

    if (0 == idx)
    {
        return;
    }

    DvectorGetElement(heap->vector, idx, &current);
    DvectorGetElement(heap->vector, PARENT_IDX(idx), &parent);

    if (heap->compare_func(current, parent) < 0)
    {
        SwapElements(heap->vector, idx, PARENT_IDX(idx));
        HeapifyUp(heap, PARENT_IDX(idx));
    }
}

static void HeapifyDown(heap_t* heap, size_t idx)
{
    void* current = NULL;
    void* left_child = NULL;
    void* right_child = NULL;

    size_t current_idx = idx;
    size_t heap_size = HeapSize(heap);
    size_t left_idx = LEFT_CHILD_IDX(idx);
    size_t right_idx = RIGHT_CHILD_IDX(idx);

    if (left_idx >= heap_size)
    {
        return;
    }

    DvectorGetElement(heap->vector, current_idx, &current);
    DvectorGetElement(heap->vector, left_idx, &left_child);

    if (right_idx >= heap_size)
    {
        if (heap->compare_func(left_child, current) < 0)
        {
            SwapElements(heap->vector, left_idx, current_idx);
        }

        return;
    }

    DvectorGetElement(heap->vector, right_idx, &right_child);

    if ((heap->compare_func(left_child, right_child)) < 0 &&
        (heap->compare_func(left_child, current) < 0))
    {
        SwapElements(heap->vector, left_idx, current_idx);
        HeapifyDown(heap, left_idx);
    }
    else if (heap->compare_func(right_child, current) < 0)
    {
        SwapElements(heap->vector, right_idx, current_idx);
        HeapifyDown(heap, right_idx);
    }
}

/*--------------------------------API functions-------------------------------*/
heap_t* HeapCreate(compare_func_t compare_func)
{
    heap_t* heap = NULL;

    assert(compare_func);

    heap = (heap_t*)malloc(sizeof(heap_t));

    if (NULL == heap)
    {
        return NULL;
    }

    heap->vector = DvectorCreate(VECTOR_CAPACITY, sizeof(void*));

    if (NULL == heap->vector)
    {
        free(heap);
        return NULL;
    }

    heap->compare_func = compare_func;

    return heap;
}

void HeapDestroy(heap_t* heap)
{
    assert(heap);

    DvectorDestroy(heap->vector);
    free(heap);
}

int HeapPush(heap_t* heap, void* data)
{
    int push_result = 0;

    assert(heap);

    push_result = DvectorPushBack(heap->vector, &data);

    if (0 != push_result)
    {
        return push_result;
    }

    HeapifyUp(heap, HeapSize(heap) - 1);

    return push_result;
}

int HeapPop(heap_t* heap)
{
    int pop_result = 0;
    void* last_element = NULL;
    size_t heap_size = 0;

    assert(heap);
    assert(0 == HeapIsEmpty(heap));

    heap_size = DvectorSize(heap->vector);

    DvectorGetElement(heap->vector, heap_size - 1, &last_element);

    pop_result = DvectorPopBack(heap->vector);

    if (0 != pop_result)
    {
        return pop_result;
    }

    if (!HeapIsEmpty(heap))
    {
        DvectorSetElement(heap->vector, 0, &last_element);
        HeapifyDown(heap, 0);
    }

    return pop_result;
}

void* HeapPeek(const heap_t* heap)
{
    void* data = NULL;

    assert(heap);
    assert(0 == HeapIsEmpty(heap));

    DvectorGetElement(heap->vector, 0, &data);

    return data;
}

size_t HeapSize(const heap_t* heap)
{
    assert(heap);

    return DvectorSize(heap->vector);
}

int HeapIsEmpty(const heap_t* heap)
{
    assert(heap);

    return (0 == DvectorSize(heap->vector));
}

static ssize_t FindIndex(dvector_t* vector, void* param,
                        is_match_t is_match)
{
    ssize_t i = 0;
    void* runner = NULL;
    size_t vector_size = DvectorSize(vector);


    for (; i < (ssize_t)vector_size; ++i)
    {
        DvectorGetElement(vector, i, &runner);

        if (1 == is_match(runner, param))
        {
            return i;
        }
    }

    return -1;
}

void* HeapRemove(heap_t* heap, void* param, is_match_t is_match)
{
    ssize_t remove_idx = 0;
    void* data_removed = NULL;
    size_t heap_size = DvectorSize(heap->vector);

    assert(heap);

    remove_idx = FindIndex(heap->vector, param, is_match);

    if (-1 == remove_idx)
    {
        return NULL;
    }

    DvectorGetElement(heap->vector, (size_t)remove_idx, &data_removed);
    SwapElements(heap->vector, (size_t)remove_idx, heap_size - 1);
    DvectorPopBack(heap->vector);
    HeapifyDown(heap, (size_t)remove_idx);

    return data_removed;
}
