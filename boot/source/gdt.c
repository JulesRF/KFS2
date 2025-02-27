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



// Declare GDT and pointer in C
struct GDTEntry gdt[3];
struct GDTPtr gdtp;

// Set an entry in the GDT (index, base address, limit, access byte, granularity)
void set_gdt_entry(int index, uint32 base, uint32 limit, uint8 access, uint8 granularity) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

    gdt[index].access = access;
}

// Inline assembly to load the GDT
extern void load_gdt(struct GDTPtr* gdtp);

// // Initialize the GDT (Null, Code, Data)
void init_gdt() {
    uint32 gdt_base = 0x00000800;

    gdtp.limit = (sizeof(struct GDTEntry) * 3) - 1;
    gdtp.base = gdt_base;  // Set the GDT pointer base to 0x00000800

    // Null descriptor (must be zero)
    set_gdt_entry(0, 0, 0, 0, 0x0);

    set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xC); // KERNEL CODE
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xC); // KERNEL DATA
    set_gdt_entry(3, 0, 0xFFFFF, 0xFA, 0xC); // USER CODE
    set_gdt_entry(4, 0, 0xFFFFF, 0xF2, 0xC); // USER DATA
    set_gdt_entry(5, 0, 0xFFFFF, 0x89, 0x0); // TASK STATE

    // Load the GDT
    load_gdt(&gdtp);
}

// Function to read the GDT base address using `sgdt`
uint32 read_gdt_base() {
    struct {
        uint16 limit;
        uint32 base;
    } gdtp;  // Declare the struct to hold the GDT pointer information

    asm volatile (
        "sgdt %[ptr]"      // Store GDT pointer into `ptr`
        : [ptr] "=m" (gdtp)  // Store the result in the gdtp struct
    );

    return gdtp.base;  // Return the GDT base address
}

void verify_gdt() {
    uint32 gdt_base = read_gdt_base();
    print_string("GDT Base Address: ", L_GREEN);  // Assuming COLOR_GREEN is defined
    ft_putnbr_hex(gdt_base, L_GREEN);
    print_string("\n", L_GREEN);

    // Check if the GDT base is correct
    if (gdt_base == 0x00000800) {
        print_string("GDT base is correctly set to 0x00000800!\n", L_GREEN);
    } else {
        print_string("ERROR: GDT base is not correctly set.\n", L_RED);
    }
}