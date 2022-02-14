# Distributed IoT Framework

The repository is work in progress. Any feedback is appreciated. There is no uniform example of how it works at the moment. The elements of the framework can be seen using tests in `tests` directory. Goal is to create framework which can be used as foundation of distributed application without any centralised elements.

## Architecture

### Communication
Nodes are communicating through always open UDP sockets. Port 6300 is in use. Nodes see each others as IPv4 addresses.

### Protocol
Nodes communicate with each other using simple flag-based protocol, encapsulated in UDP packets.

## Implementation
Source code is stored in `src`. It contains logging class, UDP communication handler and simple message handler. Program is now based on UDP protocol with sending and non-blocking packet receiving capabilities on same thread. Message handler is programmed to read incoming packet and respond.

