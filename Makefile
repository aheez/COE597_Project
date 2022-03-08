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
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O2 -g 
# CFLAGS += -MDD -MP -MF"$(@:%.o=.d)"
CFLAGS += $(C_INC)

###############################################################################
# LDFLAGS
LDFLAGS = -s -lc

###############################################################################
# Libraries
###############################################################################
LIBS = -lm -lpthread -lrt `allegro-config --libs`

###############################################################################
# OBJECTS
OBJECTS = $(addprefix $(BLD_DIR)/,$(notdir $(SRC_FILES:.c=.o)))
vpath %.c $(sort $(SRC_DIR))

###############################################################################
# Build rules
###############################################################################
all: $(BLD_DIR) $(TARGET) 

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

$(BLD_DIR)/%.o: %.c Makefile | $(BLD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJECTS):$(SRC_FILES)
# 	$(CC) -c $(CFLAGS) -W $< -o $@

$(BLD_DIR):
	mkdir -p $@

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