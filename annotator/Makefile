CXX = g++
CFLAGS = -fno-rtti

LLVM_SRC_PATH = /cygdrive/c/Users/catalyst/Documents/coverage/llvm-3.3.src
LLVM_BUILD_PATH = /cygdrive/c/Users/catalyst/Documents/coverage/build

LLVM_BIN_PATH = $(LLVM_BUILD_PATH)/Release+Asserts/bin
LLVM_LIBS=core mc bitreader bitwriter all
LLVM_COMP_CONFIG_COMMAND:= $(shell $(LLVM_BIN_PATH)/llvm-config --cxxflags)
LLVM_LINK_CONFIG_COMMAND:= $(shell $(LLVM_BIN_PATH)/llvm-config --ldflags \
                                        --libs $(LLVM_LIBS))
CLANG_BUILD_FLAGS = -I$(LLVM_SRC_PATH)/tools/clang/include \
                                      -I$(LLVM_BUILD_PATH)/tools/clang/include

CLANGLIBS = \
  -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse \
  -lclangSema -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore \
  -lclangAnalysis -lclangARCMigrate -lclangRewriteCore \
  -lclangEdit -lclangAST -lclangLex -lclangBasic \
  -lclangRewriteFrontend

APP=coverage-annotator.exe
OBJS=Annotator.o coverage-annotator.o

all: $(APP)

%.o: %.cpp
	$(CXX) $< -c $(CFLAGS) -o $@ \
              $(CLANG_BUILD_FLAGS) $(LLVM_COMP_CONFIG_COMMAND)

$(APP): $(OBJS)
	$(CXX) $(OBJS) $(CFLAGS) -o $@ \
              $(CLANG_BUILD_FLAGS) $(CLANGLIBS) $(LLVM_LINK_CONFIG_COMMAND)

clean:
	rm -rf $(OBJS) $(APP)

