SPL_INCLUDE_DIR = $(realpath ./include)
SPL_LIB_DIR     = $(realpath ../../lib)
SPL_BIN_DIR     = $(realpath ../../bin)
SPL_SRC_DIR     = $(realpath ./src)

SPL_LIB         = $(SPL_LIB_DIR)/libspl.a

INCOMPLETE      = $(SPL_SRC_DIR)/spl_last.c $(SPL_SRC_DIR)/spl_ls.c

vpath %.c $(SPL_SRC_DIR)
vpath %.h $(SPL_INCLUDE_DIR)

SRCS       = $(wildcard $(SPL_SRC_DIR)/*.c)

ELFS       = $(filter-out $(patsubst $(SPL_SRC_DIR)/%.c,%,$(INCOMPLETE)),$(patsubst $(SPL_SRC_DIR)/%.c,%,$(SRCS)))
CC         = gcc-15
CFLAGS    += -Wall -g

ASMS       = $(patsubst $(SPL_SRC_DIR)/%.c,%.s,$(SRCS))
ASMFLAGS   = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions

# HACK: crack it by assembly
ADDERSS_SANITIZER_FALGS += -g3 -fno-omit-frame-pointer -fno-common -fsanitize=address

all: $(ELFS) 

asm: $(ASMS)

%.s:%.c
	$(CC) $(CFLAGS) $(ASMFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@

# 链接顺序：在 GCC 命令中，源文件（或 .o 文件）必须写在库参数（-l）的前面
# 如果顺序颠倒，链接器可能会因为还没遇到需要解析的符号而忽略库，导致链接失败
# $< is the source file. $@ is the target file.
%:%.c
	$(CC) $(CFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@
	# $(CC) $(CFLAGS) $(ADDERSS_SANITIZER_FALGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@-asan

clean:
	-rm -f $(SPL_BIN_DIR)/$(ELFS)

.PHONY: all clean asm
