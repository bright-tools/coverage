TEST_FILES=$(wildcard *.c)

TEST_REPORTS=$(TEST_FILES:.c=.cannot)

ANNOTATE=../../../build4/bin/Debug/coverage.exe

all: $(TEST_REPORTS)

clean:
	rm -f $(TEST_REPORTS)

%.cannot: %.c $(ANNOTATE)
	$(ANNOTATE) $< -- > $@
