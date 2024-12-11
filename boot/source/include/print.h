#ifndef PRINT_H
# define PRINT_H

# define VGA_ADDRESS 0xB8000
# define VGA_SIZE 80 * 25

# define BLACK 0
# define BLUE 1
# define GREEN 2
# define CYAN 3
# define RED 4
# define MAGENTA 5
# define BROWN 6
# define L_GREY 7
# define D_GREY 8
# define L_BLUE 9
# define L_GREEN 10
# define L_CYAN 11
# define L_RED 12
# define L_MAGENTA 13
# define YELLOW 14
# define WHITE 15

# include "kfs.h"

extern uint16	*vga_buffer;
extern uint16	terminal_buffer[2][VGA_ADDRESS];
extern uint32	terminal_index[2];
extern uint32	vga_index;
extern uint32	screen;
extern uint32	line_size[2];

void    clear_screen(int screen);
void	ft_switch_screen();
void    print_string(char* str, unsigned char color);
void    print_char(char str, unsigned char color);
void	ft_backspace();
int     ft_isnewl(const char *str, int i);
void	ft_prompt();
void	ft_putnbr_hex(int nbr, unsigned char color);
void	ft_putnbr(int nbr, unsigned char color);
void    ft_printerr(char *str);
void	ft_ctrl_backspace();
void    *ft_memset(void *b, int c, uint16 len);
void	print_status( void );

#endif