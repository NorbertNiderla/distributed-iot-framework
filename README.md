# IoT Communication App

## Features
* Communication App Sources
* Multi-app Contenerized Model

### Communication App
Source code is stored in `src`. It contains logging class, UDP communication handler and simple message handler. Program is now based on UDP protocol with sending and non-blocking packet receiving capabilities on same thread. Message handler is programmed to respond to incoming packets, and can send packets by itself.

### Multi-app Contenerized Model
Example with two apps working in Docker environment simultaneously is  stored in `example/` directory. Program is compiled on host machine, in next step Docker Image with Communication App is built with `Dockerfile`. With `docker-compose.yaml` network is generated where every node is started with image from previous step.

In order to run it, Cmake build files must be generated:
```bash
cd example/
mkdir build
cd build/
cmake ../
cd ../
```

#### Automation
Script `run_network.sh` can be run in order to build program and set up network in Docker environment.