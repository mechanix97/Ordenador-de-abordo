CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3

INC_DIR = inc
SRC = $(wildcard src/*)
OBJECTS = $(foreach file, $(SRC), out/$(basename $(notdir $(file))).o)


all:  out $(OBJECTS) out/app.bin 
	@echo Building...
	@echo Done

out:
	@mkdir -p $@

out/%.o: src/%.c
	@echo CC $<
	@$(CC) $(CFLAGS) -c -o $@ $< -I$(INC_DIR)

out/crt.o: crt.s
	$(AS) -o out/crt.o crt.s

out/app.elf: linker.ld out/crt.o $(OBJECTS)
	$(LD) -T linker.ld -o out/app.elf out/crt.o out/*.o
out/app.bin: out/app.elf
	$(BIN) -O binary out/app.elf out/app.bin

clean:
	rm -rf out

flash: out/app.bin
	$(STL) write out/app.bin 0x8000000

erase:
	$(STL) erase

