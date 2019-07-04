# cpp-py-proto example

### Build
1. Generate protobuf sources.  
   `make protobuf`
2. Compile example program/  
   `scons`
3. Generate test data.  
   `python src/gen.py`
4. Run test.  
   `./build1/main`

### Notes

1. oneof bug with CPP @ d1305dde
