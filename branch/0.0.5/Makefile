SRCS = twiny.cpp happyhttp.cpp base64.cpp settings.cpp common.cpp

ifdef DEBUG
  CXXFLAGS = -ggdb -DDEBUG
endif

ifndef WIN32
EXE = twiny
endif

ifdef WIN32
# need to link with winsock2
LDFLAGS += -lws2_32
EXE = twiny.exe
endif

#for Apple iPhone version. Big thanks to Sasha Ryzhkov <sash13@bigmir.net>
ifdef MAC
  CXX = arm-apple-darwin9-g++ -I"/var/include/c++/4.0.0" -I"/usr/include" 
  CFLAGS += -I"/var/include/c++/4.0.0" 
  CFLAGS += -I"/usr/include" 
endif

OBJS = $(patsubst %.cpp,%.o,$(SRCS) )


# -------------

$(EXE): $(OBJS)
	$(CXX) -o  $@ $(OBJS) $(LDFLAGS)
	strip $(EXE)

ifdef WIN32
clean:
	del /f $(OBJS)
	del /f .depend
	del /f *.*~
	del /f twiny.exe
endif
ifndef WIN32
clean:
	rm -f $(OBJS)
	rm -f .depend
	rm -f *.*~
	rm -f twiny
endif

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
