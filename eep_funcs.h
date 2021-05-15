#ifndef _EEP_FUNCS_H
#define _EEP_FUNCS_H

/* (c) copyright fenugrec 2016
 * GPLv3
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stypes.h"

//call ROM's  eeprom_read function. Does nothing if eep_setptr() hasn't been called yet

void eep_read16(uint8_t addr, uint16_t *dest);

//set the address of the ROM's eeprom_read function
void eep_setptr(uint32_t newaddr);


#endif
