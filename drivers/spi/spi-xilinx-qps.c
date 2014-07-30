/*
 *
 * Xilinx PS Quad-SPI (QSPI) controller driver (master mode only)
 *
 * (c) 2009-2011 Xilinx, Inc.
 *
 * based on Xilinx PS SPI Driver (xspips.c)
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/xilinx_devices.h>

/*
 * Name of this driver
 */
#define DRIVER_NAME			"xqspips"

/*
 * Register offset definitions
 */
#define XQSPIPS_CONFIG_OFFSET		0x00 /* Configuration  Register, RW */
#define XQSPIPS_STATUS_OFFSET		0x04 /* Interrupt Status Register, RO */
#define XQSPIPS_IEN_OFFSET		0x08 /* Interrupt Enable Register, WO */
#define XQSPIPS_IDIS_OFFSET		0x0C /* Interrupt Disable Reg, WO */
#define XQSPIPS_IMASK_OFFSET		0x10 /* Interrupt Enabled Mask Reg,RO */
#define XQSPIPS_ENABLE_OFFSET		0x14 /* Enable/Disable Register, RW */
#define XQSPIPS_DELAY_OFFSET		0x18 /* Delay Register, RW */
#define XQSPIPS_TXD_00_00_OFFSET	0x1C /* Transmit 4-byte inst, WO */
#define XQSPIPS_TXD_00_01_OFFSET	0x80 /* Transmit 1-byte inst, WO */
#define XQSPIPS_TXD_00_10_OFFSET	0x84 /* Transmit 2-byte inst, WO */
#define XQSPIPS_TXD_00_11_OFFSET	0x88 /* Transmit 3-byte inst, WO */
#define XQSPIPS_RXD_OFFSET		0x20 /* Data Receive Register, RO */
#define XQSPIPS_SIC_OFFSET		0x24 /* Slave Idle Count Register, RW */
#define XQSPIPS_TX_THRESH_OFFSET	0x28 /* TX FIFO Watermark Reg, RW */
#define XQSPIPS_RX_THRESH_OFFSET	0x2C /* RX FIFO Watermark Reg, RW */
#define XQSPIPS_GPIO_OFFSET		0x30 /* GPIO Register, RW */
#define XQSPIPS_LINEAR_CFG_OFFSET	0xA0 /* Linear Adapter Config Ref, RW */
#define XQSPIPS_MOD_ID_OFFSET		0xFC /* Module ID Register, RO */

/*
 * QSPI Configuration Register bit Masks
 *
 * This register contains various control bits that effect the operation
 * of the QSPI controller
 */
#define XQSPIPS_CONFIG_MANSRT_MASK	0x00010000 /* Manual TX Start */
#define XQSPIPS_CONFIG_CPHA_MASK	0x00000004 /* Clock Phase Control */
#define XQSPIPS_CONFIG_CPOL_MASK	0x00000002 /* Clock Polarity Control */
#define XQSPIPS_CONFIG_SSCTRL_MASK	0x00003C00 /* Slave Select Mask */

/*
 * QSPI Interrupt Registers bit Masks
 *
 * All the four interrupt registers (Status/Mask/Enable/Disable) have the same
 * bit definitions.
 */
#define XQSPIPS_IXR_TXNFULL_MASK	0x00000004 /* QSPI TX FIFO Overflow */
#define XQSPIPS_IXR_TXFULL_MASK		0x00000008 /* QSPI TX FIFO is full */
#define XQSPIPS_IXR_RXNEMTY_MASK	0x00000010 /* QSPI RX FIFO Not Empty */
#define XQSPIPS_IXR_ALL_MASK		(XQSPIPS_IXR_TXNFULL_MASK)

/*
 * QSPI Enable Register bit Masks
 *
 * This register is used to enable or disable the QSPI controller
 */
#define XQSPIPS_ENABLE_ENABLE_MASK	0x00000001 /* QSPI Enable Bit Mask */

/*
 * The modebits configurable by the driver to make the SPI support different
 * data formats
 */
#define MODEBITS			(SPI_CPOL | SPI_CPHA)

/*
 * Definitions for the status of queue
 */
#define XQSPIPS_QUEUE_STOPPED		0
#define XQSPIPS_QUEUE_RUNNING		1

/*
 * Definitions of the flash commands
 */
/* Flash opcodes in ascending order */
#define	XQSPIPS_FLASH_OPCODE_WRSR	0x01	/* Write status register */
#define	XQSPIPS_FLASH_OPCODE_PP		0x02	/* Page program */
#define	XQSPIPS_FLASH_OPCODE_NORM_READ	0x03	/* Normal read data bytes */
#define	XQSPIPS_FLASH_OPCODE_WRDS	0x04	/* Write disable */
#define	XQSPIPS_FLASH_OPCODE_RDSR1	0x05	/* Read status register 1 */
#define	XQSPIPS_FLASH_OPCODE_WREN	0x06	/* Write enable */
#define	XQSPIPS_FLASH_OPCODE_FAST_READ	0x0B	/* Fast read data bytes */
#define	XQSPIPS_FLASH_OPCODE_BE_4K	0x20	/* Erase 4KiB block */
#define	XQSPIPS_FLASH_OPCODE_RDSR2	0x35	/* Read status register 2 */
#define	XQSPIPS_FLASH_OPCODE_DUAL_READ	0x3B	/* Dual read data bytes */
#define	XQSPIPS_FLASH_OPCODE_BE_32K	0x52	/* Erase 32KiB block */
#define	XQSPIPS_FLASH_OPCODE_QUAD_READ	0x6B	/* Quad read data bytes */
#define	XQSPIPS_FLASH_OPCODE_ERASE_SUS	0x75	/* Erase suspend */
#define	XQSPIPS_FLASH_OPCODE_ERASE_RES	0x7A	/* Erase resume */
#define	XQSPIPS_FLASH_OPCODE_RDID	0x9F	/* Read JEDEC ID */
#define	XQSPIPS_FLASH_OPCODE_BE		0xC7	/* Erase whole flash block */
#define	XQSPIPS_FLASH_OPCODE_SE		0xD8	/* Sector erase (usually 64KB)*/

/*
 * Macros for the QSPI controller read/write
 */
#define xqspips_read(addr)		__raw_readl(addr)
#define xqspips_write(addr, val)	__raw_writel((val), (addr))

/**
 * struct xqspips - Defines qspi driver instance
 * @workqueue:		Queue of all the transfers
 * @work:		Information about current transfer
 * @queue:		Head of the queue
 * @queue_state:	Queue status
 * @regs:		Virtual address of the QSPI controller registers
 * @devclk		Pointer to the peripheral clock
 * @aperclk		Pointer to the APER clock
 * @clk_rate_change_nb	Notifier block for clock frequency change callback
 * @irq:		IRQ number
 * @speed_hz:		Current QSPI bus clock speed in Hz
 * @trans_queue_lock:	Lock used for accessing transfer queue
 * @config_reg_lock:	Lock used for accessing configuration register
 * @txbuf: 		Pointer	to the TX buffer
 * @rxbuf:		Pointer to the RX buffer
 * @bytes_to_transfer:	Number of bytes left to transfer
 * @bytes_to_receive:	Number of bytes left to receive
 * @dev_busy:		Device busy flag
 * @done:		Transfer complete status
 * @is_inst:		Flag to indicate the first message in a Transfer request
 * @is_dual:		Flag to indicate whether dual flash memories are used
 **/
struct xqspips {
	struct workqueue_struct *workqueue;
	struct work_struct work;
	struct list_head queue;
	u8 queue_state;
	void __iomem *regs;
	struct clk *devclk;
	struct clk *aperclk;
	struct notifier_block clk_rate_change_nb;
	u32 irq;
	u32 speed_hz;
	spinlock_t trans_queue_lock;
	spinlock_t config_reg_lock;
	const void *txbuf;
	void *rxbuf;
	int bytes_to_transfer;
	int bytes_to_receive;
	u8 dev_busy;
	struct completion done;
	bool is_inst;
	bool is_dual;
};

/**
 * struct xqspips_inst_format - Defines qspi flash instruction format
 * @opcode:		Operational code of instruction
 * @inst_size:		Size of the instruction including address bytes
 * @offset:		Register address where instruction has to be written
 **/
struct xqspips_inst_format {
	u8 opcode;
	u8 inst_size;
	u8 offset;
};

/*
 * List of all the QSPI instructions and its format
 */
static struct xqspips_inst_format __devinitdata flash_inst[] = {
	{ XQSPIPS_FLASH_OPCODE_WREN, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_WRDS, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_RDSR1, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_RDSR2, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_WRSR, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_PP, 4, XQSPIPS_TXD_00_00_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_SE, 4, XQSPIPS_TXD_00_00_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_BE_32K, 4, XQSPIPS_TXD_00_00_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_BE_4K, 4, XQSPIPS_TXD_00_00_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_BE, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_ERASE_SUS, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_ERASE_RES, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_RDID, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_NORM_READ, 4, XQSPIPS_TXD_00_00_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_FAST_READ, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_DUAL_READ, 1, XQSPIPS_TXD_00_01_OFFSET },
	{ XQSPIPS_FLASH_OPCODE_QUAD_READ, 1, XQSPIPS_TXD_00_01_OFFSET },
	/* Add all the instructions supported by the flash device */
};

/**
 * xqspips_init_hw - Initialize the hardware
 * @regs_base:	Base address of QSPI controller
 * @is_dual:	Indicates whether dual memories are used
 *
 * The default settings of the QSPI controller's configurable parameters on
 * reset are
 *	- Master mode
 * 	- Baud rate divisor is set to 2
 *	- Threshold value for TX FIFO not full interrupt is set to 1
 *	- Flash memory interface mode enabled
 *	- Size of the word to be transferred as 8 bit
 * This function performs the following actions
 *	- Disable and clear all the interrupts
 *	- Enable manual slave select
 *	- Enable manual start
 *	- Deselect all the chip select lines
 *	- Set the size of the word to be transferred as 32 bit
 *	- Set the little endian mode of TX FIFO and
 *	- Enable the QSPI controller
 **/
static void xqspips_init_hw(void __iomem *regs_base, int is_dual)
{
	u32 config_reg;

	xqspips_write(regs_base + XQSPIPS_ENABLE_OFFSET,
		~XQSPIPS_ENABLE_ENABLE_MASK);
	xqspips_write(regs_base + XQSPIPS_IDIS_OFFSET, 0x7F);

	/* Disable linear mode as the boot loader may have used it */
	xqspips_write(regs_base + XQSPIPS_LINEAR_CFG_OFFSET, 0);

	/* Clear the RX FIFO */
	while (xqspips_read(regs_base + XQSPIPS_STATUS_OFFSET) &
			XQSPIPS_IXR_RXNEMTY_MASK)
		xqspips_read(regs_base + XQSPIPS_RXD_OFFSET);

	xqspips_write(regs_base + XQSPIPS_STATUS_OFFSET , 0x7F);
	config_reg = xqspips_read(regs_base + XQSPIPS_CONFIG_OFFSET);
	config_reg &= 0xFBFFFFFF; /* Set little endian mode of TX FIFO */
	config_reg |= 0x8000FCC1;
	xqspips_write(regs_base + XQSPIPS_CONFIG_OFFSET, config_reg);

	if (is_dual == 1)
		/* Enable two memories on seperate buses */
		xqspips_write(regs_base + XQSPIPS_LINEAR_CFG_OFFSET,
			      0x6400016B);

	xqspips_write(regs_base + XQSPIPS_ENABLE_OFFSET,
			XQSPIPS_ENABLE_ENABLE_MASK);
}

/**
 * xqspips_copy_read_data - Copy data to RX buffer
 * @xqspi:	Pointer to the xqspips structure
 * @data:	The 32 bit variable where data is stored
 * @size:	Number of bytes to be copied from data to RX buffer
 **/
static void xqspips_copy_read_data(struct xqspips *xqspi, u32 data, u8 size)
{
	u8 byte3;

	if (xqspi->rxbuf) {
		switch (size) {
		case 1:
			*((u8 *)xqspi->rxbuf) = data;
			xqspi->rxbuf += 1;
			break;
		case 2:
			*((u16 *)xqspi->rxbuf) = data;
			xqspi->rxbuf += 2;
			break;
		case 3:
			*((u16 *)xqspi->rxbuf) = data;
			xqspi->rxbuf += 2;
			byte3 = (u8)(data >> 16);
			*((u8 *)xqspi->rxbuf) = byte3;
			xqspi->rxbuf += 1;
			break;
		case 4:
			(*(u32 *)xqspi->rxbuf) = data;
			xqspi->rxbuf += 4;
			break;
		default:
			/* This will never execute */
			break;
		}
	}
	xqspi->bytes_to_receive -= size;
	if (xqspi->bytes_to_receive < 0)
		xqspi->bytes_to_receive = 0;
}

/**
 * xqspips_copy_write_data - Copy data from TX buffer
 * @xqspi:	Pointer to the xqspips structure
 * @data:	Pointer to the 32 bit variable where data is to be copied
 * @size:	Number of bytes to be copied from TX buffer to data
 **/
static void xqspips_copy_write_data(struct xqspips *xqspi, u32 *data, u8 size)
{

	if (xqspi->txbuf) {
		switch (size) {
		case 1:
			*data = *((u8 *)xqspi->txbuf);
			xqspi->txbuf += 1;
			*data |= 0xFFFFFF00;
			break;
		case 2:
			*data = *((u16 *)xqspi->txbuf);
			xqspi->txbuf += 2;
			*data |= 0xFFFF0000;
			break;
		case 3:
			*data = *((u16 *)xqspi->txbuf);
			xqspi->txbuf += 2;
			*data |= (*((u8 *)xqspi->txbuf) << 16);
			xqspi->txbuf += 1;
			*data |= 0xFF000000;
			break;
		case 4:
			*data = *((u32 *)xqspi->txbuf);
			xqspi->txbuf += 4;
			break;
		default:
			/* This will never execute */
			break;
		}
	} else {
		*data = 0;
	}

	xqspi->bytes_to_transfer -= size;
	if (xqspi->bytes_to_transfer < 0)
		xqspi->bytes_to_transfer = 0;
}

/**
 * xqspips_chipselect - Select or deselect the chip select line
 * @qspi:	Pointer to the spi_device structure
 * @is_on:	Select(1) or deselect (0) the chip select line
 **/
static void xqspips_chipselect(struct spi_device *qspi, int is_on)
{
	struct xqspips *xqspi = spi_master_get_devdata(qspi->master);
	u32 config_reg;
	unsigned long flags;

	spin_lock_irqsave(&xqspi->config_reg_lock, flags);

	config_reg = xqspips_read(xqspi->regs + XQSPIPS_CONFIG_OFFSET);

	if (is_on) {
		/* Select the slave */
		config_reg &= ~XQSPIPS_CONFIG_SSCTRL_MASK;
		config_reg |= (((~(0x0001 << qspi->chip_select)) << 10) &
				XQSPIPS_CONFIG_SSCTRL_MASK);
	} else {
		/* Deselect the slave */
		config_reg |= XQSPIPS_CONFIG_SSCTRL_MASK;
	}

	xqspips_write(xqspi->regs + XQSPIPS_CONFIG_OFFSET, config_reg);

	spin_unlock_irqrestore(&xqspi->config_reg_lock, flags);
}

/**
 * xqspips_setup_transfer - Configure QSPI controller for specified transfer
 * @qspi:	Pointer to the spi_device structure
 * @transfer:	Pointer to the spi_transfer structure which provides information
 *		about next transfer setup parameters
 *
 * Sets the operational mode of QSPI controller for the next QSPI transfer and
 * sets the requested clock frequency.
 *
 * returns:	0 on success and -EINVAL on invalid input parameter
 *
 * Note: If the requested frequency is not an exact match with what can be
 * obtained using the prescalar value, the driver sets the clock frequency which
 * is lower than the requested frequency (maximum lower) for the transfer. If
 * the requested frequency is higher or lower than that is supported by the QSPI
 * controller the driver will set the highest or lowest frequency supported by
 * controller.
 **/
static int xqspips_setup_transfer(struct spi_device *qspi,
		struct spi_transfer *transfer)
{
	struct xqspips *xqspi = spi_master_get_devdata(qspi->master);
	u32 config_reg;
	u32 req_hz;
	u32 baud_rate_val = 0;
	unsigned long flags;

	req_hz = (transfer) ? transfer->speed_hz : qspi->max_speed_hz;

	if (qspi->mode & ~MODEBITS) {
		dev_err(&qspi->dev, "%s, unsupported mode bits %x\n",
			__func__, qspi->mode & ~MODEBITS);
		return -EINVAL;
	}

	spin_lock_irqsave(&xqspi->config_reg_lock, flags);

	config_reg = xqspips_read(xqspi->regs + XQSPIPS_CONFIG_OFFSET);

	/* Set the QSPI clock phase and clock polarity */
	config_reg &= (~XQSPIPS_CONFIG_CPHA_MASK) &
				(~XQSPIPS_CONFIG_CPOL_MASK);
	if (qspi->mode & SPI_CPHA)
		config_reg |= XQSPIPS_CONFIG_CPHA_MASK;
	if (qspi->mode & SPI_CPOL)
		config_reg |= XQSPIPS_CONFIG_CPOL_MASK;

	/* Set the clock frequency */
	if (xqspi->speed_hz != req_hz) {
		baud_rate_val = 0;
		while ((baud_rate_val < 8)  &&
			(clk_get_rate(xqspi->devclk) / (2 << baud_rate_val)) >
			req_hz)
				baud_rate_val++;
		config_reg &= 0xFFFFFFC7;
		config_reg |= (baud_rate_val << 3);
		xqspi->speed_hz = req_hz;
	}

	xqspips_write(xqspi->regs + XQSPIPS_CONFIG_OFFSET, config_reg);

	spin_unlock_irqrestore(&xqspi->config_reg_lock, flags);

	dev_dbg(&qspi->dev, "%s, mode %d, %u bits/w, %u clock speed\n",
		__func__, qspi->mode & MODEBITS, qspi->bits_per_word,
		xqspi->speed_hz);

	return 0;
}

/**
 * xqspips_setup - Configure the QSPI controller
 * @qspi:	Pointer to the spi_device structure
 *
 * Sets the operational mode of QSPI controller for the next QSPI transfer, baud
 * rate and divisor value to setup the requested qspi clock.
 *
 * returns:	0 on success and error value on failure
 **/
static int xqspips_setup(struct spi_device *qspi)
{

	if (qspi->mode & SPI_LSB_FIRST)
		return -EINVAL;

	if (!qspi->max_speed_hz)
		return -EINVAL;

	if (!qspi->bits_per_word)
		qspi->bits_per_word = 32;

	return xqspips_setup_transfer(qspi, NULL);
}

/**
 * xqspips_fill_tx_fifo - Fills the TX FIFO with as many bytes as possible
 * @xqspi:	Pointer to the xqspips structure
 **/
static void xqspips_fill_tx_fifo(struct xqspips *xqspi)
{
	u32 data = 0;

	while ((!(xqspips_read(xqspi->regs + XQSPIPS_STATUS_OFFSET) &
		XQSPIPS_IXR_TXFULL_MASK)) && (xqspi->bytes_to_transfer > 0)) {
		if (xqspi->bytes_to_transfer < 4)
			xqspips_copy_write_data(xqspi, &data,
				xqspi->bytes_to_transfer);
		else
			xqspips_copy_write_data(xqspi, &data, 4);

		xqspips_write(xqspi->regs + XQSPIPS_TXD_00_00_OFFSET, data);
	}
}

/**
 * xqspips_irq - Interrupt service routine of the QSPI controller
 * @irq:	IRQ number
 * @dev_id:	Pointer to the xqspi structure
 *
 * This function handles TX empty only.
 * On TX empty interrupt this function reads the received data from RX FIFO and
 * fills the TX FIFO if there is any data remaining to be transferred.
 *
 * returns:	IRQ_HANDLED always
 **/
static irqreturn_t xqspips_irq(int irq, void *dev_id)
{
	struct xqspips *xqspi = dev_id;
	u32 intr_status;

	intr_status = xqspips_read(xqspi->regs + XQSPIPS_STATUS_OFFSET);
	xqspips_write(xqspi->regs + XQSPIPS_STATUS_OFFSET , intr_status);
	xqspips_write(xqspi->regs + XQSPIPS_IDIS_OFFSET,
			XQSPIPS_IXR_ALL_MASK);

	if ((intr_status & XQSPIPS_IXR_TXNFULL_MASK) ||
		   (intr_status & XQSPIPS_IXR_RXNEMTY_MASK)) {
		/* This bit is set when Tx FIFO has < THRESHOLD entries. We have
		   the THRESHOLD value set to 1, so this bit indicates Tx FIFO
		   is empty */
		u32 config_reg;

		/* Read out the data from the RX FIFO */
		while (xqspips_read(xqspi->regs + XQSPIPS_STATUS_OFFSET) &
			XQSPIPS_IXR_RXNEMTY_MASK) {
			u32 data;

			data = xqspips_read(xqspi->regs + XQSPIPS_RXD_OFFSET);

			if (xqspi->bytes_to_receive < 4)
				xqspips_copy_read_data(xqspi, data,
					xqspi->bytes_to_receive);
			else
				xqspips_copy_read_data(xqspi, data, 4);
		}

		if (xqspi->bytes_to_transfer) {
			/* There is more data to send */
			xqspips_fill_tx_fifo(xqspi);

			xqspips_write(xqspi->regs + XQSPIPS_IEN_OFFSET,
					XQSPIPS_IXR_ALL_MASK);

			spin_lock(&xqspi->config_reg_lock);
			config_reg = xqspips_read(xqspi->regs +
						XQSPIPS_CONFIG_OFFSET);

			config_reg |= XQSPIPS_CONFIG_MANSRT_MASK;
			xqspips_write(xqspi->regs + XQSPIPS_CONFIG_OFFSET,
				config_reg);
			spin_unlock(&xqspi->config_reg_lock);
		} else {
			/* If transfer and receive is completed then only send
			 * complete signal */
			if (xqspi->bytes_to_receive) {
				/* There is still some data to be received.
				   Enable Rx not empty interrupt */
				xqspips_write(xqspi->regs + XQSPIPS_IEN_OFFSET,
						XQSPIPS_IXR_RXNEMTY_MASK);
			} else {
				xqspips_write(xqspi->regs + XQSPIPS_IDIS_OFFSET,
						XQSPIPS_IXR_RXNEMTY_MASK);
				complete(&xqspi->done);
			}
		}
	}

	return IRQ_HANDLED;
}

/**
 * xqspips_start_transfer - Initiates the QSPI transfer
 * @qspi:	Pointer to the spi_device structure
 * @transfer:	Pointer to the spi_transfer structure which provide information
 *		about next transfer parameters
 *
 * This function fills the TX FIFO, starts the QSPI transfer, and waits for the
 * transfer to be completed.
 *
 * returns:	Number of bytes transferred in the last transfer
 **/
static int __devinit xqspips_start_transfer(struct spi_device *qspi,
			struct spi_transfer *transfer)
{
	struct xqspips *xqspi = spi_master_get_devdata(qspi->master);
	u32 config_reg;
	unsigned long flags;
	u32 data = 0;
	u8 instruction = 0;
	u8 index;
	struct xqspips_inst_format *curr_inst;

	xqspi->txbuf = transfer->tx_buf;
	xqspi->rxbuf = transfer->rx_buf;
	xqspi->bytes_to_transfer = transfer->len;
	xqspi->bytes_to_receive = transfer->len;

	if (xqspi->txbuf)
		instruction = *(u8 *)xqspi->txbuf;

	if (instruction && xqspi->is_inst) {
		for (index = 0 ; index < ARRAY_SIZE(flash_inst); index++)
			if (instruction == flash_inst[index].opcode)
				break;

		/* Instruction might have already been transmitted. This is a
		 * 'data only' transfer */
		if (index == ARRAY_SIZE(flash_inst))
			goto xfer_data;

		curr_inst = &flash_inst[index];

		/* In case of dual memories, convert 25 bit address to 24 bit
		 * address before transmitting to the 2 memories
		 */
		if ((xqspi->is_dual == 1) &&
		    ((instruction == XQSPIPS_FLASH_OPCODE_PP) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_SE) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_BE_32K) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_BE_4K) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_BE) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_NORM_READ) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_FAST_READ) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_DUAL_READ) ||
		     (instruction == XQSPIPS_FLASH_OPCODE_QUAD_READ))) {

			u8 *ptr = (u8*) (xqspi->txbuf);
			data = ((u32) ptr[1] << 24) | ((u32) ptr[2] << 16) |
				((u32) ptr[3] << 8) | ((u32) ptr[4]);
			data = data >> 1;
			ptr[1] = (u8) (data >> 16);
			ptr[2] = (u8) (data >> 8);
			ptr[3] = (u8) (data);
			xqspi->bytes_to_transfer -= 1;
			xqspi->bytes_to_receive -= 1;
		}

		/* Get the instruction */
		data = 0;
		xqspips_copy_write_data(xqspi, &data,
					curr_inst->inst_size);

		/* Write the instruction to LSB of the FIFO. The core is
		 * designed such that it is not necessary to check whether the
		 * write FIFO is full before writing. However, write would be
		 * delayed if the user tries to write when write FIFO is full
		 */
		xqspips_write(xqspi->regs + curr_inst->offset, data);
	}

xfer_data:
	INIT_COMPLETION(xqspi->done);
	/* In case of Fast, Dual and Quad reads, transmit the instruction first.
	 * Address and dummy byte will be transmitted in interrupt handler,
	 * after instruction is transmitted */
	if (((xqspi->is_inst == 0) && (xqspi->bytes_to_transfer)) ||
	     ((xqspi->bytes_to_transfer) &&
	      (instruction != XQSPIPS_FLASH_OPCODE_FAST_READ) &&
	      (instruction != XQSPIPS_FLASH_OPCODE_DUAL_READ) &&
	      (instruction != XQSPIPS_FLASH_OPCODE_QUAD_READ)))
		xqspips_fill_tx_fifo(xqspi);

	xqspips_write(xqspi->regs + XQSPIPS_IEN_OFFSET,
			XQSPIPS_IXR_ALL_MASK);
	/* Start the transfer by enabling manual start bit */
	spin_lock_irqsave(&xqspi->config_reg_lock, flags);
	config_reg = xqspips_read(xqspi->regs +
			XQSPIPS_CONFIG_OFFSET) | XQSPIPS_CONFIG_MANSRT_MASK;
	xqspips_write(xqspi->regs + XQSPIPS_CONFIG_OFFSET, config_reg);
	spin_unlock_irqrestore(&xqspi->config_reg_lock, flags);

	wait_for_completion(&xqspi->done);

	return (transfer->len) - (xqspi->bytes_to_transfer);
}

/**
 * xqspips_work_queue - Get the request from queue to perform transfers
 * @work:	Pointer to the work_struct structure
 **/
static void __devinit xqspips_work_queue(struct work_struct *work)
{
	struct xqspips *xqspi = container_of(work, struct xqspips, work);
	unsigned long flags;

	spin_lock_irqsave(&xqspi->trans_queue_lock, flags);
	xqspi->dev_busy = 1;

	/* Check if list is empty or queue is stoped */
	if (list_empty(&xqspi->queue) ||
		xqspi->queue_state == XQSPIPS_QUEUE_STOPPED) {
		xqspi->dev_busy = 0;
		spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);
		return;
	}

	/* Keep requesting transfer till list is empty */
	while (!list_empty(&xqspi->queue)) {
		struct spi_message *msg;
		struct spi_device *qspi;
		struct spi_transfer *transfer = NULL;
		unsigned cs_change = 1;
		int status = 0;

		msg = container_of(xqspi->queue.next, struct spi_message,
					queue);
		list_del_init(&msg->queue);
		spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);
		qspi = msg->spi;

		list_for_each_entry(transfer, &msg->transfers, transfer_list) {
			if (transfer->bits_per_word || transfer->speed_hz) {
				status = xqspips_setup_transfer(qspi, transfer);
				if (status < 0)
					break;
			}

			/* Select the chip if required */
			if (cs_change) {
				xqspips_chipselect(qspi, 1);
				xqspi->is_inst = 1;
			}

			cs_change = transfer->cs_change;

			if (!transfer->tx_buf && !transfer->rx_buf &&
				transfer->len) {
				status = -EINVAL;
				break;
			}

			/* Request the transfer */
			if (transfer->len) {
				status = xqspips_start_transfer(qspi, transfer);
				xqspi->is_inst = 0;
			}

			if (status != transfer->len) {
				if (status > 0)
					status = -EMSGSIZE;
				break;
			}
			msg->actual_length += status;
			status = 0;

			if (transfer->delay_usecs)
				udelay(transfer->delay_usecs);

			if (cs_change)
				/* Deselect the chip */
				xqspips_chipselect(qspi, 0);

			if (transfer->transfer_list.next == &msg->transfers)
				break;
		}

		msg->status = status;
		msg->complete(msg->context);

		xqspips_setup_transfer(qspi, NULL);

		if (!(status == 0 && cs_change))
			xqspips_chipselect(qspi, 0);

		spin_lock_irqsave(&xqspi->trans_queue_lock, flags);
	}
	xqspi->dev_busy = 0;
	spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);
}

/**
 * xqspips_transfer - Add a new transfer request at the tail of work queue
 * @qspi:	Pointer to the spi_device structure
 * @message:	Pointer to the spi_transfer structure which provides information
 *		about next transfer parameters
 *
 * returns:	0 on success, -EINVAL on invalid input parameter and
 *		-ESHUTDOWN if queue is stopped by module unload function
 **/
static int
xqspips_transfer(struct spi_device *qspi, struct spi_message *message)
{
	struct xqspips *xqspi = spi_master_get_devdata(qspi->master);
	struct spi_transfer *transfer;
	unsigned long flags;

	if (xqspi->queue_state == XQSPIPS_QUEUE_STOPPED)
		return -ESHUTDOWN;

	message->actual_length = 0;
	message->status = -EINPROGRESS;

	/* Check each transfer's parameters */
	list_for_each_entry(transfer, &message->transfers, transfer_list) {
		if (!transfer->tx_buf && !transfer->rx_buf && transfer->len)
			return -EINVAL;
		/* QSPI controller supports only 32 bit transfers whereas higher
		 * layer drivers request 8 bit transfers. Re-visit at a later
		 * time */
		/* if (bits_per_word != 32)
			return -EINVAL; */
	}

	spin_lock_irqsave(&xqspi->trans_queue_lock, flags);
	list_add_tail(&message->queue, &xqspi->queue);
	if (!xqspi->dev_busy)
		queue_work(xqspi->workqueue, &xqspi->work);
	spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);

	return 0;
}

/**
 * xqspips_start_queue - Starts the queue of the QSPI driver
 * @xqspi:	Pointer to the xqspips structure
 *
 * returns:	0 on success and -EBUSY if queue is already running or device is
 *		busy
 **/
static inline int xqspips_start_queue(struct xqspips *xqspi)
{
	unsigned long flags;

	spin_lock_irqsave(&xqspi->trans_queue_lock, flags);

	if (xqspi->queue_state == XQSPIPS_QUEUE_RUNNING || xqspi->dev_busy) {
		spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);
		return -EBUSY;
	}

	xqspi->queue_state = XQSPIPS_QUEUE_RUNNING;
	spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);

	return 0;
}

/**
 * xqspips_stop_queue - Stops the queue of the QSPI driver
 * @xqspi:	Pointer to the xqspips structure
 *
 * This function waits till queue is empty and then stops the queue.
 * Maximum time out is set to 5 seconds.
 *
 * returns:	0 on success and -EBUSY if queue is not empty or device is busy
 **/
static inline int xqspips_stop_queue(struct xqspips *xqspi)
{
	unsigned long flags;
	unsigned limit = 500;
	int ret = 0;

	if (xqspi->queue_state != XQSPIPS_QUEUE_RUNNING)
		return ret;

	spin_lock_irqsave(&xqspi->trans_queue_lock, flags);

	while ((!list_empty(&xqspi->queue) || xqspi->dev_busy) && limit--) {
		spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);
		msleep(10);
		spin_lock_irqsave(&xqspi->trans_queue_lock, flags);
	}

	if (!list_empty(&xqspi->queue) || xqspi->dev_busy)
		ret = -EBUSY;

	if (ret == 0)
		xqspi->queue_state = XQSPIPS_QUEUE_STOPPED;

	spin_unlock_irqrestore(&xqspi->trans_queue_lock, flags);

	return ret;
}

/**
 * xqspips_destroy_queue - Destroys the queue of the QSPI driver
 * @xqspi:	Pointer to the xqspips structure
 *
 * returns:	0 on success and error value on failure
 **/
static inline int xqspips_destroy_queue(struct xqspips *xqspi)
{
	int ret;

	ret = xqspips_stop_queue(xqspi);
	if (ret != 0)
		return ret;

	destroy_workqueue(xqspi->workqueue);

	return 0;
}

static int xqspips_clk_notifier_cb(struct notifier_block *nb,
		unsigned long event, void *data)
{
	switch (event) {
	case PRE_RATE_CHANGE:
		/* if a rate change is announced we need to check whether we can
		 * maintain the current frequency by changing the clock
		 * dividers. And we may have to suspend operation and return
		 * after the rate change or its abort
		 */
		return NOTIFY_OK;
	case POST_RATE_CHANGE:
		return NOTIFY_OK;
	case ABORT_RATE_CHANGE:
	default:
		return NOTIFY_DONE;
	}
}

#ifdef CONFIG_PM_SLEEP
/**
 * xqspips_suspend - Suspend method for the QSPI driver
 * @_dev:	Address of the platform_device structure
 *
 * This function stops the QSPI driver queue and disables the QSPI controller
 *
 * returns:	0 on success and error value on error
 **/
static int xqspips_suspend(struct device *_dev)
{
	struct platform_device *pdev = container_of(_dev,
			struct platform_device, dev);
	struct spi_master *master = platform_get_drvdata(pdev);
	struct xqspips *xqspi = spi_master_get_devdata(master);
	int ret = 0;

	ret = xqspips_stop_queue(xqspi);
	if (ret != 0)
		return ret;

	xqspips_write(xqspi->regs + XQSPIPS_ENABLE_OFFSET,
			~XQSPIPS_ENABLE_ENABLE_MASK);

	clk_disable(xqspi->devclk);
	clk_disable(xqspi->aperclk);

	dev_dbg(&pdev->dev, "suspend succeeded\n");
	return 0;
}

/**
 * xqspips_resume - Resume method for the QSPI driver
 * @dev:	Address of the platform_device structure
 *
 * The function starts the QSPI driver queue and initializes the QSPI controller
 *
 * returns:	0 on success and error value on error
 **/
static int xqspips_resume(struct device *_dev)
{
	struct platform_device *pdev = container_of(_dev,
			struct platform_device, dev);
	struct spi_master *master = platform_get_drvdata(pdev);
	struct xqspips *xqspi = spi_master_get_devdata(master);
	int ret = 0;

	ret = clk_enable(xqspi->aperclk);
	if (ret) {
		dev_err(_dev, "Cannot enable APER clock.\n");
		return ret;
	}

	ret = clk_enable(xqspi->devclk);
	if (ret) {
		dev_err(_dev, "Cannot enable device clock.\n");
		clk_disable(xqspi->aperclk);
		return ret;
	}

	xqspips_init_hw(xqspi->regs, xqspi->is_dual);

	ret = xqspips_start_queue(xqspi);
	if (ret != 0) {
		dev_err(&pdev->dev, "problem starting queue (%d)\n", ret);
		return ret;
	}

	dev_dbg(&pdev->dev, "resume succeeded\n");
	return 0;
}
static const struct dev_pm_ops xqspips_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(xqspips_suspend, xqspips_resume)
};
#define XQSPIPS_PM	(&xqspips_dev_pm_ops)

#else /* ! CONFIG_PM_SLEEP */
#define XQSPIPS_PM	NULL
#endif /* ! CONFIG_PM_SLEEP */

/**
 * xqspips_probe - Probe method for the QSPI driver
 * @dev:	Pointer to the platform_device structure
 *
 * This function initializes the driver data structures and the hardware.
 *
 * returns:	0 on success and error value on failure
 **/
static int __devinit xqspips_probe(struct platform_device *dev)
{
	int ret = 0;
	struct spi_master *master;
	struct xqspips *xqspi;
	struct resource *r;
	const unsigned int *prop;

	master = spi_alloc_master(&dev->dev, sizeof(*xqspi));
	if (master == NULL)
		return -ENOMEM;

	xqspi = spi_master_get_devdata(master);
	master->dev.of_node = dev->dev.of_node;
	platform_set_drvdata(dev, master);

	r = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (r == NULL) {
		ret = -ENODEV;
		dev_err(&dev->dev, "platform_get_resource failed\n");
		goto put_master;
	}

	if (!request_mem_region(r->start, r->end - r->start + 1, dev->name)) {
		ret = -ENXIO;
		dev_err(&dev->dev, "request_mem_region failed\n");
		goto put_master;
	}

	xqspi->regs = ioremap(r->start, r->end - r->start + 1);
	if (xqspi->regs == NULL) {
		ret = -ENOMEM;
		dev_err(&dev->dev, "ioremap failed\n");
		goto release_mem;
	}

	xqspi->irq = platform_get_irq(dev, 0);
	if (xqspi->irq < 0) {
		ret = -ENXIO;
		dev_err(&dev->dev, "irq resource not found\n");
		goto unmap_io;
	}

	ret = request_irq(xqspi->irq, xqspips_irq, 0, dev->name, xqspi);
	if (ret != 0) {
		ret = -ENXIO;
		dev_err(&dev->dev, "request_irq failed\n");
		goto unmap_io;
	}

	prop = of_get_property(dev->dev.of_node, "is-dual", NULL);
	if (prop)
		xqspi->is_dual = be32_to_cpup(prop);
	else
		dev_warn(&dev->dev, "couldn't determine configuration info "
			 "about dual memories. defaulting to single memory\n");

	xqspi->aperclk = clk_get_sys("LQSPI_APER", NULL);
	if (IS_ERR(xqspi->aperclk)) {
		dev_err(&dev->dev, "APER clock not found.\n");
		ret = PTR_ERR(xqspi->aperclk);
		goto free_irq;
	}

	xqspi->devclk = clk_get_sys("LQSPI", NULL);
	if (IS_ERR(xqspi->devclk)) {
		dev_err(&dev->dev, "Device clock not found.\n");
		ret = PTR_ERR(xqspi->devclk);
		goto clk_put_aper;
	}

	ret = clk_prepare_enable(xqspi->aperclk);
	if (ret) {
		dev_err(&dev->dev, "Unable to enable APER clock.\n");
		goto clk_put;
	}

	ret = clk_prepare_enable(xqspi->devclk);
	if (ret) {
		dev_err(&dev->dev, "Unable to enable device clock.\n");
		goto clk_dis_aper;
	}

	xqspi->clk_rate_change_nb.notifier_call = xqspips_clk_notifier_cb;
	xqspi->clk_rate_change_nb.next = NULL;
	if (clk_notifier_register(xqspi->devclk, &xqspi->clk_rate_change_nb))
		dev_warn(&dev->dev, "Unable to register clock notifier.\n");


	/* QSPI controller initializations */
	xqspips_init_hw(xqspi->regs, xqspi->is_dual);

	init_completion(&xqspi->done);

	prop = of_get_property(dev->dev.of_node, "bus-num", NULL);
	if (prop) {
		master->bus_num = be32_to_cpup(prop);
	} else {
		ret = -ENXIO;
		dev_err(&dev->dev, "couldn't determine bus-num\n");
		goto clk_unreg_notif;
	}

	prop = of_get_property(dev->dev.of_node, "num-chip-select", NULL);
	if (prop) {
		master->num_chipselect = be32_to_cpup(prop);
	} else {
		ret = -ENXIO;
		dev_err(&dev->dev, "couldn't determine num-chip-select\n");
		goto clk_unreg_notif;
	}

	master->setup = xqspips_setup;
	master->transfer = xqspips_transfer;

	xqspi->speed_hz = clk_get_rate(xqspi->devclk) / 2;

	xqspi->dev_busy = 0;

	INIT_LIST_HEAD(&xqspi->queue);
	spin_lock_init(&xqspi->trans_queue_lock);
	spin_lock_init(&xqspi->config_reg_lock);

	xqspi->queue_state = XQSPIPS_QUEUE_STOPPED;
	xqspi->dev_busy = 0;

	INIT_WORK(&xqspi->work, xqspips_work_queue);
	xqspi->workqueue =
		create_singlethread_workqueue(dev_name(&dev->dev));
	if (!xqspi->workqueue) {
		ret = -ENOMEM;
		dev_err(&dev->dev, "problem initializing queue\n");
		goto clk_unreg_notif;
	}

	ret = xqspips_start_queue(xqspi);
	if (ret != 0) {
		dev_err(&dev->dev, "problem starting queue\n");
		goto remove_queue;
	}

	ret = spi_register_master(master);
	if (ret) {
		dev_err(&dev->dev, "spi_register_master failed\n");
		goto remove_queue;
	}

	dev_info(&dev->dev, "at 0x%08X mapped to 0x%08X, irq=%d\n", r->start,
		 (u32 __force)xqspi->regs, xqspi->irq);

	return ret;

remove_queue:
	(void)xqspips_destroy_queue(xqspi);
clk_unreg_notif:
	clk_notifier_unregister(xqspi->devclk, &xqspi->clk_rate_change_nb);
	clk_disable_unprepare(xqspi->devclk);
clk_dis_aper:
	clk_disable_unprepare(xqspi->aperclk);
clk_put:
	clk_put(xqspi->devclk);
clk_put_aper:
	clk_put(xqspi->aperclk);
free_irq:
	free_irq(xqspi->irq, xqspi);
unmap_io:
	iounmap(xqspi->regs);
release_mem:
	release_mem_region(r->start, r->end - r->start + 1);
put_master:
	platform_set_drvdata(dev, NULL);
	spi_master_put(master);
	kfree(master);
	return ret;
}

/**
 * xqspips_remove - Remove method for the QSPI driver
 * @dev:	Pointer to the platform_device structure
 *
 * This function is called if a device is physically removed from the system or
 * if the driver module is being unloaded. It frees all resources allocated to
 * the device.
 *
 * returns:	0 on success and error value on failure
 **/
static int __devexit xqspips_remove(struct platform_device *dev)
{
	struct spi_master *master = platform_get_drvdata(dev);
	struct xqspips *xqspi = spi_master_get_devdata(master);
	struct resource *r;
	int ret = 0;

	r = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (r == NULL) {
		dev_err(&dev->dev, "platform_get_resource failed\n");
		return -ENODEV;
	}

	ret = xqspips_destroy_queue(xqspi);
	if (ret != 0)
		return ret;

	xqspips_write(xqspi->regs + XQSPIPS_ENABLE_OFFSET,
			~XQSPIPS_ENABLE_ENABLE_MASK);

	free_irq(xqspi->irq, xqspi);
	iounmap(xqspi->regs);
	release_mem_region(r->start, r->end - r->start + 1);

	spi_unregister_master(master);
	spi_master_put(master);

	clk_notifier_unregister(xqspi->devclk, &xqspi->clk_rate_change_nb);
	clk_disable_unprepare(xqspi->devclk);
	clk_disable_unprepare(xqspi->aperclk);
	clk_put(xqspi->devclk);
	clk_put(xqspi->aperclk);

	kfree(master);

	/* Prevent double remove */
	platform_set_drvdata(dev, NULL);

	dev_dbg(&dev->dev, "remove succeeded\n");
	return 0;
}

/* Work with hotplug and coldplug */
MODULE_ALIAS("platform:" DRIVER_NAME);

static struct of_device_id xqspips_of_match[] __devinitdata = {
	{ .compatible = "xlnx,ps7-qspi-1.00.a", },
	{ /* end of table */}
};
MODULE_DEVICE_TABLE(of, xqspips_of_match);

/*
 * xqspips_driver - This structure defines the QSPI platform driver
 */
static struct platform_driver xqspips_driver = {
	.probe	= xqspips_probe,
	.remove	= __devexit_p(xqspips_remove),
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = xqspips_of_match,
		.pm = XQSPIPS_PM,
	},
};

module_platform_driver(xqspips_driver);

MODULE_AUTHOR("Xilinx, Inc.");
MODULE_DESCRIPTION("Xilinx PS QSPI driver");
MODULE_LICENSE("GPL");
