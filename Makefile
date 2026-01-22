NAME := bouncy
CC := cc
CFLAGS := -Wall -Wextra -Werror -Wunused-function -MMD -MP -Ofast
LDFLAGS := -lGL -lglfw -lm -ldl
INCLUDES := -I ./include

ifdef DEBUG
	CFLAGS += -g3
	CFLAGS += -D DEBUG=true
endif

ifdef FSAN
	CFLAGS += -fsanitize=address
endif

SRC_PATH := src/
OBJ_PATH := obj/

SRC := main.c \
			 balls.c \
			 scene.c \
			 render.c \
			 handler.c

SRCS := $(addprefix $(SRC_PATH), $(SRC))
OBJS := $(addprefix $(OBJ_PATH), $(SRC:.c=.o))

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

-include $(wildcard $(OBJS:.o=.d))

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

format:
	@set -e; \
	cfg_dir=""; \
	dir="$$(pwd)"; \
	home="$$HOME"; \
	while [ "$$dir" != "/" ]; do \
		if [ -f "$$dir/.clang-format" ]; then cfg_dir="$$dir"; break; fi; \
		if [ "$$dir" = "$$home" ]; then break; fi; \
		dir="$$(dirname "$$dir")"; \
	done; \
	if [ -z "$$cfg_dir" ] && [ -f "$$home/.clang-format" ]; then cfg_dir="$$home"; fi; \
	if [ -z "$$cfg_dir" ]; then echo "No .clang-format found from $$PWD to $$HOME"; exit 1; fi; \
	echo "Using $$cfg_dir/.clang-format"; \
	find ./src ./include -type f \( -name '*.c' -o -name '*.h' \) -print | \
	while IFS= read -r f; do \
		tmp="$$(mktemp)"; \
		clang-format-12 -style=file -assume-filename="$$cfg_dir/_.c" < "$$f" > "$$tmp"; \
		mv "$$tmp" "$$f"; \
	done

.PHONY: all clean fclean re format
