###############################################################################
# Name of the project Synth
# File: Makefile
# Purpose: Makefile for the project Synth
# Author: Ahmad Almofeez
# Date: 04/02/2022
# Version: 0.1
###############################################################################
# Build directory
# Directory: build
BLD_DIR = build

###############################################################################
# Source directory
# Directory: src
SRC_DIR = Src
# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

###############################################################################
# Header directory
# Directory: include
HDR_DIR = Inc
# Header files
HDR_FILES = $(wildcard $(HDR_DIR)/*.h)

C_INC =	-I$(HDR_DIR) \
		-I/usr/include/allegro5

TARGET = Synth

###############################################################################
# COMPILER
###############################################################################
CC = gcc

###############################################################################
# CFLAGS
###############################################################################
CFLAGS = -Wall -std=c11 -pedantic -O2 -g
CFLAGS += $(C_INC)

###############################################################################
# LDFLAGS
LDFLAGS = -s -lc 

###############################################################################
# Libraries
###############################################################################
LIBS = `pkg-config --libs allegro-5`

###############################################################################
# OBJECTS
OBJECTS = $(addprefix $(BLD_DIR)/,$(notdir $(SRC_FILES:.c=.o)))
vpath %.c $(sort $(SRC_DIR))

###############################################################################
# Build rules
###############################################################################
all: $(BLD_DIR) $(TARGET) 

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

$(BLD_DIR)/%.o: %.c Makefile | $(BLD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BLD_DIR):
	mkdir -p $@

run:
	./$(TARGET)

check:
	@echo "Checking..."
	@echo $(SRC_FILES)

check-all:
	@echo "Checking..."
	@echo $(SRC_FILES)
	@echo $(HDR_FILES)
	@echo $(OBJECTS)
clean:
	rm -rf $(BLD_DIR)
	rm -f $(TARGET)

# **EOF**