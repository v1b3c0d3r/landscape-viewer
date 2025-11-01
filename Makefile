CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -g -Iinclude -Iinclude/cglm/include
LDFLAGS=-lSDL2 -lGL -lGLEW -lm
SRCDIR=src
BUILDDIR=build
TARGET=$(BUILDDIR)/landscape

SRC_FILES=$(wildcard $(SRCDIR)/*.c)
CGLM_SRC_FILES=$(wildcard include/cglm/src/*.c)
OBJ_FILES=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC_FILES))
CGLM_OBJ_FILES=$(patsubst include/cglm/src/%.c,$(BUILDDIR)/cglm/%.o,$(CGLM_SRC_FILES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES) $(CGLM_OBJ_FILES)
	$(CC) $(OBJ_FILES) $(CGLM_OBJ_FILES) -o $(TARGET) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/cglm/%.o: include/cglm/src/%.c
	@mkdir -p $(BUILDDIR)/cglm
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)
