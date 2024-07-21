TARGET = kernel.elf
OBJS = main.o graphics.o font.o hankaku.o newlib_support.o console.o \
       asmfunc.o libcxx_support.o logger.o interrupt.o segment.o paging.o memory_manager.o \
       window.o layer.o timer.o frame_buffer.o acpi.o task.o syscall.o logo.o\
	   env/env.o env/cell.o env/codn.o env/compound.o env/DNA.o env/element.o env/list.o env/protein.o env/eval.o env/rand.o \
	   env/cushionmodel.o env/cushion/eval.o env/cushion/log.o env/cushion/finish.o
DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

CPPFLAGS += -I.
CFLAGS   += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJS)

kernel.elf: $(OBJS) Makefile
	~/osbook/devenv/clang+llvm-7.1.0-x86_64-linux-gnu-ubuntu-14.04/bin/ld.lld $(LDFLAGS) -o kernel.elf $(OBJS) -lc -lc++ -lc++abi

%.o: %.cpp Makefile
	~/osbook/devenv/clang+llvm-7.1.0-x86_64-linux-gnu-ubuntu-14.04/bin/clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.%.d: %.cpp
	~/osbook/devenv/clang+llvm-7.1.0-x86_64-linux-gnu-ubuntu-14.04/bin/clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

%.o: %.c Makefile
	~/osbook/devenv/clang+llvm-7.1.0-x86_64-linux-gnu-ubuntu-14.04/bin/clang $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.%.d: %.c
	~/osbook/devenv/clang+llvm-7.1.0-x86_64-linux-gnu-ubuntu-14.04/bin/clang $(CPPFLAGS) $(CFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.c=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

hankaku.bin: hankaku.txt
	../tools/makefont.py -o $@ $<

hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

.%.d: %.bin
	touch $@

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)
