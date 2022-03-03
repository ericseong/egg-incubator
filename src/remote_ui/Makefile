OUT_DIR = build
TARGET = $(OUT_DIR)/pico_http_server

CC = gcc
RM = rm -f
SED = sed
# generate dependency without mentioning system header directories
MAKEDEPEND = $(CC) -MM
CFLAGS = -O2 -Wall
LDFLAGS =

SRCS = $(wildcard *.c)

OBJS = $(addprefix $(OUT_DIR)/, $(SRCS:.c=.o))

all: $(TARGET)

clean:
	$(RM) *.o ~* *.bak
	$(RM) $(OUT_DIR)/*

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OUT_DIR)/%.o : $(OUT_DIR)/%.d
	$(CC) -c $(CFLAGS) $*.c -o $@

DEPS = $(addprefix $(OUT_DIR)/, $(SRCS:.c=.d))

$(OUT_DIR)/%.d : %.c
	@set -e; $(RM) $@; \
	$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# do not delete!
-include $(DEPS)

# eof
