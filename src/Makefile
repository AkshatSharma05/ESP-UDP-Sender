CFLAGS = -Wall -Wextra
BUILD = build

all: $(BUILD)/final

build:
	mkdir -p $(BUILD)

$(BUILD)/final: $(BUILD)/main.o $(BUILD)/sockets.o
# 	echo "Linking and finalizing"
	gcc $(CFLAGS) $(BUILD)/main.o $(BUILD)/sockets.o -o $(BUILD)/final
	chmod +x $(BUILD)/final
# 	./$(BUILD)/final

$(BUILD)/main.o: main.c
# 	echo "Compiling main.c"
	gcc $(CFLAGS) -c main.c -o $(BUILD)/main.o

$(BUILD)/sockets.o: sockets.c
# 	echo "Compiling sockets.c"
	gcc $(CFLAGS) -c sockets.c -o $(BUILD)/sockets.o

clean:
# 	echo "Removing build files..."
	rm -rf $(BUILD)/*