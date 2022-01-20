FROM ubuntu
COPY ./app /home/
WORKDIR /home/
ENTRYPOINT [ "./app"]
