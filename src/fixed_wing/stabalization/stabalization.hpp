/* Stabalization for fixed wing aircraft.
// Author: Sam Rosenstein
// Date: 5/16/17
//
//
*/
#ifndef STABALIZATION_H
#define STABALIZATION_H

#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"

#include "fixed_wing/stabalization/protos/parameters.pb.hpp"
#include "fixed_wing/stabalization/protos/aircraft_definition.pb.hpp"

#include "core/hecate/hecate.hpp"
#include "core/hecate/protos/imu.pb.hpp"


namespace fixed_wing {
  namespace stabalization {

    class Stabalization {
     public:
      Stabalization() {
        this->integral = 0;
        this->prev_error = 0;
        this->prev_input = 0;
        this->was_stopped = false;
      }

      int Stab(int setpoint, int input);

      void StopStab() {
        was_stopped = true;
      }

      fixedWing::stabalization::Parameters params;
     protected:

     private:
      int integral, prev_error;
      float prev_input;
      bool was_stopped;
    };

    class StabalizationThread : public cpp_freertos::Thread {
     public:
      // TODO(@smr277): Change this to a Timer
      StabalizationThread() : cpp_freertos::Thread("Stabalization", 1000, 1) {
        Start();
      }

      void StopStab();
      void ResumeStab();
      core::ReadWriterPreferWriterWrapper
        <fixedWing::stabalization::Parameters> roll_params;
      core::ReadWriterPreferWriterWrapper
        <fixedWing::stabalization::Parameters> pitch_params;
      core::ReadWriterPreferWriterWrapper
        <fixedWing::stabalization::Parameters> yaw_params;

      core::ReadWriterPreferWriterWrapper
        <fixedWing::aircraftDefinitions::Simple> aircraft_output;

      // Should these be mutex locked?
      int setpoint_roll;
      int setpoint_pitch;
      int setpoint_yaw;
     protected:
      virtual void Run();
     private:
      fixedWing::stabalization::Parameters local_params;
      fixedWing::aircraftDefinitions::Simple local_aircraft_output;
      core::sensors::ComputedState state;
      Stabalization stab_roll;
      Stabalization stab_pitch;
      Stabalization stab_yaw;
    };

  } // namespace fixed_wing
} // namespace stabalization

#endif // STABALIZATION_H
