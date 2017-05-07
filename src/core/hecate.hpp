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

#include "imu.pb.hpp"
#include "core.pb.hpp"
#include "read_write_lock.hpp"

namespace core {

  /*
  // Wrap a class in a ReaderWriterPreferWriter lock.
  */
  template <class T>
  class ReadWriterPreferWriterWrapper{
   public:
    ReadWriterPreferWriterWrapper() try : Lock(new cpp_freertos::ReadWriteLockPreferWriter()),
                                          box(new T())
    {}
    catch(cpp_freertos::ReadWriteLockCreateException &ex) {
      // cout << "Caught ReadWriteLockCreateException" << endl;
      // cout << ex.what() << endl;
      configASSERT(!"ReadWriteLock creation failed!");
    };

    // TODO(@smr277): Find a way to read without copying memory
    //                maybe return a const &, but need to find a simple
    //                way to grab and release the mutex
    bool Read(T *return_t) {
      Lock->ReaderLock();
      memcpy(return_t, box, sizeof(T));
      Lock->ReaderUnlock();
      return true;
    }
    bool Write(const T &box_orig) {
      Lock->WriterLock();
      memcpy(box, &box_orig, sizeof(T));
      Lock->WriterUnlock();
      return true;
    }
   private:
    cpp_freertos::ReadWriteLockPreferWriter *Lock;
    T *box;
  };

  /*
  // Hectate is a static class that contains global information.
  // Information is accessed as writer-prefered.
  */
  class Hecate {
   public:
    /* Each proto message is wrapper in a ReaderWriterPreferWriter lock.
    // The definitions for this class is above.
    // Example to read the gyro data:
    //  core::sensors::Gyro my_gyro;
    //  Hectate::gyro.Read(*my_gyro);
    //
    // Example to write gyro data:
    //  core::sensors::Gyro my_gyro;
    //  my_gyro.x = 20;
    //  Hectate:gyro.Write(my_gyro)
    */
    static ReadWriterPreferWriterWrapper<core::sensors::Gyro> gyro;
    static ReadWriterPreferWriterWrapper<core::sensors::Accel> accel;
    static ReadWriterPreferWriterWrapper<core::sensors::Location> location;
    static ReadWriterPreferWriterWrapper<core::sensors::ComputedState>
        comp_state;
    static ReadWriterPreferWriterWrapper<core::sensors::AirSpeed>
        airspeed;

   private:
    Hecate();
  };

}
