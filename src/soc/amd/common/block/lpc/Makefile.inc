ifeq ($(CONFIG_SOC_AMD_COMMON_BLOCK_LPC),y)

ramstage-y += lpc.c

bootblock-y += lpc_util.c
verstage_x86-y += lpc_util.c
romstage-y += lpc_util.c
postcar-y += lpc_util.c
ramstage-y += lpc_util.c
smm-y += lpc_util.c

endif # CONFIG_SOC_AMD_COMMON_BLOCK_LPC

ifeq ($(CONFIG_SOC_AMD_COMMON_BLOCK_LPC_SPI_DMA),y)
bootblock-y += spi_dma.c
ifneq ($(CONFIG_VBOOT_STARTS_BEFORE_BOOTBLOCK),y)
verstage-y += spi_dma.c
endif
romstage-y += spi_dma.c
ramstage-y += spi_dma.c
smm-y += spi_dma.c
endif # CONFIG_SOC_AMD_COMMON_BLOCK_LPC_SPI_DMA

ifeq ($(CONFIG_SOC_AMD_COMMON_BLOCK_USE_ESPI),y)

bootblock-y += espi_util.c
romstage-y += espi_util.c
ramstage-y += espi_util.c
verstage-y += espi_util.c

endif # CONFIG_SOC_AMD_COMMON_BLOCK_USE_ESPI
