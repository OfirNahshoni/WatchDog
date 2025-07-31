/******************************************************************************
 * File name: dvector.c
 * Owner: Ofir Nahshoni
 * Review: Amit
 * Review status: APPROVED
 ******************************************************************************/

#include <stdlib.h>			/* malloc, realloc, free */
#include <string.h>			/* memcpy */
#include <assert.h>			/* assert */

#include "dvector.h"

#define GROWTH_FACTOR (2)

struct dvector
{
	size_t size;
	size_t capacity;
	size_t element_size;
	void* array;
};

dvector_t* DvectorCreate(size_t capacity, size_t element_size)
{
	dvector_t* dvector = NULL;

	assert(capacity > 0);
	assert(element_size > 0);

	dvector = (dvector_t*)malloc(sizeof(dvector_t));

	if (NULL == dvector)
	{
		return NULL;
	}

	dvector->size = 0;
	dvector->capacity = capacity;
	dvector->element_size = element_size;

	dvector->array = malloc(capacity * element_size);
	if (NULL == dvector->array)
	{
		free(dvector);
		return NULL;
	}

	return dvector;
}

void DvectorDestroy(dvector_t* dvector)
{
	if (NULL != dvector)
	{
		free(dvector->array);
		free(dvector);
	}
}

size_t DvectorCapacity(const dvector_t* dvector)
{
	assert(NULL != dvector);

	return dvector->capacity;
}

size_t DvectorSize(const dvector_t* dvector)
{
	assert(NULL != dvector);

	return dvector->size;
}

void DvectorSetElement(dvector_t* dvector, size_t index, const void* value)
{
	assert(NULL != dvector);
	assert(NULL != dvector->array);
	assert(NULL != value);
	assert(index < dvector->size);

	memcpy(((unsigned char*)(dvector->array) + index * dvector->element_size), value, dvector->element_size);
}

void DvectorGetElement(const dvector_t* dvector, size_t index, void* dest)
{
	assert(NULL != dvector);
	assert(NULL != dvector->array);
	assert(NULL != dest);
	assert(index <= dvector->size);

	memcpy(dest, ((unsigned char*)(dvector->array) + index *
                    dvector->element_size), dvector->element_size);
}

int DvectorPushBack(dvector_t* dvector, const void* element)
{
	assert(NULL != dvector);
	assert(NULL != element);

	if (dvector->size == dvector->capacity)
	{
		DvectorResize(dvector, dvector->capacity * GROWTH_FACTOR + 1);
	}

	++(dvector->size);
	DvectorSetElement(dvector, dvector->size - 1, element);

	return 0;
}

int DvectorPopBack(dvector_t* dvector)
{
	assert(NULL != dvector);

    if ((NULL == dvector->array) || (0 == dvector->size))
    {
        return 1;
    }

    --(dvector->size);

    if ((dvector->size < (dvector->capacity / 4)) && (dvector->capacity > 4))
    {
        if (0 != DvectorShrink(dvector))
        {
            return 1;
        }
    }

    return 0;
}

int DvectorResize(dvector_t* dvector, size_t new_capacity)
{
	void* temp_array = NULL;

	assert(NULL != dvector);

	if (0 == new_capacity)
	{
		free(dvector->array);
		dvector->array = NULL;
		dvector->size = 0;
		dvector->capacity = 0;

		return 0;
	}

	temp_array = realloc(dvector->array, new_capacity * dvector->element_size);
	if (NULL == temp_array)
	{
		return 1;
	}

	if (dvector->size > new_capacity)
	{
		dvector->size = new_capacity;
	}

	dvector->array = temp_array;
	dvector->capacity = new_capacity;

	return 0;
}

int DvectorShrink(dvector_t* dvector)
{
	assert(NULL != dvector);
	assert(NULL != dvector->array);

	return (DvectorResize(dvector, dvector->size));
}
