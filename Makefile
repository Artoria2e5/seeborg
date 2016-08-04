CFLAGS = -O2 -g -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
CXXFLAGS := $(CFLAGS)
CFLAGS += -std=gnu99

TARGET_IRC=seeborg-irc
TARGET_LINEIN=seeborg-linein
TARGET_OBJS=seeborg.o seeutil.o utf8.o 
TARGET_HDRS=seeborg.h seeutil.h utf8.h required.h
TARGET_LINEIN_OBJS=seeborg-linein.o
TARGET_IRC_OBJS=seeborg-irc.o
TARGET_IRC_HDRS=seeborg-irc.h
# botnet library
TARGET_IRC_OBJS+=botnet/botnet.o botnet/dcc_chat.o botnet/dcc_send.o
TARGET_IRC_OBJS+=botnet/output.o botnet/server.o botnet/utils.o
TARGET_IRC_HDRS+=botnet/botnet.h botnet/includes.h

## Linux 
ifeq ($(shell uname), Linux)
LDFLAGS += -pthread
endif

## FreeBSD 
ifeq ($(shell uname), FreeBSD)
LDFLAGS += -pthread
endif

## MacOSX 
ifeq ($(shell uname), Darwin)
LDFLAGS += -pthread
endif

## Windows (mingw) 
ifeq ($(shell uname), MINGW32_NT-5.1)
LDFLAGS += -lwsock32
endif

all: compile

clean:
	rm -f $(TARGET_IRC) $(TARGET_LINEIN) $(TARGET_OBJS) $(TARGET_IRC_OBJS) $(TARGET_LINEIN_OBJS)

compile: $(TARGET_LINEIN) $(TARGET_IRC)

$(TARGET_IRC): $(TARGET_OBJS) $(TARGET_IRC_OBJS) $(TARGET_HDRS) $(TARGET_IRC_HDRS)
	$(CXX) $(CXXFLAGS) $(TARGET_OBJS) $(TARGET_IRC_OBJS) -o $@ $(LDFLAGS)

$(TARGET_LINEIN): $(TARGET_OBJS) $(TARGET_LINEIN_OBJS) $(TARGET_HDRS) $(TARGET_LINEIN_HDRS)
	$(CXX) $(CXXFLAGS) $(TARGET_OBJS) $(TARGET_LINEIN_OBJS) -o $@ $(LDFLAGS)
