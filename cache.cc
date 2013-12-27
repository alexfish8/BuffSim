/*
 * cache.cc
 */

#include <boost/thread.hpp>
#include <stdio.h>
#include "message_queue.h"
#include "trace.h"
#include "cache.h"

Cache::Cache(MQueue<Trace> *queue, int cache_size, int block_size) {
      this->queue = queue;
      this->cache_size = cache_size;
      this->block_size = block_size;
}

void Cache::simulate() {
  while (true) {
    boost::lock_guard<boost::mutex> lock(mut);
    Trace t = queue->dequeue();
    int bytes_remaining = t.num_bytes;
    int offset = t.offset; 
    while (bytes_remaining > 0) {
      int block = offset % block_size;
      printf("about to do cache request\n");
      do_cache_request(t.inode_no, block, t);
      printf("finished with cache request\n");
      offset += block_size;
      bytes_remaining -= block_size;
    }
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
  //boost::lock_guard<boost::mutex> lock(mut);
  return cache_size;
}



