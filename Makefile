NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
SRCS = src/main.cpp \
	src/client/client.cpp \
	src/config/address_port.cpp \
	src/config/config_parser.cpp \
	src/config/config.cpp \
	src/config/http_config.cpp \
	src/config/location_config.cpp \
	src/config/server_config.cpp \
	src/constants/constants.cpp \
	src/directory_index_builder/directory_index_builder.cpp \
	src/logger/logger.cpp \
	src/request/request_builder.cpp \
	src/response/response_builder.cpp \
	src/server/server.cpp \
	src/utilities/utilities.cpp

INC = -Isrc/client \
	-Isrc/config \
	-Isrc/constants \
	-Isrc/directory_index_builder \
	-Isrc/logger \
	-Isrc/request \
	-Isrc/response \
	-Isrc/server \
	-Isrc/utilities

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
	@$(CXX) $(CXXFLAGS) $(INC) -MMD -MP -c $< -o $@
	@echo "$(RED)Compiling $< ...$(NC)"
