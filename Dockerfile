FROM ubuntu
COPY ./build/iot-communication-app /home/
WORKDIR /home/
ENTRYPOINT [ "./iot-communication-app"]
