OUT_DIR = build
TARGET = $(OUT_DIR)/2ne1

C++ = g++
RM = rm -f
SED = sed
# generate dependency without mentioning system header directories
MAKEDEPEND = $(C++) -MM
#CPPFLAGS = -O2 -Wall -std=c++14 -DENABLE_NONBLOCKING_CLIENT_SOCKET -DTMP117_OFFSET=0.5
CPPFLAGS = -O2 -Wall -std=c++14 -DENABLE_NONBLOCKING_CLIENT_SOCKET -DTMP117_OFFSET=0.4
#CPPFLAGS = -DSKIP_WIRINGPI_SETUP -g -Wall -std=c++14
LDFLAGS = -L/usr/local/lib -lpthread -lwiringPi -ljsoncpp -lsystemd

SRCS = $(wildcard *.cpp)

OBJS = $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))

all: $(TARGET)

clean:
	$(RM) *.o ~* *.bak
	$(RM) $(OUT_DIR)/*

$(TARGET) : $(OBJS)
	$(C++) $(LDFLAGS) $^ -o $@

$(OUT_DIR)/%.o : $(OUT_DIR)/%.d
	$(C++) -c $(CPPFLAGS) $*.cpp -o $@

DEPS = $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

$(OUT_DIR)/%.d : %.cpp
	@set -e; $(RM) $@; \
	$(C++) -M $(CPPFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# do not delete!
-include $(DEPS)

# eof
