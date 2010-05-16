pop3_server: pop3_server.exe

pop3_server.exe: pop3_tables.o server.o
	gcc pop3_tables.o server.o -o pop3_server.exe -lws2_32

.c.o: pop3.h
	gcc -c $<
