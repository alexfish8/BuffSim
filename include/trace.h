/*
 * trace.h
 *
 */

#ifndef _TRACE_H_
#define _TRACE_H_

#include <string>
#include <time.h>

class Trace {
public:
  struct timespec start; // system time of creation
  long inode_no; // i-node of file being read/written to
  long offset;   // offset in file
  int num_bytes; // number of bytes being read or written
  enum Type {READ, WRITE, END} type;

  Trace(std::string);

  // time elapsed since trace creation
  struct timespec age(); 
  
  // return the sum of t1 and t2
  static struct timespec timespec_add(struct timespec t1, struct timespec t2);

  // meta-trace that denotes the end of trace stream
  static Trace end();

  // return time elapsed between start and end
  static double elapsed(struct timespec start, struct timespec end);

private:
  Trace();
};


#endif

