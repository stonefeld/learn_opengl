CC = gcc
CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Isrc -Ilib/glad/include -Ilib/glfw/include -Ilib/stb_image/include -Ilib/cglm/include
LDFLAGS = -lm -ldl -lpthread
LDFLAGS += lib/glad/src/glad.o lib/glfw/src/libglfw3.a lib/stb_image/src/stb_image.o lib/cglm/libcglm.a

OUT = build
BIN = learn_opengl
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,$(OUT)/%.o,$(SRC))

.PHONY: build run clean

build: dirs libs compile

dirs:
	mkdir -p $(OUT)

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make -s
	cd lib/stb_image && $(CC) -o src/stb_image.o -Iinclude -c src/stb_image.c
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make -s

compile: $(OBJ)
	$(CC) -o $(OUT)/$(BIN) $^ $(LDFLAGS)

$(OUT)/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run: build
	$(OUT)/$(BIN)

clean:
	rm -rf $(OUT)
	cd lib/glad && rm -f src/glad.o
	cd lib/glfw && cmake --build . --target clean
	cd lib/stb_image && rm -f src/stb_image.o
	cd lib/cglm && cmake --build . --target clean
