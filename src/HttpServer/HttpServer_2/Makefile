CXX = /usr/bin/g++
CXXFLAGS = $(COMPILE_OPT) -Wall -I  -I$(BOOST_INCLUDE) `mysql_config --cflags`
CXXLFLAGS = $(COMPILE_OPT) -Wall -lrt -L$(BOOST_LIB) `mysql_config --libs_r` -lboost_thread -lboost_system

OBJS = connection.o \
	   main.o \
	   mime_types.o \
	   reply.o \
	   request_handler.o \
	   request_parser.o \
	   server.o \
	   io_service_pool.o


SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=.%.d)
TARGET = HttpServer_2


all : $(TARGET)

include $(DEPS)
$(DEPS): .%.d: %.cpp
	@$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

$(OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(TARGET): $(OBJS) $(LIB_TAR)
	$(CXX) $(OBJS) -o $@ $(CXXLFLAGS)

ctags:
	@ctags -R -h ".c.cpp.h.hpp" -o tags

clean:
	@rm -rf $(OBJS) $(LIB_TAR) $(TARGET) core *~
