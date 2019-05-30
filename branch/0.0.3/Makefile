EXE = twiny
SRCS = main.cpp twiny.cpp happyhttp.cpp base64.cpp

ifdef DEBUG
  CXXFLAGS = -ggdb
endif

ifdef WIN32
# need to link with winsock2
LDFLAGS += -lws2_32
EXE = twiny.exe
endif

OBJS = $(patsubst %.cpp,%.o,$(SRCS) )


# -------------


$(EXE): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	strip $(EXE)


clean:
	rm -f $(OBJS)
	rm -f .depend
	rm -f *.*~

install:
	install -m 755 $(EXE) /usr/bin
uninstall:
	rm -f /usr/bin/$(EXE)


# automatically generate dependencies from our .cpp files
# (-MM tells the compiler to just output makefile rules)
depend:
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $(SRCS) > .depend

ifeq ($(wildcard .depend),.depend)
include .depend
endif
