#ifndef GDT_H
# define GDT_H
# include "kfs.h"
// Manually define fixed-width types (since we are not using stdint.h)
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;

/* GDT Entry Structure (8 bytes per entry) */
struct GDTEntry {
    uint16 limit_low;  // Lower 16 bits of segment limit
    uint16 base_low;   // Lower 16 bits of segment base
    uint8  base_middle; // Next 8 bits of segment base
    uint8  access;     // Access flags
    uint8  granularity; // Granularity and high limit
    uint8  base_high;  // Last 8 bits of segment base
} __attribute__((packed));

/* GDT Pointer Structure (6 bytes) */
struct GDTPtr {
    uint16 limit;  // Size of the GDT - 1
    uint32 base;   // Address of the GDT
} __attribute__((packed));

// Function to initialize and load the GDT
extern void load_gdt(struct GDTPtr* gdtp);
void		init_gdt(void);
void 		verify_gdt(void);


#endif
