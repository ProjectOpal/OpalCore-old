/*
// Stabalization algorithm for fixed wing aircraft.
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 5/7/2017
//
*/

#include "task.h"
#include "hecate.hpp"
#include "timer.hpp"

class Stabalization : public Timer {
 public:
  Stabalization {
    Start();
  }
 protected:
  virtual void Run();
 private:
}
