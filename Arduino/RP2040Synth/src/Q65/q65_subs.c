// q65_subs.c

/* Fortran interface for Q65 codec

   To encode a Q65 message:
   
   integer x(13)        !Message payload, 78 bits as 13 six-bit integers
   integer y(63)        !Codeword, 63 six-bit integers
   call q65_enc(imsg,icodeword)
*/

#include "qra15_65_64_irr_e23.h"	// QRA code used by Q65
#include "q65.h"
#include <stdio.h>
#include <stdlib.h>

static q65_codec_ds codec;

void q65_enc_(int x[], int y[])
{

  static int first=1;
  if (first) {
    // Set the QRA code, allocate memory, and initialize
    int rc = q65_init(&codec,&qra15_65_64_irr_e23);
    if (rc<0) {
      printf("error in q65_init()\n");
      exit(0);
    }
    first=0;
  }
  // Encode message x[13], producing codeword y[63]
  q65_encode(&codec,y,x);
}
