# cpp-py-proto example

### Build
1. Generate protobuf sources.  
   `make protobuf`
2. Compile example program.  
   `scons`
3. Generate test data.  
   `python src/gen.py`
4. Run test.  
   `./build1/main`

**One script**

```
make clean && scons -c && \
  make protobuf && scons && \
  python src/gen.py &&
  ./build1/main
```

### Notes

1. oneof bug with CPP @d1305dde
2. oneof bug also in @0.3.9.2 (3626b5c4)

    ```
    # Output of #1 and #2
    oneoftest: data/message1.pb
    top sfield=0x7ffd69e965f0, sub1 test=0x7ffd69e965d0
    s=this is the top, ptr=0x7ffd69e965f0
    OK: top.sfield=this is the top, sub1.text=
    40
    s=this is the top, ptr=0x7ffd69e965d0
    OK: top.sfield=this is the top, sub1.text=this is the top
    ```

3. oneof buf seems different @847ac296

    ```
    # Output of #3
    oneoftest: data/message1.pb
    top sfield=0x7ffc8eee0800, sub1 test=0x7ffc8eee07e0
    s=this is the top, ptr=0x7ffc8eee0800
    OK: top.sfield=this is the top, sub1.text=
    s=this is the top, ptr=0x7ffc8eee07e0
    OK: top.sfield=this is the top, sub1.text=this is the top
    ```

4. There an open issue [#175](https://github.com/nanopb/nanopb/issues/175)
