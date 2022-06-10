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

#include "functions.h"	//for set_imask etc

#include "mfg.h"
#include "platf.h"
#include "cmd_parser.h"
   

void main(void) {
	set_imask(0x0F);	// disable interrupts (mask = b'1111)

	init_mfg();
	init_platf();

	/* and lower prio mask to let WDT run */
	set_imask(0x07);

	cmd_init(SCI_DEFAULTDIV);
	cmd_loop();

	//we should never get here; if so : die
	die();

}
