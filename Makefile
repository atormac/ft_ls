MAKEFLAGS += "-s"
NAME = ft_ls
CC = cc
CFLAGS := -Wall -Wextra -Werror

SOURCE_DIR := source
SOURCES := main.c ls.c tree.c
INCLUDE_DIR = include

OBJ_DIR := obj
OBJECTS := $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))

$(shell mkdir -p $(OBJ_DIR))

#NPROCS := $(shell grep -c ^processor /proc/cpuinfo)
NPROCS := 4

target asan_flags: CFLAGS += -fsanitize=address,undefined -g
target debug_flags: CFLAGS += -gdwarf-4 -fstandalone-debug

all: $(NAME)

$(NAME): $(OBJECTS)
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

asan_flags: all
debug_flags: all

debug:
	@make fclean
	@make debug_flags -j$(NPROCS)
asan:
	@make fclean
	@make asan_flags -j$(NPROCS)

.PHONY: all re clean fclean asan debug asan_flags debug_flags
