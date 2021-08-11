OUT_DIR = build
TARGET = $(OUT_DIR)/2ne1-display-server

# added for lcd
DIR_Config   = $(HOME)/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/Config
DIR_EPD      = $(HOME)/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/LCD
DIR_FONTS    = $(HOME)/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/Fonts
DIR_GUI      = $(HOME)/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/GUI
DIR_Examples = $(HOME)/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/examples
# added for lcd

C++ = g++
RM = rm -f
SED = sed
# generate dependency without mentioning system header directories
MAKEDEPEND = $(C++) -MM
CPPFLAGS = -g -Wall -std=c++14 -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) -I $(DIR_Examples)
#CPPFLAGS = -DSKIP_WIRINGPI_SETUP -g -Wall -std=c++14 -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) -I $(DIR_Examples)
LDFLAGS = -L/usr/local/lib -lpthread -lwiringPi -ljsoncpp -llcd -lsystemd

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
