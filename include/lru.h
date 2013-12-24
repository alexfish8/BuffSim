#ifndef _LRU_H_
#define _LRU_H_

#include <map>
#include <list>


class LRU {

  public:
    LRU(int);
    ~LRU();

    void cache_access(int);
    int get_num_hits();
    int get_num_requests();
    int get_max_size();

  private:
    int num_hits;
    int num_requests;
    int max_size;

    typedef std::list<int> Cache;
    typedef std::map<int, Cache::iterator> CacheMap;

    Cache *cache;
    CacheMap *block_positions;
};

#endif



