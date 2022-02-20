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

SZ = size

###############################################################################
# Source directory
# Directory: src
SRC_DIR = src
# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

###############################################################################
# Header directory
# Directory: include
HDR_DIR = Inc
# Header files
HDR_FILES = $(wildcard $(HDR_DIR)/*.h)

C_INC = -I$(HDR_DIR)

TARGET = Synth

###############################################################################
# COMPILER
###############################################################################
CC = gcc

LD = ld

###############################################################################
# CFLAGS
###############################################################################
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O2 -g -D_POSIX_C_SOURCE=200809L
# CFLAGS += -MDD -MP -MF"$(@:%.o=.d)"
CFLAGS += $(C_INC)

###############################################################################
# LDFLAGS
LDFLAGS = -lm -lstdc++

###############################################################################
# Libraries
###############################################################################
LIBS = -lm -lpthread -lrt `allegro-config --libs`

###############################################################################
# OBJECTS
OBJECTS = $(addprefix $(BLD_DIR)/,$(notdir $(SRC_FILES:.c=.o)))

###############################################################################
# Build rules
###############################################################################
all: $(BLD_DIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): $(SRC_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

$(BLD_DIR):
	mkdir -p $(BLD_DIR)

clean:
	rm -rf $(BLD_DIR)
	rm -f $(TARGET)

# **EOF**