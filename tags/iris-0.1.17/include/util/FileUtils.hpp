/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _FILE_UTILS_HPP__
#define _FILE_UTILS_HPP__ 1

/**
  @file FileUtils.hpp
  @brief Operations with files
*/
#include "Types.h"

namespace IRIS
{

/**
  @namespace IRIS::OpenMode
  @brief File open mode
*/
namespace OpenMode
{
	static const UINT_64   READ      = 0x00000001;
	static const UINT_64   WRITE     = 0x00000002;
	static const UINT_64   READWRITE = (READ | WRITE);
	static const UINT_64   CREATE    = 0x00000004;
	static const UINT_64   TRUNCATE  = 0x00000008;
	static const UINT_64   APPEND    = 0x00000010;
}

/**
  @namespace IRIS::FileMapType
  @brief File mapping type
*/
namespace FileMapType
{
	enum MapType { READ, WRITE, READWRITE };
}

/**
  @namespace IRIS::FileCheckPathResult
  @brief Result of file path check
*/
namespace FileCheckPathResult
{
	enum CheckPathResult { OK, ERROR, FILE_NOT_FOUND, PATH_NOT_FOUND };
}

/**
  @class DirectoryCallback FileUtils.hpp <FileUtils.hpp>
  @brief Directory callback
*/
class DirectoryCallback
{
public:
	enum EntryType { FILE, DIR, OTHER };
	virtual void Entry(CCHAR_P szName, const EntryType  oEntryType) = 0;

	/**
	  @brief Destructor
	*/
	virtual ~DirectoryCallback() throw();
};

/**
  @class DirectoryCallback FileUtils.hpp <FileUtils.hpp>
  @brief Directory callback
*/
class GlobCallback
{
public:
	virtual void Entry(CCHAR_P szName) = 0;

	/**
	  @brief Destructor
	*/
	virtual ~GlobCallback() throw();
};

/**
  @class File FileUtils.hpp <FileUtils.hpp>
  @brief Operations with files
*/
class File
{
public:
	static const UINT_64   MAX_PATH_LEN;

	struct Stat
	{
		/** File size               */
		UINT_64   size;
		/** File creation time      */
		UINT_64   ctime;
		/** File modification time  */
		UINT_64   mtime;
		/** Owner Id                */
		UINT_64   uid;
		/** Group Id                */
		UINT_64   gid;
		/** List of permissions     */
		UINT_64   mode;
	};

	/**
	  @brief Get file status
	*/
	static INT_64 GetStat(const INT_32    iFile,
	                      File::Stat    & oStat);

	/**
	  @brief Open file
	*/
	static INT_64 Open(CCHAR_P        szPath,
	                   const UINT_64  iOpenMode);

	/**
	  @brief Rename file
	*/
	static INT_64 Rename(CCHAR_P  szSrc,
	                     CCHAR_P  szDst);
	/**
	  @brief Link files
	*/
	static INT_64 Link(CCHAR_P  szSrc,
	                   CCHAR_P  szDst);

	/**
	  @brief Unlink file
	*/
	static INT_64 Unlink(CCHAR_P  szPath);

	/**
	  @brief Close file
	*/
	static INT_64 Close(const INT_64  iFileHandle);

	/**
	  @brief Reposition read/write file offset
	*/
	static INT_64 Seek(const INT_32   iFile,
	                   const UINT_64  iOffset,
	                   const bool     bSeekAtStart = true);

	/**
	  @brief Change mode of file
	*/
	static INT_64 Chmod(const INT_32   iFile,
	                    const UINT_64  iFileMode);

	/**
	  @brief Change file owner
	*/
	static INT_64 Chown(const INT_32   iFile,
	                    const UINT_64  iUID,
	                    const UINT_64  iGID);

	/**
	  @brief Truncate file to specified size
	*/
	static INT_64 Truncate(const INT_32   iFile,
	                       const UINT_64  iFileSize);

	/**
	  @brief Write data to the file
	*/
	static INT_64 Write(const INT_32     iFile,
	                    const void     * vBuffer,
	                    const UINT_64    iBufferSize);

	/**
	  @brief Read data from file from specified offset
	*/
	static INT_64 Read(const INT_32     iFile,
	                   void           * vBuffer,
	                   const UINT_64    iBufferSize);

	/**
	  @brief Write data to the file to specified offset
	*/
	static INT_64 Write(const INT_32     iFile,
	                    const void     * vBuffer,
	                    const UINT_64    iBufferSize,
	                    const UINT_64    iOffset);

	/**
	  @brief Read data from file
	*/
	static INT_64 Read(const INT_32     iFile,
	                   void           * vBuffer,
	                   const UINT_64    iBufferSize,
	                   const UINT_64    iOffset);

	/**
	  @brief Map file region to memory
	*/
	static void * MapRegion(const INT_32                 iFile,
	                        const UINT_64                iStartOffset,
	                        const UINT_64                iEndOffset,
	                        const FileMapType::MapType   oMapType = FileMapType::READ);

	/**
	  @brief Sync memory region with file
	*/
	static INT_64 SyncRegion(void           * vAddress,
	                         const UINT_64    iStartOffset,
	                         const UINT_64    iEndOffset);

	/**
	  @brief Unmap file region from memory
	*/
	static void UnmapRegion(void           * vAddress,
	                        const UINT_64    iStartOffset,
	                        const UINT_64    iEndOffset);

	/**
	  @brief Check file or path exxistence
	*/
	static FileCheckPathResult::CheckPathResult CheckPath(CCHAR_P szPath);

	/**
	  @brief Create directory
	*/
	static INT_32 CreateDirectory(CCHAR_P szPath);

	/**
	  @brief Remove directory
	*/
	static INT_64 RemoveDirectory(CCHAR_P  szPath);

	/**
	  @brief Remove all subdirectories
	*/
	static INT_64 RemoveSubdirectories(CCHAR_P  szRoot,
	                                   CCHAR_P  szSubdirs);

	/**
	  @brief Traverse directory
	*/
	static INT_32 TraverseDirectory(CCHAR_P              szPath,
	                                DirectoryCallback  * pCallback,
	                                const bool           bTraverseSubdirs = true);

	/**
	  @brief Find pathnames matching a pattern
	*/
	static INT_32 Glob(CCHAR_P         szPattern,
	                   GlobCallback  * pGlobCallback);

	/**
	  @brief Change directory
	*/
	static INT_64 ChDir(CCHAR_P  szPath);

	/**
	  @brief Get real path
	*/
	static CHAR_P GetRealPath(CCHAR_P szPath, CHAR_P szRealPath);

	/**
	  @brief Get path name
	*/
	static CHAR_P Dirname(CCHAR_P szPath);
private:
};

} // namespace IRIS
#endif // _FILE_UTILS_HPP__
// End.
