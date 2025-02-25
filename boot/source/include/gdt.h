#ifndef GDT_H
# define GDT_H
# include "kfs.h"
// Manually define fixed-width types (since we are not using stdint.h)
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;

// Structure for a GDT entry (8 bytes per entry)
struct gdt_entry
{
	uint16	limit_low;
	uint16	base_low;
	uint8	base_middle;
	uint8	access;
	uint8	granularity;
	uint8	base_high;
} __attribute__((packed));

// Structure to hold the GDT pointer
struct gdt_ptr
{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));

// Function to initialize and load the GDT
extern void	load_gdt(struct gdt_ptr*);
void		init_gdt(void);

#endif
