/*
 * message_queue.h
 *
 */

#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include <queue>
#include <boost/thread.hpp>

template <class T>
class MQueue {
  private:
    std::queue<T> queue;
    bool empty;
    boost::mutex mut;
    boost::condition_variable cv;

  public:

    void enqueue(T elem) {
      boost::lock_guard<boost::mutex> lock(mut);
      queue.push(elem);
      cv.notify_one();
    }

    T dequeue() {
      boost::unique_lock<boost::mutex> lock(mut);
      while(queue.empty()) {
        cv.wait(lock);
      }
      T ret = queue.front();
      queue.pop();
      return ret;
    }

};


#endif



