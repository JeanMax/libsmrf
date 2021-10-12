#******************************************************************************#
#                                                               _.._..,_,_     #
#    Makefile                                                  (          )    #
#                                                               ]~,'-.-~~[     #
#    By: mc <mc@rmrf.fr>                                      .=])' (;  ([     #
#                                                             | ]:)   '  [     #
#      "THE BEER-WARE LICENSE" (Revision 42):                 | ]  :: '  [     #
#    As long as you retain this notice you can do whatever    '=]): .)  ([     #
#    you want with this stuff. If we meet some day, and you     |:: '   :|     #
#    think this is worth it, you can buy me a beer in return.    ~~----~~      #
#                                                                              #
#******************************************************************************#


##
## CUSTOM CONFIG
##

# name of the binary to make
PROJECT = seed

# file-names of the sources
SRC_NAME = main.c  pidof.c

# folder-names of the sources
SRC_PATH = src

# folder-names containing headers files
INC_PATH = include

# where are your tests?
TEST_DIR = test

# extra libraries needed for linking
LDLIBS =

# linking flags
LDFLAGS =

# compilation flags
CPPFLAGS =


##
## GLOBAL VARIABLES
##

# compilation/linking flags for the differents public rules
WFLAGS = -Wextra -Wall  # warnings
RCFLAGS = $(WFLAGS) -Werror -O2  # release
DCFLAGS = $(WFLAGS) -g -DDEBUG_MODE  # debug
SCFLAGS = $(DCFLAGS) -fsanitize=address,undefined  # sanitize
WWFLAGS = $(WFLAGS) -Wpedantic -Wshadow -Wconversion -Wcast-align \
  -Wstrict-prototypes -Wmissing-prototypes -Wunreachable-code -Winit-self \
  -Wmissing-declarations -Wfloat-equal -Wbad-function-cast -Wundef \
  -Waggregate-return -Wstrict-overflow=5 -Wold-style-definition  \
  -Wredundant-decls  # -Wpadded  # moar warnings

# folder used to store all compilations sub-products (.o and .d mostly)
OBJ_DIR ?= obj
OBJ_PATH ?= $(OBJ_DIR)/rel
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.c=.o))
DEP = $(OBJ:.o=.d)

# includes
INC = $(addprefix -I, $(INC_PATH))

# specify flags for commands used in the following rules
LN =		ln -s
RM =		rm -f
RMDIR =		rmdir
MKDIR =		mkdir -p
CC =		gcc  # $(shell clang --version >/dev/null 2>&1 && echo clang || echo gcc)
MAKE ?=		make -j$(shell nproc 2>/dev/null || echo 1)
SUB_MAKE =	make -C

# default to "pretty" Makefile, but you can run ´VERBOSE=t make´
# ifndef VERBOSE
#  ifndef TRAVIS
# .SILENT:
#  endif
# endif
# PRINTF = test $(VERBOSE)$(TRAVIS) || printf

# some colors for pretty printing
WHITE =		\033[37m
RED =		\033[31m
GREEN =		\033[32m
YELLOW =	\033[33m
BLUE =		\033[34m
BASIC =		\033[0m


##
## PUBLIC RULES
##

# release build
all:
	+$(MAKE) $(PROJECT) "CFLAGS = $(RCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/rel"

# masochist build
mecry:
	+$(MAKE) $(PROJECT) "CFLAGS = $(WWFLAGS)" "OBJ_PATH = $(OBJ_DIR)/rel"

# build for gdb/valgrind debugging
dev:
	+$(MAKE) $(PROJECT).dev \
		"PROJECT = $(PROJECT).dev" \
		"CFLAGS = $(DCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/dev"

# build for runtime debugging (fsanitize)
san:
	+$(MAKE) $(PROJECT).san \
		"PROJECT = $(PROJECT).san" \
		"CFLAGS = $(SCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/san"

# remove all generated .o and .d
clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) $(OBJ:$(OBJ_DIR)/rel%=$(OBJ_DIR)/dev%)
	$(RM) $(DEP:$(OBJ_DIR)/rel%=$(OBJ_DIR)/dev%)
	$(RM) $(OBJ:$(OBJ_DIR)/rel%=$(OBJ_DIR)/san%)
	$(RM) $(DEP:$(OBJ_DIR)/rel%=$(OBJ_DIR)/san%)
	test -d $(OBJ_DIR) && find $(OBJ_DIR) -name '*.[od]' | xargs $(RM) || true

# remove the generated binary, and all .o and .d
fclean: clean
	test -d $(OBJ_DIR) \
&& find $(OBJ_DIR) -type d | sort -r | xargs $(RMDIR) || true
	$(RM) $(PROJECT) $(PROJECT).san $(PROJECT).dev

# some people like it real clean
mrproper:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(TEST_DIR)
	+$(MAKE) fclean

# clean build and recompile
re: fclean
	+$(MAKE) all

# run tests on project
test: all
	$(PRINTF) "All tests passed!\n"


##
## PRIVATE RULES
##

# create binary (link)
$(PROJECT): $(OBJ)
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

# create object files (compile)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC) -MMD -MP -c $< -o $@

# create directory for compilation sub-products
$(OBJ_PATH):
	$(MKDIR) $(dir $(OBJ))

# read dependencies list generated by -MMD flag
-include $(DEP)

# just to avoid conflicts between rules and files/folders names
.PHONY: all, dev, san, mecry, $(PROJECT), \
clean, fclean, mrproper, re, test, testdev, testsan
