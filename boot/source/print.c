#include "include/kfs.h"


uint16	*vga_buffer;
uint16	terminal_buffer[2][VGA_ADDRESS];
uint32	terminal_index[2];
uint32	vga_index;
uint32	screen = 0;
uint32	line_size[2];

void	clear_screen2(int this_screen)
{
	ft_memset(terminal_buffer[this_screen], 0x00, 80 * 25 * 2);
	terminal_index[this_screen] = 0;
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

void	print_string(char* str, unsigned char color)
{
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

void	ft_backspace()
{
	if (terminal_index[screen] % 80 != 0)
	{
		if (line_size[screen] <= 80 - 8 && terminal_index[screen] % 80 <= 8)
			return ;
		terminal_index[screen]--;
		terminal_buffer[screen][terminal_index[screen]] = ' ';
		vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		line_size[screen]--;
	}
	else if (terminal_index[screen] % 80 == 0)
	{
		terminal_index[screen]--;
		terminal_buffer[screen][terminal_index[screen]] = ' ';
		vga_buffer[terminal_index[screen]] = (unsigned short)' ' | (unsigned short)WHITE << 8;
		line_size[screen]--;
	}
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
    print_string("|     dP42  oP\"Yb.   |    Welcome to KFS-1!               |    SCREEN          |", color42);
    print_string("|    dP 42  \"\' dP\'   |                                    |    CAPS_LOCK       |", color42);
    print_string("|   d242424   dP'    |    Made by jroux-fo && rdel-agu    |    SHIFT           |", color42);
    print_string("|       42  .d4242   |                                    |    CTRL            |", color42);
    print_string("*--------------------*------------------------------------*--------------------*\n", color42);

	line_size[screen] = 0;
    print_string("kfs-1 > ", L_BLUE);
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

// pitie laisse moi push
