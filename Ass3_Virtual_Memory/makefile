all: mykernel clean
mykernel: kernel.o shell.o interpreter.o shellmemory.o cpu.o pcb.o memorymanager.o
	gcc -o mykernel kernel.o shell.o interpreter.o shellmemory.o cpu.o pcb.o memorymanager.o
shell.o: shell.c interpreter.h shellmemory.h
	gcc -c shell.c
interpreter.o: interpreter.c shellmemory.h shell.h memorymanager.h
	gcc -c interpreter.c 
shellmemory.o: shellmemory.c
	gcc -c shellmemory.c
cpu.o: cpu.c
	gcc -c cpu.c
pcb.o: pcb.c
	gcc -c pcb.c
kernel.o: kernel.c cpu.h shell.h pcb.h
	gcc -c kernel.c
memorymanager.o: memorymanager.c 
	gcc -c memorymanager.c
clean:
	rm -f *.o
