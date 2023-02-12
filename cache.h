#ifndef __CACHE_H
#define __CACHE_H
#include "trace.h"

// Forward declaration of types:
typedef struct LRUNode LRUNode;
typedef struct Line Line;
typedef struct Set Set;
typedef struct Cache Cache;
typedef struct LRUResult LRUResult;

/*** General Cache Data Structures ***/

// This represents a result of accessing the cache.
// Was it a "hit" or a "miss".
enum AccessResult { HIT, COLD_MISS, CONFLICT_MISS };
typedef enum AccessResult AccessResult;

/*** LRU Data Structures ***/

// Represents a node in the LRU queue/stack linked list.
struct LRUNode {
  Line *line;
  LRUNode *next;
};

// Represents the result returned from accessing the LRU queue/stack.
struct LRUResult {
  Line *line;
  AccessResult access;
};

/*** Cache Data Structures ***/

// A Line represents a line in the cache.
struct Line {
  char valid;  // is the line valid? Treat this as a boolean value,
               // something "truth-y" is true, something "false-y" is false
  unsigned int tag;  // the tag
  char *accessed;    // the accessed bits
  int block_size;    // the number of bytes
};

// A Set represents a set in the cache.
struct Set {
  Line *lines;         // The lines in the set
  int line_count;      // The number of lines
  LRUNode *lru_queue;  // The LRU queue/stack associated with the set
};

// A Cache represents the state of the cache.
struct Cache {
  Set *sets;       // The sets in the cache
  int set_count;   // The number of sets
  int line_count;  // The number of lines
  int block_size;  // The number of bytes per block
  int set_bits;    // The number of bits used to index a set in the cache
  int block_bits;  // The number of bits used to index a byte in a block
};

Cache *make_cache(int set_count, int line_count, int block_size);
void delete_cache(Cache *cache);
int get_set(Cache *cache, address_type address);
int get_tag(Cache *cache, address_type address);
int get_byte(Cache *cache, address_type address);
AccessResult cache_access(Cache *cache, TraceLine *trace_line);

#endif
