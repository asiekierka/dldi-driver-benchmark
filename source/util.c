#include <nds.h>
#include <nds/arm9/dldi.h>
#include "util.h"

uint8_t sec_buffer[512];

uint32_t msc_find_block_count(const DISC_INTERFACE *io) {
  // Read sector 0
  io->readSectors(0, 1, sec_buffer);

  uint16_t footer = *((uint16_t*) (sec_buffer + 510));
  if (footer == 0xAA55) {
    // Valid header, but MBR or FAT?
    uint8_t boot_opcode = sec_buffer[0];
    if (boot_opcode == 0xEB || boot_opcode == 0xE9 || boot_opcode == 0xE8) {
      if (!memcmp(sec_buffer + 54, "FAT", 3) || !memcmp(sec_buffer + 82, "FAT32   ", 8)) {
        // Looks like a FAT partition.
        uint32_t total_sectors = *((uint32_t*) (sec_buffer + 32));
        if (total_sectors < 0x10000) {
          total_sectors = sec_buffer[19] | (sec_buffer[20] << 8);
        }
        return total_sectors;
      }
    }

    // Looks like an MBR header.
    uint32_t max_p_end = 0;
    for (uint16_t table_entry = 0x1BE; table_entry < 0x1FE; table_entry += 16) {
      uint32_t p_start = *((uint16_t*) (sec_buffer + table_entry + 8))
        | (*((uint16_t*) (sec_buffer + table_entry + 10)) << 16);
      uint32_t p_count = *((uint16_t*) (sec_buffer + table_entry + 12))
        | (*((uint16_t*) (sec_buffer + table_entry + 14)) << 16);
      uint32_t p_end = p_start + p_count;
      if (p_end > max_p_end)
        max_p_end = p_end;
    }
    return max_p_end;
  }

  return 0;
}
