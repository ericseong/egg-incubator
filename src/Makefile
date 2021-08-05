OUT_DIR = build
TARGET = $(OUT_DIR)/a.out

C++ = g++
RM = rm -f
SED = sed
MAKEDEPEND = $(C++) -MM # generate dependency without mentioning system header directories
CPPFLAGS = -Wall -std=c++14
LDFLAGS = -lpthread -lwiringPi -ljsoncpp

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
	$(MAKEDEPEND) $(CPPFLAGS) $< > $@.$$$$; \
	$(SED) 's/^/$(OUT_DIR)\//g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# do not delete!
-include $(DEPS)

# eof
