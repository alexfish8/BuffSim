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
int num_sims = 4;
int cache_size = 1024;
int block_size = 4096;

int main() {
  string buffer;
  vector<MQueue<Trace>*> mqueues;
  vector<LRU*> sims;
  vector<boost::thread*> workers;

  for (int i = 0; i < num_sims; i++) {
    MQueue<Trace> *mqueue = new MQueue<Trace>();
    mqueues.push_back(mqueue);
    LRU *lru = new LRU(mqueue, cache_size, block_size);
    sims.push_back(lru);
//    boost::thread t (boost::bind(&Cache::simulate, lru));
    workers.push_back(new boost::thread(boost::bind(&Cache::simulate, lru)));
  }

  int j = 0;
  while (getline(cin, buffer)) {
    Trace t(buffer);
    for (int i = 0; i < num_sims; i++) {
      mqueues[(i + j) % num_sims]->enqueue(t);
    }

    //printf("type: <%d>, inode: <%d>, offset: <%d>, num_bytes: <%d>\n", t.type, t.inode_no, t.offset, t.num_bytes);

    j++;
  }


  for (int i = 0; i < num_sims; i++) {
    mqueues[i]->enqueue(Trace::end());
  }

  for (int i = 0; i < num_sims; i++) {
    printf("waiting for thread %d\n", i);
    workers[i]->join();
  

    printf("cache hits: %d\n", sims[i]->get_num_hits());
    printf("cache requests: %d\n", sims[i]->get_num_requests());

    struct timespec total_latency = sims[i]->get_total_latency();
    int num_traces = sims[i]->get_num_traces();
    double nsec = (double) total_latency.tv_nsec / 1000000000L;
    double time = total_latency.tv_sec + nsec;
    printf("total latency: %f sec\n", time); 
    printf("average latency per trace: %f sec\n", time / num_traces);

  }

}
