SPL_INCLUDE_DIR = ../include
SPL_LIB_DIR     = ../lib
SPL_BIN_DIR     = ../bin
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
CFLAGS    += -Wall -g

ASMS       = $(patsubst %.c,%.s,$(SRCS))
ASMFLAGS   = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions

all: $(ELFS) 

asm: $(ASMS)

%.s:%.c
	$(CC) $(CFLAGS) $(ASMFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@

# 链接顺序：在 GCC 命令中，源文件（或 .o 文件）必须写在库参数（-l）的前面
# 如果顺序颠倒，链接器可能会因为还没遇到需要解析的符号而忽略库，导致链接失败
# $< is the source file. $@ is the target file.
%:%.c
	$(CC) $(CFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@

clean:
	-rm -f $(SPL_BIN_DIR)/$(ELFS)

.PHONY: all clean asm
