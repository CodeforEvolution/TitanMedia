#if !defined(SONORK_CODEC_IO_H)
#define SONORK_CODEC_IO_H

/*
	Sonork Messaging System

	Portions Copyright (C) 2001 Sonork SRL:

	This program is free software; you can redistribute it and/or modify
	it under the terms of the Sonork Source Code License (SSCL) Version 1.

	You should have received a copy of the SSCL	along with this program;
	if not, write to sscl@sonork.com.

	You should NOT use this source code before reading and accepting the
	Sonork Source Code License (SSCL), doing so will indicate your agreement
	to the the terms which may be differ for each version of the software.

	This comment section, indicating the existence and requirement of
	acceptance of the SSCL may not be removed from the source code.
*/


#include "srk_data_types.h"


// -------------------------------------------------------
// These flags to be used while encoding UserDataNotes

#define SONORK_UDN_ENCODE_NOTES		0x100000
#define SONORK_UDN_ENCODE_AUTH		0x200000
#define SONORK_UDN_ENCODE_PASSWORD	0x400000
#define SONORK_UDN_ENCODE_SIZE_MASK 	0x0fffff

inline UINT
	CODEC_Size(const SONORK_DWORD2*)  	{ return sizeof(SONORK_DWORD2);}

inline UINT
	CODEC_Size(const SONORK_DWORD4*)  	{ return sizeof(SONORK_DWORD4);}

inline UINT
	CODEC_Size(const TSonorkAuth1*)	  	{ return sizeof(TSonorkAuth1);}

inline UINT
	CODEC_Size(const TSonorkAuth2*)	  	{ return sizeof(TSonorkAuth2);}

inline UINT
	CODEC_Size(const TSonorkCtrlMsg*) 	{ return sizeof(TSonorkCtrlMsg);}

inline UINT
	CODEC_Size1(const TSonorkLocus1*)	{ return sizeof(TSonorkLocus1);}

UINT
	CODEC_Locus2Size_obsolete(const TSonorkLocusAtom*);

UINT
	CODEC_Locus3Size_obsolete(const TSonorkLocusAtom*);



UINT
	CODEC_Size(SONORK_USER_INFO_LEVEL, DWORD version);

UINT
	CODEC_Size(const TSonorkPhysAddr*V);

UINT
	CODEC_UserAddrSize_obsolete(const TSonorkUserAddr *V , BOOL V15Addr);

UINT
	CODEC_OldSidNotifSize_obsolete(const TSonorkSidNotification*pV);

inline UINT
	CODEC_Size(const TSonorkOldMsgNotification*)
	{ return sizeof(TSonorkOldMsgNotification); }

UINT
	CODEC_Size(const TSonorkShortString*V);

inline UINT
	CODEC_Size(const TSonorkServiceLocus1*)
	{ return sizeof(TSonorkServiceLocus1);}

#if SONORK_CODEC_LEVEL>5
UINT	CODEC_Size(const TSonorkUserDataNotes*,UINT encode_flags,DWORD version);
#endif

class 	TSonorkCodecIO
{
protected:
	struct {
		union {
		const 	BYTE*rd;
			BYTE*wr;
		}ptr;
		DWORD			limit;
		DWORD			index;
		SONORK_ATOM_TYPE	type;
		SONORK_RESULT   	result;
		struct {
			SONORK_C_CSTR	module;
			DWORD		line;
		}error;
		DWORD			version;
	}codec;

	TSonorkCodecIO(const void*ptr, DWORD pSize, SONORK_ATOM_TYPE pType, DWORD pVersion);
public:

	DWORD
		Offset() const
		{ return codec.index;}

	DWORD
		Size()	const
		{ return codec.index;}

	DWORD
		Limit() const
		{ return codec.limit;}

	DWORD
		RemainingSize()	const
		{ return Limit()-Size();}

	DWORD
		SetVersion(DWORD v);

	
	SONORK_ATOM_TYPE
		DataType() const
		{ return codec.type;}

	SONORK_RESULT
		Result() const
		{ return codec.result;}

	SONORK_C_CSTR
		ErrorModule()  const
		{ return codec.error.module;}

	DWORD
		Version() const
		{ return codec.version; }
		
	DWORD
		ErrorLine() const
		{ return codec.error.line;}

	bool
		CodecOk() const
		{ return codec.result==SONORK_RESULT_OK;}

	const BYTE*
		OffsetPtr() const
		{ return codec.ptr.rd+codec.index;}

	void
		SetError(SONORK_RESULT,SONORK_C_CSTR module,DWORD module_line, DWORD ex_size) ;

	void
		SetErrorLine(SONORK_C_CSTR module,DWORD module_line, DWORD ex_size) ;

	void
		SetBadCodecError(SONORK_C_CSTR module,DWORD module_line) ;



};

class 	TSonorkCodecWriter
:public TSonorkCodecIO
{
private:
	BYTE	*EofPtr(){return codec.ptr.wr+codec.index;}
public:
	TSonorkCodecWriter(void*pPtr,UINT pSize, SONORK_ATOM_TYPE pType, DWORD pVersion)
		:TSonorkCodecIO(pPtr,pSize,pType,pVersion){}

	void
		Skip(DWORD bytes);
        
	bool
		AddDW(DWORD**);
		
	void
		WriteRaw(const void *ptr,UINT size);

	void
		Write(const struct TSonorkCodecAtom*);

	void
		Write(const struct TSonorkShortString*);

	void
		WriteAtom(const struct TSonorkCodecAtom*A){Write(A);}

#if defined(SONORK_REVB)

	void
		WriteDW(const DWORD v);

	void
		WriteDWN(const DWORD*v, int n);

	void
		WriteW(const WORD v);

	void
		WriteWN(const WORD*v,int n);

#else

	void    WriteDW(const DWORD v)
		{ WriteRaw(&v,SIZEOF_DWORD);}

	void    WriteDWN(const DWORD*v, int n)
		{ WriteRaw(v,SIZEOF_DWORD*n);}

	void    WriteW(const WORD v)
		{ WriteRaw(&v,sizeof(WORD));}

	void 	WriteWN(const WORD*v,int n)
		{ WriteRaw(&v,sizeof(WORD)*n);}

#endif

	void 	WriteDW2(const SONORK_DWORD2*pV)
		{ WriteDWN((const DWORD*)pV,2);}

	void 	WriteDW4(const SONORK_DWORD4*pV)
		{ WriteDWN((const DWORD*)pV,4);}

	void 	WriteAU1(const TSonorkAuth1*pV)
		{ WriteDWN((const DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	WriteAU2(const TSonorkAuth2*pV)
		{ WriteDWN((const DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	WriteCM1(const TSonorkCtrlMsg*pV)
		{ WriteDWN((const DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	WriteSL1(const TSonorkServiceLocus1*pV)
		{ WriteDWN((const DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	WriteUL1(const TSonorkLocus1*pV)
		{ WriteDWN((const DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	WritePA1(const TSonorkPhysAddr*pV);

	void 	WriteUL2_obsolete(const TSonorkLocusAtom*pV);

	void	WriteUL3_obsolete(const TSonorkLocusAtom*pV);

	void 	WriteUA_obsolete( const TSonorkUserAddr*pV , BOOL V15Addr);
	
	void 	WriteSN1_obsolete(const TSonorkSidNotification*pV);

	void 	WriteMN1(const TSonorkOldMsgNotification*pV)
		{WriteDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD); }


	void 	WriteUI(const TSonorkUserInfo1*pV, SONORK_USER_INFO_LEVEL);

#if SONORK_CODEC_LEVEL>5
	void	WriteUDN(const TSonorkUserDataNotes*,UINT encode_flags);
#endif



};
class 	TSonorkCodecReader
:public TSonorkCodecIO
{
private:
	const BYTE*
		EofPtr()
		{return codec.ptr.rd+codec.index;}

public:
	TSonorkCodecReader(const void*pPtr,UINT pSize,SONORK_ATOM_TYPE pType, DWORD pVersion)
		:TSonorkCodecIO(pPtr,pSize,pType,pVersion){}

	void	Skip(DWORD bytes);

	void	ReadRaw(void *ptr,UINT size);

	void	Read(struct TSonorkCodecAtom*);

	void	Read(struct TSonorkShortString*);

#if defined(SONORK_BYTE_REVERSE)
	void	ReadDW(DWORD*v);

	void	ReadDWN(DWORD*v,int n);

	void	ReadW(WORD*v);

	void	ReadWN(WORD*v,int n);

#else
	void	ReadDW(DWORD*v)
		{ ReadRaw(v,SIZEOF_DWORD); 	}

	void	ReadDWN(DWORD*v,int n)
		{ ReadRaw(v,SIZEOF_DWORD*n); 	}

	void	ReadW(WORD*v)
		{ ReadRaw(v,sizeof(WORD)); 	}

	void	ReadWN(WORD*v,int n)
		{ ReadRaw(v,sizeof(WORD)*n); 	}
#endif

	void 	ReadDW2(SONORK_DWORD2*pV)
		{ ReadDWN((DWORD*)pV,2);}

	void 	ReadDW4(SONORK_DWORD4*pV)
		{ ReadDWN((DWORD*)pV,4);}

	void 	ReadAU1(TSonorkAuth1*pV)
		{ ReadDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	ReadAU2(TSonorkAuth2*pV)
		{ ReadDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	ReadCM1(TSonorkCtrlMsg*pV)
		{ ReadDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	ReadUL1(TSonorkLocus1*pV)
		{ ReadDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	ReadSL1(TSonorkServiceLocus1*pV)
		{ ReadDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD);}

	void 	ReadPA1(TSonorkPhysAddr*V);

	void	ReadUL2_obsolete(TSonorkLocusAtom*pV);

	void	ReadUL3_obsolete(TSonorkLocusAtom*pV);

	void 	ReadUA_obsolete(TSonorkUserAddr*V, BOOL V15Addr);

	void 	ReadSN1_obsolete(TSonorkSidNotification*pV);

	void 	ReadMN1(TSonorkOldMsgNotification*pV)
		{ ReadDWN((DWORD*)pV,sizeof(*pV)/SIZEOF_DWORD); }

	void 	ReadUI(TSonorkUserInfo1*pV, SONORK_USER_INFO_LEVEL);
	
#if SONORK_CODEC_LEVEL>5

	void	ReadUDN(TSonorkUserDataNotes*);

#endif


};


class  TSonorkPacketWriter
:public TSonorkCodecWriter
{
public:
	TSonorkPacketWriter(void*pPtr,UINT pSize, DWORD version)
		:TSonorkCodecWriter(pPtr,pSize,SONORK_ATOM_PACKET,version){}
};


class  TSonorkPacketReader
:public TSonorkCodecReader
{
public:
	TSonorkPacketReader(const void*pPtr,UINT pSize,DWORD pVersion)
		:TSonorkCodecReader(pPtr,pSize,SONORK_ATOM_PACKET,pVersion){}
};

// TSonorkCodecShortStringAtom and TSonorkCodecLCStringAtom
// are equivalent: Both write to memory in the same format
// the diference is that the first one takes a TSonorkShortString
// as the source/target while the second takes a string pointer
// and a length.

struct  TSonorkCodecShortStringAtom
:public TSonorkCodecAtom
{
private:
	TSonorkShortString		*str;

	void	CODEC_WriteDataMem(TSonorkCodecWriter& CODEC) const
	{
		CODEC.Write( str );
	}
	void	CODEC_ReadDataMem(TSonorkCodecReader& CODEC)
	{
		CODEC.Read( str );
	}

public:

	TSonorkCodecShortStringAtom(TSonorkShortString*p_str){str=p_str;}

	DWORD
		CODEC_MaxDataSize() const
		{ return 0xfffff;	}

	SONORK_ATOM_TYPE
		CODEC_DataType() const
		{ return SONORK_ATOM_SHORT_STRING;}


	DWORD	CODEC_DataSize(DWORD)	const
		{ return ::CODEC_Size( str );	}

	void
		CODEC_Clear(){	str->Clear();	}

	void
		SetStr(TSonorkShortString*S){str=S;}

};


// TSonorkCodecLCStringAtom
//  Length-Prefixed CString
//   writes/reads in the same format as TSonorkCodecShortStringAtom
// NB!!: This class has no constructor, caller MUST initialize
//	both <ptr> and <length_or_size> before using it,
//	the intialization depends on whether the atom will
//	be used to write or to read the string:
//  - Set length_or_size to the buffer's size (not the length) when reading
//  - Set length_or_size to the string's length when writing.


struct TSonorkCodecLCStringAtom
:public TSonorkCodecAtom
{
	char*				ptr;
	int 				length_or_size;

	SONORK_ATOM_TYPE
		CODEC_DataType()		const
		{	return SONORK_ATOM_SHORT_STRING;	}

	DWORD
		CODEC_DataSize(DWORD)	const
		{ return (DWORD)length_or_size + sizeof(DWORD); }

	void
		CODEC_Clear()
		{	*ptr=0;		}

	void	CODEC_WriteDataMem(TSonorkCodecWriter&) const;

	void	CODEC_ReadDataMem(TSonorkCodecReader&);

};


#define CODEC_WRITE_DW2(var,mem)	_codec_write_dword_2(mem,var)
#define CODEC_WRITE_DWN(var,mem,n)	_codec_write_dword_n(mem,var,n)

#define CODEC_READ_DW2(var,mem)		_codec_write_dword_2(var,mem)
#define CODEC_READ_DWN(var,mem,n)	_codec_write_dword_n(var,mem,n)



#if defined( SONORK_BYTE_REVERSE )
DWORD	_codec_write_dword_2(void*tgt,const void*src);
DWORD	_codec_write_dword_n(void*,const void*, const UINT dwords);
#else
inline DWORD _codec_write_dword_2(void*tgt,const void*src)
{
	memcpy(tgt,src,SIZEOF_DWORD*2);
	return SIZEOF_DWORD*2;
}
inline DWORD _codec_write_dword_n(void*tgt,const void*src, const UINT dwords)
{
	memcpy(tgt,src,SIZEOF_DWORD*dwords);
	return SIZEOF_DWORD*dwords;
}
#endif

#endif
