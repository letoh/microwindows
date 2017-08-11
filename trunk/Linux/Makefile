# shared

SHARED_SRCS += ../Shared/src/button.cpp
SHARED_SRCS += ../Shared/src/control.cpp
SHARED_SRCS += ../Shared/src/desktop.cpp
SHARED_SRCS += ../Shared/src/engfont.c
SHARED_SRCS += ../Shared/src/freearea.cpp
SHARED_SRCS += ../Shared/src/gobject.cpp
SHARED_SRCS += ../Shared/src/idler.cpp
SHARED_SRCS += ../Shared/src/list.cpp
SHARED_SRCS += ../Shared/src/message.cpp
SHARED_SRCS += ../Shared/src/model.cpp
SHARED_SRCS += ../Shared/src/mouse.cpp
SHARED_SRCS += ../Shared/src/multview.cpp
SHARED_SRCS += ../Shared/src/shape.cpp
SHARED_SRCS += ../Shared/src/stackque.cpp
SHARED_SRCS += ../Shared/src/view.cpp
SHARED_SRCS += ../Shared/src/viewlist.cpp
SHARED_SRCS += ../Shared/src/viewport.cpp
SHARED_SRCS += ../Shared/src/window.cpp

SHARED_TEMP = $(SHARED_SRCS:.c=.o)
SHARED_OBJS = $(SHARED_TEMP:.cpp=.o)

# applet

APPLET_SRCS += ../Applets/about.cpp
APPLET_SRCS += ../Applets/calc.cpp
APPLET_SRCS += ../Applets/cube.cpp
APPLET_SRCS += ../Applets/digclock.cpp
APPLET_SRCS += ../Applets/draw.cpp

APPLET_OBJS = $(APPLET_SRCS:.cpp=.o)

# platform

PLAT_SRCS += src/gui.cpp
PLAT_SRCS += src/lowgdi.cpp
PLAT_SRCS += src/port.cpp
PLAT_SRCS += src/timer.cpp

PLAT_OBJS = $(PLAT_SRCS:.cpp=.o)


# application

HELLO_SRCS += hello.cpp
HELLO_OBJS = $(SHARED_OBJS) $(PLAT_OBJS) $(HELLO_SRCS:.cpp=.o)

DEMO_SRCS += demo.cpp
DEMO_OBJS = $(SHARED_OBJS) $(PLAT_OBJS) $(APPLET_OBJS) $(DEMO_SRCS:.cpp=.o)


CFLAGS   += -I../Shared/include -Iinclude -std=c99 `pkg-config --cflags x11`
CXXFLAGS += -I../Shared/include -Iinclude -std=c++98 `pkg-config --cflags x11`
LDFLAGS  += `pkg-config --libs x11`

BIN = hello demo

all: $(BIN)

hello: $(HELLO_OBJS)
	@$(CXX) -o $@ $^ $(LDFLAGS)


demo: $(DEMO_OBJS)
	@$(CXX) -o $@ $^ $(LDFLAGS)


clean:
	@rm -f $(BIN) $(SHARED_OBJS) $(APPLET_OBJS) $(PLAT_OBJS) *.o *~ src/*~ include/*~
