/**
 * @file cache_simulator.c
 * @brief Source file to setup caches and simualte replacement
 */

#include <stdio.h>
#include "cache_simulator.h"


int main () {

	/* Structs holding all the info for each replacement policy */
	cache_t fifo_cache = { 0 };
	fifo_cache.policy = FIFO;

	cache_t lru_cache = { 0 };
	lru_cache.policy = LRU;

	cache_t lfu_cache = { 0 };
	lfu_cache.policy = FIFO;

	FILE* file = fopen("test_files/input/multiplication_input.txt", "r");
	char buffer[32] = { 0 };
	char test = 'a';
	int num = 0;
	while(fscanf(file, "%s %c %d\n", buffer, &test, &num) == 3)
		printf("Address: %s, Type: %c, Value: %d\n", buffer, test, num);
	fclose(file);
	return 0;
}

