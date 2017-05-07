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

ReadWriterPreferWriterWrapper<sensors::Gyro> Hecate::gyro;
ReadWriterPreferWriterWrapper<sensors::Accel> Hecate::accel;
ReadWriterPreferWriterWrapper<sensors::Location> Hecate::location;
ReadWriterPreferWriterWrapper<sensors::ComputedState> Hecate::comp_state;
ReadWriterPreferWriterWrapper<sensors::AirSpeed> Hecate::airspeed;
