#ifndef _SIMULATION_H_
#define _SIMULATION_H_


class Simulation {
  public:
    virtual ~Simulation();

  
    virtual void reqeust(int) = 0;
    virtual int num_hits();
    virtual int num_requests();

};

#endif 


