SPL_INCLUDE_DIR = $(realpath ./include)
SPL_LIB_DIR     = $(realpath ../../lib)
SPL_BIN_DIR     = $(realpath ../../bin)
SPL_SRC_DIR     = $(realpath ./src)

SPL_LIB         = $(SPL_LIB_DIR)/libspl.a

INCOMPLETE      = $(SPL_SRC_DIR)/spl_last.c

vpath %.c $(SPL_SRC_DIR)
vpath %.h $(SPL_INCLUDE_DIR)

# SRCS       = $(wildcard $(SPL_SRC_DIR)/*.c) $(wildcard $(SPL_SRC_DIR)/*/*.c)
# ELFS       = $(notdir $(filter-out $(patsubst $(SPL_SRC_DIR)/%.c,%,$(INCOMPLETE)),$(patsubst $(SPL_SRC_DIR)/%.c,%,$(SRCS))))

SRCS := $(shell find $(SPL_SRC_DIR) -type f -name "*.c")
# ELFS := $(foreach src,$(SRCS),$(basename $(notdir $(src))))

define create_elfs_rules
ELFS += $(2)
$(2):$(1) 
	$(CC) $(CFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $$< -lspl -o $(SPL_BIN_DIR)/$$@
endef

CC         = gcc-15
CFLAGS    += -Wall -g3

ASMS       = $(patsubst $(SPL_SRC_DIR)/%.c,%.s,$(SRCS))
ASMFLAGS   = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions

# HACK: crack it by assembly
ADDERSS_SANITIZER_FALGS += -g3 -fno-omit-frame-pointer -fno-common -fsanitize=address


asm: $(ASMS)

echo:
	@echo $(SRCS)
	@echo $(ELFS)

%.s:%.c
	$(CC) $(CFLAGS) $(ASMFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@

# 链接顺序：在 GCC 命令中，源文件（或 .o 文件）必须写在库参数（-l）的前面
# 如果顺序颠倒，链接器可能会因为还没遇到需要解析的符号而忽略库，导致链接失败
# $< is the source file. $@ is the target file.
# %:%.c
# 	$(CC) $(CFLAGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@
	# $(CC) $(CFLAGS) $(ADDERSS_SANITIZER_FALGS) -I$(SPL_INCLUDE_DIR) -L$(SPL_LIB_DIR) $< -lspl -o $(SPL_BIN_DIR)/$@-asan

$(foreach src,$(SRCS),$(eval $(call create_elfs_rules,$(src),$(basename $(notdir $(src))))))

clean:
	-rm -f $(SPL_BIN_DIR)/$(ELFS)

all: $(ELFS) 

.PHONY: all clean asm echo
