// qracodes.h
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

#ifndef _qracodes_h_
#define _qracodes_h_

// type of codes
#define QRATYPE_NORMAL			0x00 // normal code 
#define QRATYPE_CRC 			0x01 // code with crc - last information symbol is a CRC-6
#define QRATYPE_CRCPUNCTURED 	0x02 // the CRC-6 symbol is punctured (not sent along the channel)
#define QRATYPE_CRCPUNCTURED2 	0x03 // code with CRC-12. The two crc symbols are punctured


typedef struct {
	// code parameters
	const int K;			// number of information symbols
	const int N;			// codeword length in symbols
	const int m;			// bits/symbol
	const int M;			// Symbol alphabet cardinality (2^m)
	const int a;			// code grouping factor
	const int NC;			// number of check symbols (N-K)
	const int V;			// number of variables in the code graph (N)
	const int C;			// number of factors in the code graph (N +(N-K)+1)
	const int NMSG;		// number of msgs in the code graph
	const int MAXVDEG;		// maximum variable degree 
	const int MAXCDEG;		// maximum factor degree
	const int type;		// see QRATYPE_xx defines
	const float R;			// code rate (K/N)
	const char  name[64];	// code name
	// tables used by the encoder
	const int	 *acc_input_idx;	
	const int   *acc_input_wlog;
	const int	 *gflog;
	const int   *gfexp;
	// tables used by the decoder -------------------------
	const int *msgw;
	const int *vdeg;
	const int *cdeg;
	const int  *v2cmidx;
	const int  *c2vmidx;
	const int  *gfpmat;
} qracode;

#ifdef __cplusplus
extern "C" {
#endif

int   qra_encode(const qracode *pcode, int *y, const int *x);

#ifdef __cplusplus
}
#endif

#endif // _qracodes_h_
