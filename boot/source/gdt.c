/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:18 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/12 13:22:19 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"

struct gdt_entry gdt[3];
struct gdt_ptr gp;

void init_gdt() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32)&gdt;

    // Null segment
    gdt[0] = (struct gdt_entry){0, 0, 0, 0, 0, 0};

    // Code segment
    gdt[1] = (struct gdt_entry){
        .base_low = 0,
        .base_middle = 0,
        .base_high = 0,
        .limit_low = 0xFFFF,
        .granularity = 0xCF, // 4K granularity, 32-bit opcodes
        .access = 0x9A       // Code segment, executable, readable
    };

    // Data segment
    gdt[2] = (struct gdt_entry){
        .base_low = 0,
        .base_middle = 0,
        .base_high = 0,
        .limit_low = 0xFFFF,
        .granularity = 0xCF, // 4K granularity, 32-bit opcodes
        .access = 0x92       // Data segment, writable
    };
}