TEST_FILES=$(wildcard *.c)

TEST_REPORTS=$(foreach F,$(TEST_FILES:.c=.cannot),out/$F)

ANNOTATE=../../../build4/bin/Debug/coverage.exe
OUTPUT_DIR=out

all: $(OUTPUT_DIR) $(TEST_REPORTS)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

clean:
	rm -f $(TEST_REPORTS)

$(OUTPUT_DIR)/%.cannot: %.c $(ANNOTATE)
	$(ANNOTATE) $< -- > $@
