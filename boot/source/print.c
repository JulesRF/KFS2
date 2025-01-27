#include "include/kfs.h"


uint16	*vga_buffer;
uint16	terminal_buffer[2][VGA_ADDRESS];
uint32	terminal_index[2];
uint32	cursor_index[2] = {5000, 5000};
uint32	vga_index;
uint32	screen = 0;
uint32	line_size[2];

void	clear_screen2(int this_screen)
{
	ft_memset(terminal_buffer[this_screen], 0x00, 80 * 25 * 2);
	terminal_index[this_screen] = 0;
	cursor_index[this_screen] = 5000;
	line_size[this_screen] = 0;
}

void	clear_screen(int this_screen)
{
	if (this_screen == 100)
	{
		ft_memset(vga_buffer, 0x00, 80 * 25 * 2);
		vga_index = 0;
	}
	else
		clear_screen2(this_screen);
}

void	ft_switch_screen()
{
	unsigned int index = 0;

	clear_screen(100);
	if (screen == 0)
		screen = 1;
	else if (screen == 1)
		screen = 0;
	while (index < 80 * 25 * 2)
	{
		vga_buffer[index] = terminal_buffer[screen][index];
		index++;
	}
	vga_index = terminal_index[screen];
	print_status();	
}

void	ft_scroll_screen()
{
	int	index = 7 * 80;

	while(index < 80 * 25 * 2 - 80)
	{
		terminal_buffer[screen][index] = terminal_buffer[screen][index + 80];
		vga_buffer[index] = vga_buffer[index + 80];
		index++;
	}
	ft_memset(terminal_buffer[screen] + index, 0x00, 80 * 2);
	ft_memset(vga_buffer + index, 0x00, 80 * 2);
	terminal_index[screen] -= 80;
	reset_cursor();
}

void	ft_print_shifted(char *str, unsigned char color)
{
	char buffer[256];
	unsigned int index = 0;
	unsigned int jndex = 0;
	unsigned int kndex = 0;

	// copy command to buffer
	while (current_commands[screen][index])
	{
		if (current_commands[screen][index] == '\n')
			return ;
		buffer[index] = current_commands[screen][index];
		index++;
	}
	buffer[index] = '\0';

	//clean command from cursor_index to end
	index = commands_index[screen] - (terminal_index[screen] - cursor_index[screen]); // ex: 5 = 10 - (100 - 95)
	jndex = cursor_index[screen];													  // ex: 95 = 95
	while (jndex <= terminal_index[screen])											  // ex: 95 <= 100
	{
		current_commands[screen][index] = ' ';
		terminal_buffer[screen][jndex] = ' ';
		vga_buffer[jndex] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		index++;
		jndex++;
	}

	//write the added string/character at the cursor index
	index = commands_index[screen] - (terminal_index[screen] - cursor_index[screen]);
	jndex = 0;
	while (str[jndex])
	{
		current_commands[screen][index] = str[jndex];
		terminal_buffer[screen][cursor_index[screen]] = (unsigned short)str[jndex] | (unsigned short)color << 8;
		vga_buffer[cursor_index[screen]] = terminal_buffer[screen][cursor_index[screen]];
		jndex++;
		index++;
		terminal_index[screen]++;
		commands_index[screen]++;
		cursor_index[screen]++;
		line_size[screen]++;
	}

	//copy the content of buffer into command and vga
	index = commands_index[screen] - (terminal_index[screen] - cursor_index[screen] + jndex);
	// jndex = commands_index[screen];
	kndex = 0;
	while(buffer[index])
	{
		current_commands[screen][index + 1] = buffer[index];
		terminal_buffer[screen][cursor_index[screen] + kndex] = (unsigned short)buffer[index] | (unsigned short)color << 8;
		vga_buffer[cursor_index[screen] + kndex] = terminal_buffer[screen][cursor_index[screen] + kndex];
		index++;
		// jndex++;
		kndex++;
		terminal_index[screen]++;
		commands_index[screen]++;
	}
	kndex++;
	while (index <= 257)
	{
		current_commands[screen][index + 1] = '\0';
		terminal_buffer[screen][cursor_index[screen] + kndex] = '\0';
		vga_buffer[cursor_index[screen] + kndex] = terminal_buffer[screen][cursor_index[screen] + kndex];
		kndex++;
		index++;
	}
	commands_index[screen]--;
	// print_debug(current_commands[screen], WHITE);
}

void	print_string(char* str, unsigned char color)
{
	if ((cursor_index[screen] != terminal_index[screen]) && cursor_index[screen] != 5000)
	{
		// print_debug("cursor_index[screen] != terminal_index[screen]", WHITE);
		ft_print_shifted(str, color);
		return;
	}
	else if (cursor_index[screen] == terminal_index[screen])
		cursor_index[screen] = 5000;
    int index = 0;
    while (str[index])
    {
        ft_isnewl(str, index);
		if (str[index] == 8)
			ft_backspace();
		else if (str[index] != '\n')
		{
			if (terminal_index[screen] == 80 * 25)
				ft_scroll_screen();
	        terminal_buffer[screen][terminal_index[screen]] = (unsigned short)str[index] | (unsigned short)color << 8;
			vga_buffer[terminal_index[screen]] = terminal_buffer[screen][terminal_index[screen]];
		}
		terminal_index[screen]++;
        index++;
		line_size[screen]++;
    }
}

void	print_char(char str, unsigned char color)
{
    int index = 0;
    
    terminal_buffer[screen][terminal_index[screen]] = str | (unsigned short)color << 8;
	vga_buffer[terminal_index[screen]] = terminal_buffer[screen][terminal_index[screen]];

    index++;
    terminal_index[screen]++;
}

void	ft_goback()
{
	if (cursor_index[screen] == 5000)
		cursor_index[screen] = terminal_index[screen];
	// if (cursor_index[screen] % 80 != 0)
	// {
	if (line_size[screen] <= 80 - 8 && cursor_index[screen] % 80 <= 8)
		return ;
	cursor_index[screen]--;
	// print_debug("CA MARCHE", WHITE);
	// }
	// else if (cursor_index[screen] % 80 )
}

void	ft_goforward()
{
	if (cursor_index[screen] == 5000)
		cursor_index[screen] = terminal_index[screen];
	if (((cursor_index[screen] + 1 ) > terminal_index[screen]) && cursor_index[screen] != 5000)
		return ;
	if (terminal_buffer[screen][cursor_index[screen] + 1] == '\0')
		return ;
	cursor_index[screen]++;
}

void	ft_back_shift()
{
	char buffer[256];
	unsigned int index = 0;
	unsigned int jndex = 0;
	unsigned int kndex = 0;

	// 1string de base: "salut ca va?
	// 2string de base: "salut a va?"
	
	// copy command to buffer
	while (current_commands[screen][index])
	{
		if (current_commands[screen][index] == '\n')
			return ;
		buffer[index] = current_commands[screen][index];
		index++;
	}
	//CURRENT_COMMANDS: "salut ca va?"
	buffer[index] = '\0';

	//clean command from cursor_index to end
	index = commands_index[screen] - (terminal_index[screen] - cursor_index[screen]); // ex: 5 = 10 - (100 - 95)
	jndex = cursor_index[screen];													  // ex: 95 = 95
	while (jndex <= terminal_index[screen])											  // ex: 95 <= 100
	{
		current_commands[screen][index] = ' ';
		terminal_buffer[screen][jndex] = ' ';
		vga_buffer[jndex] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		index++;
		jndex++;
	}
	//CURRENT_COMMANDS: "salut c"

	//delete the character on the cursor and move back the cursor
	index = commands_index[screen] - (terminal_index[screen] - cursor_index[screen]);
	current_commands[screen][index - 1] = ' ';
	cursor_index[screen]--;
	terminal_buffer[screen][cursor_index[screen]] = ' ';
	vga_buffer[cursor_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
	// terminal_index[screen]--;
	line_size[screen]--;
	//CURRENT_COMMANDS: "salut "
	// print_debug("                                      ", WHITE);
	// print_debug(current_commands[screen], WHITE);

	//copy the content of buffer into command and vga
	kndex = 0;
	// index--;
	while(buffer[index])
	{
		current_commands[screen][index - 1] = buffer[index];
		terminal_buffer[screen][cursor_index[screen] + kndex] = (unsigned short)buffer[index] | (unsigned short)WHITE << 8;
		vga_buffer[cursor_index[screen] + kndex] = terminal_buffer[screen][cursor_index[screen] + kndex];
		index++;
		kndex++;
		// terminal_index[screen]++;
		// commands_index[screen]++;
	}
	kndex++;
	while (index <= 257)
	{
		current_commands[screen][index - 1] = '\0';
		terminal_buffer[screen][cursor_index[screen] + kndex] = '\0';
		vga_buffer[cursor_index[screen] + kndex] = terminal_buffer[screen][cursor_index[screen] + kndex];
		kndex++;
		index++;
	}
	//CURRENT_COMMANDS: "salut a va"
	// commands_index[screen]--;
}

void	ft_backspace()
{
	// if (terminal_index[screen] % 80 != 0)
	// {
	if ((cursor_index[screen] != terminal_index[screen]) && cursor_index[screen] != 5000)
	{
		if (cursor_index[screen] <= (terminal_index[screen] - commands_index[screen]))
			return ;
		ft_back_shift();
		// commands_index[screen]--;
		return;
	}
	else if (cursor_index[screen] == terminal_index[screen])
		cursor_index[screen] = 5000;
	if (line_size[screen] <= 80 - 8 && terminal_index[screen] % 80 <= 8)
		return ;
	terminal_index[screen]--;
	terminal_buffer[screen][terminal_index[screen]] = ' ';
	vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
	line_size[screen]--;
	// }
	// else if (terminal_index[screen] % 80 == 0)
	// {
	// 	terminal_index[screen]--;
	// 	terminal_buffer[screen][terminal_index[screen]] = ' ';
	// 	vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
	// 	line_size[screen]--;
	// }
}

int	ft_isnewl(const char *str, int i)
{
    if (str[i] == '\n')
    {
        terminal_index[screen] = terminal_index[screen] + 80 - (terminal_index[screen] % 80) - 1;
        return (1);
    }
    return (0);
}

void	ft_prompt()
{
	int color42 = BROWN;

    print_string("*--------------------*------------------------------------*--------------------*", color42);
    print_string("|     dP42  oP\"Yb.   |    Welcome to KFS-2!               |    SCREEN          |", color42);
    print_string("|    dP 42  \"\' dP\'   |                                    |    CAPS_LOCK       |", color42);
    print_string("|   d242424   dP'    |    Made by jroux-fo && rdel-agu    |    SHIFT           |", color42);
    print_string("|       42  .d4242   |                                    |    CTRL            |", color42);
    print_string("*--------------------*------------------------------------*--------------------*\n", color42);

	line_size[screen] = 0;
    print_string("kfs-2 > ", L_BLUE);
}

void	ft_putnbr_hex(int nbr, unsigned char color)
{
	char	base[16] = "0123456789ABCDEF";
	int		size = 16;

	if (nbr < 0)
	{
		print_char('-', color);
		nbr = nbr * -1;
	}
	if (nbr <= size)
		print_char(base[nbr % size], color);
	else
	{
		ft_putnbr_hex(nbr / size, color);
		print_char(base[nbr % size], color);
	}
}

void	ft_putnbr(int nbr, unsigned char color)
{
	if (nbr < 0)
	{
		print_char('-', WHITE);
		nbr = nbr * -1;
	}
	if (nbr <= 9)
		print_char(nbr + '0', color);
	else
	{
		ft_putnbr(nbr / 10, color);
		print_char(nbr % 10 + '0', color);
	}
}

void    ft_printerr(char *str)
{
    print_string(str, RED);
}

void    *ft_memset(void *b, int c, uint16 len)
{
    uint16            i;
    unsigned char    *dest;

    i = 0;
    dest = (unsigned char *)b;
    while (i < len)
    {
        dest[i] = (unsigned char)c;
        i++;
    }
    return (dest);
}

void	isOn(int status, int pos) {

	if (status == 0) {

		int	isOnColor = L_RED;
		vga_buffer[pos] = ('O') | isOnColor << 8;
		vga_buffer[pos + 1] = ('F') | isOnColor << 8;
		vga_buffer[pos + 2] = ('F') | isOnColor << 8;
	}
	else {

		int	isOnColor = L_GREEN;
		vga_buffer[pos] = ('O') | isOnColor << 8;
		vga_buffer[pos + 1] = ('N') | isOnColor << 8;
		vga_buffer[pos + 2] = (' ') | isOnColor << 8;
	}
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	i;

	i = 0;
	while (((unsigned char)s1[i] || (unsigned char)s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	print_status( void ) {

	vga_buffer[154] = (screen + '1') | WHITE << 8;
	isOn(isCapsPressed, 154 + 80);
	isOn(isShiftPressed, 154 + 80 * 2);
	isOn(isCtrlPressed, 154 + 80 * 3);
}

void	print_debug(char *str, unsigned char color) {

	int	index = 0;
	int	place = 346;
	
	while (str[index])
	{
		vga_buffer[place] = (unsigned short)str[index] | (unsigned short)color << 8;
		index++;
		place++;
	}
}

void	print_char_debug2(char str, unsigned char color)
{
    
	vga_buffer[346] = (unsigned short)str | (unsigned short)color << 8;
}


void	putnbr_debug2(int nbr, unsigned char color)
{
	if (nbr < 0)
	{
		print_char_debug('-', WHITE);
		nbr = nbr * -1;
	}
	if (nbr <= 9)
		print_char_debug(nbr + '0', color);
	else
	{
		putnbr_debug(nbr / 10, color);
		print_char_debug(nbr % 10 + '0', color);
	}
}


void	print_char_debug(char str, unsigned char color)
{
    
	vga_buffer[0] = (unsigned short)str | (unsigned short)color << 8;
}

void	putnbr_debug(int nbr, unsigned char color)
{
	if (nbr < 0)
	{
		print_char_debug('-', WHITE);
		nbr = nbr * -1;
	}
	if (nbr <= 9)
		print_char_debug(nbr + '0', color);
	else
	{
		putnbr_debug(nbr / 10, color);
		print_char_debug(nbr % 10 + '0', color);
	}
}

// pitie laisse moi push
