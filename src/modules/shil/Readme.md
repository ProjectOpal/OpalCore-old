#SHIL - Software/Hardware In the Loop

SHIL provides a connection to simulators in order to simulate an aircraft flying. It currently only supports SIL, Software In the Loop, there the entire autopilot is simulated on a Linux machine with the aircraft simulated within FlightGear.

##Setup Steps
Note: This currently only runs on Linux, and has only been tested on:

* Ubuntu 14.04
* Ubuntu 16.04

1. Download [FlightGear](http://www.flightgear.org/download/)
2. Build the autopilot to SIL (instructions TBD)  
3. Startup FlightGear as follows:
`fgfs --generic=socket,out,50,localhost,6789,udp,fg_out`

##Debugging help
- To view the FlightGear property tree, you can startup FlightGear with it's http server by: `fgfs --httpd=<port>`
- To view the packets coming from FlightGear you can use netcat: `nc -ul <port>`