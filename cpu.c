#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

int read_address(CPU *cpu, TraceLine *trace_line) {
  return fscanf(cpu->address_trace, "%c %x,%c\n", &trace_line->operation,
                &trace_line->address, &trace_line->size);
}

CPU *make_cpu(Cache *cache, const char *address_trace_file) {
  CPU *cpu = (CPU *)(malloc(sizeof(CPU)));
  cpu->cache = cache;
  cpu->address_count = 0;
  cpu->hits = 0;
  cpu->cold = 0;
  cpu->conflict = 0;
  cpu->address_trace = fopen(address_trace_file, "r");
  return cpu;
}

void delete_cpu(CPU *cpu) { free(cpu); }

void run_cpu(CPU *cpu) {
  TraceLine trace_line;
  while (read_address(cpu, &trace_line) != EOF) {
    cpu->address_count++;
    AccessResult result = cache_access(cpu->cache, &trace_line);
    if (result == HIT) {
      cpu->hits++;
    } else if (result == COLD_MISS) {
      cpu->cold++;
    } else {
      cpu->conflict++;
    }
  }

  int miss = cpu->cold + cpu->conflict;
  float hit_rate = ((float)(cpu->hits)) / ((float)(cpu->hits + miss));
  float miss_rate = 1.0f - hit_rate;

  printf("hits: %d misses: %d evictions: %d hrate: %f mrate: %f\n", cpu->hits,
         cpu->cold + cpu->conflict, cpu->conflict, hit_rate, miss_rate);
}
