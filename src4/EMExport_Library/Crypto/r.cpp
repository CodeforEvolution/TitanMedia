#include "r.h"

#include <stdio.h>
#include <memory.h>
#include <string.h>

unsigned char PN1,PN2,PN3;
unsigned short OldKey[4];
#define _WIN_32
#define _USE_ASM

#define NROUNDS 32

#define rol(x,n)  (((x)<<(n)) | ((x)>>(8*sizeof(x)-(n))))
#define ror(x,n)  (((x)>>(n)) | ((x)<<(8*sizeof(x)-(n))))

#define substLong(t) ( (UDWORD)SubstTable[(int)t&255] | \
           ((UDWORD)SubstTable[(int)(t>> 8)&255]<< 8) | \
           ((UDWORD)SubstTable[(int)(t>>16)&255]<<16) | \
           ((UDWORD)SubstTable[(int)(t>>24)&255]<<24) )


UDWORD CRCTab[256];

UBYTE SubstTable[256];
UBYTE InitSubstTable[256]={
  215, 19,149, 35, 73,197,192,205,249, 28, 16,119, 48,221,  2, 42,
  232,  1,177,233, 14, 88,219, 25,223,195,244, 90, 87,239,153,137,
  255,199,147, 70, 92, 66,246, 13,216, 40, 62, 29,217,230, 86,  6,
   71, 24,171,196,101,113,218,123, 93, 91,163,178,202, 67, 44,235,
  107,250, 75,234, 49,167,125,211, 83,114,157,144, 32,193,143, 36,
  158,124,247,187, 89,214,141, 47,121,228, 61,130,213,194,174,251,
   97,110, 54,229,115, 57,152, 94,105,243,212, 55,209,245, 63, 11,
  164,200, 31,156, 81,176,227, 21, 76, 99,139,188,127, 17,248, 51,
  207,120,189,210,  8,226, 41, 72,183,203,135,165,166, 60, 98,  7,
  122, 38,155,170, 69,172,252,238, 39,134, 59,128,236, 27,240, 80,
  131,  3, 85,206,145, 79,154,142,159,220,201,133, 74, 64, 20,129,
  224,185,138,103,173,182, 43, 34,254, 82,198,151,231,180, 58, 10,
  118, 26,102, 12, 50,132, 22,191,136,111,162,179, 45,  4,148,108,
  161, 56, 78,126,242,222, 15,175,146, 23, 33,241,181,190, 77,225,
    0, 46,169,186, 68, 95,237, 65, 53,208,253,168,  9, 18,100, 52,
  116,184,160, 96,109, 37, 30,106,140,104,150,  5,204,117,112, 84
};

UDWORD Key[4];


void EncryptBlock(UBYTE *Buf)
{
  int I;

  UDWORD A,B,C,D,T,TA,TB;
#ifdef NON_INTEL_BYTE_ORDER
  A=((UDWORD)Buf[0]|((UDWORD)Buf[1]<<8)|((UDWORD)Buf[2]<<16)|
      ((UDWORD)Buf[3]<<24))^Key[0];
  B=((UDWORD)Buf[4]|((UDWORD)Buf[5]<<8)|((UDWORD)Buf[6]<<16)|
      ((UDWORD)Buf[7]<<24))^Key[1];
  C=((UDWORD)Buf[8]|((UDWORD)Buf[9]<<8)|((UDWORD)Buf[10]<<16)|
      ((UDWORD)Buf[11]<<24))^Key[2];
  D=((UDWORD)Buf[12]|((UDWORD)Buf[13]<<8)|((UDWORD)Buf[14]<<16)|
      ((UDWORD)Buf[15]<<24))^Key[3];
#else
  UDWORD *BufPtr;
  BufPtr=(UDWORD *)Buf;
  A=BufPtr[0]^Key[0];
  B=BufPtr[1]^Key[1];
  C=BufPtr[2]^Key[2];
  D=BufPtr[3]^Key[3];
#endif
  for(I=0;I<NROUNDS;I++)
  {
    T=((C+rol(D,11))^Key[I&3]);
    TA=A^substLong(T);
    T=((D^rol(C,17))+Key[I&3]);
    TB=B^substLong(T);
    A=C;
    B=D;
    C=TA;
    D=TB;
  }
#ifdef NON_INTEL_BYTE_ORDER
  C^=Key[0];
  Buf[0]=(UBYTE)C;
  Buf[1]=(UBYTE)(C>>8);
  Buf[2]=(UBYTE)(C>>16);
  Buf[3]=(UBYTE)(C>>24);
  D^=Key[1];
  Buf[4]=(UBYTE)D;
  Buf[5]=(UBYTE)(D>>8);
  Buf[6]=(UBYTE)(D>>16);
  Buf[7]=(UBYTE)(D>>24);
  A^=Key[2];
  Buf[8]=(UBYTE)A;
  Buf[9]=(UBYTE)(A>>8);
  Buf[10]=(UBYTE)(A>>16);
  Buf[11]=(UBYTE)(A>>24);
  B^=Key[3];
  Buf[12]=(UBYTE)B;
  Buf[13]=(UBYTE)(B>>8);
  Buf[14]=(UBYTE)(B>>16);
  Buf[15]=(UBYTE)(B>>24);
#else
  BufPtr[0]=C^Key[0];
  BufPtr[1]=D^Key[1];
  BufPtr[2]=A^Key[2];
  BufPtr[3]=B^Key[3];
#endif
  UpdKeys(Buf);
}


void DecryptBlock(UBYTE *Buf)
{
  int I;
  UBYTE InBuf[16];
  UDWORD A,B,C,D,T,TA,TB;
#ifdef NON_INTEL_BYTE_ORDER
  A=((UDWORD)Buf[0]|((UDWORD)Buf[1]<<8)|((UDWORD)Buf[2]<<16)|
      ((UDWORD)Buf[3]<<24))^Key[0];
  B=((UDWORD)Buf[4]|((UDWORD)Buf[5]<<8)|((UDWORD)Buf[6]<<16)|
      ((UDWORD)Buf[7]<<24))^Key[1];
  C=((UDWORD)Buf[8]|((UDWORD)Buf[9]<<8)|((UDWORD)Buf[10]<<16)|
      ((UDWORD)Buf[11]<<24))^Key[2];
  D=((UDWORD)Buf[12]|((UDWORD)Buf[13]<<8)|((UDWORD)Buf[14]<<16)|
      ((UDWORD)Buf[15]<<24))^Key[3];
#else
  UDWORD *BufPtr;
  BufPtr=(UDWORD *)Buf;
  A=BufPtr[0]^Key[0];                       /* xxx may be this can be       */
  B=BufPtr[1]^Key[1];                       /* optimized in assembler       */
  C=BufPtr[2]^Key[2];
  D=BufPtr[3]^Key[3];
#endif
  memcpy(InBuf,Buf,sizeof(InBuf));
  for(I=NROUNDS-1;I>=0;I--)
  {
    T=((C+rol(D,11))^Key[I&3]);
    TA=A^substLong(T);
    T=((D^rol(C,17))+Key[I&3]);
    TB=B^substLong(T);
    A=C;
    B=D;
    C=TA;
    D=TB;
  }
#ifdef NON_INTEL_BYTE_ORDER
  C^=Key[0];
  Buf[0]=(UBYTE)C;
  Buf[1]=(UBYTE)(C>>8);
  Buf[2]=(UBYTE)(C>>16);
  Buf[3]=(UBYTE)(C>>24);
  D^=Key[1];
  Buf[4]=(UBYTE)D;
  Buf[5]=(UBYTE)(D>>8);
  Buf[6]=(UBYTE)(D>>16);
  Buf[7]=(UBYTE)(D>>24);
  A^=Key[2];
  Buf[8]=(UBYTE)A;
  Buf[9]=(UBYTE)(A>>8);
  Buf[10]=(UBYTE)(A>>16);
  Buf[11]=(UBYTE)(A>>24);
  B^=Key[3];
  Buf[12]=(UBYTE)B;
  Buf[13]=(UBYTE)(B>>8);
  Buf[14]=(UBYTE)(B>>16);
  Buf[15]=(UBYTE)(B>>24);
#else
  BufPtr[0]=C^Key[0];
  BufPtr[1]=D^Key[1];
  BufPtr[2]=A^Key[2];
  BufPtr[3]=B^Key[3];
#endif
  UpdKeys(InBuf);
}


void UpdKeys(UBYTE *Buf)
{
  int I;
  for (I=0;I<16;I+=4)
  {
    Key[0]^=CRCTab[Buf[I]];                 /* xxx may be I'll rewrite this */
    Key[1]^=CRCTab[Buf[I+1]];               /* in asm for speedup           */
    Key[2]^=CRCTab[Buf[I+2]];
    Key[3]^=CRCTab[Buf[I+3]];
  }
}

void SetCryptKeys(char *Password)
{
  unsigned int I,J,K,PswLength;
  unsigned char N1,N2;
  unsigned char Psw[256];
  
#if !defined _USE_ASM
  UBYTE Ch;
#endif

  SetOldKeys(Password);

  Key[0]=0xD3A3B879L;
  Key[1]=0x3F6D12F7L;
  Key[2]=0x7515A235L;
  Key[3]=0xA4E7F123L;
  memset(Psw,0,sizeof(Psw));
  strcpy((char *)Psw,Password);
  PswLength=strlen(Password);
  memcpy(SubstTable,InitSubstTable,sizeof(SubstTable));
  
  for (J=0;J<256;J++)
    for (I=0;I<PswLength;I+=2)
    {
      N2=(unsigned char)CRCTab[(Psw[I+1]+J)&0xFF];
      for (K=1, N1=(unsigned char)CRCTab[(Psw[I]-J)&0xFF];
           (N1!=N2) && (N1 < 256);      /* I had to add "&& (N1 < 256)",    */
           N1++, K++)                   /* because the system crashed with  */
          {                             /* encrypted RARs                   */
#ifdef _USE_ASM

#ifdef _WIN_32
          __asm {

                    mov ebx, Offset SubstTable
                    mov edx, ebx

                    xor ecx, ecx            // read SubstTable[N1]...
                    mov cl, N1
                    add ebx, ecx
                    mov al, byte ptr[ebx]
              
                    mov cl, N1              // read SubstTable[(N1+I+K)&0xFF]...
                    add ecx, I
                    add ecx, K
                    and ecx, 0xFF
                    add edx, ecx
                    mov ah, byte ptr[edx]

                    mov  byte ptr[ebx], ah  // and write back
                    mov  byte ptr[edx], al

               }
#else
                     __asm__ __volatile__ (
                    "
                    xorl %%ecx, %%ecx
                    movl %2, %%ecx                     /* ecx = N1 */
                    mov %%ebx, %%edx
                    addl %%ecx, %%ebx

                    addl %0, %%ecx
                    addl %1, %%ecx
                    andl $0x000000FF, %%ecx
                    addl %%ecx, %%edx
                    
                    movb (%%ebx), %%al
                    movb (%%edx), %%ah

                    movb  %%ah, (%%ebx)     /* and write back */
                    movb  %%al, (%%edx)
                    "
                    : : "g" ((long)I),
                        "g" ((long)K),
                        "g" ((long)N1),
                        "ebx"((long)SubstTable)
                    : "ecx", "edx"
     
              );
#endif
                     
#else
            /* Swap(&SubstTable[N1],&SubstTable[(N1+I+K)&0xFF]);            */
             Ch=SubstTable[N1];         
             SubstTable[N1]=SubstTable[(N1+I+K)&0xFF];
             SubstTable[(N1+I+K)&0xFF]=Ch;
#endif
          }
    }
  for (I=0;I<PswLength;I+=16)
    EncryptBlock(&Psw[I]);
}


void SetOldKeys(char *Password)
{
  UDWORD PswCRC;
  UBYTE Ch;
  PswCRC=CalcCRC32(0xFFFFFFFFL,(UBYTE*)Password,strlen(Password));
  OldKey[0]=(UWORD)PswCRC;
  OldKey[1]=(UWORD)(PswCRC>>16);
  OldKey[2]=OldKey[3]=0;
  PN1=PN2=PN3=0;
  while ((Ch=*Password)!=0)
  {
    PN1+=Ch;
    PN2^=Ch;
    PN3+=Ch;
    PN3=(UBYTE)rol(PN3,1);
    OldKey[2]^=((UWORD)(Ch^CRCTab[Ch]));
    OldKey[3]+=((UWORD)(Ch+(CRCTab[Ch]>>16)));
    Password++;
  }
}

void InitCRC(void)
{
  int I, J;
  UDWORD C;
  for (I=0;I<256;I++)
  {
    for (C=I,J=0;J<8;J++)
      C=(C & 1) ? (C>>1)^0xEDB88320L : (C>>1);
    CRCTab[I]=C;
  }
}


UDWORD CalcCRC32(UDWORD StartCRC,UBYTE *Addr,UDWORD Size)
{
  unsigned int I;
  for (I=0; I<Size; I++)
    StartCRC = CRCTab[(UBYTE)StartCRC ^ Addr[I]] ^ (StartCRC >> 8);
  return(StartCRC);
}
























