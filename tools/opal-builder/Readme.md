# Opal Builder
Opal builder is the script to setup an Opal Pilot!
### How to use
`python3 opal-builder.py <root-autopilot-durectory>`
or, from the root of your autopilot directory, run `.\opal-builder`. 
### What Opal Builder does
Opal builder performs many items, but overall it helps a developer and users configure thier autopilots. More techinically:

- Scans to find all local modules
- Finds all proto inputs and outputs for modules
- Let's the user configure which modules to compile into the opal pilot
- Automatically "connects" the modules using the proto information. Provides the user a graph showing the connections, and allows for user reconfiguration of the connections if possible.
- Automatically creates the `main.cpp` file
- Automatically creates the main `CMakeLists.txt` file for cached compiling

### GUI
The only GUI currently is the graph presentation like the following: ![](OpalCore/tools/opal-builder/resources/example_graph.jpg)