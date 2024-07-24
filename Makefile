
OBJDIR=./.obj
SRCDIR=./
CXXSOURCES=main.cc
CSOURCES=smbgw.c smbproto.c butil.c gwtcp.c
OBJECTS=$(CXXSOURCES:%.cc=$(OBJDIR)/%.o) $(CSOURCES:%.c=$(OBJDIR)/%.o)

SMBGW_DIR=../smbgw
SMBGW_BUILD_DIR=../smbgw-build

OS_CFLAGS=

ifeq ($(OS),Windows_NT)
    OS_CFLAGS += -DWIN32
 	OS_CFLAGS += -Wl,--subsystem,console
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        CFLAGS += -DAMD64 -m32
		ARCH=x86_64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        OS_CFLAGS += -DIA32
		ARCH=x86_32
    endif
	SYSLIBS +=-lws2_32
else
ifeq ($(OS),iOS)
	CC=clang
	AR=libtool
	ARFLAGS=-static -o
    OS_CFLAGS += -DIOS -arch armv7 -arch arm64	\
                -fmessage-length=0 -fdiagnostics-show-note-include-stack \
                -fmacro-backtrace-limit=0 -Wno-trigraphs \
                -fpascal-strings -fno-common -Wno-missing-field-initializers \
                -fstrict-aliasing -Wprotocol -Wdeprecated-declarations -miphoneos-version-min=9.0 \
				-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS12.1.sdk \
				-Wno-sign-conversion -fembed-bitcode-marker
	ARCH=arm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_CFLAGS += -DLinux
		OS=Linux
		ifdef FORCE32
			CFLAGS += -m32
		endif
    endif
    ifeq ($(UNAME_S),Darwin)
        OS_CFLAGS += -DOSX
		OS=OSX
    endif
    UNAME_M := $(shell uname -m)
    ifeq ($(UNAME_M),x86_64)
        OS_CFLAGS += -DAMD64
		ARCH=x86_64
    else
    ifeq ($(UNAME_M),i386)
		ARCH=x86_32
    else
    ifneq ($(filter %86,$(UNAME_M)),)
        OS_CFLAGS += -DIA32
		ARCH=IA32
    else
    ifneq ($(filter arm%,$(UNAME_M)),)
        OS_CFLAGS += -DARM
		ARCH=arm
	else
		ARCH=unkown
    endif
    endif
    endif
    endif
endif
endif

CXX=g++
CXXFLAGS = -g -O0 -std=c++11 -Wno-deprecated-declarations $(OS_CFLAGS) \
	-I$(SMBGW_DIR)/include -I$(SMBGW_DIR)/lib  -I$(TINK_DIR) -I$(TINK_DIR)/cc	\
	-I$(TINK_BUILD_DIR)/__third_party/com_google_absl/src	\
	-I$(TINK_BUILD_DIR)/__third_party/com_google_protobuf/src/src \
	-I$(TINK_BUILD_DIR)/__generated

CC=gcc
CFLAGS = -g -O0  $(OS_CFLAGS) \
	-I$(SMBGW_DIR)/include -I$(SMBGW_DIR)/lib  -I$(TINK_DIR) -I$(TINK_DIR)/cc	\
	-I$(TINK_BUILD_DIR)/__third_party/com_google_absl/src	\
	-I$(TINK_BUILD_DIR)/__third_party/com_google_protobuf/src/src \
	-I$(TINK_BUILD_DIR)/__generated

LIBRARIES  =

ifeq ($(OS),Linux)
LIBRARIES += -lpthread
endif

ifdef HAS_TINK
LIBRARIES += TINK_LIBRARIES -levent -lcares
endif

FRAMEWORKS =
ifeq ($(OS),OSX)
FRAMEWORKS = -framework CoreFoundation
endif

smbgw: $(OBJECTS) $(LIBRARIES)
	@ echo link $@
	@ $(CXX) $(CXXLDFLAGS) $(FRAMEWORKS) -o $@ $^

main.o: main.cc aead.cc butul.c

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(OBJDIR)
	@echo c++ compile $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo c compile $<
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	@rm -rf $(OBJDIR)/*
	@rm smbgw

cleanall:
	@rm -rf $(OBJDIR)/*
	@rm smbgw
	@make -C ../smbgw-build clean

