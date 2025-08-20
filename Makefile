# Makefileの構文に注意:
# - ifeqでは「実行」できない(echoなどができない)
# - 同じ行にコメントと命令があるとぶっ壊れる(文字列とスペースの取り扱いがひどいので)
# - ifeqでは(タブではなく)スペースが必要(文字列とスペースの取り扱いがひどいので)
# - rulesでは(スペースではなく)タブが必要(文字列とスペースの取り扱いがひどいので)

# compiler environment
#CXX		= g++
CXX			= nvc++

# options
GPU			?= managed
# managed, nonmanaged, no
DEBUG		?= yes
# yes, no
OPTIMIZE	?= yes
# yes, no

# flags
STD			= -std=c++17
CXXFLAGS	= -Wall -Wextra
LDFLAGS 	=

# options to add for GPU
ifeq ($(GPU), managed)
    CXXFLAGS	+= -acc=gpu -gpu=cc80,mem:managed -Minfo=accel,opt
    LDFLAGS		+= -acc=gpu -gpu=mem:managed
else ifeq ($(GPU), nonmanaged)
    CXXFLAGS	+= -acc=gpu -gpu=cc80 -Minfo=accel,opt
    LDFLAGS		+= -acc=gpu
endif

# options to add for debugging
ifeq ($(DEBUG), yes)
    CXXFLAGS 	+= -O0 -g
else
    CXXFLAGS   	+= -O3 -fast
endif

# options to add for optimizing
ifeq ($(OPTIMIZE), yes)
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


# rules
all:	$(EXEC)

$(EXEC): $(OBJ) $(LIBS)
	$(CXX) $(STD) $(CXXFLAGS) $(ARGS) -o $@ $(OBJ) $(LDFLAGS)

$(BUILD_DIR)/%.o:	%.cpp
	@mkdir -p ./$(BUILD_DIR)/$(SRC_DIR)
	$(CXX) $(STD) $(CXXFLAGS) $(ARGS) $(INCLUDE) -o $@ -c $<

# clean up
.PHONY: clean

clean:
	rm -rf $(EXEC) $(OBJ)
