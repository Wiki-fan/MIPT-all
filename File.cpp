#include "stdafx.h"
#include "File.h"

namespace my
{
#ifndef __GNUC__
CFile::CFile( const wchar_t* filename, bool write )
{
	_wfopen_s( &f, filename, write ? L"wb+" : L"rb+" );
}
#else
CFile::CFile( const char *filename, bool write )
{
	f = fopen( filename, write ? "wb+" : "rb+" );
}
#endif

CFile::~CFile()
{
	fclose( f );
}

mysize CFile::ReadToArr( type *buffer, mysize count, mysize offset )
{
#ifndef __GNUC__
	_fseeki64( f, offset * sizeof( type ), SEEK_SET );
#else
	fseeko64( f, offset * sizeof( type ), SEEK_SET );
#endif
#ifdef STDOUT_CFILE_DEBUG
	perror( "ReadToArr fseek: " );
#endif
	mysize size = fread( buffer, sizeof( type ), count, f );
#ifdef STDOUT_CFILE_DEBUG
	perror( "ReadToArr fread: " );
#endif
	return size;
}

mysize CFile::WriteFromArr( type *buffer, mysize count, mysize offset )
{
#ifndef __GNUC__
	_fseeki64( f, offset * sizeof( type ), SEEK_SET );
#else
	fseeko64( f, offset * sizeof( type ), SEEK_SET );
#endif
#ifdef STDOUT_CFILE_DEBUG
	perror( "WriteToArr fseek: " );
#endif
	mysize size = fwrite( buffer, sizeof( type ), count, f );
#ifdef STDOUT_CFILE_DEBUG
	perror( "WriteFromArr fwrite: " );
#endif
	return size;
}

}
// namespace my
