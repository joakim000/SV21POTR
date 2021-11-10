BUILD := build


all:
	@gcc -g -c lib/crc/crc_types.c -o $(BUILD)/types.o
	@gcc -g -c lib/crc/crc-w.c -o $(BUILD)/crc.o
	@gcc -g test/crc/test.c $(BUILD)/types.o $(BUILD)/crc.o -Ilib/crc -lunity -o build/main
	@./build/main