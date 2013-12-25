/*
 * cache.cc
 */

#include <boost/thread.hpp>
#include "message_queue.h"
#include "trace.h"
#include "cache.h"

void Cache::simulate() {
  while (true) {
    boost::lock_guard<boost::mutex> lock(mut);
    Trace t = queue->dequeue();
    do_cache_request(t);
  }
}

int Cache::get_num_hits() {
  boost::lock_guard<boost::mutex> lock(mut);
  return num_hits;
}

int Cache::get_num_requests() {
  boost::lock_guard<boost::mutex> lock(mut);
  return num_requests;
}

int Cache::get_block_size() {
  return block_size;
}

int Cache::get_cache_size() {
  boost::lock_guard<boost::mutex> lock(mut);
  return cache_size;
}


