/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "FileHandleCache.hpp"
#include "FileUtils.hpp"
#include "UnicodeUtils.hpp"

namespace IRIS
{

//
// Constructor
//
FileHandleCache::FileNameRecordEvictor::FileNameRecordEvictor(STLW::map<INT_64, FileHandleCache::FileNameRecord>  & mICloseMap): mCloseMap(mICloseMap)
{
	;;
}

//
// Eviction callback
//
void FileHandleCache::FileNameRecordEvictor::Evict(const FileHandleCache::FileNameRecord  & oKey, const INT_64  & oVal)
{
	File::Close(oVal);
	mCloseMap.erase(oVal);
}

//
// Compare two records
//
bool FileHandleCache::FileNameRecordComparator::operator() (const FileHandleCache::FileNameRecord & oX, const FileHandleCache::FileNameRecord & oY) const
{
	const INT_32 iRC = Unicode::Compare(oX.filename.data(), oX.filename.size(),
	                                    oY.filename.data(), oY.filename.size());
	if (iRC == 0) { return oX.mode < oY.mode; }

return iRC < 0;
}

//
// Constructor
//
FileHandleCache::FileHandleCache(const UINT_64 iIMaxFiles): iMaxFiles(iIMaxFiles), oEvictor(mCloseMap), oCache(iMaxFiles, &oEvictor)
{
	;;
}

//
// Open or create file
//
INT_64 FileHandleCache::Open(CCHAR_P        szFile,
                             const UINT_64  iMode)
{
	FileNameRecord oFileNameRecord;
	oFileNameRecord.filename = szFile;
	oFileNameRecord.mode     = iMode;

	// Check cache first
	INT_64 iFilehandle = -1;
	TCache::State oRC = oCache.Get(oFileNameRecord, iFilehandle);
	if (oRC == TCache::OK) { return iFilehandle; }

	// Try to open file
	iFilehandle = File::Open(szFile, iMode);
	if (iFilehandle == -1) { return iFilehandle; }

	// Store file handle in cache
	oCache.Insert(oFileNameRecord, iFilehandle);
	mCloseMap[iFilehandle] = oFileNameRecord;

	oRC = oCache.Get(oFileNameRecord, iFilehandle);
	if (oRC == TCache::OK) { return iFilehandle; }

return -1;
}

//
// Close file
//
void FileHandleCache::Close(const INT_64  iFileHandle)
{
	STLW::map<INT_64, FileHandleCache::FileNameRecord>::iterator itmCloseMap = mCloseMap.find(iFileHandle);
	if (itmCloseMap != mCloseMap.end()) { oCache.Release(itmCloseMap -> second); }
}

//
// Force to close file and clear correspond cache record
//
void FileHandleCache::ForceClose(const INT_64  iFileHandle)
{
	STLW::map<INT_64, FileHandleCache::FileNameRecord>::iterator itmCloseMap = mCloseMap.find(iFileHandle);
	if (itmCloseMap != mCloseMap.end()) { oCache.Remove(itmCloseMap -> second); }
}

//
// Resolve file handle by file name
//
INT_64  FileHandleCache::Resolve(CCHAR_P        szFile,
                                 const UINT_64  iMode)

{
	FileNameRecord oFileNameRecord;
	oFileNameRecord.filename = szFile;
	oFileNameRecord.mode     = iMode;

	// Check cache
	INT_64 iFilehandle = -1;
	TCache::State oRC = oCache.Get(oFileNameRecord, iFilehandle);
	if (oRC == TCache::OK) { return iFilehandle; }

return -1;
}

//
// Destructor
//
FileHandleCache::~FileHandleCache() throw()
{
	STLW::map<INT_64, FileHandleCache::FileNameRecord>::iterator itmCloseMap = mCloseMap.begin();
	while (itmCloseMap != mCloseMap.end())
	{
		File::Close(itmCloseMap -> first);
		++itmCloseMap;
	}
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class AutoCloseFile
//

//
// Constructor
//
AutoCloseFile::AutoCloseFile(FileHandleCache  * pIFileHandleCache,
                             INT_64             iIFileHandle): pFileHandleCache(pIFileHandleCache),
                                                               iFileHandle(iIFileHandle)
{
	;;
}

//
// Get file handle
//
INT_64 AutoCloseFile::operator()() { return iFileHandle; }

//
// Destructor
//
AutoCloseFile::~AutoCloseFile() throw()
{
	pFileHandleCache -> Close(iFileHandle);
}

} // namespace IRIS
// End.
