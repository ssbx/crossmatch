# This Makefile requires GNU Make.

# flag for linking with non-deferred option (default is deferred mode)
NONDEFER=false


ARCH     := $(shell arch)
OBJ_HOME := .$(ARCH)
LIB      := libCrossMatch.a
LIB_HOME := lib

#PROF	:= -pg
PROF	:=

INCS    := -Idata -Idata/consumer -Idata/consumer/file -Idata/producer -Idata/producer/file -Ifilter -Imatcher -Irefine -Iutil

CXX     := g++

# DEFS options are: 
#                   PROFILE PROFILE_ACTIVE_LIST
#                   PROFILE_MEMORY PROFILE_MEMORY_DETAILS 
#                   PROFILE_PAGE_FAULTS PROFILE_PAGE_FAULT_DETAILS
#                   PROFILE_BLOCKED_IO
#                   POOL_STORAGE POOL_STATS CONTRACT TIMER
#                   SHOW_CANDS

DEFS	:= -O -ffloat-store -DPROFILE -DPROFILE_ACTIVE_LIST -DTIMER
#DEFS	:= -g -DPROFILE -DPROFILE_ACTIVE_LIST -DTIMER -DSTRING_LEAK_DEBUG

all: apps


.SUFFIXES:

$(OBJ_HOME)/%.o: %.cpp
	$(CXX) $(PROF) $(DEFS) $(INCS) -ansi -pedantic -Wall -W -Wno-long-long -o $@ -c $<

LIB_SRC := $(shell ls data/*.cpp data/consumer/*.cpp data/consumer/file/*.cpp data/producer/*.cpp data/producer/file/*.cpp filter/*.cpp matcher/*.cpp refine/*.cpp util/*.cpp)

LIB_OBJS := $(LIB_SRC:%.cpp=$(OBJ_HOME)/%.o)

TEST_SRC := tests/cm_file.cpp tests/read_file.cpp tests/nn_file.cpp tests/frcm_file.cpp

TEST_OBJS := $(TEST_SRC:%.cpp=$(OBJ_HOME)/%.o)

.PRECIOUS: $(LIB_OBJS) $(TEST_OBJS)



lib: make_obj_dir make_lib_dir $(LIB_OBJS)
	ar ruv $(LIB_HOME)/$(LIB) $(LIB_OBJS)
	if [ -f /bin/ranlib -o -f /usr/bin/ranlib ]; then \
		ranlib $(LIB_HOME)/$(LIB) ;\
	fi

apps: lib $(TEST_OBJS) cm_file read_file nn_file frcm_file

cm_file: lib $(TEST_OBJS)
	$(CXX) $(PROF) $(DEFS) -o tests/$@ $(OBJ_HOME)/tests/$@.o $(LIB_HOME)/$(LIB)

nn_file: lib $(TEST_OBJS)
	$(CXX) $(PROF) $(DEFS) -o tests/$@ $(OBJ_HOME)/tests/$@.o $(LIB_HOME)/$(LIB)

frcm_file: lib $(TEST_OBJS)
	$(CXX) $(PROF) $(DEFS) -o tests/$@ $(OBJ_HOME)/tests/$@.o $(LIB_HOME)/$(LIB)

read_file: lib $(TEST_OBJS)
	$(CXX) $(PROF) $(DEFS) -o tests/$@ $(OBJ_HOME)/tests/$@.o $(LIB_HOME)/$(LIB)


tests: apps
	tests/read_file tests data1 ascii
	tests/read_file tests data2 ascii
	tests/cm_file 0 tests/data1.txt ascii 0.000305553 0 tests/data2.txt ascii 0.00030554 1.96 index as filter
	tests/nn_file 0 tests/data1.txt ascii 10 index filter
	tests/frcm_file 0 tests/data1.txt ascii 0 tests/data2.txt ascii 10 index as filter frcm_out

cross:
	tests/cm_file 0 tests/data1.txt ascii 0.000305553 0 tests/data2.txt ascii 0.00030554 1.96 index as filter

make_obj_dir:
	if [ ! -d $(OBJ_HOME)/data ]; then mkdir -p $(OBJ_HOME)/data; fi
	if [ ! -d $(OBJ_HOME)/data/consumer ]; then mkdir -p $(OBJ_HOME)/data/consumer; fi
	if [ ! -d $(OBJ_HOME)/data/consumer/file ]; then mkdir -p $(OBJ_HOME)/data/consumer/file; fi
	if [ ! -d $(OBJ_HOME)/data/producer ]; then mkdir -p $(OBJ_HOME)/data/producer; fi
	if [ ! -d $(OBJ_HOME)/data/producer/file ]; then mkdir -p $(OBJ_HOME)/data/producer/file; fi
	if [ ! -d $(OBJ_HOME)/filter ]; then mkdir -p $(OBJ_HOME)/filter; fi
	if [ ! -d $(OBJ_HOME)/matcher ]; then mkdir -p $(OBJ_HOME)/matcher; fi
	if [ ! -d $(OBJ_HOME)/refine ]; then mkdir -p $(OBJ_HOME)/refine; fi
	if [ ! -d $(OBJ_HOME)/util ]; then mkdir -p $(OBJ_HOME)/util; fi
	if [ ! -d $(OBJ_HOME)/tests ]; then mkdir -p $(OBJ_HOME)/tests; fi

make_lib_dir:
	if [ ! -d $(LIB_HOME) ]; then mkdir $(LIB_HOME); fi

clean:
	$(RM) -f $(LIB_OBJS) $(TEST_OBJS) $(LIB_HOME)/$(LIB) *.o

clobber: clean
	$(RM) -rf $(OBJ_HOME) $(LIB_HOME) *~ */*~ */*/*~ */*/*/*~

