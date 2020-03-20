# Include the nanopb provided Makefile rules
include ../../extra/nanopb.mk

# Compiler flags to enable all warnings & debug info
CFLAGS = -Wall -Werror -g -O0
CFLAGS += -I$(NANOPB_DIR)
CXXFLAGS = -Wno-c++11-extensions

CXXC = g++

# C source code files that are required
CXXSRC  = benchmark.cpp      # The main program
CSRC += benchmark.pb.c     # The compiled protocol definition
CSRC += $(NANOPB_DIR)/pb_encode.c  # The nanopb encoder
CSRC += $(NANOPB_DIR)/pb_decode.c # The nanopb decoder
CSRC += $(NANOPB_DIR)/pb_common.c  # The nanopb common parts

# Build rule for the main program
benchmark: $(CSRC) $(CXXSRC)
	$(CXXC) $(CFLAGS) $(CXXFLAGS) -obenchmark $(CXXSRC) -x c $(CSRC)

# Build rule for the protocol
benchmark.pb.c: benchmark.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. benchmark.proto

