all: exempi sidecar_xmp

exempi.o: exempi.c
	gcc -fPIC -g -c -Wall exempi.c `pkg-config --cflags exempi-2.0`

exempi: exempi.o
	gcc -shared -Wl,-soname,ll_io_exempi.so.1  -o ll_io_exempi.so.1.0.0 exempi.o -lc `pkg-config --libs exempi-2.0`

sidecar_xmp.o: sidecar_xmp.c
	gcc -fPIC -g -c -Wall sidecar_xmp.c `pkg-config --cflags exempi-2.0`

sidecar_xmp: sidecar_xmp.o
	gcc -shared -Wl,-soname,ll_io_sidecar_xmp.so.1  -o ll_io_sidecar_xmp.so.1.0.0 sidecar_xmp.o -lc `pkg-config --libs exempi-2.0`


clean:
	rm -rf *.o *.so.*

