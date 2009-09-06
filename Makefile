#
#

TGTDIR = ./
#TGTDIR = ../bin
FNAMEIRC = seeborg-irc
FNAMELINEIN = seeborg-linein

##
## If you're on Windows using MingW, uncomment the line below
#LDFLAGS += -lwsock32

##
## If you're getting link errors on any unix, try uncommeting this line below
CFUSER = -pthread

#CFCPU = -march=native
CFOPT = -O2 -finline -funroll-loops

CFDEBUG = -ggdb3
#CFDEBUG += -pg
#CFDEBUG += -DPROFILE

#CFDEBUG += -Wall
#CFDEBUG += -Wextra

#LDFLAGS = -s

SRCS = seeborg.cpp seeutil.cpp utf8.c

# -------
#
# -------
CC = gcc
CXX = g++
CFLAGS = $(CFCPU) $(CFOPT) $(CFDEBUG) $(CFUSER)
CXXFLAGS = $(CFLAGS)

SRC_IRC = $(FNAMEIRC).cpp botnet/botnet.c botnet/dcc_chat.c botnet/dcc_send.c botnet/output.c \
    botnet/server.c botnet/utils.c
SRC_LINEIN = $(FNAMELINEIN).cpp

TGT_IRC = $(TGTDIR)/$(FNAMEIRC)
TGT_LINEIN = $(TGTDIR)/$(FNAMELINEIN)

OBJ_IRCTMP = $(SRC_IRC:%.cpp=%.o)
OBJ_IRC = $(OBJ_IRCTMP:%.c=%.o)

OBJ_LINEINTMP = $(SRC_LINEIN:%.cpp=%.o)
OBJ_LINEIN = $(OBJ_LINEINTMP:%.c=%.o)

OBJSTMP = $(SRCS:%.cpp=%.o)
OBJS = $(OBJSTMP:%.c=%.o)

all: compile

clean:
	rm -f $(TGT_IRC) $(TGT_LINEIN) $(OBJS) $(OBJ_IRC) $(OBJ_LINEIN)

compile: makedirs $(TGT_LINEIN) $(TGT_IRC)

$(TGT_IRC): $(OBJS) $(OBJ_IRC)
	@echo Linking $@...
	$(CXX) $(CXXFLAGS) $(OBJS) $(OBJ_IRC) -o $@ $(LDFLAGS)

$(TGT_LINEIN): $(OBJS) $(OBJ_LINEIN)
	@echo Linking $@...
	$(CXX) $(CXXFLAGS) $(OBJS) $(OBJ_LINEIN) -o $@ $(LDFLAGS)

.cpp.o:
	@echo Compiling $@...
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

.c.o:
	@echo Compiling $@...
	@$(CC) -c $< -o $@ $(CFLAGS)

makedirs:
	@if [ ! -d $(TGTDIR) ];then mkdir $(TGTDIR);fi
