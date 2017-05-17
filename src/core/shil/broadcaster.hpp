/* Broadcasts serialized protos over TCP
// Author: Sam Rosenstein (samr37l@gmail.com)
// Date: 5/17/17
*/

#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <chrono>

#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"

#include "core/shil/PracticalSocket.h"

namespace shil {
  class Broadcaster : public cpp_freertos::Thread {
   public:
    // TODO(@smr277): Change this to a Timer
    Broadcaster(unsigned short port) :
      cpp_freertos::Thread("Broadcaster", 1000, 1),
      servSock(new TCPServerSocket(port))
    {
      Start();
    };
   protected:
    virtual void Run();
   private:
    std::unique_ptr<TCPServerSocket> servSock;
    void HandleTCPClient(TCPSocket *sock);
  };

} // namespace shil

#endif // BROADCASTER_H
