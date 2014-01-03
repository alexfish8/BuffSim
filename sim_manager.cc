/*
 * sim_manager.cc
 */

#include <time.h>
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include <stdio.h>

#include "trace.h"
#include "cache.h"
#include "lru.h"
#include "message_queue.h"

using namespace std;

static const int num_args = 0;
static const int default_num_sims = 4;
static const int default_cache_size = 1024;
static const int default_block_size = 4096;
static const int max_sims = 8;

int main(int argc, char *argv[]) {
  string buffer;
  vector<MQueue<Trace>*> mqueues;
  vector<LRU*> sims;
  vector<boost::thread*> workers;
  vector<struct timespec> start_times;

  if (argc < num_args) {
    printf("Usage: sim_manager [num_threads] [cache_size] [block_size]\n");
    exit(EXIT_FAILURE);
  }

  int num_sims = default_num_sims;
  int cache_size = default_cache_size;
  int block_size = default_block_size;

  if (argc > 1)
    num_sims = atoi(argv[1]) < max_sims ? atoi(argv[1]) : max_sims;
  if (argc > 2)
    cache_size = atoi(argv[2]);
	if (argc > 3)
    block_size = atoi(argv[3]);


  for (int i = 0; i < num_sims; i++) {
    MQueue<Trace> *mqueue = new MQueue<Trace>();
    mqueues.push_back(mqueue);
    LRU *lru = new LRU(mqueue, cache_size, block_size);
    sims.push_back(lru);
    workers.push_back(new boost::thread(boost::bind(&Cache::simulate, lru)));

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    start_times.push_back(start);
  }

  int j = 0;
  while (getline(cin, buffer)) {
    Trace t(buffer);
    for (int i = 0; i < num_sims; i++) {
      mqueues[(i + j) % num_sims]->enqueue(t);
    }
    j++;
  }


  for (int i = 0; i < num_sims; i++) {
    mqueues[i]->enqueue(Trace::end());
  }

  for (int i = 0; i < num_sims; i++) {
    printf("\n");
    printf("Thread %d\n", i);
    printf("----------\n");
    struct timespec end;
    workers[i]->join();
    clock_gettime(CLOCK_MONOTONIC, &end);
  
    // simulation results
    printf("cache hits: %d\n", sims[i]->get_num_hits());
    printf("cache requests: %d\n", sims[i]->get_num_requests());

    // performance information
    int num_traces = sims[i]->get_num_traces();
    double elapsed = Trace::elapsed(start_times[i], end);
    printf("total latency: %10.9f sec\n", elapsed); 
    printf("average latency per trace: %10.9f sec\n", elapsed / num_traces);
  }
}

