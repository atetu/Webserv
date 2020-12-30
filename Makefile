# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/13 15:11:33 by ecaceres          #+#    #+#              #
#    Updated: 2020/11/30 16:18:47 by alicetetu        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

USE_FSANITIZE			= false
RUN_TESTS				= 0

SOURCE_DIR				= source
PROJECT_MAKE			= make -s -C $(SOURCE_DIR) USE_FSANITIZE=$(USE_FSANITIZE) RUN_TESTS=$(RUN_TESTS)

all: touch_test_files
	@$(PROJECT_MAKE) all
    
fclean:
	@$(PROJECT_MAKE) fclean
    
clean:
	@$(PROJECT_MAKE) clean
    
re:
	@$(PROJECT_MAKE) re

norm:
	@$(PROJECT_MAKE) norm

update:
	@$(PROJECT_MAKE) update

run: RUN_TESTS=0
run: all
	@$(PROJECT_MAKE) ARGS="$(ARGS)" run

test: RUN_TESTS=1
test: all
	@$(PROJECT_MAKE) ARGS="$(ARGS)" run
	
touch_test_files:
	@#echo ${RUN_TESTS} $$(bash -c "test -f '.test'; echo $$?") # DEBUG
	@bash -c "if [[ ('${RUN_TESTS}' = 0 && -f '.test') || ('${RUN_TESTS}' != 0 && ! -f '.test') ]]; then touch source/main.cpp source/tests/units/*.cpp; fi"
	@bash -c "if [[ '${RUN_TESTS}' = 0 && -f '.test' ]]; then rm .test; fi"
	@bash -c "if [[ '${RUN_TESTS}' != 0 && ! -f '.test' ]]; then touch .test; fi"

.PHONY: all clean fclean re norm update test
