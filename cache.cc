/*
 * cache.cc
 */

#include <stdio.h>
#include <time.h>
#include <boost/thread.hpp>
#include "message_queue.h"
#include "trace.h"
#include "cache.h"

Cache::Cache(MQueue<Trace> *queue, int cache_size, int block_size) {
      this->queue = queue;
      this->cache_size = cache_size;
      this->block_size = block_size;
			total_latency.tv_sec = 0;
			total_latency.tv_nsec = 0;
}

void Cache::simulate() {
  while (true) {
    Trace t = queue->dequeue();
    if (t.type == Trace::END) {
      break;
    }

    int bytes_remaining = t.num_bytes;
    int offset = t.offset; 
    while (bytes_remaining > 0) {
      int block = offset % block_size;
      do_cache_request(t.inode_no, block, t);
      offset += block_size;
      bytes_remaining -= block_size;
    }

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);

    total_latency = Trace::timespec_add(total_latency, t.age());
    num_traces++;
  }
}

int Cache::get_num_hits() {
  return num_hits;
}

int Cache::get_num_requests() {
  return num_requests;
}

int Cache::set_num_hits(int x) {
  num_hits = x;
}

int Cache::set_num_requests(int x) {
  num_requests = x;
}


int Cache::get_block_size() {
  return block_size;
}

int Cache::get_cache_size() {
  return cache_size;
}

int Cache::get_num_traces() {
  return num_traces;
}

struct timespec Cache::get_total_latency() {
  return total_latency;
}



