/*
 *  $Id: identify.c,v 1.1 2001/12/20 16:49:11 davej Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  NatSemi specific parts.
 */
#include <stdio.h>
#include "../x86info.h"

void Identify_NatSemi (unsigned int maxi, struct cpudata *cpu)
{
	unsigned long eax, ebx, ecx, edx;
	cpu->vendor = VENDOR_NATSEMI;

	/* Do standard stuff */
	if (maxi >= 1) {
		cpuid (cpu->number, 1, &eax, &ebx, &ecx, &edx);
		cpu->stepping = eax & 0xf;
		cpu->model = (eax >> 4) & 0xf;
		cpu->family = (eax >> 8) & 0xf;

		switch (cpu->family) {
		case 5:
			switch (cpu->model) {
				case 4:		sprintf (cpu->name, "%s", "Geode GX1");
							break;
				default:	sprintf (cpu->name, "%s", "Unknown CPU");
							break;
			}
			break;

		default:
			printf ("Unknown CPU");
			break;
		}
	}
}


void display_NatSemi_info(unsigned int maxi, unsigned int maxei, struct cpudata *cpu)
{
	unsigned int i;
	unsigned long eax, ebx, ecx, edx, tmp=0;

	printf ("Family: %d Model: %d Stepping: %d [%s]\n",
		cpu->family, cpu->model, cpu->stepping, cpu->name);
	get_model_name (maxei, cpu);

	if (maxi != 0 && show_registers) {
		/* Dump extended info in raw hex */
		for (i = 0x00000000; i <= maxi; i++) {
			cpuid (cpu->number, i, &eax, &ebx, &ecx, &edx);
			printf ("eax in: 0x%x, eax = %08lx ebx = %08lx ecx = %08lx edx = %08lx\n", i, eax, ebx, ecx, edx);
		}
		printf ("\n");
	}

	if (maxei != 0 && show_registers) {
		/* Dump extended info in raw hex */
		for (i = 0x80000000; i <= maxei; i++) {
			cpuid (cpu->number, i, &eax, &ebx, &ecx, &edx);
			printf ("eax in: 0x%x, eax = %08lx ebx = %08lx ecx = %08lx edx = %08lx\n", i, eax, ebx, ecx, edx);
		}
		printf ("\n");
	}

	cpuid (cpu->number, 0x00000001, &eax, &ebx, &ecx, &edx);
	if (maxei >= 0x80000001)
		cpuid (cpu->number, 0x80000001, &eax, &ebx, &ecx, &tmp);
	decode_feature_flags (cpu, edx, tmp);
}
