CFLAGS = -Wall -g
all: lib_test bindings_test

lib_test: test
	./test

test: tests/test.c tests/test.h liblicense.o
	$(CC) $(CFLAGS) -o $@ $^

liblicense.o: liblicense.c liblicense.h
	$(CC) $(CFLAGS) -c $^

bindings: python

bindings_test: bindings python_test

python: bindings/python/liblicense.c liblicense.o
	cd bindings/python/ && python setup.py build

python_test:
	python bindings/python/tests/test.py

clean:
	rm test liblicense.o bindings/python/build -rf
