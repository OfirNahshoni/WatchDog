/* heap.h */

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>     /* size_t */

typedef struct heap heap_t;
typedef int (*compare_func_t)(const void* data, const void* param);
typedef int (*is_match_t)(const void* data1, const void* data2);


/*
*	@desc:				Allocates new heap based on @compare_func
*	@param:				@compare_func: compare function returns zero if equal,
*						negative if @data1 is less than @data2 and otherwise
*						postive
*	@return:			Newly allocated heap
*	@error:				Returns NULL if allocation failed
*	@time complexity:	O(malloc) for both AC/WC
*	@space complexity:	O(malloc) for both AC/WC
*/
heap_t* HeapCreate(compare_func_t compare_func);


/*
*	@desc:				Frees @heap using @HeapCreate
*	@param:				@heap: preallocated heap
*	@return:			None
*	@error:				Undefined behavior if @heap is invalid
*	@time complexity:	O(free) for both AC/WC
*	@space complexity:	O(free) for both AC/WC
*/
void HeapDestroy(heap_t* heap);


/*
*	@desc:				Pushes @data to @heap
*	@param:				@heap: preallocated heap
*						@data: user data to insert
*	@return:			Zero if function successful otherwise non zero
*	@error:				Undefined behavior if @heap is invalid
*						Returns nonzero value if allocation failed
*	@time complexity:	O(log(n)) AC and O(n) for WC
*	@space complexity:	O(1) for AC and O(n) for WC
*/
int HeapPush(heap_t* heap, void* data);


/*
*	@desc:				Pops the first element from @heap
*	@param:				@heap: preallocated heap
*	@return:			Zero if function successful otherwise non zero
*	@error:				Undefined behavior if @heap is invalid or @heap is empty
*						Returns nonzero value if allocation failed
*	@time complexity:	O(log(n)) AC and O(n) for WC
*	@space complexity:	O(1) for AC and O(n) for WC
*/
int HeapPop(heap_t* heap);


/*
*	@desc:				Returns the first element from @heap
*	@param:				@heap: preallocated heap
*	@return:			First element data
*	@error:				Undefined behavior if @heap is invalid or @heap is empty
*	@time complexity:	O(1) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
void* HeapPeek(const heap_t* heap);


/*
*	@desc:				Returns the count of elements in @heap
*	@param:				@heap: preallocated heap
*	@return:			Returns the count of elements in @heap
*	@error:				Undefined behavior if @heap is invalid
*	@time complexity:	O(1) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
size_t HeapSize(const heap_t* heap);


/*
*	@desc:				Checks if @heap is empty
*	@param:				@heap: preallocated heap
*	@return:			Returns one if @heap is empty otherwise zero
*	@error:				Undefined behavior if @heap is invalid
*	@time complexity:	O(1) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
int HeapIsEmpty(const heap_t* heap);


/*
*	@desc:				Removes the first element matches @is_match with @param
*	@param:				@heap: preallocated heap
*						@param: User param to match with
*						@is_match: match function for checking if element
*						matches with @param
*	@return:			Returns the removed element or NULL if not found
*	@error:				Undefined behavior if @heap is invalid or @is_match is
*						invalid
*	@time complexity:	O(n * is_match) for both AC/WC
*	@space complexity:	O(is_match) for both AC/WC
*/
void* HeapRemove(heap_t* heap, void* param, is_match_t is_match);

#endif /* __HEAP_H__ */
