SPL_INCLUDE_DIR = $(realpath ../include)
SPL_LIB_DIR     = $(realpath ../lib)
SPL_BIN_DIR     = $(realpath ../bin)
CURRENT_DIR     = $(realpath .)

SPL_LIB         = $(SPL_LIB_DIR)/libspl.a
SPL_HDRS        = \
common_hdrs.h\
dir_utils.h\
error_exits.h\
escapes.h\
get_nums.h\
hash.h\
ps_utils.h\
show_time.h\
sys_hdrs.h\
time_utils.h

SRCS       = $(wildcard *.c)
ELFS       = $(patsubst %.c,%,$(SRCS))
CC         = gcc-15
CFLAGS    += -Wall -g

ASMS       = $(patsubst %.c,%.s,$(SRCS))
ASMFLAGS   = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions

all: $(ELFS) 

asm: $(ASMS)

%.s:$(CURRENT_DIR)/%.c
	$(CC) $(CFLAGS) $(ASMFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@

%:$(CURRENT_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@

clean:
	-rm -f $(SPL_BIN_DIR)/$(ELFS)

.PHONY: all clean asm
