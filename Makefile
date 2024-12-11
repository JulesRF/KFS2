# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/29 14:00:09 by rdel-agu          #+#    #+#              #
#    Updated: 2024/12/11 10:53:11 by rdel-agu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = kfs

BIN = $(NAME).bin

ISO_DIR = iso

ISO = $(NAME).iso

CC = gcc

LD = ld

NASM = nasm

RM = rm

CFLAGS = -fno-builtin \
		 -fno-exceptions \
		 -fno-stack-protector \
		 -nostdlib \
		 -nodefaultlibs \
		 -Wall \
		 -Werror \
		 -Wextra \
		 -g3 \
		 -m32

CFILES = boot/source/kernel.c \
		 boot/source/print.c \
		 boot/source/shell-ter.c \
		 boot/source/lib.c \
		 boot/source/io.c \
		 boot/source/keyboard.c \
		 boot/source/gdt.c

OBJ = $(CFILES:.c=.o)

CHEADER = boot/source/include/
		 
CFG = boot/grub/grub.cfg

BOOT = boot/boot.asm

BOOT_OBJ = $(BOOT:.asm=.o)

LINKER = boot/linker.ld

.PHONY: all boot link iso clean fclean run kernel

all: boot $(OBJ) link iso 

boot : $(BOOT_OBJ)
	@echo " \033[0;31mPhase boot\033[30m"
	$(NASM) -f elf32 $(BOOT) -o $(BOOT_OBJ)
	@echo " \033[0;32mDone.\033[0m"

%.o: %.c
	@echo " \033[0;31mPhase C vers obj\033[30m"
	$(CC) $(CFLAGS)  -c $< -o $@
	@echo " \033[0;32mDone.\033[0m"

%.o : %.asm
	@echo " \033[0;31mPhase asm vers obj\033[30m"
	$(NASM) -f elf32 -g -F dwarf $< -o $@
	@echo " \033[0;32mDone.\033[0m"

link: $(BOOT_OBJ) $(OBJ)
	@echo " \033[0;31mPhase link\033[30m"
	$(LD) -m elf_i386 -T $(LINKER) -o $(BIN) $(BOOT_OBJ) $(OBJ)
	@echo " \033[0;32mDone.\033[0m"

iso:
	@echo " \033[0;31mPhase iso\033[30m"
	mkdir -pv $(ISO_DIR)/boot/grub
	cp $(BIN) $(ISO_DIR)/boot/$(BIN)
	cp $(CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)
	@echo " \033[0;32mDone.\033[0m"

clean:
	@echo " \033[0;31mCleaning objects!\033[30m"
	$(RM) -f $(BOOT_OBJ)
	$(RM) -f $(OBJ)
	@echo " \033[0;32mObjects eliminated!\033[0m"
		
fclean: clean
	@echo " \033[0;31mNow let's deep clean.\033[30m"
	$(RM) -f $(BIN)
	$(RM) -f $(ISO_DIR)/boot/$(BIN)
	$(RM) -f $(ISO_DIR)/boot/grub/$(CFG)
	$(RM) -rf $(ISO_DIR)
	$(RM) -f $(NAME).iso
	@echo " \033[0;32mDone.\033[0m"

re: fclean all

run:
	qemu-system-i386 -cdrom $(ISO)

kernel:
	qemu-system-i386 -kernel $(BIN)

gdb:
	qemu-system-i386 -s -S -kernel $(BIN)
