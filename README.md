# Introduction
The goal of this project is to propose a solution to the C++ test I was asking the candidates to write. Once the code skeletton started, I wanted to develop more and more some components, such as the data manager.
 
## Requirements

The system represents a V2X (Vehicle to Everything) end-point. It is embedded in a car and receives data from other cars through V2X messages.
- TCP Server : Manage the connections with the remote TCP Clients and handle data
- Data Manager: Multi-feeder / multi-consumer message queuing / dispatching template class
- Calculator: 

## Architecture

### Proxy 

### Data Manager

# Build
```
mkdir build && cd build
cmake ..
make
```
