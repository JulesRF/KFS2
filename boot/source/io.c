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
