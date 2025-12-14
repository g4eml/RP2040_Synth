#ifndef Q65_SUBS_H
#define Q65_SUBS_H

#ifdef __cplusplus
extern "C" {
#endif

// Fortran-callable Q65 encoder
// Input:  msg[13] = 13 six-bit integers (78-bit payload)
// Output: codeword[63] = 63 Q65 symbols (0..63)
void q65_enc_(int msg[13], int codeword[63]);

#ifdef __cplusplus
}
#endif

#endif // Q65_SUBS_H
