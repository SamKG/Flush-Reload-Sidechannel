
all:
	make -f Makefile_sender
	make -f Makefile_receiver
clean:
	make -f Makefile_sender clean
	make -f Makefile_receiver clean
