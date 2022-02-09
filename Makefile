# You can put your build options here

EXECSUFFIX = .exe

program = getJSONX$(EXECSUFFIX) print_tokens$(EXECSUFFIX)
headers = jsmnx.h array.h stringx.h

all: $(program)

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

%$(EXECSUFFIX): %.o
	$(CC) -o $@ $(CFLAGS) $^ -ljsmn

test:
	

clean:
	rm -f *.$(EXECSUFFIX)

.PHONY: all clean test
