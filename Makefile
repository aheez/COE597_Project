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
CFLAGS = -Wall -g3
CFLAGS += $(C_INC)

###############################################################################
# LDFLAGS
LDFLAGS = -s -lc 

###############################################################################
# Libraries
###############################################################################
LIBS = -lm `pkg-config --libs allegro-5 allegro_audio-5 allegro_font-5 allegro_primitives-5`

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
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(BLD_DIR):
	mkdir -p $@

run:
	./$(TARGET)

debug:
	gdb ./$(TARGET)

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