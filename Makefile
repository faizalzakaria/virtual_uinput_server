all: server getevent

server:
	make -C src
	mkdir -p bin
	@cp -v src/$@ bin/

getevent:
	@cp -v src/$@ bin/

clean:
	make -C src clean
	@find . -name "server" -o -name "client" | xargs rm -rvf
	@rm -rf bin/*
	@rm -rf lib/*

