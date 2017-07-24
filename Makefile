# dep: jieba's dict
JIEBA_DATA_DIR ?= ./jieba_dict

CFLAGS = -flto -O3 -g -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -DJIEBA_DATA_DIR=\"$(JIEBA_DATA_DIR)\"
CXXFLAGS := $(CFLAGS) -std=gnu++11 -I$(CURDIR)/cppjieba/include -I $(CURDIR)/cppjieba/deps
CFLAGS += -std=gnu11
LDFLAGS = -O1 -lopencc

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

DESTDIR ?= dist

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

.PHONY: all compile install
all: compile

install: compile
	install -d -m755 $(DESTDIR)/$(JIEBA_DATA_DIR)
	install -t $(DESTDIR) $(TARGET_IRC) $(TARGET_LINEIN)
	install -t $(DESTDIR)/$(JIEBA_DATA_DIR) jieba_dict/jieba.dict.utf8 jieba_dict/hmm_model.utf8 jieba_dict/user.dict.utf8 jieba_dict/idf.utf8 jieba_dict/stop_words.utf8  

clean:
	rm -f $(TARGET_IRC) $(TARGET_LINEIN) $(TARGET_OBJS) $(TARGET_IRC_OBJS) $(TARGET_LINEIN_OBJS)

compile: $(TARGET_LINEIN) $(TARGET_IRC)

$(TARGET_IRC): $(TARGET_OBJS) $(TARGET_IRC_OBJS) $(TARGET_HDRS) $(TARGET_IRC_HDRS)
	$(CXX) $(CXXFLAGS) $(TARGET_OBJS) $(TARGET_IRC_OBJS) -o $@ $(LDFLAGS)

$(TARGET_LINEIN): $(TARGET_OBJS) $(TARGET_LINEIN_OBJS) $(TARGET_HDRS) $(TARGET_LINEIN_HDRS)
	$(CXX) $(CXXFLAGS) $(TARGET_OBJS) $(TARGET_LINEIN_OBJS) -o $@ $(LDFLAGS)

