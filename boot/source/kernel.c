#include "include/kfs.h"

int	main(void)
{
	//GDT TABLE INITIALISATION
	init_descriptor();

	//VGA INITIALISATION
	screen = 0;
	vga_buffer = (uint16 *)VGA_ADDRESS;
	vga_index = 0;

	//VGA CLEANING
	clear_screen(100);
	clear_screen(0);
	clear_screen(1);

	//STARTING SHELL
    shell_ter();
}
