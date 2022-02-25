all: pico_http_server

clean:
	@rm -rf *.o
	@rm -rf pico_http_server

pico_http_server: main.o httpd.o
	gcc -o pico_http_server $^

main.o: main.c httpd.h
	gcc -c -o main.o main.c

httpd.o: httpd.c httpd.h
	gcc -c -o httpd.o httpd.c

