/* Stabalization for fixed wing aircraft.
// Author: Sam Rosenstein
// Date: 5/16/17
//
//
*/
#include "fixed_wing/stabalization/stabalization.hpp"


using namespace core;
using namespace core::sensors;
using namespace fixed_wing::stabalization;
using namespace fixedWing;


int Stabalization::Stab(const int setpoint, const int input) {
  if (this->was_stopped) {
    this->prev_input = input;
    this->integral = 0;
    this->was_stopped = false;
  }
  int error = setpoint - input;
  int der_input = input - prev_input;
  this->integral += this->params.ki * error;
  if (this->integral > params.max) this->integral = this->params.max;
  int output = this->params.kp * error + this->integral
    - this->params.kd * der_input;
  if (output > this->params.max) output = this->params.max;
  this->prev_error = error;
  this->prev_input = input;
  return output;
}

void StabalizationThread::Run() {
  while(1) {
    roll_params.Read(&(this->stab_roll.params));
    pitch_params.Read(&(this->stab_pitch.params));
    yaw_params.Read(&(this->stab_yaw.params));
    this->local_aircraft_output.aileron =
      stab_roll.Stab(this->setpoint_roll, Hecate::comp_state->roll);
    this->local_aircraft_output.elevator =
      stab_pitch.Stab(this->setpoint_pitch, Hecate::comp_state->pitch);
    this->local_aircraft_output.rudder = stab_yaw.Stab(this->setpoint_yaw,
                                                       Hecate::comp_state->yaw);
    this->aircraft_output.Write(this->local_aircraft_output);
  }
}
