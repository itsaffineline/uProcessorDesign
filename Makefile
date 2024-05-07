build: main.hex

OBJDIR := build

CFLAGS := --debug --iram-size 256
#OBJS := $(addprefix $(OBJDIR)/,lcdDriver.rel rtc.rel io.rel input.rel uart.rel menu.rel programs.rel)
OBJS := $(addprefix $(OBJDIR)/, $(shell find . -name '*.c' | grep -v main.c | sed -r 's/\.\/([a-zA-Z0-9_]+)\.c/\1.rel/'))
include_directory := headers

$(OBJDIR)/%.rel: %.c | $(OBJDIR)/.stamp
	sdcc $(CFLAGS) -c $< -I $(include_directory) -o build/

$(OBJDIR)/.stamp:
	mkdir $(OBJDIR)
	touch $@

main.hex: $(OBJS) main.c
	sdcc --debug main.c $(OBJS) -I $(include_directory) -o build/
	packihx build/main.ihx > main.hex
	cp main.hex ~/virtfiles/main.hex

debug: main.hex
	cp $(OBJDIR)/main.cdb .
	cp $(OBJDIR)/main.ihx .

clean:
	rm -rf build/
	rm -f *{.cdb,.ihx,.hex,.hashes,~}

