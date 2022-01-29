cmake --build ./build/
retVal=$?
if [ $retVal -ne 0 ]; then
    echo "Error"
    exit $retVal
fi
docker build . -t iot_app:latest
docker-compose -f docker-compose.yaml up