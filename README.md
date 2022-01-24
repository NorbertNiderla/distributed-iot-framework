# IoT Communication App

## Features
* Communication App
* Multi-app Contenerized Model

### Communication App
Source code is stored in `src`. Program can be built by `make` command in main directory. Program is based on UDP communication handler with sending and non-blocking receiving packets capabilities on same thread. Application is programmed to respond to incoming packets, and can send packets by itself.

### Multi-app Contenerized Model
Program is compiled on host machine, in next step Docker Image of Ubuntu with Communication App is built with `Dockerfile`. With `docker-compose.yaml` network is generated where every node is started with image from previous step.

### Automation
Script `run_network.sh` should be run in order to build program and set up network in Docker environment.