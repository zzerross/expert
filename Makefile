CFLAGS := -Wall
CFLAGS += -D_pl_=3
TARGET := user
IN := in0

$(TARGET): FORCE
	g++ $(CFLAGS) -o $@ main.cpp $(TARGET).cpp
	./$(TARGET) < $(IN) 2>&1 | tee $(TARGET).txt

.PHONY: FORCE
