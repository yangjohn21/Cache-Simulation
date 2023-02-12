#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "cpu.h"

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf(
        "usage: cache-sim <set bits> <associativity> <block bits> "
        "<tracefile>\n");
    exit(1);
  }
  int sets = atoi(argv[1]);
  int lines = atoi(argv[2]);
  int bytes = atoi(argv[3]);
  char *file = argv[4];

  Cache *cache = make_cache(sets, lines, bytes);
  CPU *cpu = make_cpu(cache, file);

  run_cpu(cpu);

  delete_cpu(cpu);
  delete_cache(cache);
}
