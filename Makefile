SRC_FOLDER = ./src/
BUILD_FOLDER = ./build/
TARGETS = $(basename $(notdir $(wildcard $(SRC_FOLDER)*.cpp)))
BUILD = $(addsuffix .o, $(TARGETS))

all: $(BUILD)
	g++ $(addprefix $(BUILD_FOLDER), $(BUILD)) -o app -pthread

%.o: $(SRC_FOLDER)%.cpp
	g++ -c $^ -o $(addprefix $(BUILD_FOLDER), $@) -pthread

.PHONY: clean
clean:
	rm -f $(BUILD_FOLDER)*.o
