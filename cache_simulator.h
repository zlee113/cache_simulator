/**
 * @file cache_simulator.h
 * @brief Header file for the cache simulator containing the functions and
 * structs needed to simulate
 */

#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

/* Struct for holding information for each way */
typedef struct {
  int address;   /* Address for pointer of data */
  int value;     /* Value of data */
  int freq;      /* Read frequency since added to cache */
  int recency;   /* Last read */
  int timestamp; /* Timestamp when first added */
} line_t;

/* Enum to define which replacement policy is used for cache */
typedef enum { FIFO, LRU, LFU, LRFU, LFU_DA } replacement_policy_e;

/* Struct holding all the information of the cache */
typedef struct {
  int accesses;     /* Total number of cache accesses */
  int read_hits;    /* Total number of read hits */
  int read_misses;  /* Total number of read misses */
  int write_hits;   /* Total number of write hits */
  int write_misses; /* Total number of write misses */
  replacement_policy_e policy; /* Replacement policy for cache */
} cache_t;


#endif /* CACHE_SIMULATOR_H */
