/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "LoggerFile.hpp"
#include "LogPriorities.hpp"

#include <stdio.h>

namespace IRIS
{

//
// A constructor
//
LoggerFile::LoggerFile(FILE * oIF): F(oIF) { ;; }

//
// Write message to log file
//
Logger::State LoggerFile::WriteLog(const LogPriority::LogPriorities  ePriority,
                                   CCHAR_P                           szString,
                                   const UINT_32                     iStringLen)
{
	fprintf(F, "[%s] %.*s\n", LogPriority::GetPrintableState(ePriority), iStringLen, szString);

return Logger::OK;
}

//
// A destructor
//
LoggerFile::~LoggerFile() throw() { ;; }

} // namespace IRIS
// End.
