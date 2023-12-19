all:
	gcc -c -fpic lt_zeromq.c
	gcc -shared -o liblt_zeromq.so lt_zeromq.o -lzmq
	gcc -c -fpic lt_api.c
	gcc -shared -o liblt_api.so lt_api.o
	gcc -c -fpic lt_posix.c
	gcc -shared -o liblt_posix.so lt_posix.o lt_api.o
	gcc -c -fpic lt_python3.c
	gcc -shared -o liblt_python3.so lt_python3.o lt_api.o