NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
SRCS = main.cpp \
	server.cpp \
	config.cpp \
	http_config.cpp \
	server_config.cpp \
	location_config.cpp \
	response_builder.cpp \
	request_parser.cpp \
	config_parser.cpp

DEPS = $(patsubst %.cpp,%.d,$(SRCS))
OBJECTS = $(patsubst %.cpp,%.o,$(SRCS))
RED = \033[1;31m
GREEN = \033[1;32m
NC = \033[1;0m

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(RED)Compiling $(NAME)...$(NC)"
	@echo "$(RED)Linking...$(NC)"
	@$(CXX) $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)Finished linking && compiling...$(NC)"

clean:
	@echo "$(RED)Cleaning objects...$(NC)"
	@rm -rf $(OBJECTS)
	@echo "$(GREEN)Cleaned objects...$(NC)"
	@echo "$(RED)Cleaning dependencies...$(NC)"
	@rm -rf $(DEPS)
	@echo "$(GREEN)Cleaned dependencies...$(NC)"

fclean: clean
	@echo "$(RED)Cleaning $(NAME)...$(NC)"
	@rm -rf $(NAME)
	@echo "$(GREEN)Cleaned $(NAME)...$(NC)"

re: fclean all

.PHONY: clean fclean all re

-include $(DEPS)

%.cpp:

%.o: %.cpp Makefile
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "$(RED)Compiling $< ...$(NC)"
