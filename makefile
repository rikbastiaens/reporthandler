CC = g++
CCFLAGS = -Wall
INCDIRS = -I/usr/include/mysql -I/usr/include/cppconn
LINKDIRS = -L/usr/include/mysql -L/usr/include/cppconn
LINKFLAGS = -lmysqlcppconn -lcgicc
OBJ = cplusplus.o

dbhandler.cgi: cplusplus.o
	$(CC) $(OBJ) -o dbhandler.cgi $(CCFLAGS) $(INCDIRS) $(LINKDIRS) $(LINKFLAGS) 
cplusplus.o:
	$(CC) -c cplusplus.cpp $(INCDIRS) $(LINKDIRS) $(LINKFLAGS)
clean:
	rm *.o dbhandler.cgi
