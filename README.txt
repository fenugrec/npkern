**************************
nisprog reflashing kernel
**************************

See "doc/COMPILING.txt", and "doc/USING.txt", for details.

In this document:
* General info
* Kernel Features
* Limitations
* Source code structure
* Porting to other devices / testing on different ECUs



# General info

Project presentation:
http://www.romraider.com/forum/viewtopic.php?f=45&t=12999

Original crowdfunding campaign:
https://www.crowdsupply.com/nisprog/reflashing-kernel

This is open-source, thanks to all the campaign backers. See "SPONSORS.txt" !

This project completes an entirely open-source solution for reflashing Nissan/Infiniti ECUs over a K-line communications link.

## Which ECUs Will This Work On?
Most gasoline Nissan / Infiniti ECUs from ~ 2002 onwards share very similar ECU hardware, based on SuperH microcontrollers manufactured by Renesas (previously Hitachi). This project supports ECUs that use the OBD-II K line signal for diagnostics communications.

## How Does the Reflash Process Work?
The process is carried out entirely over the OBD-II K Line serial communications link through an undocumented set of manufacturer-defined extensions to the standard ISO14230 protocol. Recently, the necessary commands have been reverse-engineered revealing the required steps:

1. Establish connection to the ECU.
2. Send the first stage data payload: this is the kernel, a simple program that will receive commands and data for the reflash.
3. Make the ECU run the kernel. From this point on, the kernel runs from RAM and effectively takes control of the whole ECU.
4. Send the reflash commands and new firmware data to the kernel.
5. Reset the ECU: the new firmware will now be executed, and the kernel will be completely removed.


# Kernel Features
The basic kernel is an implementation of an ISO14230-compliant protocol with extensions; it implements the following requests:

- fast ROM dump (tested up to 5.4 kB/s; typical speed is ~100 B/s without a kernel)
- Read ROM/RAM/external EEPROM
- Write to RAM
- Erase flash ROM block
- Write flash ROM block

# Limitations
There are several important points to be aware of when using Nisprog:

- The ECU is a safety-critical system in a car. Reflashing an ECU can void warranties, reduce the vehicle's reliability, and cause a whole variety of undesirable consequences. Use of this project and any associated tools (freediag, Nisprog, etc) is of course entirely at the user's risk. Standard disclaimers apply.

- Reflashing an ECU may be illegal in some areas. Responsibility in this matter lies again with the user.

- Diesel ECUs are not supported, the reflashing process is entirely different.

- Tuning and modifying ROMs are complex skills that can only be acquired through significant investments of time and effort!

- These SH705* ICs are typically rated for about hundred re-write cycles, beyond which Flash retention may degrade.
Experience seems to indicate the actual endurance can be significantly higher, but as with any Flash memory, excessive re-writes should be avoided (for example, live-tuning applications)



# Source code structure

precompiled/* : kernels compiled with the latest source release (possibly not as up-to-date as the git repo)
reg_defines/* : includes for peripheral register address definitions

cmd_parser* : command parser and dispatcher for the iso14230 communications over K line
eep_funcs* : onboard EEPROM access helpers / functions
functions.h : helpers for low-level SuperH intrinsics (setting special registers etc)
intprg, ivect* : interrupt vectors and handlers
iso_cmds.h : definitions for supported ISO commands / SIDs
lkr_* : linker script, this defines where the kernel will be compiled + loaded in RAM
main.c : main
platf* : this is to split the CPU (platform)-specific code from the generic code.
pl_flash_*: platform-specific reflash back-end etc.
start_705x.s : initial self-loader code, this is the first thing that runs at the RAMjump step.
stypes.h : shorthand for common types



# Porting to other devices
(WIP)
- verify if the linker script is adequate
- implement the functions declared in platf.h, in a new platf_XYZ.c file or maybe inside one of the existing platf*.c files if similar enough
- check if the WDT function will be adequate (i.e. toggle a certain pin every 2ms). 
- on that topic : the kernel obtains the WDT pin info at startup (see main.c , "struct rj_preload"). How this works is:
 -- just before running the kernel, it fills in a few RAM values at a certain location (always at FFFF8000 for 7055/7058 ?? uncertain).
 -- these values are the WDT pin port address, WDT pin #, and some other unknown values.

# License
This project and its source code is licensed under the GPLv3
```
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
```