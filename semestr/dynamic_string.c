#include "dynamic_string.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void dynamic_string_init(dynamic_string* s) 
{
	s->buffer = NULL;
	s->length = 0;
	s->capacity = 0;
}

void dynamic_string_free(dynamic_string* s)
{
	free(s->buffer);
}

int dynamic_string_push_back(dynamic_string* s, char item) 
{
	if (!s)
		return EINVAL;

	if (s->length + 2 > s->capacity)
	{
		size_t newCapacity = s->capacity * 2;
		if (newCapacity < 2)
			newCapacity = 2;
		char* newBuffer = realloc(s->buffer, newCapacity);
		
		if (!newBuffer)
			return ENOMEM;

		memset(newBuffer + s->capacity, 0, newCapacity - s->capacity);

		s->buffer = newBuffer;
		s->capacity = newCapacity;
	}

	s->buffer[s->length++] = item;
}

size_t dynamic_string_get_hash(dynamic_string* s) 
{
	// Algorithm from http://www.cse.yorku.ca/~oz/hash.html
	size_t hash = 5381;
	for (size_t i = 0; s->buffer[i]; i++)
		hash = hash * 33 ^ s->buffer[i];
	return hash;
}

dynamic_string* dynamic_string_copy(dynamic_string* s)
{
	dynamic_string* output = malloc(sizeof(dynamic_string));
	output->length = s->length;
	output->capacity = s->length;
	output->buffer = malloc(s->length + 1);
	strcpy(output->buffer, s->buffer);
	return output;
}