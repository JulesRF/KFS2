/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:14 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/20 15:28:33 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"

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
		print_string("halt      Stops CPU processes\n", WHITE);
		print_string("stack     prints the kernel stack\n", WHITE);
		print_string("clear     clears the screen (same as CTRL + L)\n", WHITE);
	}
	else if (ft_strcmp(str, "stack") == 0) {
		print_stack_thing();
	}
	else if (ft_strcmp(str, "gdt") == 0) {
		verify_gdt();
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