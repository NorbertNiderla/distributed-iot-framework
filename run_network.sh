make
docker build . -t iot_app:latest --no-cache
docker-compose -f docker-compose.yaml up