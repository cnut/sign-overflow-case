.PHONY: all
all: O3 O0 O3check disassemble

O3:
	$(CC) -g -O3 main.cc sim_seek.cc -o overflow-under-O3
	$(CC) -g -O3 main_seek.cc -o combine-under-O3

O0:
	$(CC) -g -O0 main.cc sim_seek.cc -o overflow-under-O0
	$(CC) -g -O0 main_seek.cc -o combine-under-O0

O3check:
	$(CC) -g -fsanitize=signed-integer-overflow -O3 main.cc sim_seek.cc -o overflow-under-O3check
	$(CC) -g -fsanitize=signed-integer-overflow -O3 main_seek.cc -o combine-under-O3check

disassemble:
	objdump -S overflow-under-O3 > overflow-under-O3.asm
	objdump -S overflow-under-O0 > overflow-under-O0.asm
	objdump -S overflow-under-O3check > overflow-under-O3check.asm
	objdump -S combine-under-O3 > combine-under-O3.asm
	objdump -S combine-under-O0 > combine-under-O0.asm
	objdump -S combine-under-O3check > combine-under-O3check.asm


clean:
	rm -fr overflow-under-* combine-under-*
