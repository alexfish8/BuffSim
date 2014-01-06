#ifndef _LRU_H_
#define _LRU_H_

#include <map>
#include <list>
#include <string>
#include "message_queue.h"
#include "trace.h"
#include "cache.h"

class LRU : public Cache {

  private:
    typedef std::pair<int, int>  elem;
    typedef std::list<elem> cache_t;
    typedef cache_t::iterator cache_elem;
    typedef std::map<elem, cache_elem> cache_map;

    cache_t cache;
    cache_map in_cache;

  public:
    LRU(MQueue<Trace>* queue, int max_cache_size, int block_size);
    void do_cache_request(int inode, int block, Trace t);
    std::string name();
};



#endif



