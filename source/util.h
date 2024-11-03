// SPDX-License-Identifier: MIT
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2024

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <nds.h>
#include <nds/arm9/dldi.h>

uint32_t msc_find_block_count(const DISC_INTERFACE *io);

#endif /* _UTIL_H_ */
