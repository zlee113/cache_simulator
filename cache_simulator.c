/**
 * @file cache_simulator.c
 * @brief Source file to setup caches and simualte replacement
 */

#include "cache_simulator.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>

int iterator = 0;


/* Age every elements frequency in cache based on the current min */
void dynamically_age(cache_t *cache, int32_t index) {
  for (int i = 0; i < NUM_OF_WAYS; i++) {
    cache->cache[index][i].freq -= cache->min;
    }
}

/* Find the current minimum frequency value in the set */
void get_min(cache_t *cache, int32_t index) {

  int min = cache->cache[index][0].freq;

  for (int i = 0; i < NUM_OF_WAYS; i++) {
    if (cache->cache[index][i].freq < min) {
      min = cache->cache[index][i].freq;
    }
  }

  cache->min = min;

}


/**
 * @brief Replace a single line in cache based on the replacement policy
 *
 * @param cache The cache with all its parameters
 * @param index The set of the caches index
 * @param line The line being used to replace with
 */
void replace_cache_line(cache_t *cache, uint32_t index, line_t *line) {

  int freq_low = cache->cache[index][0].freq;
  int recency_low = cache->cache[index][0].recency;
  int timestamp_low = cache->cache[index][0].timestamp;
  int new_index = 0;
  if (cache->policy == LFU_DA) {
    dynamically_age(cache, index);
    get_min(cache, index);
  }
  for (int i = 0; i < NUM_OF_WAYS; i++) {
    switch (cache->policy) {
    case FIFO:
      if (timestamp_low > cache->cache[index][i].timestamp) {
        timestamp_low = cache->cache[index][i].timestamp;
        new_index = i;
      }
      break;

    case LRU:
      if (recency_low > cache->cache[index][i].recency) {
        recency_low = cache->cache[index][i].recency;
        new_index = i;
      }
      break;

    case LFU:
      if (freq_low > cache->cache[index][i].freq) {
        freq_low = cache->cache[index][i].freq;
        new_index = i;
      }
      break;

    case LRFU:
      break;

    case LFU_DA:
      if (freq_low > cache->cache[index][i].freq) {
        freq_low = cache->cache[index][i].freq;
        new_index = i;
      }
      break;

    default:
      printf("Bad policy");
      break;
    }
  }

  cache->cache[index][new_index] = *line;
}

/**
 * @brief Attempt to read cache or replace if doesn't work
 *
 * @param cache The cache with all its parameters
 * @param index The set of the caches index
 * @param line The line being used to replace with
 */
void read_cache(cache_t *cache, uint32_t index, line_t *line) {
  cache->accesses++;
  bool hit = false;
  for (int i = 0; i < NUM_OF_WAYS; i++) {
    if (cache->cache[index][i].value == line->value &&
        cache->cache[index][i].address == line->address) {
      cache->read_hits++;
      cache->cache[index][i].recency = iterator;
      cache->cache[index][i].freq++;
      if (cache->policy == LFU_DA) {
        cache->cache[index][i].freq += cache->min + 1;
      }
      hit = true;
      break;
    }
    else if (cache->cache[index][i].address == 0) {
      cache->read_misses++;
      cache->cache[index][i] = *line;
      hit = true;
      break;
    }
  }
  if (!hit) {
    cache->read_misses++;
    line->freq += cache->min;
    replace_cache_line(cache, index, line);
  }
}

/**
 * @brief Attempt to write cache or replace if doesn't work
 *
 * @param cache The cache with all its parameters
 * @param index The set of the caches index
 * @param line The line being used to replace with
 */
void write_cache(cache_t *cache, uint32_t index, line_t *line) {
  cache->accesses++;
  bool hit = false;
  for (int i = 0; i < NUM_OF_WAYS; i++) {
    if (cache->cache[index][i].value == line->value &&
        cache->cache[index][i].address == line->address) {
      cache->write_hits++;
      cache->cache[index][i].recency = iterator;
      cache->cache[index][i].freq++;
      if (cache->policy == LFU_DA) {
        cache->cache[index][i].freq += cache->min + 1;
      }
      hit = true;
      break;
    }
    else if (cache->cache[index][i].address == 0) {
      cache->write_misses++;
      cache->cache[index][i] = *line;
      if (cache->policy == LFU_DA) {
        cache->cache[index][i].freq += cache->min + 1;
      }
      hit = true;
      break;
    }
  }
  if (!hit) {
    cache->write_misses++;
    line->freq += cache->min;
    replace_cache_line(cache, index, line);
  }
}

void check_and_replace_cache(cache_t *cache, line_t *line, char type) {
  uint32_t index = (line->address / NUM_OF_SETS) % NUM_OF_SETS;
  if (type == 'R') {
    read_cache(cache, index, line);
  } else {
    write_cache(cache, index, line);
  }
}

void print_cache_stats(cache_t *cache) {
  if (cache->policy == 0)
    printf("Stats for FIFO\n");
  else if (cache->policy == 1)
    printf("Stats for LRU\n");
  else if (cache->policy == 2)
    printf("Stats for LFU\n");
  else if (cache->policy == 3)
    printf("Stats for LFRU\n");
  else if (cache->policy == 4)
    printf("Stats for LFU Dynamic Aging\n");
  printf("Cache Accesses: %d\n", cache->accesses);
  printf("Read Hits:      %d\n", cache->read_hits);
  printf("Read Misses:    %d\n", cache->read_misses);
  printf("Write Hits:     %d\n", cache->write_hits);
  printf("Write Misses:   %d\n\n", cache->write_misses);
}
 
int main() {

  /* Structs holding all the info for each replacement policy */
  cache_t fifo_cache = {0};
  fifo_cache.policy = FIFO;

  cache_t lru_cache = {0};
  lru_cache.policy = LRU;

  cache_t lfu_cache = {0};
  lfu_cache.policy = LFU;

  cache_t lrfu_cache = {0};
  lrfu_cache.policy = LRFU;

  cache_t lfu_da_cache = {0};
  lfu_da_cache.policy = LFU_DA;

  /* Open sample file and read in line by line */
  FILE *file = fopen("test_files/input/multiplication_input.txt", "r");
  int buffer = 0;
  char type = 'a';
  int val = 0;

  while (fscanf(file, "%x %c %d\n", &buffer, &type, &val) == 3) {
    line_t line = {buffer, val, 0, iterator, iterator};
    check_and_replace_cache(&fifo_cache, &line, type);
    check_and_replace_cache(&lru_cache, &line, type);
    check_and_replace_cache(&lfu_cache, &line, type);
    check_and_replace_cache(&lrfu_cache, &line, type);
    check_and_replace_cache(&lfu_da_cache, &line, type);
    iterator++;
  }
  fclose(file);

  print_cache_stats(&fifo_cache);
  print_cache_stats(&lru_cache);
  print_cache_stats(&lfu_cache);
  print_cache_stats(&lrfu_cache);
  print_cache_stats(&lfu_da_cache);

  return 0;
}
