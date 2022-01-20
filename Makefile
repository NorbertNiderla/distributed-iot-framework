SRC_FOLDER = './src/'

all:
	g++ $(SRC_FOLDER)udp_client.cpp $(SRC_FOLDER)logger.cpp $(SRC_FOLDER)main.cpp -o app

.PHONY: clean
clean:
	rm -f *.o
