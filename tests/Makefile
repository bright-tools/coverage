TEST_FILES=$(wildcard *.c)

TEST_REPORTS=$(foreach F,$(TEST_FILES:.c=.cannot),out/$F)

ANNOTATE_CMD=../../../build4/bin/Debug/coverage.exe
ANNOTATE_OPT=-r=.
ANNOTATE=$(ANNOTATE_CMD) $(ANNOTATE_OPT)
OUTPUT_DIR=out
INDENT_CMD=indent
INDENT_OPT=-nut
# Could replace this with "cat" if "indent" not installed?
INDENT=$(INDENT_CMD) $(INDENT_OPT)

all: $(OUTPUT_DIR) $(TEST_REPORTS)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

clean:
	rm -f $(TEST_REPORTS)

$(OUTPUT_DIR)/%.cannot: %.c $(ANNOTATE_CMD)
	$(ANNOTATE) $< -- | $(INDENT) > $@
