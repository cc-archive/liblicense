CFLAGS = -Wall -g
all: lib_test bindings_test

lib_test: test
	./test

test: tests/test.c tests/test.h liblicense.a
	$(CC) $(CFLAGS) -o $@ $^ -lraptor

liblicense.a: write_license.o read_license.o system_default.o  module_wrangler.o system_licenses.o
	ar r $@ $^

system_default.o: system_default.c system_default.h
	$(CC) $(CFLAGS) -c $^

system_licenses.o: system_licenses.c system_licenses.h
	$(CC) $(CFLAGS) -c $^

write_license.o: write_license.c write_license.h
	$(CC) $(CFLAGS) -c $^

read_license.o: read_license.c read_license.h
	$(CC) $(CFLAGS) -c $^

module_wrangler.o: module_wrangler.c module_wrangler.h
	$(CC) $(CFLAGS) -c $^

bindings: python

bindings_test: bindings python_test

python: bindings/python/liblicense.c liblicense.a
	cd bindings/python/ && python setup.py build -f --build-lib ./tests

python_test:
	python bindings/python/tests/test.py

clean:
	rm test *.o *.a bindings/python/build bindings/python/tests/*.so -rf
