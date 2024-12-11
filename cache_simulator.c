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

void replace_cache_line(cache_t *cache, uint32_t index, line_t *line) {

  int freq_low = cache->cache[index][0].freq;
  int recency_low = cache->cache[index][0].recency;
  int timestamp_low = cache->cache[index][0].timestamp;
  int new_index = 0;
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
      break;

    default:
      printf("Bad policy");
      break;
    }
  }

  cache->cache[index][new_index] = *line;
}

void read_cache(cache_t *cache, uint32_t index, line_t *line) {
  cache->accesses++;
  bool hit = false;
  for (int i = 0; i < NUM_OF_WAYS; i++) {
    if (cache->cache[index][i].value == line->value &&
        cache->cache[index][i].address == line->address) {
      cache->read_hits++;
      cache->cache[index][i].recency = iterator;
      cache->cache[index][i].freq++;
      hit = true;
      break;
    }
  }
  if (!hit) {
    cache->read_misses++;
    replace_cache_line(cache, index, line);
  }
}

void write_cache(cache_t *cache, uint32_t index, line_t *line) {
  cache->accesses++;
  bool hit = false;
  for (int i = 0; i < NUM_OF_WAYS; i++) {
    if (cache->cache[index][i].value == line->value &&
        cache->cache[index][i].address == line->address) {
      cache->write_hits++;
      cache->cache[index][i].recency = iterator;
      cache->cache[index][i].freq++;
      hit = true;
      break;
    }
  }
  if (!hit) {
    cache->write_misses++;
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
  printf("Stats for %d\n", cache->policy);
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

  FILE *file = fopen("test_files/input/multiplication_input.txt", "r");
  int buffer = 0;
  char type = 'a';
  int val = 0;

  while (fscanf(file, "%x %c %d\n", &buffer, &type, &val) == 3) {
    line_t line = {buffer, val, 0, 0, 0};
    check_and_replace_cache(&fifo_cache, &line, type);
    check_and_replace_cache(&lru_cache, &line, type);
    check_and_replace_cache(&lfu_cache, &line, type);
    iterator++;
  }
  fclose(file);

  print_cache_stats(&fifo_cache);
  print_cache_stats(&lru_cache);
  print_cache_stats(&lfu_cache);

  return 0;
}
