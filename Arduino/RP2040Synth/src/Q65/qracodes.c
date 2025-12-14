// qracodes.c
// Q-ary RA codes encoding/decoding functions
// 
// (c) 2016 - Nico Palermo, IV3NWV - Microtelecom Srl, Italy
// ------------------------------------------------------------------------------
// This file is part of the qracodes project, a Forward Error Control
// encoding/decoding package based on Q-ary RA (Repeat and Accumulate) LDPC codes.
//
//    qracodes is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//    qracodes is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with qracodes source distribution.  
//    If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "qracodes.h"

int qra_encode(const qracode *pcode, int *y, const int *x)
{
	int k,j,kk,jj;
	int t, chk = 0;

	const int K = pcode->K;
	const int M = pcode->M;
	const int NC= pcode->NC;
	const int a = pcode->a;
	const int  *acc_input_idx  = pcode->acc_input_idx;
	const int *acc_input_wlog = pcode->acc_input_wlog;
	const int  *gflog		   = pcode->gflog;
	const int *gfexp          = pcode->gfexp;

	// copy the systematic symbols to destination
	memcpy(y,x,K*sizeof(int));

	y = y+K;	// point to check symbols

	// compute the code check symbols as a weighted accumulation of a permutated
	// sequence of the (repeated) systematic input symbols:
	// chk(k+1) = x(idx(k))*alfa^(logw(k)) + chk(k)
	// (all operations performed over GF(M))

	if (a==1) { // grouping factor = 1
		for (k=0;k<NC;k++) {
			t = x[acc_input_idx[k]];
			if (t) {
				// multiply input by weight[k] and xor it with previous check
				t = (gflog[t] + acc_input_wlog[k])%(M-1);
				t = gfexp[t];
				chk ^=t;
				}
			y[k] = chk;
			}
			return 1;
		}
	else { // grouping factor > 1
		for (k=0;k<NC;k++) {
			kk = a*k;
			for (j=0;j<a;j++) {
				jj = kk+j;
				// irregular grouping support
				if (acc_input_idx[jj]<0)
					continue;
				t = x[acc_input_idx[jj]];
				if (t) {
					// multiply input by weight[k] and xor it with previous check
					t = (gflog[t] + acc_input_wlog[jj])%(M-1);
					t = gfexp[t];
					chk ^=t;
					}
				}
			y[k] = chk;
			}
			return 1;
		} 
}

