/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qvy <qvy@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:18 by rdel-agu          #+#    #+#             */
/*   Updated: 2025/02/23 18:45:24 by qvy              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/gdt.h"

// Define GDT with 6 entries + 1 NULL entry
struct gdt_entry gdt[] = {
    {0, 0, 0, 0, 0, 0},                 // Null Descriptor (must be present)
    {0xFFFF, 0, 0, 0x9A, 0xCF, 0},      // Kernel Code Segment (0x08)
    {0xFFFF, 0, 0, 0x92, 0xCF, 0},      // Kernel Data Segment (0x10)
    {0xFFFF, 0, 0, 0x96, 0xCF, 0},      // Kernel Stack Segment (0x18)
    {0xFFFF, 0, 0, 0xFA, 0xCF, 0},      // User Code Segment (0x20)
    {0xFFFF, 0, 0, 0xF2, 0xCF, 0},      // User Data Segment (0x28)
    {0xFFFF, 0, 0, 0xF6, 0xCF, 0},      // User Stack Segment (0x30)
};
 
struct gdt_ptr gp = {
    sizeof(gdt) - 1,                    // GDT size (number of bytes - 1)
    (uint32)(unsigned long)&gdt
};

// Assembly function to load the GDT declared in boot.asm
extern void load_gdt(struct gdt_ptr*);

// Function to initialize and load the GDT
void init_gdt() {
    load_gdt(&gp);
}
