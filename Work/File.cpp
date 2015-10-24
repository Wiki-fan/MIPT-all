#include "stdafx.h"
#include "File.h"

namespace my
{

CFile::CFile( const wchar_t* filename, bool write )
{
	_wfopen_s( &f, filename, write ? L"wb+" : L"rb+" );
}

CFile::~CFile()
{
	fclose( f );
}

mysize CFile::ReadToArr( type* buffer, mysize count, mysize offset )
{
	_fseeki64( f, offset*sizeof( type ), SEEK_SET );
#ifdef STDOUT_CFILE_DEBUG
	perror( "ReadToArr fseek: " );
#endif
	mysize size = fread( buffer, sizeof( type ), count, f );
#ifdef STDOUT_CFILE_DEBUG
	perror( "ReadToArr fread: " );
#endif
	return size;
}

mysize CFile::WriteFromArr( type* buffer, mysize count, mysize offset )
{
	_fseeki64( f, offset*sizeof( type ), SEEK_SET );
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
