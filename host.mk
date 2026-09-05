##########################################################################################################################
# Cross-Platform Host Makefile (Windows / macOS / Linux)
# Swapped Platform/STM to Platform/Host and stripped MCU-specific components
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on host gcc/clang)
# ------------------------------------------------

# Detect the operating system
OS := $(shell uname -s 2>/dev/null || echo Windows_NT)

######################################
# target
######################################
NAME = sphinx-fc-firm-v2

# OS-specific target extensions and commands
ifeq ($(findstring Windows_NT,$(OS)),Windows_NT)
    TARGET  := $(NAME).exe
    RM      := del /Q /S
    MKDIR   := mkdir
    FIX_SL  = $(subst /,\,$1)
else
    TARGET  := $(NAME)
    RM      := rm -rf
    MKDIR   := mkdir -p
    FIX_SL  = $1
endif

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

######################################
# paths
######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES = 

# Wildcard inclusions
C_SOURCES += $(wildcard Shared/**/*.c)
C_SOURCES += $(wildcard App/**/*.c)
C_SOURCES += $(wildcard App/Algorithm/**/*.c)
C_SOURCES += $(wildcard App/Algorithm/*.c)

# Platform/Host logic
C_SOURCES += $(wildcard Platform/Host/**/*.c)
C_SOURCES += $(wildcard Platform/Host/*.c)

print-%:
	@echo '$*=$($*)'

ASM_SOURCES = 
ASMM_SOURCES = 

#######################################
# binaries
#######################################
# Use standard system gcc/clang
CC = gcc
SZ = size

#######################################
# CFLAGS
#######################################
# Macros for host gcc
C_DEFS = \
-DSIM

# C includes (Targeting Platform/Host instead of Platform/STM)
C_INCLUDES = \
-IShared/Algorithm \
-IShared/IO \
-IShared/State \
-IApp/Algorithm/altimeter \
-IApp/config \
-IApp/data \
-IPlatform/Host \
-IPlatform/Host/State

# Compile gcc flags
CFLAGS += $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
LIBS = -lm
LIBDIR = 
LDFLAGS = $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(NAME).map,--cref

# Omit --gc-sections on macOS since the Apple linker handles dead code stripping differently
ifneq ($(OS),Darwin)
    LDFLAGS += -Wl,--gc-sections
endif

# default action: build the application
all: $(BUILD_DIR)/$(TARGET)

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR):
	@$(MKDIR) $(call FIX_SL,$(BUILD_DIR))

#######################################
# clean up
#######################################
clean:
	@$(RM) $(call FIX_SL,$(BUILD_DIR))
	
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
