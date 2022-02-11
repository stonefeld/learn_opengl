CC = gcc
CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Isrc -Ilib/glad/include -Ilib/glfw/include -Ilib/stb_image/include -Ilib/cglm/include
LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a lib/stb_image/src/stb_image.o lib/cglm/libcglm.a -lm

ifeq ($(OS), Windows_NT)
	LDFLAGS += -mwindows -lpthread
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		LDFLAGS += -ldl -lpthread
	endif
	ifeq ($(UNAME_S), Darwin)
		LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
	endif
endif

OUT = build
BIN = learn_opengl
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,$(OUT)/%.o,$(SRC))

.PHONY: build run clean

build: dirs libs compile

dirs:
ifeq ($(OS), Windows_NT)
	if not exist $(OUT) md $(OUT)
else
	mkdir -p $(OUT)
endif

libs:
ifeq ($(OS), Windows_NT)
	cd lib\glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib\glfw && cmake -G "MinGW Makefiles" . -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF && make -s
	cd lib\stb_image && $(CC) -o src/stb_image.o -Iinclude -c src/stb_image.c
	cd lib\cglm && cmake -G "MinGW Makefiles" . -DCGLM_STATIC=ON && make -s
else
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF && make -s
	cd lib/stb_image && $(CC) -o src/stb_image.o -Iinclude -c src/stb_image.c
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make -s
endif

compile: $(OBJ)
	$(CC) -o $(OUT)/$(BIN) $^ $(LDFLAGS)

$(OUT)/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run: build
ifeq ($(OS), Windows_NT)
	$(OUT)\$(BIN)
else
	$(OUT)/$(BIN)
endif

clean:
ifeq ($(OS), Windows_NT)
	rmdir /s /q $(OUT)
	cd lib\glad && del src\glad.o
	cd lib\glfw && cmake --build . --target clean
	cd lib\stb_image && del src\stb_image.o
	cd lib\cglm && cmake --build . --target clean
else
	rm -rf $(OUT)
	cd lib/glad && rm -f src/glad.o
	cd lib/glfw && cmake --build . --target clean
	cd lib/stb_image && rm -f src/stb_image.o
	cd lib/cglm && cmake --build . --target clean
endif
