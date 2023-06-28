#include "hash_map.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void hash_map_init(hash_map* map, void (*free_value)(void*))
{
	map->length = 0;
	map->capacity = 0;
	map->entries = NULL;
	map->free_value = free_value;
}

int hash_map_add(hash_map* map, dynamic_string* key, void* value)
{
	if (map->length * 2 >= map->capacity) 
	{
		size_t new_capacity = map->capacity * 2;
		if (new_capacity < 2)
			new_capacity = 2;
		hash_map_entry* new_entries = calloc(new_capacity, sizeof(hash_map_entry));
		if (!new_entries)
			return ENOMEM;

		size_t old_capacity = map->capacity;
		hash_map_entry* old_entries = map->entries;

		map->length = 0;
		map->capacity = new_capacity;
		map->entries = new_entries;

		for (size_t i = 0; i < old_capacity; i++)
		{
			if (old_entries[i].key != NULL) 
				hash_map_add(map, old_entries[i].key, old_entries[i].value);
		}
	}

	hash_map_entry* entry = hash_map_find(map, key);

	// Этот ключ уже существует
	if (entry->key != NULL)
		return 0;

	entry->key = key;
	entry->value = value;
	map->length++;
	return 1;
}

void hash_map_free(hash_map* map)
{
	for (size_t i = 0; i < map->capacity; i++)
	{
		if (map->entries[i].key != NULL) 
		{
			(*(map->free_value))(map->entries[i].value);

			dynamic_string_free(map->entries[i].key);
			free(map->entries[i].key);
		}
	}

	free(map->entries);
	hash_map_init(map, map->free_value);
}

hash_map_entry* hash_map_find(hash_map* map, dynamic_string* key)
{
	size_t index = dynamic_string_get_hash(key) % map->capacity;
	while (map->entries[index].key != NULL)
	{
		if (!strcmp(map->entries[index].key->buffer, key->buffer))
			return &(map->entries[index]);

		index = (index + 1) % map->capacity;
	}

	return &(map->entries[index]);
}

void hash_map_delete(hash_map* map, hash_map_entry* entry)
{
	size_t index = entry - map->entries;

	dynamic_string_free(map->entries[index].key);
	map->free_value(map->entries[index].value);
	map->entries[index].key = NULL;
	map->entries[index].value = NULL;
	map->length--;

	while (map->entries[index].key != NULL) 
	{
		dynamic_string* tmpKey = map->entries[index].key;
		void* tmpValue = map->entries[index].value;

		map->entries[index].key = NULL;
		map->entries[index].value = NULL;

		hash_map_add(map, tmpKey, tmpValue);

		index = (index + 1) % map->capacity;
	}
}