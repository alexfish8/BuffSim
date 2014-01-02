/*
 * cache.h
 */

#ifndef _CACHE_H__
#define _CACHE_H__

#include <boost/thread.hpp>
#include <time.h>
#include "message_queue.h"
#include "trace.h"

class Cache {
  public:
    Cache(MQueue<Trace>*, int, int);

    virtual void do_cache_request(int inode, int block, Trace t) = 0;
    void simulate();
    int get_cache_size();
    int get_block_size();
    int get_num_hits();
    int get_num_requests();
    int set_num_hits(int);
    int set_num_requests(int);
    int get_num_traces();
    struct timespec get_total_latency();

  protected:
    MQueue<Trace> *queue;
    int cache_size; // maximum number of blocks
    int block_size; // block size in bytes
    int num_hits;
    int num_requests;
    int num_traces;
    struct timespec total_latency;
};

#endif 

