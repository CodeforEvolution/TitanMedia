
typedef unsigned char    UBYTE;             /* WIN32 definitions            */
typedef unsigned short   UWORD;
typedef unsigned long    UDWORD;


void UpdKeys(UBYTE *Buf);
void SetCryptKeys(char *Password);
void SetOldKeys(char *Password);
void DecryptBlock(unsigned char *Buf);
void EncryptBlock(UBYTE *Buf);
void InitCRC(void);
UDWORD CalcCRC32(UDWORD StartCRC,UBYTE *Addr,UDWORD Size);
