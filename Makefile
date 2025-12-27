CFLAGS = -Wall -Wextra
BUILD = build

all: $(BUILD)/final

build:
	mkdir -p $(BUILD)

build/final: $(BUILD)/main.o 
# 	echo "Linking and finalizing"
	gcc $(CFLAGS) $(BUILD)/main.o -o $(BUILD)/final
	chmod +x $(BUILD)/final
# 	./$(BUILD)/final

$(BUILD)/main.o: main.c
# 	echo "Compiling main.c"
	gcc $(CFLAGS) -c main.c -o $(BUILD)/main.o

clean:
# 	echo "Removing build files..."
	rm -rf $(BUILD)/*