#ifndef __ESP_ACCELERATOR_H__
#define __ESP_ACCELERATOR_H__

#define BIT(nr)			(1UL << (nr))

/***
 * ESP accelerator common definitions and registers offset
 ***/

/* bank(0): CMD (reset if cleared) */
#define CMD_REG 0x00
#define CMD_MASK_START BIT(0)

/* bank(1): STATUS (idle when cleared) - Read only */
#define STATUS_REG 0x04
#define STATUS_MASK_RUN  BIT(0)
#define STATUS_MASK_DONE BIT(1)
#define STATUS_MASK_ERR  BIT(2)

/* bank(2)        : SELECT (which accelerator will run in 1 hot encoding) */
#define SELECT_REG 0x08

/* bank(3)        : RESERVED - Read only */
#define DEVID_REG 0x0c

/* bank(4)        : PT_ADDRESS (page table bus address) */
#define PT_ADDRESS_REG 0x10

/* bank(5)        : PT_NCHUNK (number of physical contiguous buffers in memory) */
#define PT_NCHUNK_REG 0x14

/* bank(6)        : PT_SHIFT (log2(cunk size)) */
#define PT_SHIFT_REG 0x18

/* bank(7)        : PT_NCHUNK_MAX (maximum number of chunks supported) - Read only */
#define PT_NCHUNK_MAX_REG 0x1c

/* bank(8)        : RESERVED */

/* bank(9)        : RESERVED */

/* bank(10)       : RESERVED */

/* bank(11)       : RESERVED */

/* bank(12)       : SRC_OFFSET (offset in bytes from beginning of physical buffer) */
#define SRC_OFFSET_REG 0x30

/* bank(13)       : DST_OFFSET (offset in bytes from beginning of physical buffer) */
#define DST_OFFSET_REG 0x34

/* bank(14)       : RESERVED */

/* bank(15)       : RESERVED */

/* bank(16 to 28) : USR (user defined) */

/* bank(29)       : EXP_ADDR (bits 29:0 address an SRAM expanding the register bank) */
#define EXP_ADDR_REG 0x74
#define EXT_MASK_R BIT(30)
#define EXT_MASK_W BIT(31)

/* bank(30)       : EXP_DI (data to be written to the expansion SRAM) */
#define EXP_DI_REG 0x78

/* bank(31)       : EXP_DO (data read from the exansion SRAM) */
#define EXP_DO_REG 0x7c

#endif /* __ESP_ACCELERATOR_H__ */