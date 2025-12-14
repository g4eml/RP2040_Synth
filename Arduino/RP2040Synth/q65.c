// q65.c
// q65 modes encoding/decoding functions
// 
// (c) 2020 - Nico Palermo, IV3NWV - Microtelecom Srl, Italy
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "q65.h"

static int	_q65_crc6(int *x, int sz);
static void _q65_crc12(int *y, int *x, int sz);

float q65_llh;

int q65_init(q65_codec_ds *pCodec, 	const qracode *pqracode)
{
	float	R;		// code effective rate (after puncturing)
	int		nm;		// bits per symbol

	if (!pCodec)
		return -1;		// why do you called me?

	if (!pqracode)
		return -2;		// invalid qra code

	if (pqracode->M!=64)
		return -3;		// q65 supports only codes over GF(64)

	pCodec->pQraCode = pqracode;

	// allocate buffers used by encoding/decoding functions
	pCodec->x			= (int*)malloc(pqracode->K*sizeof(int));
	pCodec->y			= (int*)malloc(pqracode->N*sizeof(int));
	pCodec->qra_v2cmsg	= (float*)malloc(pqracode->NMSG*pqracode->M*sizeof(float));
	pCodec->qra_c2vmsg	= (float*)malloc(pqracode->NMSG*pqracode->M*sizeof(float));
	pCodec->ix			= (float*)malloc(pqracode->N*pqracode->M*sizeof(float));
	pCodec->ex			= (float*)malloc(pqracode->N*pqracode->M*sizeof(float));

	if (pCodec->x== NULL			||
		pCodec->y== NULL			||
		pCodec->qra_v2cmsg== NULL	||
		pCodec->qra_c2vmsg== NULL	||
		pCodec->ix== NULL			||
		pCodec->ex== NULL) {
			q65_free(pCodec);
			return -4; // out of memory
		}

	return 1;
}

void q65_free(q65_codec_ds *pCodec)
{
	if (!pCodec)
		return;

	// free internal buffers
	if (pCodec->x!=NULL)
		free(pCodec->x);

	if (pCodec->y!=NULL)
		free(pCodec->y);

	if (pCodec->qra_v2cmsg!=NULL)
		free(pCodec->qra_v2cmsg);

	if (pCodec->qra_c2vmsg!=NULL)
		free(pCodec->qra_c2vmsg);

	if (pCodec->ix!=NULL)
		free(pCodec->ix);

	if (pCodec->ex!=NULL)
		free(pCodec->ex);

	pCodec->pQraCode	= NULL;
	pCodec->x			= NULL;
	pCodec->y			= NULL;
	pCodec->qra_v2cmsg	= NULL;
	pCodec->qra_c2vmsg	= NULL;
	pCodec->qra_v2cmsg	= NULL;
	pCodec->ix			= NULL;
	pCodec->ex			= NULL;

	return;
}

int q65_encode(const q65_codec_ds *pCodec, int *pOutputCodeword, const int *pInputMsg)
{
	const qracode *pQraCode;
	int *px;
	int *py;
	int nK;
	int nN;

	if (!pCodec)
		return -1;	// which codec?

	pQraCode = pCodec->pQraCode;
	px = pCodec->x;
	py = pCodec->y;
	nK = _q65_get_message_length(pQraCode);
	nN = _q65_get_codeword_length(pQraCode);

	// copy the information symbols into the internal buffer
	memcpy(px,pInputMsg,nK*sizeof(int));

	// compute and append the appropriate CRC if required
	switch (pQraCode->type) {
		case QRATYPE_NORMAL:
			break;
		case QRATYPE_CRC:
		case QRATYPE_CRCPUNCTURED:
			px[nK] = _q65_crc6(px,nK);
			break;
		case QRATYPE_CRCPUNCTURED2:
			_q65_crc12(px+nK,px,nK);
			break;
		default:
			return -2;	// code type not supported
	}

	// encode with the given qra code
	qra_encode(pQraCode,py,px);

	// puncture the CRC symbols as required
	// and copy the result to the destination buffer
	switch (pQraCode->type) {
		case QRATYPE_NORMAL:
		case QRATYPE_CRC:
			// no puncturing
			memcpy(pOutputCodeword,py,nN*sizeof(int));
			break;
		case QRATYPE_CRCPUNCTURED:
			// strip the single CRC symbol from the encoded codeword
			memcpy(pOutputCodeword,py,nK*sizeof(int));				// copy the systematic symbols 
			memcpy(pOutputCodeword+nK,py+nK+1,(nN-nK)*sizeof(int));	// copy the check symbols skipping the CRC symbol
			break;
		case QRATYPE_CRCPUNCTURED2:
			// strip the 2 CRC symbols from the encoded codeword
			memcpy(pOutputCodeword,py,nK*sizeof(int));				// copy the systematic symbols
			memcpy(pOutputCodeword+nK,py+nK+2,(nN-nK)*sizeof(int)); // copy the check symbols skipping the two CRC symbols 
			break;
		default:
			return -2;	// code type unsupported
	}

	return 1; // ok
}

// // helper functions -------------------------------------------------------------

int _q65_get_message_length(const qracode *pCode)
{
	// return the actual information message length (in symbols)
	// excluding any punctured symbol

	int nMsgLength;

	switch (pCode->type) {
		case QRATYPE_NORMAL:
			nMsgLength = pCode->K;
			break;
		case QRATYPE_CRC:
		case QRATYPE_CRCPUNCTURED:
			// one information symbol of the underlying qra code is reserved for CRC
			nMsgLength = pCode->K-1;
			break;
		case QRATYPE_CRCPUNCTURED2:
			// two code information symbols are reserved for CRC
			nMsgLength = pCode->K-2;
			break;
		default:
			nMsgLength = -1;
	}

	return nMsgLength;
}

int _q65_get_codeword_length(const qracode *pCode)
{
	// return the actual codeword length (in symbols)
	// excluding any punctured symbol
	
	int nCwLength;

	switch (pCode->type) {
		case QRATYPE_NORMAL:
		case QRATYPE_CRC:
			// no puncturing
			nCwLength = pCode->N;
			break;
		case QRATYPE_CRCPUNCTURED:
			// the CRC symbol is punctured
			nCwLength = pCode->N-1;
			break;
		case QRATYPE_CRCPUNCTURED2:
			// the two CRC symbols are punctured
			nCwLength = pCode->N-2;
			break;
		default:
			nCwLength = -1;
	}

	return nCwLength;
}


// CRC generation functions

// crc-6 generator polynomial
// g(x) = x^6 + x + 1  
#define CRC6_GEN_POL 0x30		// MSB=a0 LSB=a5    

// crc-12 generator polynomial
// g(x) = x^12 + x^11 + x^3 + x^2 + x + 1  
#define CRC12_GEN_POL 0xF01		// MSB=a0 LSB=a11

// g(x) = x^6 + x^2 + x + 1 (as suggested by Joe. See i.e.:  https://users.ece.cmu.edu/~koopman/crc/)
// #define CRC6_GEN_POL 0x38  // MSB=a0 LSB=a5. Simulation results are similar


static int _q65_crc6(int *x, int sz)
{
	int k,j,t,sr = 0;
	for (k=0;k<sz;k++) {
		t = x[k];
		for (j=0;j<6;j++) {
			if ((t^sr)&0x01)
				sr = (sr>>1) ^ CRC6_GEN_POL;
			else
				sr = (sr>>1);
			t>>=1;
			}
		}

	return sr;
}

static void _q65_crc12(int *y, int *x, int sz)
{
	int k,j,t,sr = 0;
	for (k=0;k<sz;k++) {
		t = x[k];
		for (j=0;j<6;j++) {
			if ((t^sr)&0x01)
				sr = (sr>>1) ^ CRC12_GEN_POL;
			else
				sr = (sr>>1);
			t>>=1;
			}
		}

	y[0] = sr&0x3F; 
	y[1] = (sr>>6);
}
