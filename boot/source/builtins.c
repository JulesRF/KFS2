/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:14 by rdel-agu          #+#    #+#             */
/*   Updated: 2025/03/13 16:27:41 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"
#define GET_STACK_POINTER(x)	asm volatile("mov %%esp, %0" : "=r"(x) ::)
#define GET_STACK_FRAME(x)		asm volatile("mov %%ebp, %0" : "=r"(x) ::)

void	hex_to_str(unsigned int addr, char *result, int size)
{
	int	len;
	char	base_str[16] = "0123456789abcdef";

	len = size - 1;
	ft_memset(result, '0', size);
	result[size - 1] = 0;
	while (addr != 0)
	{
		result[--len] = base_str[addr % 16];
		addr = addr / 16;
	}
}

void	hexdump(uint32 addr, int limit)
{
	char *c = (char *)addr;
	char str_addr[9];
	char tmpaddr[9];
	int i;
	uint32 previous;

	print_string("\n", WHITE);
	if (limit <= 0)
		return;
	for (i = 0; i < limit; i++)
	{
		if ((i % 16) == 0) // 16 = size line
		{
			if (i != 0)
			{
				previous = addr - 16;
				while (previous < addr)
				{
					if (*(char *)previous <= 32)
						print_char('.', RED);
					else
						print_char(*(char *)previous, L_GREEN);
					previous++;
				}
				print_string("\n", WHITE);
			}
			if ((uint32)0x00000800 == addr)
			{
				ft_memset(tmpaddr, 0, sizeof(tmpaddr));
				hex_to_str(addr, tmpaddr ,sizeof(addr));
				print_string("0x", L_GREEN);
				print_string(tmpaddr, L_GREEN);
				print_string(": ", L_GREEN);
				// print_string("\n", CYAN);
				// ft_putnbr_hex(addr, CYAN);

			}
			else
			{
				// char tmpaddr[9];
				ft_memset(tmpaddr, 0, sizeof(tmpaddr));
				hex_to_str(addr, tmpaddr ,sizeof(addr));
				print_string("0x", WHITE);
				print_string(tmpaddr, WHITE);
				// print_string("\n", L_BLUE);
				// ft_putnbr_hex(addr, L_BLUE);
				print_string(": ", WHITE);

			}
		}
		hex_to_str((uint32)c[i], str_addr, 3);
		if ((uint32)c[i] == 0) // == 00
		{
			print_string(str_addr, BROWN);
			print_string(" ", WHITE);
		}
		else
		{
			print_string(str_addr, L_GREEN);
			print_string(" ", WHITE);
		}
		addr++;
	}
	// print_debug(ft_itoa(i), RED);
	for (i = 0; i < ((limit % 16) * 3); i++) // last line
		print_char(' ', WHITE);
	if ((limit % 16) == 0)
		previous = addr - 16;
	else
		previous = addr - (limit % 16);
	while (previous < addr)
	{
		if (*(char *)previous <= 32)
			print_char('.', RED);
		else
			print_char(*(char *)previous, L_GREEN);
		previous++;
	}
	print_string("\n", WHITE);
}

void	print_gdtr( void ) {

	struct {
		uint16 limit;
		uint32 base;
	} gdtr;
	asm volatile ("sgdt %0" : "=m"(gdtr));
	print_string("\n0x", WHITE);
	ft_putnbr_hex( gdtr.base , L_RED);
	print_string("\n", WHITE);
}


// uint32_t read_gdt_base() {
//     uint16 size;
//     uint32 base;

//     asm volatile (
//         "sgdt %[ptr]"      // Store GDT pointer into `ptr`
//         : [ptr] "=m" ((struct { uint16 limit; uint32 base; }) { size, base })
//     );

//     return base;  // Return the GDT base address
// }


void	print_stack_thing( void )
{
	uint32	esp;
	// uint32	*ebp;
	int		i = 0;

	asm volatile ("mov %%esp, %0" : "=r"(esp));
	// asm volatile ("mov %%ebp, %0" : "=r"(ebp));

	print_string("\n", L_BLUE);
	while (i < 15)
	{
		// if (i % 3 == 0)
		// 	print_string("\n", WHITE);
		// else
		// print_string(" | ", WHITE);
		uint32 *ptr = (uint32 *)(esp + i * 4);
		print_string("0x", WHITE);
		ft_putnbr_hex(esp + i * 4, L_RED);
		print_string(" -> ", WHITE);
		print_string("0x", WHITE);
		ft_putnbr_hex(ptr[i] * 4, L_RED);
		// print_char(esp, BLUE);
		print_string(" ", RED);
		// ft_putnbr(i, BLUE);
		print_string("\n", RED);
		i++;
	}
	i = 0;
	// print_string("\n", RED);

	// ft_putnbr_hex(ebp, L_RED);
	// print_string("\n", WHITE);

}

int		ft_is_only_space(char *str) {
	
	int	i;

	i = 0;
	while (str[i]) {
		
		if (str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int		ft_is_nl(char *str)
{
	if (ft_strlen(str) == 0)
		return (0);
	return (1);
}

void    interpretor(char *str)
{
    if (ft_strcmp(str, "help") == 0)
	{
		print_string("\n--- HELP ---\n", WHITE);
		print_string("help      print a short builtin man\n", WHITE);
		print_string("reboot    Reboots the kernel\n", WHITE);
		print_string("poweroff  Powers off the machine\n", WHITE);
		print_string("halt      Stops CPU processes\n", WHITE);
		print_string("stack     prints the kernel stack\n", WHITE);
		print_string("gdt       prints memory around 0x800 to prove that the GDT is set\n", WHITE);
	}
	else if (ft_strcmp(str, "stack") == 0) {
		// print_stack_thing();
		uint32 sp;
		int sf;
		GET_STACK_POINTER(sp);
		GET_STACK_FRAME(sf);
		hexdump(sp, sf - sp);
	}
	else if (ft_strcmp(str, "gdt") == 0) {
		hexdump(0x000007c0, 100);
	}
	else if (ft_strcmp(str, "halt") == 0) {
		halt();
	}
	else if (ft_strcmp(str, "reboot") == 0) {
		reboot();
	}
	else if (ft_strcmp(str, "poweroff") == 0) {
		poweroff();
	}
	else if (ft_is_nl(str) == 0)
		print_string("\n", WHITE);
	else if (ft_is_only_space(str) == 0)
		print_string("\n", WHITE);
	else{
		print_string("\ncommand not found: ", WHITE);
		print_string(str, WHITE);
		print_string("\n", WHITE);
	}
}
