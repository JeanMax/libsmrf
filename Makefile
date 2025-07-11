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

# name of the lib to make
PROJECT = libsmrf$(OS).a

# file-names of the sources
SRC_NAME = smrf.c  proc.c  d2structs.c  \
           parse/parse_level.c  parse/parse_unit.c \
           sdk/level.c  sdk/player.c  sdk/monster.c  \
           sdk/object.c  sdk/missile.c  sdk/tile.c  \
           util/log.c  util/hash.c

# name of the example to make
PROJECT_EXAMPLE = smrfexample

# file-name of the example source
SRC_EXAMPLE = example/example.c

# folder-names of the sources
SRC_PATH = src

# folder-names containing headers files
INC_PATH = include include/smrf

# where are your tests?
TEST_DIR = test

# extra libraries needed for linking
LDLIBS = -pthread

# linking flags
LDFLAGS =
RLDFLAGS = -s -flto=auto -Wl,-O2,--sort-common,--as-needed  # release

# compilation flags
CPPFLAGS =


##
## GLOBAL VARIABLES
##

# compilation/linking flags for the differents public rules
WFLAGS = -std=c18 -Wextra -Wall -Wpadded  # warnings
RCFLAGS = $(WFLAGS) -O3 -fomit-frame-pointer -flto=auto  # release
DCFLAGS = $(WFLAGS) -g -Og -DNDEBUG # -D_FORTIFY_SOURCE=2  # debug
SCFLAGS = $(DCFLAGS) -fsanitize=address,undefined  # sanitize
WWFLAGS = $(WFLAGS) -Wpedantic -Wshadow -Wconversion -Wcast-align \
  -Wstrict-prototypes -Wmissing-prototypes -Wunreachable-code -Winit-self \
  -Wmissing-declarations -Wfloat-equal -Wbad-function-cast -Wundef \
  -Waggregate-return -Wstrict-overflow=5 -Wold-style-definition  \
  -Wredundant-decls -Wpadded -Winline   # moar warnings

# folder used to store all compilations sub-products (.o and .d mostly)
OBJ_DIR ?= obj
OBJ_PATH ?= $(OBJ_DIR)/rel
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.c=.o))
DEP = $(OBJ:.o=.d)

# includes
INC = $(addprefix -I, $(INC_PATH))

# specify flags for commands used in the following rules
LN = ln -s
RM = rm -f
RMDIR = rmdir
MKDIR = mkdir -p
# CC = gcc  # $(shell clang --version >/dev/null 2>&1 && echo clang || echo gcc)
MAKE = make -j
SUB_MAKE = make -C

# try to be cross-platform
UNAME_S = $(shell uname -s)
ifeq ($(OS), Windows_NT)
    AR = x86_64-w64-mingw32-gcc-ar
    CC = x86_64-w64-mingw32-gcc -static -static-libgcc

else ifeq ($(UNAME_S), Linux)
    CC += -march=native

endif

##
## PUBLIC RULES
##

# release build
all:
	+$(MAKE) $(PROJECT) "CFLAGS = $(RCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/rel$(OS)" \
        "LDFLAGS = $(RLDFLAGS)"

# masochist build
mecry:
	+$(MAKE) $(PROJECT) "CFLAGS = $(WWFLAGS)" "OBJ_PATH = $(OBJ_DIR)/rel$(OS)"

# build for gdb/valgrind debugging
dev:
	+$(MAKE) $(PROJECT:.a=_dev.a) \
		"PROJECT = $(PROJECT:.a=_dev.a)" "PROJECT_EXAMPLE = $(PROJECT_EXAMPLE)_dev" \
		"CFLAGS = $(DCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/dev$(OS)"

# build for runtime debugging (fsanitize)
san:
	+$(MAKE) $(PROJECT:.a=_san.a) \
		"PROJECT = $(PROJECT:.a=_san.a)" "PROJECT_EXAMPLE = $(PROJECT_EXAMPLE)_san" \
		"CFLAGS = $(SCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/san$(OS)" "CC = clang"

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
	$(RM) $(PROJECT) $(PROJECT:.a=_dev.a) $(PROJECT:.a=_san.a) \
$(PROJECT_EXAMPLE){,_san,_dev}{,.exe}

# some people like it real clean
mrproper:
	$(RM) -r $(OBJ_DIR)
	+$(MAKE) fclean

# clean build and recompile
re: fclean
	+$(MAKE) all

# run tests on project
test: all
	$(PRINTF) "All tests passed!\n"

# show all TODO in project
todo:
	grep --color=auto -riIn --exclude-dir .git --exclude-dir txt --exclude Makefile -E '[^s]todo' .


##
## PRIVATE RULES
##

# create archive
$(PROJECT): $(OBJ) $(SRC_EXAMPLE)
	$(AR) -rcs $@ $(OBJ)
	+$(MAKE) $(PROJECT_EXAMPLE) || true

# create example binary
$(PROJECT_EXAMPLE): $(SRC_EXAMPLE) $(PROJECT)
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(SRC_EXAMPLE) $(PROJECT) $(LDLIBS) -o $@


# create object files (compile)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC) -MMD -MP -c $< -o $@

# create directory for compilation sub-products
$(OBJ_PATH):
	$(MKDIR) $(dir $(OBJ))

# read dependencies list generated by -MMD flag
-include $(DEP)

# just to avoid conflicts between rules and files/folders names
.PHONY: all, dev, san, mecry, $(PROJECT), todo, \
clean, fclean, mrproper, re, test, testdev, testsan
