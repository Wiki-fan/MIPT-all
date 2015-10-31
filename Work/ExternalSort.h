#pragma once

namespace my
{

// Внешняя сотрировка, работающая с long long.
#ifndef __GLIBC__
void ExternalSort( const wchar_t* sourceFileName, const wchar_t* targetFilename );
#else
void ExternalSort( const char* sourceFileName, const char* targetFilename );
#endif
void GenFile();
bool CheckFile();

} // namespace my
