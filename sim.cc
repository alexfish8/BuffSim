/*
 * sim.cc
 *
 * A single-threaded buffer cache simulator
 */


#include <string>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include "trace.h"
#include "cache.h"
#include "lru.h"

int cache_size = 1024;
int block_size = 4096;

using namespace std;

int main() {
  LRU sim(NULL, cache_size, block_size);
  struct timespec total_latency;
  total_latency.tv_sec = 0;
  total_latency.tv_nsec = 0;

  int num_traces = 0;
	string buffer;
  while (getline(cin, buffer)) {
    Trace t(buffer);

    int bytes_remaining = t.num_bytes;
    int offset = t.offset; 
    while (bytes_remaining > 0) {
      int block = offset % block_size;
      sim.do_cache_request(t.inode_no, block, t);
      offset += block_size;
      bytes_remaining -= block_size;
    }

    total_latency = Trace::timespec_add(total_latency, t.age());
    num_traces++;
  }

  double nsec = (double) total_latency.tv_nsec / 1000000000L;
  double time = total_latency.tv_sec + nsec;
  printf("total latency: %10.9f sec\n", time); 
  printf("average latency per trace: %10.9f sec\n", time / num_traces);
}


