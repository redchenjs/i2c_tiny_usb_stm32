#
# Common make definition
#

# Compiler
CROSS_COMPILE = arm-none-eabi-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size
MKDIR = mkdir
SED = sed
CP = cp
RM = rm
PYTHON ?= python

check_defined = \
    $(strip $(foreach 1,$1, \
    $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
    $(error Undefined make flag: $1$(if $2, ($2))))

# Build directory
BUILD = build

# Compiler flags
CFLAGS += \
	-fsingle-precision-constant \
	-fno-strict-aliasing \
	-Wdouble-promotion \
	-Wno-endif-labels \
	-Wstrict-prototypes \
	-Wall \
	-Wextra \
	-Werror \
	-Werror-implicit-function-declaration \
	-Wfatal-errors \
	-Wfloat-equal \
	-Wundef \
	-Wshadow \
	-Wwrite-strings \
	-Wsign-compare \
	-Wmissing-format-attribute \
	-Wno-deprecated-declarations \
	-Wunreachable-code \
	-Wno-error=unused-function \
	-Wno-error=unused-parameter \
	-Wno-error=lto-type-mismatch \
	-ffunction-sections \
	-fdata-sections

# Debugging / Optimization
ifeq ($(DEBUG), 1)
    CFLAGS += -Og -ggdb
else
    CFLAGS += -Os
endif

# TUSB Logging option
ifneq ($(LOG),)
    CFLAGS += -DCFG_TUSB_DEBUG=$(LOG)
endif
