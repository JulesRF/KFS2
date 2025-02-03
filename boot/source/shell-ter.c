#include "include/kfs.h"
#include "include/io.h"

//cursor component

uint16 get_cursor_position( void ) {

    uint16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;
    return (pos);
}

void    modify_cursor_position( uint16 pos ) {

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8) ((pos >> 8) & 0xFF));
}

void	reset_cursor( void )
{
	vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
	// vga_buffer[terminal_index[screen] + 1] = (unsigned short)' ' | (unsigned short)WHITE << 8;
	// uint16 tmp_pos = terminal_index[screen] + 1;
    uint16 tmp_pos;
    if (cursor_index[screen] == 5000)
    	tmp_pos = terminal_index[screen];
    else
        tmp_pos = cursor_index[screen];
	// modify_cursor_position(tmp_pos - 1);
    modify_cursor_position(tmp_pos);
	print_status();
}

void shell_ter( void ) {

	ft_prompt();
	screen = 1;
	ft_prompt();
	screen = 0;

		vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
    	// vga_buffer[terminal_index[screen] + 1] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		// uint16 tmp_pos = terminal_index[screen] + 1;
        uint16 tmp_pos;
        if (cursor_index[screen] == 5000)
	    	tmp_pos = terminal_index[screen];
        else
            tmp_pos = cursor_index[screen];
        // modify_cursor_position(tmp_pos - 1);
        modify_cursor_position(tmp_pos);
        print_status();
    
    while(69) {

        keyboard_init();
		vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		// vga_buffer[terminal_index[screen] + 1] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		// uint16 pos = terminal_index[screen + 1];
		// uint16 pos = terminal_index[screen];
        uint16 pos;
         if (cursor_index[screen] == 5000)
	    	pos = terminal_index[screen];
        else
            pos = cursor_index[screen];
        // modify_cursor_position(pos - 1);
        modify_cursor_position(pos);
        print_status();
    }
}
