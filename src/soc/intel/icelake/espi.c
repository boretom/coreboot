/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/device.h>
#include <device/pci.h>
#include <pc80/isa-dma.h>
#include <pc80/i8259.h>
#include <arch/io.h>
#include <device/pci_ops.h>
#include <arch/ioapic.h>
#include <intelblocks/itss.h>
#include <intelblocks/lpc_lib.h>
#include <intelblocks/pcr.h>
#include <soc/espi.h>
#include <soc/iomap.h>
#include <soc/irq.h>
#include <soc/pci_devs.h>
#include <soc/pcr_ids.h>
#include <soc/soc_chip.h>

void soc_get_gen_io_dec_range(uint32_t *gen_io_dec)
{
	const config_t *config = config_of_soc();

	gen_io_dec[0] = config->gen1_dec;
	gen_io_dec[1] = config->gen2_dec;
	gen_io_dec[2] = config->gen3_dec;
	gen_io_dec[3] = config->gen4_dec;
}

void soc_setup_dmi_pcr_io_dec(uint32_t *gen_io_dec)
{
	/* Mirror these same settings in DMI PCR */
	pcr_write32(PID_DMI, PCR_DMI_LPCLGIR1, gen_io_dec[0]);
	pcr_write32(PID_DMI, PCR_DMI_LPCLGIR2, gen_io_dec[1]);
	pcr_write32(PID_DMI, PCR_DMI_LPCLGIR3, gen_io_dec[2]);
	pcr_write32(PID_DMI, PCR_DMI_LPCLGIR4, gen_io_dec[3]);
}

#if ENV_RAMSTAGE
static void soc_mirror_dmi_pcr_io_dec(void)
{
	struct device *dev = pcidev_on_root(PCH_DEV_SLOT_ESPI, 0);
	uint32_t io_dec_arr[] = {
		pci_read_config32(dev, ESPI_GEN1_DEC),
		pci_read_config32(dev, ESPI_GEN2_DEC),
		pci_read_config32(dev, ESPI_GEN3_DEC),
		pci_read_config32(dev, ESPI_GEN4_DEC),
	};
	/* Mirror these same settings in DMI PCR */
	soc_setup_dmi_pcr_io_dec(&io_dec_arr[0]);
}

void lpc_soc_init(struct device *dev)
{
	/* Legacy initialization */
	isa_dma_init();
	pch_misc_init();

	/* Enable CLKRUN_EN for power gating ESPI */
	lpc_enable_pci_clk_cntl();

	/* Set ESPI Serial IRQ mode */
	if (CONFIG(SERIRQ_CONTINUOUS_MODE))
		lpc_set_serirq_mode(SERIRQ_CONTINUOUS);
	else
		lpc_set_serirq_mode(SERIRQ_QUIET);

	/* Interrupt configuration */
	pch_enable_ioapic();
	pch_pirq_init();
	setup_i8259();
	i8259_configure_irq_trigger(9, 1);
	soc_mirror_dmi_pcr_io_dec();
}

/* Fill up ESPI IO resource structure inside SoC directory */
void pch_lpc_soc_fill_io_resources(struct device *dev)
{
	/*
	 * PMC pci device gets hidden from PCI bus due to Silicon
	 * policy hence bind ACPI BASE aka ABASE (offset 0x20) with
	 * ESPI IO resources to ensure that ABASE falls under PCI reserved
	 * IO memory range.
	 *
	 * Note: Don't add any more resource with same offset 0x20
	 * under this device space.
	 */
	pch_lpc_add_new_resource(dev, PCI_BASE_ADDRESS_4,
			ACPI_BASE_ADDRESS, ACPI_BASE_SIZE, IORESOURCE_IO |
			IORESOURCE_ASSIGNED | IORESOURCE_FIXED);
}

#endif
