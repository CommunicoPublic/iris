/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "FileUtils.hpp"

#include <sys/mman.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef sun
#include <limits.h>
#endif

namespace IRIS
{

const UINT_64   File::MAX_PATH_LEN = PATH_MAX;

//
// Destructor
//
DirectoryCallback::~DirectoryCallback() throw() { ;; }

//
// Destructor
//
GlobCallback::~GlobCallback() throw() { ;; }

//
// Write data to the file
//
INT_64 File::GetStat(const INT_32    iFile,
                     File::Stat    & oStat)
{
	struct stat oTMP;
	const INT_32 iRC = fstat(iFile, &oTMP);

	oStat.size  = oTMP.st_size;
	oStat.ctime = oTMP.st_ctime;
	oStat.mtime = oTMP.st_mtime;
	oStat.uid   = oTMP.st_uid;
	oStat.gid   = oTMP.st_gid;
	oStat.mode  = oTMP.st_mode;

return iRC;
}

//
// Open file
//
INT_64 File::Open(CCHAR_P        szPath,
                  const UINT_64  iOpenMode)
{
	INT_32 iMode = 0;
	INT_32 iPermissions = 0;
	if ((iOpenMode & OpenMode::READWRITE) == OpenMode::READWRITE)
	{
		iMode = O_RDWR;
	}
	else
	{
		if (iOpenMode & OpenMode::READ)  { iMode |= O_RDONLY; }
		if (iOpenMode & OpenMode::WRITE) { iMode |= O_WRONLY; }
	}

	if (iOpenMode & OpenMode::CREATE)
	{
		iMode |= O_CREAT;
		iPermissions = 0660;
	}

	if (iOpenMode & OpenMode::APPEND) { iMode |= O_APPEND; }

	if (iOpenMode & OpenMode::TRUNCATE) { iMode |= O_TRUNC; }

return open(szPath, iMode, iPermissions);
}

//
// Rename file
//
INT_64 File::Rename(CCHAR_P  szSrc,
                    CCHAR_P  szDst)
{
	;;
return rename(szSrc, szDst);
}

//
// Link files
//
INT_64 File::Link(CCHAR_P  szSrc,
                  CCHAR_P  szDst)
{
	;;
return link(szSrc, szDst);
}

//
// Unlink file
//
INT_64 File::Unlink(CCHAR_P  szPath)
{
	;;
return unlink(szPath);
}

//
// Closefile
//
INT_64 File::Close(const INT_64 iFileHandle)
{
	;;
return close(iFileHandle);
}

//
// Reposition read/write file offset
//
INT_64 File::Seek(const INT_32   iFile,
                  const UINT_64  iOffset,
                  const bool     bSeekAtStart)
{
	INT_32 iWhence = SEEK_END;
	if (bSeekAtStart) { iWhence = SEEK_SET; }

return lseek(iFile, iOffset, iWhence);
}

//
// Change mode of file
//
INT_64 File::Chmod(const INT_32   iFile,
                   const UINT_64  iFileMode)
{
	;;
return fchmod(iFile, iFileMode);
}

//
// Change file owner
//
INT_64 File::Chown(const INT_32   iFile,
                   const UINT_64  iUID,
                   const UINT_64  iGID)
{
	;;
return fchown(iFile, iUID, iGID);
}

//
// Truncate file to specified size
//
INT_64 File::Truncate(const INT_32   iFile,
                      const UINT_64  iFileSize)
{
	;;
return ftruncate(iFile, iFileSize);
}

//
// Write data to the file
//
INT_64 File::Write(const INT_32     iFile,
                   const void     * vBuffer,
                   const UINT_64    iBufferSize)
{
	;;
return write(iFile, vBuffer, iBufferSize);
}

//
// Read data from file
//
INT_64 File::Read(const INT_32     iFile,
                  void           * vBuffer,
                  const UINT_64    iBufferSize)
{
	;;
return read(iFile, vBuffer, iBufferSize);
}

//
// Write data to the file
//
INT_64 File::Write(const INT_32     iFile,
                    const void     * vBuffer,
                    const UINT_64    iBufferSize,
                    const UINT_64    iOffset)
{
	;;
return pwrite(iFile, vBuffer, iBufferSize, iOffset);
}

//
// Read data from file
//
INT_64 File::Read(const INT_32     iFile,
                   void           * vBuffer,
                   const UINT_64    iBufferSize,
                   const UINT_64    iOffset)
{
	;;
return pread(iFile, vBuffer, iBufferSize, iOffset);
}

//
// Map file region to memory
//
void * File::MapRegion(const INT_32    iFile,
                       const UINT_64   iStartOffset,
                       const UINT_64   iEndOffset,
                       const FileMapType::MapType  oMapType)
{
	INT_32 iProt = 0;
	switch (oMapType)
	{
		case FileMapType::READ:
			iProt = PROT_READ;
			break;
		case FileMapType::WRITE:
			iProt = PROT_WRITE;
			break;
		case FileMapType::READWRITE:
			iProt = PROT_READ | PROT_WRITE;
			break;
	}

	void * vData = mmap(NULL, iEndOffset - iStartOffset, iProt, MAP_SHARED, iFile, iStartOffset);
	if (vData == MAP_FAILED) { return NULL; }

return vData;
}

//
// Sync memory region with file
//
INT_64 File::SyncRegion(void           * vAddress,
                        const UINT_64    iStartOffset,
                        const UINT_64    iEndOffset)
{
return msync(vAddress, iEndOffset - iStartOffset, MS_SYNC);
}

//
// Unmap file region from memory
//
void File::UnmapRegion(void           * vAddress,
                       const UINT_64    iStartOffset,
                       const UINT_64    iEndOffset)
{
	munmap(vAddress, iEndOffset - iStartOffset);
}

//
// Check file or path exxistence
//
FileCheckPathResult::CheckPathResult File::CheckPath(CCHAR_P szPath)
{
	struct stat oTMP;
	const INT_32 iRC = stat(szPath, &oTMP);
	if (iRC == 0) { return FileCheckPathResult::OK; }
	switch(errno)
	{
		case ENOENT:
			return FileCheckPathResult::FILE_NOT_FOUND;
		case ENOTDIR:
			return FileCheckPathResult::PATH_NOT_FOUND;
		default:
			return FileCheckPathResult::ERROR;
	}
return FileCheckPathResult::ERROR;
}

//
// Create directory
//
INT_32 File::CreateDirectory(CCHAR_P szPath)
{
	CHAR_8 sPath[MAXPATHLEN + 1];

	CCHAR_P itszPath = szPath;
	UINT_32 iPos = 0;
	while(iPos < MAXPATHLEN)
	{
		if ((*itszPath == '/' || *itszPath == '\0') && iPos > 0)
		{
			sPath[iPos] = '\0';
			const INT_32 iRC = mkdir(sPath, 0755);
			if (iRC == -1 && errno != EEXIST) { return -1; }
			if (*itszPath == '\0') { break; }
		}
		sPath[iPos] = *itszPath;
		++iPos;
		++itszPath;
	}
return 0;
}

//
// Remove directory
//
INT_64 File::RemoveDirectory(CCHAR_P  szPath)
{
	;;
return rmdir(szPath);
}

//
// Remove all subdirectories
//
INT_64 File::RemoveSubdirectories(CCHAR_P  szRoot,
                                  CCHAR_P  szSubdirs)
{
	UINT_32 iRootLen    = strlen(szRoot);
	UINT_32 iSubdirsLen = strlen(szSubdirs);

	// Check root path
	while (iRootLen > 0 && szRoot[iRootLen - 1] == '/') { --iRootLen; }

	// Check subdirectories path
	while (iSubdirsLen > 0 && szSubdirs[iSubdirsLen - 1] == '/') { --iSubdirsLen; }
	while (iSubdirsLen > 0 && *szSubdirs == '/')
	{
		++szSubdirs;
		--iSubdirsLen;
	}
	if (iSubdirsLen == 0) { return ENOENT; }

	// Wrong path
	if (iRootLen + iSubdirsLen + 1> MAXPATHLEN) { return ENAMETOOLONG; }

	CHAR_8 sPath[MAXPATHLEN + 1];
	memcpy(sPath, szRoot, iRootLen);
	sPath[iRootLen] = '/';
	memcpy(sPath + iRootLen + 1, szSubdirs, iSubdirsLen);
	sPath[iRootLen + iSubdirsLen + 1] = '\0';

	CCHAR_P itsPathEnd = sPath + iRootLen;
	CHAR_P  itsPath    = sPath + iRootLen + iSubdirsLen;

	for (;;)
	{
		if(rmdir(sPath) == -1) { return errno; }

		while (itsPath != itsPathEnd && *itsPath != '/') { --itsPath; }

		if (itsPath == itsPathEnd) { break; }

		*itsPath = '\0';
	}

return 0;
}

//
// Traverse directory
//
INT_32 File::TraverseDirectory(CCHAR_P              szPath,
                               DirectoryCallback  * pCallback,
                               const bool           bTraverseSubdirs)
{
	enum EntryType { E_UNK, E_REG, E_DIR };

	DIR * pDir = opendir(szPath);
	if (pDir == NULL) { return -1; }

	struct dirent  oDirEntry;
	struct dirent  * pDirEntryResult;
	while(readdir_r(pDir, &oDirEntry, &pDirEntryResult) == 0)
	{
		if (pDirEntryResult == NULL) { break; }

		if (strcmp(oDirEntry.d_name, ".") == 0 ||
		    strcmp(oDirEntry.d_name, "..") == 0) { continue; }

		DirectoryCallback::EntryType oEntryType = DirectoryCallback::OTHER;

		CHAR_8 sFullPath[MAXPATHLEN + 1];
		snprintf(sFullPath, MAXPATHLEN, "%s/%s", szPath, oDirEntry.d_name);
		EntryType eType = E_UNK;
#if defined(sun) || defined(_AIX)
		struct stat oStat;
		stat(oDirEntry.d_name, &oStat);
		if      (S_ISREG(oStat.st_mode)) { eType = E_REG; }
		else if (S_ISDIR(oStat.st_mode)) { eType = E_DIR; }
#else
		if      (oDirEntry.d_type == DT_REG) { eType = E_REG; }
		else if (oDirEntry.d_type == DT_DIR) { eType = E_DIR; }
#endif

		if (eType == E_REG)
		{
			oEntryType = DirectoryCallback::FILE;
		}
		else if (eType == E_DIR)
		{
			if (bTraverseSubdirs) { TraverseDirectory(sFullPath, pCallback, bTraverseSubdirs); }
			oEntryType = DirectoryCallback::DIR;
		}

		pCallback -> Entry(sFullPath, oEntryType);
	}
	closedir(pDir);
return 0;
}

//
// Find pathnames matching a pattern
///
INT_32 File::Glob(CCHAR_P         szPattern,
                  GlobCallback  * pGlobCallback)
{
	glob_t oGlob;

	const INT_32 iRC = glob(szPattern, GLOB_NOSORT, NULL, &oGlob);
	if (iRC != 0) { return -1; }

#ifdef GLOB_NOMATCH
	const UINT_32 iElts = oGlob.gl_pathc;
#else
	const UINT_32 iElts = oGlob.gl_matchc;
#endif
	for (UINT_32 iPos = 0; iPos < iElts; ++iPos)
	{
		pGlobCallback -> Entry(oGlob.gl_pathv[iPos]);
	}

	globfree(&oGlob);

return 0;
}

//
// Change directory
//
INT_64 File::ChDir(CCHAR_P  szPath)
{
	;;
return chdir(szPath);
}

//
// Get real path
//
CHAR_P File::GetRealPath(CCHAR_P szPath, CHAR_P szRealPath)
{
#if (__NetBSD__)
	if (szRealPath == NULL)
	{
		char sTMP[MAXPATHLEN];
		CHAR_P szResult = realpath(szPath, sTMP);
		if (szResult == NULL) { return NULL; }
		return strdup(szResult);
	}
#endif
return realpath(szPath, szRealPath);
}

//
// Get path name
//
CHAR_P File::Dirname(CCHAR_P szPath)
{
	if (szPath == NULL || *szPath == 0) { return NULL; }

	struct stat oStat;
	if (stat(szPath, &oStat) == -1) { return NULL; }

	// Directory?
	if (S_ISDIR(oStat.st_mode)) { return strdup(szPath); }

	// File?
	if (S_ISREG(oStat.st_mode))
	{
		CHAR_P szTMP = strdup(szPath);
		CHAR_P szEnd = szTMP + strlen(szTMP);
		while (szEnd != szTMP && *szEnd != '/') { *szEnd-- = '\0'; }
		if (szEnd == szTMP) { *szTMP = '.'; }

		return szTMP;
	}

return NULL;
}

} // namespace IRIS
// End.
