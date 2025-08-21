# Makefileの構文に注意:
# - ifeqでは「実行」できない(echoなどができない)
# - 同じ行にコメントと命令があるとぶっ壊れる(文字列とスペースの取り扱いがひどいので)
# - ifeqでは(タブではなく)スペースが必要(文字列とスペースの取り扱いがひどいので)
# - rulesでは(スペースではなく)タブが必要(文字列とスペースの取り扱いがひどいので)


# ========== compiling options ==========
# compiler environment
CXX			= g++
#CXX		= nvc++

# options
GPU			?= no
# managed, nonmanaged, no
DEBUG		?= yes
# yes, no
OPTIMIZE	?= no
# yes, no

# flags
STD			= -std=c++17
CXXFLAGS	= -Wall -Wextra
LDFLAGS 	=

# options to add for GPU
ifeq ($(strip $(GPU)), managed)
    CXXFLAGS	+= -acc=gpu -gpu=cc80,mem:managed -Minfo=accel,opt
    LDFLAGS		+= -acc=gpu -gpu=mem:managed
else ifeq ($(strip $(GPU)), nonmanaged)
    CXXFLAGS	+= -acc=gpu -gpu=cc80 -Minfo=accel,opt
    LDFLAGS		+= -acc=gpu
endif

# options to add for debugging
ifeq ($(strip $(DEBUG)), yes)
    CXXFLAGS 	+= -O0 -g
else
    CXXFLAGS   	+= -O3 -fast
endif

# options to add for optimizing
ifeq ($(strip $(OPTIMIZE)), yes)
    CXXFLAGS 	+= -Mfprelaxed=rsqrt
endif


# libraries to include
LIBS     	=
INCLUDE  	= 
#INCLUDE  	= -I /usr/local/include


# sources
SRC_DIR 	= ./source
SRC         = $(wildcard $(SRC_DIR)/*.cpp)

# objects
BUILD_DIR 	= ./build
OBJ			= $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(SRC)))

# executable
EXEC		= ./bin/collapse


# ========== documentation options ==========
# doxygen
DOXYGEN  ?= doxygen

# Doxyfile: Doxygen configuration file
DOXYFILE ?= ./Doxyfile

# Documentation output directory
DOC_DIR  ?= ./document



# ========== rules ==========
all:	$(EXEC)

$(EXEC): $(OBJ) $(LIBS)
	mkdir -p ./bin
	$(CXX) $(STD) $(CXXFLAGS) $(ARGS) -o $@ $(OBJ) $(LDFLAGS)

$(BUILD_DIR)/%.o:	%.cpp
	mkdir -p ./$(BUILD_DIR)/$(SRC_DIR)
	$(CXX) $(STD) $(CXXFLAGS) $(ARGS) $(INCLUDE) -o $@ -c $<


# phony targets (non-files)
.PHONY: clean doc document

# clean up
clean:
	rm -rf $(EXEC) $(OBJ)

# create documentation
doc document: doc-check
	mkdir -p $(DOC_DIR)
	$(DOXYGEN) $(DOXYFILE)

# check if doxygen is available
doc-check:
	@command -v $(DOXYGEN) >/dev/null || { echo "Doxygen not found! Please install doxygen and set path to it."; exit 1; }

# clean documentation
doc-clean:
	rm -rf $(DOC_DIR)/*

# open documentation
doc-open open: doc
	open $(DOC_DIR)/html/index.html || true

# help command
help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all        - Build the executable (default for no target)"
	@echo "  clean      - Remove build artifacts"
	@echo "  doc        - Generate documentation"
	@echo "  document   - Alias for doc"
	@echo "  doc-check  - Check if doxygen is available"
	@echo "  doc-clean  - Clean documentation output"
	@echo "  doc-open   - Open the generated documentation in a web browser"
	@echo "  open       - Alias for doc-open"
	@echo "  help       - Show this help message"
