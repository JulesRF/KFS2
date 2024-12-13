/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:06:33 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/12 14:20:32 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"
#define SIZE_COMMAND 29

int		isShiftPressed = 0;
int		isCapsPressed  = 0;
int		isCtrlPressed  = 0;
char	current_commands[SIZE_COMMAND];
int		commands_index = 0;

char*	scancode_strings[] = {

		"ERROR",
		""/*ESC*/, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", ""/*BACKSPACE*/,
		""/*TAB*/,	  "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\n",
		""/*L_CTRL*/,	 "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`",
		""/*L_SHFT*/, "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", ""/*R_SHFT*/, "*"/*NUMPAD STAR*/,
				""/*L_ALT*/, " "/*SPACE*/, "" /*CAPS_LOCK*/
};

char*	scancode_shift[] = {

		"ERROR",
		""/*ESC*/, "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", ""/*BACKSPACE*/,
		""/*TAB*/,	  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "\n",
		""/*L_CTRL*/,	 "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "~",
		""/*L_SHFT*/, "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", ""/*R_SHFT*/, "*"/*NUMPAD STAR*/,
				""/*L_ALT*/, " "/*SPACE*/, "" /*CAPS_LOCK*/  
};

// ESC			0x01
// BACKSPACE	0x0E
// TAB			0x0F
// L_CTRL		0x1D
// L_SHFT		0x2A
// R_SHFT		0x36
// L_ALT		0x38

uint8	keyboard_read_input() {

	while(!(inb(0x64) & 1));
	return inb(0x60);
}

void	print_letters(uint8 scancode) {
	
	int	 temp_color = WHITE;

	// Check if scancode is within valid range
	if (scancode <= 0x3A) {

		// SHIFT PRESS
		if ((scancode == 0x2A) || scancode == 0x36 )
			isShiftPressed = 1;
		// LEFT CONTROL PRESS
		if (scancode == 0x1D)
			isCtrlPressed = 1;
		// BACKSPACE PRESS
		if (scancode == 0x0E) {
			ft_backspace();
			if (commands_index > 0)
			{
				current_commands[commands_index - 1] = ' ';
				commands_index--;
				print_debug(current_commands, RED);
			}
		}
		// CAPSLOCK PRESS
		if (scancode == 0x3A) {

			if (isCapsPressed == 1)
				isCapsPressed = 0;
			else
				isCapsPressed = 1;
		}
		// CTRL SHORTCUTS HANDLER
		if (isCtrlPressed == 1) {

			if (scancode == 0x26) {

				clear_screen(100);
				clear_screen(screen);
				ft_prompt();
				reset_cursor();
				print_string(current_commands, WHITE);
			}
			// TODO faire le ctrl + backspace pour retirer un mot entier
			if (scancode == 0x0E) {
				
			}
		}
		// SHIFT HANDLER
		else if (isShiftPressed == 1 || isCapsPressed == 1)
		{
			print_string(scancode_shift[scancode], temp_color);
			if (scancode != 0x1C && scancode != 0x2A && scancode != 0x36 && scancode != 0x0E && commands_index < SIZE_COMMAND - 1) {

				current_commands[commands_index] = scancode_shift[scancode][0];
				commands_index++;
				// print_debug(current_commands, RED);
			}
		}
		else
		{
			print_string(scancode_strings[scancode], temp_color);
			if (scancode != 0x1C && scancode != 0x0E && commands_index < SIZE_COMMAND - 1) {

				current_commands[commands_index] = scancode_strings[scancode][0];
				commands_index++;
				// print_debug(current_commands, RED);
			}
		}
		// ENTER HANDLER
		if (scancode == 0x1C && isCtrlPressed == 0) {

			current_commands[SIZE_COMMAND] = '\0';
			// print_debug(current_commands, RED);
			interpretor(current_commands);
			for (int i = 0; i < SIZE_COMMAND; i++)
				current_commands[i] = '\0';
			commands_index = 0;


			print_string("kfs-2 > ", L_BLUE);
			line_size[screen] = 0;
			reset_cursor();
			// TODO
		}
		// F1-F10 PRESS
	} else if (scancode >= 0x3B && scancode <=0x44 ) {
		
		if (scancode == 0x3B) { // F1

		   ft_switch_screen();
		   reset_cursor();
		}
		if (scancode == 0x3C) { // F2
			
		}
		// KEY RELEASE
	} else if(scancode <= 0x39 + 0x80) {

		// SHIFT RELEASE
		if ((scancode - 0x80 == 0x2A) || (scancode - 0x80 == 0x36))
			isShiftPressed = 0;
		// CTRL RELEASE
		if (scancode - 0x80 == 0x1D)
			isCtrlPressed = 0;
		// ARROWS HANDLER
	} else if (scancode == 0xCB || scancode == 0xC8 || scancode == 0xCD || scancode == 0xD0) {

		switch (scancode) {												
			case 0xCB:
				// print_string("left", L_CYAN);
				break;
			case 0xC8:
				// print_string("up", L_CYAN);
				break;
			case 0xCD:
				// print_string("right", L_CYAN);
				break;
			case 0xD0:
				// print_string("down", L_CYAN);
				break;
		}
		// scancode debug
	} else {

		// print_string("Unknown key\n", temp_color);
		// ft_putnbr_hex(scancode, RED);
	}
	putnbr_debug(commands_index, RED);
}

void	keyboard_init() {
	
	uint8 scancode = keyboard_read_input();
	print_letters(scancode);
}
