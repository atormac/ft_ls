MAKEFLAGS += "-s"
NAME = ft_ls
CC = cc
CFLAGS := -Wall -Wextra -Werror

SOURCE_DIR := source
SOURCES := main.c ls.c stack.c print.c str.c qsort.c
INCLUDE_DIR = include

OBJ_DIR := obj
OBJECTS := $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))

$(shell mkdir -p $(OBJ_DIR))

#NPROCS := $(shell grep -c ^processor /proc/cpuinfo)
NPROCS := 4

target asan_flags: CFLAGS += -fsanitize=address,undefined -g
target debug_flags: CFLAGS += -gdwarf-4 -fstandalone-debug
target bench_flags: CFLAGS += -O3 -DBENCH


all: $(NAME)

$(NAME): $(OBJECTS)
	@echo $(CFLAGS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c
	@echo [+] $<
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re:
	@make fclean
	@make all -j$(NPROCS)

bench_flags: all

asan_flags: all
debug_flags: all

bench:
	@make fclean
	@make bench_flags -j$(NPROCS)

debug:
	@make fclean
	@make debug_flags -j$(NPROCS)
asan:
	@echo [ASAN]
	@make fclean
	@make asan_flags -j$(NPROCS)

.PHONY: all re clean fclean bench bench_flags asan debug asan_flags debug_flags
