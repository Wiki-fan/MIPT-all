#include "stdafx.h"
#include "SortedListFromFile.h"

namespace my
{

CSortedListFromFile::~CSortedListFromFile()
{
	delete[] buf;
}

type CSortedListFromFile::getNext( my::CFile& f, mysize intsInPiece, mysize intsInChunk )
{
	type ret = buf[index++];
	ReadIfNeeded( f, intsInPiece, intsInChunk );

	return ret;
}

void CSortedListFromFile::ReadIfNeeded( my::CFile& f, mysize intsInPiece, mysize intsInChunk )
{
	// Не конец чанка.
	if( offset - 1 < chunkend ) {
		// Надо считать новый piece.
		if( index == size ) {
			mysize blocksize = chunkend - offset < intsInPiece ? chunkend - offset : intsInPiece;
			size = f.ReadToArr( buf, blocksize, offset );
			offset += size;
			index = 0;
		}
	} else {
		size = 0;
	}

}

} // namespace my