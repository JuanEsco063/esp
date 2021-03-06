/*

    LEON2/3 LIBIO low-level routines 
    Written by Jiri Gaisler.
    Copyright (C) 2004  Cobham Gaisler AB

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

	.text
	.global _prom_ft, _clean_ram0, _clean_ram0_size, _clean_ram1, _clean_ram1_size

_prom_ft:
	/* leon2 */
	set	LEON_REG, %g1
	ld	[%g1 + 0x08], %g3
	set	0x200, %g4		! Ram edac enable
	or	%g3, %g4, %g3
  	st	%g3, [%g1 + 0x08]
	
	retl
	 nop





