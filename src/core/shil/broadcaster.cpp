/* Broadcasts serialized protos over TCP
// Author: Sam Rosenstein (samr37l@gmail.com)
// Date: 5/17/17
*/

#include "core/shil/broadcaster.hpp"
#include <cstring>


using namespace shil;
using namespace std;

void Broadcaster::Run() {
  while(1) {
    try {
      this->HandleTCPClient(this->servSock->accept());
    } catch (SocketException &e) {
    }
  }
}

void Broadcaster::HandleTCPClient(TCPSocket *sock) {
  char echoBuffer[20];
  string tmp = "hello world!";
  strcpy(echoBuffer, tmp.c_str());
  sock->send(echoBuffer, tmp.length());
}
