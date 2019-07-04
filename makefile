PHONY := protobuf main

PROTOC = protoc
PROTOC_C_PLUGIN = --plugin=protoc-gen-nanopb=nanopb/generator/protoc-gen-nanopb

PROTO_SPECS  = $(wildcard src/*.proto)
PROTO_C_SOURCE = $(patsubst %.proto,%.pb.c,$(PROTO_SPECS))
PROTO_H_SOURCE = $(patsubst %.proto,%.pb.h,$(PROTO_SPECS))
PROTO_PY_SOURCE = $(patsubst %.proto,%_pb2.py,$(PROTO_SPECS))
PROTO_SOURCE = $(PROTO_C_SOURCE) $(PROTO_H_SOURCE) $(PROTO_PY_SOURCE)
PROTO_C_RUNTIME = $(wildcard nanopb/*.c)

all: protobuf

protobuf: $(PROTO_SOURCE)

%.pb.c %.pb.h: %.proto
	$(PROTOC) $(PROTOC_C_PLUGIN) --nanopb_out=. $^

%_pb2.py: %.proto
	protoc --python_out=. $^

clean:
	rm -rf $(PROTO_SOURCE) main.o