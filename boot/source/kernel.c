#include "include/kfs.h"

int	main(void)
{
	screen = 0;
	vga_buffer = (uint16 *)VGA_ADDRESS;
	vga_index = 0;

	clear_screen(100);
	clear_screen(0);
	clear_screen(1);
    shell_ter();
}
