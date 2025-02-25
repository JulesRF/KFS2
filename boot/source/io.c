/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:05:09 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/11 11:47:26 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"

uint8	inb(uint16 port) {

	uint8	ret;

	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return (ret);
}

void	outb(uint16 port, uint8 value) {

	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void	outw(uint16 port, uint16 value) {

	asm volatile ("outw %0, %1" :: "a" (value), "Nd" (port));
}

void halt()
{
	asm volatile ("cli");
	asm volatile ("hlt");
}

void reboot()
{
    uint8 good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    halt();
}

void poweroff()
{
	outw(0x604, 0x2000);
}