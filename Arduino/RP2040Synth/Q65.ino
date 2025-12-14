//Q65 top level. Calls functions extracted from the WJST-X source code. Relevent licences apply. 

#include "src/Q65/q65_subs.h" 

int msg[13] = {0};
int codeword[63];  // Encoded symbols
int channelsymbols[85];   //Channel symbols including sync characters (Tone 0)

//Encodes a ^5 Standard message in the form 'CQ CALL LOC' 
//message parameter must have call and loc seperated by space.

void Q65Encode(const char *message, uint8_t *buffer)
{
  char from[7];
  char grid[5];

  splitUpToTwo(message,from,grid);
  
   encode_wsjt_message("CQ",from,grid,msg);
   q65_enc_(msg, codeword);
   maptoChannelSymbols();
   for(int i =0;i<85;i++)
    {
        buffer[i] = channelsymbols[i]; 
    }
}

uint8_t sync[22] = {0,8,11,12,14,21,22,25,26,32,34,37,45,49,54,59,61,65,68,73,75,84};

void maptoChannelSymbols(void)
{
int from = 0;
int j = 0;

for(int to = 0;to < 85;to++)
 {
  if(to == sync[j])
   {
    j=j+1;
    channelsymbols[to] = 0;
   }
  else
   {
     channelsymbols[to] = codeword[from++] +1;
   }

 }
}



// WSJT-X special callsign codes (from packjt.cpp)
static int32_t special_call_code(const char *call) {
    if (strcmp(call, "CQ") == 0)       return 2;
    if (strcmp(call, "QRZ") == 0)      return 1;
    if (strcmp(call, "DE") == 0)       return 0;
    return -1; // not special
}

static uint32_t pack_callsign_wsjt(const char *call)
{
    int32_t sp = special_call_code(call);
    if (sp >= 0)
        return (uint32_t)sp;       // SPECIAL CALLSIGN → use reserved code

    // Otherwise pack normally (base-37, padded to length 6)
    return stdCallToC28(call);
}


// ----------------------------------------------------------
// MAIN FUNCTION: text message → 13 WSJT-X symbols (0–63)
// ----------------------------------------------------------
void encode_wsjt_message(const char *call1,
                         const char *call2,
                         const char *grid4,
                         int symbols13[13])
{
    uint32_t c1 = pack_callsign_wsjt(call1);   // 28 bits
    uint32_t c2 = pack_callsign_wsjt(call2);   // 28 bits
    uint16_t g  = grid4ToG15(grid4);         // 15 bits

    // Build 78-bit payload (type=1)

   
    // 28 bits = CALL1
    // 1 bit = /r
    // 28 bits = CALL2
    // 1 bit = /r
    // 1 Bit = R
    // 15 bits = GRID
    // 3 bits = type(1)
    // 1 bit packing

    uint8_t b[78] = {0};

    // c1 → bits 0..27 (28 bits)
    for (int i=0;i<28;i++) b[i]  = (c1 >> (27-i)) & 1;
    // rover bit
    b[28]=0;
    // c2 → bits 29..56 (28 bits)
    for (int i=0;i<28;i++) b[29+i] = (c2 >> (27-i)) & 1;
    // rover bit
    b[57]=0;
    //R bit
    b[58]=0;
    // grid → bits 59..73 (15 bits)
    for (int i=0;i<15;i++) b[59+i] = (g >> (14-i)) & 1;

        //message type = 1
    b[74]=0;
    b[75]=0;
    b[76]=1;

    // packing bit
    b[77] = 0;


    // Extract 13 symbols
    for (int s=0; s<13; s++) 
    {
        uint8_t v = 0;
        for (int k=0;k<6;k++) 
        {
            int bitIndex = s*6 + k;
            v = (v << 1) | (b[bitIndex] & 1);
        }
        symbols13[s] = v & 0x3F;
    }
}


// Adjust input to 6 characters (pad with space if short)
void adjustCall(const char* call, char* modcall) 
{
  for (int i = 0; i < 6; i++)             // replace any nulls with spaces
  {
    if (call[i] == '\0')
    {
     modcall[i] = ' ';
    } 
    else
    {
      modcall[i] = call[i];
    }
  }

  if((call[1] >='0') && (call[1] <= '9'))      // if the second character is a number
   {                                          //move everything up one character
    for(int i = 5;i>0;i--)
     {
      modcall[i] = call[i-1];
     }
     modcall[0] = ' ';                       //and add a space at the start
   }


}

// Find index of character in charset (returns 0 if not found)
int charIndex(const char* charset, char c) {
  for (int i = 0; charset[i] != '\0'; i++) {
    if (charset[i] == c) return i;
  }
  return 0;
}

void splitUpToTwo(const char *input, char *part1, char *part2) 
{
  char *parts[2] = { part1, part2 };
  int partIndex = 0;
  int idx = 0;

  while (partIndex < 2 && input[idx] != '\0') 
  {

    // Skip leading spaces for this part
    while (input[idx] == ' ') idx++;

    // If we hit end after skipping spaces → stop
    if (input[idx] == '\0') break;

    // Copy characters until next space or end
    int p = 0;
    while (input[idx] != ' ' && input[idx] != '\0') 
    {
      parts[partIndex][p++] = input[idx++];
    }
    parts[partIndex][p] = '\0';  // terminate this part

    partIndex++;
  }

  // Fill remaining parts with empty strings
  while (partIndex < 2) 
  {
    parts[partIndex][0] = '\0';
    partIndex++;
  }
}


// Convert callsign to C28 number
uint32_t stdCallToC28(const char* call) 
{
// Character sets for each position
const char a1[] = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char a2[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char a3[] = "0123456789";
const char a4[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char modcall[8];

// Constants from Fortran
const uint32_t NTOKENS = 2063592;
const uint32_t MAX22 = 4194304;

 adjustCall(call,modcall);

  int i1 = charIndex(a1, modcall[0]);
  int i2 = charIndex(a2, modcall[1]);
  int i3 = charIndex(a3, modcall[2]);
  int i4 = charIndex(a4, modcall[3]);
  int i5 = charIndex(a4, modcall[4]);
  int i6 = charIndex(a4, modcall[5]);

  uint32_t n28 = NTOKENS + MAX22 +
                 36UL*10UL*27UL*27UL*27UL*i1 +
                 10UL*27UL*27UL*27UL*i2 +
                 27UL*27UL*27UL*i3 +
                 27UL*27UL*i4 +
                 27UL*i5 +
                 i6;

  return n28;
}


// Check if string is a valid 4-character grid
bool isGrid4(const char* w) 
{
  return strlen(w) == 4 &&
         w[0] >= 'A' && w[0] <= 'R' &&
         w[1] >= 'A' && w[1] <= 'R' &&
         w[2] >= '0' && w[2] <= '9' &&
         w[3] >= '0' && w[3] <= '9';
}

// Convert grid4 or signal report to g15 value
int grid4ToG15(const char* w) 
{
  const int MAXGRID4 = 32400;
  int igrid4 = 0;

  if (isGrid4(w) && strcmp(w, "RR73") != 0) 
  {
    int j1 = (w[0] - 'A') * 18 * 10 * 10;
    int j2 = (w[1] - 'A') * 10 * 10;
    int j3 = (w[2] - '0') * 10;
    int j4 = (w[3] - '0');
    igrid4 = j1 + j2 + j3 + j4;
  } 
  else 
  {
    char c1 = w[0];
    int irpt = 0;

    if ((c1 != '+' && c1 != '-') && strcmp(w, "RRR") != 0 && strcmp(w, "RR73") != 0 &&
        strcmp(w, "73") != 0 && strlen(w) != 0) 
    {
      return -1; // invalid input
    }

    if (c1 == '+' || c1 == '-') 
    {
      irpt = atoi(w) + 35;  // signal report adjustment
    } 
    else if (strlen(w) == 0) 
    {
      irpt = 1;
    } 
    else if (strcmp(w, "RRR") == 0) 
    {
      irpt = 2;
    } 
    else if (strcmp(w, "RR73") == 0) 
    {
      irpt = 3;
    } 
    else if (strcmp(w, "73") == 0) 
    {
      irpt = 4;
    }

    igrid4 = MAXGRID4 + irpt;
  }

  return igrid4;
}


