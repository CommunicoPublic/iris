/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _FILE_HANDLE_CACHE_HPP__
#define _FILE_HANDLE_CACHE_HPP__ 1

/**
  @file FileHandleCache.hpp
  @brief Cache for opened file handles
*/

#include "LRUCache.hpp"
#include "STLString.hpp"
#include "STLMap.hpp"

namespace IRIS
{
/**
  @class FileHandleCache FileHandleCache.hpp <FileHandleCache.hpp>
  @brief Cache for opened file handles
*/
class FileHandleCache
{
public:
	/**
	  @brief Constructor
	  @param iIMaxFiles - Max. number of files in cache
	*/
	FileHandleCache(const UINT_64 iIMaxFiles);

	/**
	  @brief Open or create file
	  @param szFile - file name
	  @param iMode - file mode
	  @return File handle, or -1 if any error occured
	*/
	INT_64 Open(CCHAR_P        szFile,
	            const UINT_64  iMode);

	/**
	  @brief Close file
	  @param iFileHandle - file handle
	*/
	void Close(const INT_64  iFileHandle);

	/**
	  @brief Force to close file and clear correspond cache record
	  @param iFileHandle - file handle
	*/
	void ForceClose(const INT_64  iFileHandle);

	/**
	  @brief Resolve file handle by file name
	  @param szFile - file name to resolve
	  @param iMode - file mode
	  @return -1, if not found, file handle - otherwise
	*/
	INT_64 Resolve(CCHAR_P        szFile,
	               const UINT_64  iMode);

	/**
	  @brief Destructor
	*/
	~FileHandleCache() throw();

private:
	// Does not exist
	FileHandleCache(const FileHandleCache  & oRhs);
	FileHandleCache & operator=(const FileHandleCache  & oRhs);

	/**
	  @struct FileNameRecord FileHandleCache.hpp <FileHandleCache.hpp>
	  @brief Record for file name
	*/
	struct FileNameRecord
	{
		/** File name        */
		STLW::string   filename;
		/** File open mode   */
		UINT_64        mode;
	};

	/**
	  @class FileNameRecordEvictor FileHandleCache.hpp <FileHandleCache.hpp>
	  @brief Cache eviction callback
	*/
	class FileNameRecordEvictor
	{
	public:
		/**
		  @brief Constructor
		  @param mICloseMap - Opened filehandle => FileNameRecord map
		*/
		FileNameRecordEvictor(STLW::map<INT_64, FileNameRecord>  & mICloseMap);

		/**
		  @brief Evict record from cache
		  @param oKey - cache key
		  @param oVal - cache value
		*/
		void Evict(const FileNameRecord  & oKey, const INT_64  & oVal);
	private:
		/** mICloseMap - Opened filehandle => FileNameRecord map */
		STLW::map<INT_64, FileNameRecord>  & mCloseMap;
	};

	/**
	  @class FileNameRecordComparator FileHandleCache.hpp <FileHandleCache.hpp>
	  @brief Comparator for index records storage
	*/
	class FileNameRecordComparator
	{
	public:
		bool operator()(const FileNameRecord & oX, const FileNameRecord & oY) const;
	};

	typedef LRUCache<FileNameRecord,
	                 INT_64,
	                 FileNameRecordEvictor,
	                 NullCacheRestoreCallback<FileNameRecord, INT_64>,
	                 FileNameRecordComparator>  TCache;

	/** Max. number of files in cache */
	const UINT_64                      iMaxFiles;
	/** Map */
	STLW::map<INT_64, FileNameRecord>  mCloseMap;
	/** File handle evictor           */
	FileNameRecordEvictor              oEvictor;
	/** LRU Cache                     */
	TCache                             oCache;
};

/**
  @class AutoCloseFile FileHandleCache.hpp <FileHandleCache.hpp>
  @brief Automatically close file
*/
class AutoCloseFile
{
public:
	/**
	  @brief Constructor
	  @param pIFileHandleCache - File cache object
	  @param iIFileHandle - opened file handle
	*/
	AutoCloseFile(FileHandleCache  * pIFileHandleCache,
	              INT_64             iIFileHandle);

	/**
	  @brief Get file handle
	*/
	INT_64 operator()();

	/**
	  @brief Destructor
	*/
	~AutoCloseFile() throw();
private:
	AutoCloseFile(const AutoCloseFile & oRhs);
	AutoCloseFile & operator=(const AutoCloseFile & oRhs);

	/** File handle cache */
	FileHandleCache  * pFileHandleCache;
	/** File handle       */
	INT_64             iFileHandle;
};

} // namespace IRIS
#endif // _FILE_HANDLE_CACHE_HPP__
// End.

