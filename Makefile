.PHONY : clean all server client test

BINDIR := bin
OBJDIR := obj
SRCDIR := src
LIBDIR := lib
XNRWDIR := $(LIBDIR)/XNRW
XNRWOBJDIR := $(XNRWDIR)/obj
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

SERVER := $(BINDIR)/server
CLIENT := $(BINDIR)/client
SERVER_SRC := $(SRCDIR)/server.cpp
CLIENT_SRC := $(SRCDIR)/client.cpp
SERVER_OBJ := $(OBJDIR)/server.o
CLIENT_OBJ := $(OBJDIR)/client.o

XNRW_ThreadPool := $(XNRWOBJDIR)/ThreadPool.o

CXXFLAGS := --std=c++11 -O3 -D NDEBUG -lpthread -I$(LIBDIR)

TEST_PORT := 1234
TEST_NUM_THREADS := 8
TEST_NUM_MESSAGE := 100
TEST_MESSAGE_SIZE := 16777216

TEST_PARAMS := $(TEST_PORT) $(TEST_NUM_THREADS) $(TEST_NUM_MESSAGES) $(TEST_MESSAGE_SIZE)

all : server client

server : $(SERVER)
client : $(CLIENT)
$(XNRW_ThreadPool) :
	$(MAKE) -C $(XNRWDIR) ThreadPool -j8

$(SERVER) : $(SERVER_OBJ) $(XNRW_ThreadPool) | $(BINDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(CLIENT) : $(CLIENT_OBJ) $(XNRW_ThreadPool) | $(BINDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BINDIR) :
	mkdir $@

$(OBJDIR) :
	mkdir $@

$(OBJECTS) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)

test : all
	$(SERVER) $(TEST_PARAMS) &; sleep 5; $(CLIENT) $(TEST_PARAMS)

clean :
	$(RM) $(OBJECTS) $(SERVER) $(CLIENT)
