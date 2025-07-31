/* heap_p_queue.h */

#ifndef __HEAP_PQ_H__
#define __HEAP_PQ_H__

#include <stddef.h>		/* size_t */

typedef struct heap_pq heap_pq_t;

/*
*   @desc:          Allocates Priority Queue.
*   @params: 		@priority_func: Compare function that the priority is sorted
*									by.
*   @return value:  Pointer to the allocated Priority Queue
*   @error: 		NULL if allocation fails
*					Undefined behavior if @compare_func is not valid
*   @time complex: 	O(malloc) for both AC/WC
*   @space complex: O(malloc) for both AC/WC
*/
heap_pq_t* HeapPQCreate(int (*compare_func)(const void*, const void*));

/*
*   @desc: 	        Frees Priority Queue. Must be created using @PQCreate.
*   @params: 	    @pq: Priority queue to free.
*   @return value: 	None
*   @error: 		Undefined behavior if @pq is not valid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
void HeapPQDestroy(heap_pq_t* heap_pq);

/*
*   @desc: 	     Enqueues an item to @pq with @data.
*   @params: 	    	@pq : pre allocated priority queue.
*				@data: data of the new element
*   @return value: 	returns 0 on success
*   @error: 		In the event insertion fails(due to allocation) will return
* 				    non zero value.
*					Undefined Behavior if @pq is not valid.
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int HeapPQEnqueue(heap_pq_t* heap_pq, void* data);

/*
*   @desc:		   	Removes the first element from @pq.
*   @params: 	   	@pq : pre allocated priority queue.
*   @return value: 	the data of the removed element
*   @error: 	   	Undefined Behavior if @pq is invalid or empty.
*   @time complex: 	O(1) for both AC/WC.
*   @space complex: O(1) for both AC/WC.
*/
void* HeapPQDequeue(heap_pq_t* heap_pq);

/*
*   @desc: 		   	Returns the data of the first element in @pq.
*   @params: 	   	@pq : pre allocated priority queue.
*   @return value: 	The data of the first element.
*   @error: 	   	Undefined behavior if @pq is invalid or empty.
*   @time complex: 	O(1) for both AC/WC.
*   @space complex: O(1) for both AC/WC.
*/
void* HeapPQPeek(const heap_pq_t* heap_pq);

/*
*   @desc: 		   	Checks if @pq is empty.
*   @params: 	   	@pq : pre allocated priority queue.
*   @return value: 	1 if the queue is empty 0 otherwise.
*   @error: 	   	Undefined behavior if @pq is invalid.
*   @time complex: 	O(1) for both AC/WC.
*   @space complex: O(1) for both AC/WC.
*/
int HeapPQIsEmpty(const heap_pq_t* heap_pq);

/*
*   @desc: 	        Returns the number of elements in @pq
*   @params: 		@pq : pre allocated priority queue.
*   @return value: 	Number of elements in the queue.
*   @error: 		Undefined behavior if @pq is invalid.
*   @time complex: 	O(n) for both AC/WC.
*   @space complex: O(1) for both AC/WC.
*/
size_t HeapPQSize(const heap_pq_t* heap_pq);

/*
*   @desc: 	        Removes all the elements in @pq.
*   @params: 		@pq : pre allocated priority queue.
*   @return value: 	None
*   @error: 		Undefined behavior if @pq is invalid.
*   @time complex: 	O(n) for both AC/WC.
*   @space complex: O(1) for both AC/WC.
*/
void HeapPQClear(heap_pq_t* heap_pq);
/*
*   @desc:          	Removes the first element matching @param in @is_match
					param and returns its value. Will return NULL if not
					found such element
*	@params:        	@pq : pre allocated priority queue.
*	@return value:		The data of the erased element if found and NULL otherwise
*	@error:			Undefined behavior if @is_match or @pq is invalid
*	@time complex:		O(n) for both AC/WC.
*	@space complex:	O(1) for both AC/WC.
*/
void* HeapPQErase(heap_pq_t* heap_pq, int (*is_match)(const void*, const void*),
                    const void* param);

#endif  /* __HEAP_PQ_H__ */
