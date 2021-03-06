/* SPDX-License-Identifier: GPL-2.0-only */

#include <cpu/cpu.h>
#include <cpu/x86/lapic.h>
#include <cpu/x86/lapic_def.h>
#include <cpu/x86/msr.h>
#include <console/console.h>
#include <stdint.h>

void enable_lapic(void)
{
	msr_t msr;
	msr = rdmsr(LAPIC_BASE_MSR);
	msr.hi &= 0xffffff00;
	msr.lo &= ~LAPIC_BASE_MSR_ADDR_MASK;
	msr.lo |= LAPIC_DEFAULT_BASE;
	msr.lo |= LAPIC_BASE_MSR_ENABLE;
	wrmsr(LAPIC_BASE_MSR, msr);
}

void disable_lapic(void)
{
	msr_t msr;
	msr = rdmsr(LAPIC_BASE_MSR);
	msr.lo &= ~LAPIC_BASE_MSR_ENABLE;
	wrmsr(LAPIC_BASE_MSR, msr);
}

uintptr_t cpu_get_lapic_addr(void)
{
	return LAPIC_DEFAULT_BASE;
}

/* See if I need to initialize the local APIC */
static int need_lapic_init(void)
{
	return CONFIG(SMP) || CONFIG(IOAPIC);
}

static void lapic_virtual_wire_mode_init(void)
{
	/* this is so interrupts work. This is very limited scope --
	 * linux will do better later, we hope ...
	 */
	/* this is the first way we learned to do it. It fails on real SMP
	 * stuff. So we have to do things differently ...
	 * see the Intel mp1.4 spec, page A-3
	 */

	printk(BIOS_INFO, "Setting up local APIC...\n");

	/* Enable the local APIC */
	enable_lapic();

	/*
	 * Set Task Priority to 'accept all'.
	 */
	lapic_update32(LAPIC_TASKPRI, ~LAPIC_TPRI_MASK, 0);

	/* Put the local APIC in virtual wire mode */
	lapic_update32(LAPIC_SPIV, ~LAPIC_VECTOR_MASK, LAPIC_SPIV_ENABLE);

	uint32_t mask = LAPIC_LVT_MASKED | LAPIC_LVT_LEVEL_TRIGGER | LAPIC_LVT_REMOTE_IRR |
			LAPIC_INPUT_POLARITY | LAPIC_SEND_PENDING | LAPIC_LVT_RESERVED_1 |
			LAPIC_DELIVERY_MODE_MASK;

	lapic_update32(LAPIC_LVT0, ~mask, LAPIC_LVT_REMOTE_IRR | LAPIC_SEND_PENDING |
					  LAPIC_DELIVERY_MODE_EXTINT);

	lapic_update32(LAPIC_LVT1, ~mask, LAPIC_LVT_REMOTE_IRR | LAPIC_SEND_PENDING |
					  LAPIC_DELIVERY_MODE_NMI);

	printk(BIOS_DEBUG, " apic_id: 0x%x ", lapicid());
	printk(BIOS_INFO, "done.\n");
}

void setup_lapic(void)
{
	if (need_lapic_init())
		lapic_virtual_wire_mode_init();
	else
		disable_lapic();
}
