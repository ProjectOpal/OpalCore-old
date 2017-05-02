/*
// Conglomerated, centralized objects to share state and sensor data
//
// Author: Samuel Rosenstein, samr37l@gmail.com
// Date: 4/25/2017
//
// Hecate is a centralized location for the storage of objects that will be
// accessible by any system. Each object has a Read/Write lock with
// writer preffered. For access to objects, just include this file.
*/

#include "hecate.hpp"

using namespace core;

template <class T>
bool ReadWriterPreferWriterWrapper<T>::Read(T *return_t) {
  Lock->ReaderLock();
  T(this->box);
  Lock->ReaderUnlock();
  return true;
}


template <class T>
bool ReadWriterPreferWriterWrapper<T>::Write(const T &box_orig) {
  Lock->WriterLock();
  this->box(box_orig);
  Lock->WriterUnlock();
  return true;
}