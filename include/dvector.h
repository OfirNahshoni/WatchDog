/******************************************************************************
 * File name: dvector.h
 * Owner: Ofir Nahshoni
 * Review: Aviv
 * Review status: APPROVED
 ******************************************************************************/

#ifndef __DVECTOR_H__
#define __DVECTOR_H__

#include <stddef.h>			/* size_t */

typedef struct dvector dvector_t;

/*
* 	@Desc: Allocates memory for a dynamic vector
* 	@Params: @capacity of dynamic vector (number of elements) and element_size
*            (sizeof elements in bytes)
* 	@Return value: function returns a pointer to dynamic vector
*                  if succeeded or NULL if memory allocation failed
*/
dvector_t* DvectorCreate(size_t capacity, size_t element_size);

/*
* 	@Desc: Frees the memory allocated for the dynamic vector
* 	@Params: @dvector - pointer to a pre-allocated dvector_t data type
* 	@Return: none
*/
void DvectorDestroy(dvector_t* dvector);

/*
* 	@Desc: Returns the capacity of the @dvector
* 	@Params: @dvector  pointer to a pre-allocated dvector_t data type
* 	@Return: Return the capacity of the @dvector
*/
size_t DvectorCapacity(const dvector_t* dvector);

/*
* 	@Desc: Returns the amount of elements currently stored in the dvector
* 	@Params: @dvector pointer to a pre-allocated dvector_t data type
* 	@Return: Returns the size of the @dvector
*/
size_t DvectorSize(const dvector_t* dvector);

/*
* 	@Desc: Set specific value to the element at the index given in the Dvector.
* 	@Params: pointer to a pre-allocated dvector_t data type, index,
*            void* value to set in a specific index .
* 	@Return: none
*/
void DvectorSetElement(dvector_t* dvector, size_t index, const void* value);

/*
* 	@Desc: Get the value of an element denoted by a specific index
* 	@Params: pointer to a pre-allocated dvector_t data type, index,
*            void* to the return element.
* 	@Return: none
*/
void DvectorGetElement(const dvector_t* dvector, size_t index, void* dest);

/*
* 	@Desc: Pushes an element to the last valid element index (DvectorSize)
* 	@Params: Pointer to a pre-allocated dvector_t data type,
*            void* to the return element.
* 	@Return: Return success (0) or failure (1)
*/
int DvectorPushBack(dvector_t* dvector, const void* element);

/*
* 	@Desc: Pops out the element at the last valid index in dvector (DvectorSize)
* 	@Params: Pointer to a pre-allocated dvector_t data type.
* 	@Return: Return success (0) or failure (1). If this element is required
*            use DvectorGetElement before.
*/
int DvectorPopBack(dvector_t* dvector);

/*
* 	@Desc: Update the capacity of the dvector.
* 	@Params: pointer to a pre-allocated dvector_t data type,
*            size_t new capacity for the dvector
* 	@Return: (0) if success or (1) for failure
* 	Edge Cases:
* 	1) (new_capacity = 0) -> array should be freed and set to null
*      (set size & capacity to zero) and return SUCCESS.
* 	2) (size > new_capacity) -> size = new_capacity and return SUCCESS.
* 	3) memory reallocation failure -> return FAILURE.
*	4) (new_capacity = old_capacity) -> return SUCCESS.
* 	5) (dvector = NULL) -> assert.
*/
int DvectorResize(dvector_t* dvector, size_t new_capacity);

/*
* 	@Desc: Shrink to size.
* 	@Params: pointer to a pre-allocated dvector_t data type
* 	@Return: (0) if success or (1) for failure
* 	Edge Cases:
* 	1) (capacity = size) -> return SUCCESS.
* 	2) memory reallocation failure -> return FAILURE.
* 	3) memory reallocation failure, while trying to shrink -> ensure that the
*      dvector remains in its state.
* 	4) (dvector = NULL) -> assert.
*/
int DvectorShrink(dvector_t* dvector);

#endif  /* End of header guard Dvector */
