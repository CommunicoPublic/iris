/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

/**
  @file IrisFileLogger.hpp
  @brief Parametrized file logger module
*/

// C++ Includes
#include "ConfigHelper.hpp"
#include "DataBuffer.hpp"
#include "Debug.hpp"
#include "FileUtils.hpp"
#include "GlobalContext.hpp"
#include "LoadableObject.hpp"
#include "Logger.hpp"
#include "LoggerContext.hpp"
#include "LoggerObject.hpp"
#include "Mutex.hpp"
#include "UnicodeUtils.hpp"
#include "SignalHandler.hpp"
#include "SystemVars.hpp"

#include <stdlib.h>

#ifndef C_LOGGER_START_MESSAGE_LEN
#define C_LOGGER_START_MESSAGE_LEN 512
#endif // C_LOGGER_START_MESSAGE_LEN

/**
  @namespace IRIS_MOD_FL
  @brief Iris file logger namespace
*/
namespace IRIS_MOD_FL
{

// FWD
class FileLogger;

/**
  @class LoggerSignalHandler LoggerSignalHandler.hpp <LoggerSignalHandler.hpp>
  @brief Signal handler implementation, need for log rotation
*/
class LoggerSignalHandler:
  public IRIS::UserSignalHandler
{
public:
	/**
	  @brief Constructor
	  @param oILogger - file logger
	*/
	LoggerSignalHandler(FileLogger  & oILogger);

	/**
	  @brief A signal handler
	*/
	void Handler(INT_32 iSignal);

	/**
	  @brief A destructor
	*/
	~LoggerSignalHandler() throw();
private:
	// Does not exist
	LoggerSignalHandler(const LoggerSignalHandler  & oRhs);
	LoggerSignalHandler& operator=(const LoggerSignalHandler  & oRhs);

	/** Logger object */
	FileLogger     & oLogger;
};

/**
  @class LogWriter IrisFileLogger.cpp <IrisFileLogger.cpp>
  @brief Log writer
*/
class LogWriter
{
public:
	/**
	  @brief type cast operator
	  @return reference to DataBuffer object
	*/
	operator IRIS::DataBuffer &();

	/**
	  @brief Flush data
	*/
	void Flush(const bool bForce = false);
private:
	friend class FileLogger;
	// Does not exist
	LogWriter(const LogWriter  & oRhs);
	LogWriter& operator=(const LogWriter  & oRhs);

	/** File handle         */
	INT_64            & iFileHandle;
	/** Max buffer size     */
	const UINT_64       iMaxBufferSize;
	/** Data buffer         */
	IRIS::DataBuffer    oDataBuffer;

	/**
	  @brief Constrcutor
	  @param iIFileHandle - log file handle
	  @param iIMaxBufferSize - max. log buffer size
	*/
	LogWriter(INT_64         & iIFileHandle,
	          const UINT_64    iIMaxBufferSize);

	/**
	  @brief A destrcutor
	*/
	~LogWriter() throw();
};

/**
  @class FileLogger IrisFileLogger.cpp <IrisFileLogger.cpp>
  @brief Loadable object; logger subsystem
*/
class FileLogger:
 public IRIS::LoggerObject
{
public:
	/**
	  @brief A constructor
	*/
	FileLogger();

	/**
	  @brief Close and open log files
	*/
	void ReopenLog();

	/**
	  @brief Switch priority from/to debug
	*/
	void SwitchPriority();

	/**
	  @brief A destructor
	*/
	~FileLogger() throw();
private:
	// Does not exist
	FileLogger(const FileLogger  & oRhs);
	FileLogger& operator=(const FileLogger  & oRhs);

	/** Base priority            */
	IRIS::LogPriority::LogPriorities  oBasePriority;
	/** Actual priority            */
	IRIS::LogPriority::LogPriorities  oActualPriority;
	/** Error log record format    */
	STLW::string           sErrorLogFormat;
	/** Error log file             */
	STLW::string           sErrorLogFile;
	/** Error log file permissions */
	INT_32                 iErrorLogPerms;
	/** Error log owner            */
	STLW::string           sErrorLogUser;
	/** Error log owner uid        */
	INT_64                 iErrorLogUID;
	/** Error log owner group      */
	STLW::string           sErrorLogGroup;
	/** Error log owner group gid  */
	INT_64                 iErrorLogGID;
	/** Error log buffer size    */
	INT_64                 iErrorLogBufferSize;

	/** Custom log record format   */
	STLW::string           sCustomLogFormat;
	/** Custom log file            */
	STLW::string           sCustomLogFile;
	/** Custom log file permissions */
	INT_32                 iCustomLogPerms;
	/** Custom log owner           */
	STLW::string           sCustomLogUser;
	/** Custom log owner uid       */
	INT_64                 iCustomLogUID;
	/** Custom log owner group     */
	STLW::string           sCustomLogGroup;
	/** Custom log owner group gid */
	INT_64                 iCustomLogGID;
	/** Custom log buffer size    */
	INT_64                 iCustomLogBufferSize;

	/** Error log file handle     */
	INT_64                 iErrorLogFileHandle;
	/** Custom log file handle    */
	INT_64                 iCustomLogFileHandle;
	/** Error log writer         */
	LogWriter            * pErrorLogWriter;
	/** Error log writer         */
	LogWriter            * pCustomLogWriter;

	/** Error log mutex           */
	IRIS::Mutex            oErrorLogMutex;
	/** Transfer log mutex        */
	IRIS::Mutex            oTransferLogMutex;
	/** Logger signel handler     */
	LoggerSignalHandler    oLoggerSignalHandler;

	/**
	  @brief Initialize module
	  @param oGlobalContext - global server context
	  @param oConfig - configuration data
	  @param oSigHandler - signal handler
	  @param oLogger - temporary logger
	*/
	INT_32 InitModule(IRIS::GlobalContext    & oGlobalContext,
	                  const IRIS::VariantNC  & oConfig,
	                  IRIS::SignalHandler    & oSigHandler,
	                  IRIS::Logger           & oLogger);

	/**
	  @brief Get error log
	*/
	IRIS::Logger * GetErrorLog(const IRIS::LoggerContext  * pLoggerContext);

	/**
	  Get custom log
	*/
	IRIS::Logger * GetCustomLog(const IRIS::LoggerContext  * pLoggerContext);

	/**
	  @brief Destroy module
	*/
	INT_32 DestroyModule(IRIS::GlobalContext  & oGlobalContext,
	                     IRIS::SignalHandler  & oSigHandler,
	                     IRIS::Logger         & oLogger);

	/**
	  @brief Get object type
	  @return Human-readable type of object
	*/
	CCHAR_P GetObjectType() const;

	/**
	  @brief Get object name
	  @return Human-readable name of object
	*/
	CCHAR_P GetObjectName() const;
};

EXPORT_HANDLER(FileLogger)

/**
  @class ParametrizedLogger IrisFileLogger.cpp <IrisFileLogger.cpp>
  @brief Parametrized logger
*/
class ParametrizedLogger:
  public IRIS::Logger
{
public:
	/**
	  @brief A constructor
	  @param iFileHandle - log file descriptor
	  @param oMutex - Mutex for writing
	*/
	ParametrizedLogger(IRIS::LogPriority::LogPriorities  & oILoggerBasePriority,
	                   LogWriter                         & oILogWriter,
	                   const STLW::string                & sILogFormat,
	                   const IRIS::LoggerContext         * pILoggerContext,
	                   IRIS::Mutex                       & oMutex);

	/**
	  @brief A destructor
	*/
	~ParametrizedLogger() throw();

private:
	// Does not exist
	ParametrizedLogger(const ParametrizedLogger  & oRhs);
	ParametrizedLogger& operator=(const ParametrizedLogger  & oRhs);

	/** Logger base priority              */
	IRIS::LogPriority::LogPriorities  & oLoggerBasePriority;
	/** Writer                            */
	LogWriter                         & oLogWriter;
	/** Parametrized string               */
	const STLW::string                & sLogFormat;
	/** Configuration data                */
	const IRIS::LoggerContext         * pLoggerContext;
	/** Mutex for writing                 */
	IRIS::Mutex                       & oMutex;
	/** Data buffer                       */
	IRIS::DataBuffer                    oDataBuffer;
	/** Data buffer                       */
	STLW::vector<Logger *>       vHandlers;

	/**
	  @brief Log message with formatted parameters
	*/
	IRIS::Logger::State LogMessage(const IRIS::LogPriority::LogPriorities  ePriority,
	                               CCHAR_P                                 szFormat,
	                               va_list                                 aArgList);

	/**
	  @brief Write message to log file
	  @param ePriority - priority level
	  @param szString - message to store in file
	  @param iStringLen - string length
	  @return 0- if success, -1 - if any error occured
	*/
	IRIS::Logger::State WriteLog(const IRIS::LogPriority::LogPriorities  ePriority,
	                             CCHAR_P                                 szString,
	                             const UINT_32                           iStringLen);
};


// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class LogWriter
//

//
// Type cast operator
//
LogWriter::operator IRIS::DataBuffer &() { return oDataBuffer; }

//
// Flush data
//
void LogWriter::Flush(const bool  bForce)
{
	// Log file is corrupt or not opened
	if (iFileHandle == -1)
	{
		IRIS::File::Write(fileno(stderr), oDataBuffer.Data(), oDataBuffer.Size());
		oDataBuffer.Clear();
		return;
	}
	// Flush data
	if (bForce || iMaxBufferSize == UINT_64(-1) || oDataBuffer.Size() >= iMaxBufferSize)
	{
		IRIS::File::Write(iFileHandle, oDataBuffer.Data(), oDataBuffer.Size());
		oDataBuffer.Clear();
	}
}

//
// Constrcutor
//
LogWriter::LogWriter(INT_64         & iIFileHandle,
                     const UINT_64    iIMaxBufferSize): iFileHandle(iIFileHandle),
                                                        iMaxBufferSize(iIMaxBufferSize)
{
	;;
}

//
// A destrcutor
//
LogWriter::~LogWriter() throw()
{
	Flush(true);
}

//
// Constructor
//
LoggerSignalHandler::LoggerSignalHandler(FileLogger  & oILogger): oLogger(oILogger) { ;; }

//
// A signal handler
//
void LoggerSignalHandler::Handler(INT_32 iSignal)
{
	// Re-open logs
	if (iSignal == SIGHUP)
	{
		oLogger.ReopenLog();
	}
	// Change priority
	else if (iSignal == SIGUSR1)
	{
		oLogger.SwitchPriority();
	}
}

//
// A destructor
//
LoggerSignalHandler::~LoggerSignalHandler() throw() { ;; }

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class ParametrizedLogger
//

//
// A constructor
//
ParametrizedLogger::ParametrizedLogger(IRIS::LogPriority::LogPriorities  & oILoggerBasePriority,
                                       LogWriter                         & oILogWriter,
                                       const STLW::string                & sILogFormat,
                                       const IRIS::LoggerContext         * pILoggerContext,
                                       IRIS::Mutex                       & oIMutex): oLoggerBasePriority(oILoggerBasePriority),
                                                                                     oLogWriter(oILogWriter),
                                                                                     sLogFormat(sILogFormat),
                                                                                     pLoggerContext(pILoggerContext),
                                                                                     oMutex(oIMutex)
{
	;;
}

//
// Format time
//
static STLW::string FormatDate(const time_t  iTime)
{
	static CCHAR_P szMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };
	static CCHAR_P szWdays[]  = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", NULL };

	// Convert timestamp to human-readable string
	CHAR_8 sTimestamp[32];

	struct tm oTime;
	gmtime_r(&iTime, &oTime);

	snprintf(sTimestamp, 32, "%s %s %02d %02d:%02d:%02d %04d", szWdays[oTime.tm_wday],
	                                                           szMonths[oTime.tm_mon],
	                                                           oTime.tm_mday,
	                                                           oTime.tm_hour,
	                                                           oTime.tm_min,
	                                                           oTime.tm_sec,
	                                                           oTime.tm_year + 1900);

return sTimestamp;
}

#if defined(sun) || defined(_AIX)
//
// Get GMT offset
//
static INT_32 GetGMTOff(const time_t       iTime,
                        const struct tm  & oLocalTime)
{
	struct tm oGMTime;
	gmtime_r(&iTime, &oGMTime);

	const INT_32 iLeapYearL = oLocalTime.tm_year + 1900;
	const INT_32 iLeapYearG = oGMTime.tm_year + 1900;

	const INT_32 iLeapYear = (iLeapYearL / 4 - iLeapYearG / 4) -
	                         (iLeapYearL / 100 - iLeapYearG / 100) +
	                         (iLeapYearL - iLeapYearG) * 365;

	const INT_32 iDays = oLocalTime.tm_yday - oGMTime.tm_yday + iLeapYear;

	const INT_32 iHours = iDays * 24 + oLocalTime.tm_hour - oGMTime.tm_hour;

return (iHours * 60 + oLocalTime.tm_min - oGMTime.tm_min) * 60;
}
#endif

//
// Log message with formatted parameters
//
IRIS::Logger::State ParametrizedLogger::ParametrizedLogger::LogMessage(const IRIS::LogPriority::LogPriorities  ePriority,
                                                                       CCHAR_P                                 szFormat,
                                                                       va_list                                 aArgList)
{
	// Message not logged, incorrect priority level
	if (ePriority > oLoggerBasePriority) { return LOGIC_ERROR; }

	{
		va_list vaTMP;
		va_copy(vaTMP, aArgList);

		CHAR_8 szStaticBuffer[C_LOGGER_START_MESSAGE_LEN];
		// Format value
		const INT_32 iFmtLen = vsnprintf(szStaticBuffer, C_LOGGER_START_MESSAGE_LEN - 1, szFormat, vaTMP);
		if (iFmtLen < C_LOGGER_START_MESSAGE_LEN)
		{
			// Write to logger
			return WriteLog(ePriority, szStaticBuffer, iFmtLen);
		}
	}

	INT_32 iResultLen = C_LOGGER_START_MESSAGE_LEN;
	CHAR_P szResult   = NULL;
	for(;;)
	{
		va_list vaTMP;
		va_copy(vaTMP, aArgList);

		// TODO: Improve performance
		iResultLen <<= 1;
		szResult = (CHAR_P)malloc(iResultLen);
		// Format value
		const INT_32 iFmtLen = vsnprintf(szResult, iResultLen - 1, szFormat, vaTMP);
		if (iFmtLen < iResultLen)
		{
			// Write to logger
			State eRC = WriteLog(ePriority, szResult, iFmtLen);
			free(szResult);
			return eRC;
		}

		free(szResult);
	}
// Never happened
return OK;
}


//
// Write message to log file
//
IRIS::Logger::State ParametrizedLogger::ParametrizedLogger::WriteLog(const IRIS::LogPriority::LogPriorities  ePriority,
                                                                     CCHAR_P                                 szString,
                                                                     const UINT_32                           iStringLen)
{
	using namespace IRIS;
	MutexLocker oMutexLocker(oMutex);
/*
	# $timegm.unix      - current GMT unixtime, seconds
	# $timegm.ascii     - current GMT time, ascii representation [Wkd Mmm DD HH:MM:SS YYYY]
	# $time.unix        - current LOCAL unixtime, seconds
	# $time.ascii       - current LOCAL time, ascii representation
*/
	SystemVars::Time oTimeGM = SystemVars::GetTime();

	IRIS::DataBuffer & oDataBuffer = oLogWriter;

	struct tm oLocalTime;
	time_t iTime = oTimeGM.sec;
	localtime_r(&iTime, &oLocalTime);

	STLW::string::const_iterator itsLogFormat = sLogFormat.begin();
	STLW::stringstream sStream;
	for (;;)
	{
		STLW::string::const_iterator itsTMP = itsLogFormat;
		while(itsLogFormat != sLogFormat.end() && *itsLogFormat != '$') { ++itsLogFormat; }
		sStream << STLW::string(itsTMP, itsLogFormat);

		if (itsLogFormat == sLogFormat.end()) { break; }

		itsTMP = itsLogFormat++;
		while(itsLogFormat != sLogFormat.end())
		{
			const CHAR_8 chTMP = *itsLogFormat;
			if (!((chTMP >= 'a' && chTMP <= 'z') ||
			      (chTMP >= 'A' && chTMP <= 'Z') ||
			      (chTMP >= '0' && chTMP <= '9') ||
			       chTMP == '.' || chTMP == '_')) { break; }
			++itsLogFormat;
		}

		const STLW::string sParam(++itsTMP, itsLogFormat);
		if      (sParam == "message.str")  { sStream << STLW::string(szString, iStringLen); }
		else if (sParam == "timegm.unix")  { sStream << oTimeGM.sec << '.' << oTimeGM.nsec; }
		else if (sParam == "time.unix")
		{
#if defined(sun) || defined(_AIX)
			sStream << oTimeGM.sec + GetGMTOff(iTime, oLocalTime) << '.' << oTimeGM.nsec;
#else
			sStream << oTimeGM.sec + oLocalTime.tm_gmtoff << '.' << oTimeGM.nsec;
#endif
		}
		else if (sParam == "timegm.ascii") { sStream << FormatDate(oTimeGM.sec); }
		else if (sParam == "time.ascii")
		{
#if defined(sun) || defined(_AIX)
			sStream << FormatDate(oTimeGM.sec + GetGMTOff(iTime, oLocalTime));
#else
			sStream << FormatDate(oTimeGM.sec + oLocalTime.tm_gmtoff);
#endif
		}
		else if (sParam == "priority")     { sStream << LogPriority::GetPrintableState(ePriority); }
		else
		{
			if (pLoggerContext == NULL)
			{
				sStream << "-";
			}
			else
			{
				STLW::string sValue;
				pLoggerContext -> GetParam(sParam, sValue);
				sStream << sValue;
			}
		}
	}

	const STLW::string sResult = sStream.str();
	STLW::string::const_iterator itsResult = sResult.begin();
	for (;;)
	{
		STLW::string::const_iterator itsStart = itsResult;
		while (itsResult != sResult.end() && *itsResult >= ' ') { ++itsResult; }
		oDataBuffer.Append(itsStart, itsResult);
		if (itsResult == sResult.end()) { break; }

		const UCHAR_8 ucTMP = *itsResult;
		switch (ucTMP)
		{
			case '\n':
				oDataBuffer.Append("\\n");
				break;
			case '\r':
				oDataBuffer.Append("\\r");
				break;
			case '\t':
				oDataBuffer.Append("\\t");
				break;
			default:
				{
					CHAR_8 szBuf[6];
					INT_32 iBufLen = snprintf(szBuf, 6, "\\x%02X", ucTMP);
					oDataBuffer.Append(szBuf, iBufLen);
				}
		}

		++itsResult;
		if (itsResult == sResult.end()) { break; }
	}
	oDataBuffer.Append("\n");
	oLogWriter.Flush();

return IRIS::Logger::OK;
}

//
// A destructor
//
ParametrizedLogger::~ParametrizedLogger() throw()
{
	;;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class FileLogger
//

//
// A constructor
//
FileLogger::FileLogger(): iErrorLogFileHandle(-1),
                          iCustomLogFileHandle(-1),
                          pErrorLogWriter(NULL),
                          pCustomLogWriter(NULL),
                          oLoggerSignalHandler(*this)
{
	;;
}

//
// Initialize module
//
INT_32 FileLogger::InitModule(IRIS::GlobalContext    & oGlobalContext,
                              const IRIS::VariantNC  & oConfig,
                              IRIS::SignalHandler    & oSigHandler,
                              IRIS::Logger           & oLogger)
{
	using namespace IRIS;

	// Re-open log files
	oSigHandler.RegisterHandler(SIGHUP,  &oLoggerSignalHandler);
	oSigHandler.RegisterHandler(SIGUSR1, &oLoggerSignalHandler);
	oSigHandler.RegisterHandler(SIGUSR2, &oLoggerSignalHandler);

	STLW::string sTMP = oConfig["LogLevel"].Str();

	static CCHAR_P  aPriorities[] = { "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", NULL };
	static LogPriority::LogPriorities oPriorities[] = { LogPriority::LOG_EMERG,   LogPriority::LOG_ALERT,
	                                                    LogPriority::LOG_CRIT,    LogPriority::LOG_ERROR,
	                                                    LogPriority::LOG_WARNING, LogPriority::LOG_NOTICE,
	                                                    LogPriority::LOG_INFO,    LogPriority::LOG_DEBUG };

	oActualPriority = oBasePriority = LogPriority::LOG_DEBUG;
	CCHAR_P      * aPriority = aPriorities;
	IRIS::LogPriority::LogPriorities  * oPriority = oPriorities;
	while (*aPriority != NULL)
	{
		if (Unicode::CompareIgnoreCase(sTMP.data(), sTMP.size(), *aPriority, strlen(*aPriority)) == 0)
		{
			oActualPriority = oBasePriority = *oPriority;
			break;
		}
		++aPriority;
		++oPriority;
	}

	iErrorLogBufferSize = iCustomLogBufferSize = -1;

	sErrorLogFormat.assign(oConfig["ErrorLogFormat"]);
	if (sErrorLogFormat.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter %s/%s/ErrorLogFormat not set", GetObjectType(), GetObjectName());
		return -1;
	}

	sErrorLogFile.assign(oConfig["ErrorLog"]);
	if (sErrorLogFile.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter %s/%s/ErrorLog not set", GetObjectType(), GetObjectName());
		return -1;
	}

	sCustomLogFormat.assign(oConfig["CustomLogFormat"]);
	if (sCustomLogFormat.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter %s/%s/CustomLogFormat not set", GetObjectType(), GetObjectName());
		return -1;
	}

	sCustomLogFile.assign(oConfig["CustomLog"]);
	if (sCustomLogFile.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter `%s/%s/CustomLog` not set", GetObjectType(), GetObjectName());
		return -1;
	}

	// Error log permissions
	char * pEnd = NULL;
	iErrorLogPerms = strtol(oConfig["ErrorLogPerms"].Str().c_str(), &pEnd, 8);
	if (iErrorLogPerms == 0) { oLogger.Info("Parameter `%s/%s/ErrorLogPerms` not set", GetObjectType(), GetObjectName()); }

	// User and group
	ConfigHelper::State oConfRC = ConfigHelper::ParseUserGroup(oConfig["ErrorLogOwner"],
	                                                           sErrorLogUser,
	                                                           iErrorLogUID,
	                                                           sErrorLogGroup,
	                                                           iErrorLogGID);
	switch(oConfRC)
	{
		case ConfigHelper::NOT_NEED:
			oLogger.Info("Parameter `%s/%s/ErrorLogOwner` is useful only if Iris starting from root user", GetObjectType(), GetObjectName());
			break;

		case ConfigHelper::CONFIG_ERROR:
			oLogger.Info("Parameter `%s/%s/ErrorLogOwner` not set", GetObjectType(), GetObjectName());
			break;

		case ConfigHelper::NO_SUCH_USER:
			oLogger.Emerg("Parameter `%s/%s/ErrorLogOwner`: no such user: `%s`", GetObjectType(), GetObjectName(), sErrorLogUser.c_str());
			return -1;

		case ConfigHelper::NO_SUCH_GROUP:
			oLogger.Emerg("Parameter `%s/%s/ErrorLogOwner`: no such group: `%s`", GetObjectType(), GetObjectName(), sErrorLogGroup.c_str());
			return -1;
		default:
			;;
	}

	// Buffer size for error log
	iErrorLogBufferSize = ConfigHelper::ParseSize(oConfig["ErrorLogBufferSize"]);
	if (iErrorLogBufferSize == 0) { oLogger.Info("Parameter `%s/%s/ErrorLogBufferSize` not set", GetObjectType(), GetObjectName()); }

	// Error log permissions
	iCustomLogPerms = strtol(oConfig["CustomLogPerms"].Str().c_str(), &pEnd, 8);
	if (iCustomLogPerms == 0) { oLogger.Info("Parameter `%s/%s/CustomLogPerms` not set", GetObjectType(), GetObjectName()); }

	// User and group
	oConfRC = ConfigHelper::ParseUserGroup(oConfig["CustomLogOwner"],
	                                       sCustomLogUser,
	                                       iCustomLogUID,
	                                       sCustomLogGroup,
	                                       iCustomLogGID);
	switch(oConfRC)
	{
		case ConfigHelper::NOT_NEED:
			oLogger.Info("Parameter `%s/%s/CustomLogOwner` is useful only if Iris starting from root user", GetObjectType(), GetObjectName());
			break;

		case ConfigHelper::CONFIG_ERROR:
			oLogger.Info("Parameter `%s/%s/CustomLogOwner` not set", GetObjectType(), GetObjectName());
			break;

		case ConfigHelper::NO_SUCH_USER:
			oLogger.Emerg("Parameter `%s/%s/CustomLogOwner`: no such user: `%s`", GetObjectType(), GetObjectName(), sErrorLogUser.c_str());
			return -1;

		case ConfigHelper::NO_SUCH_GROUP:
			oLogger.Emerg("Parameter `%s/%s/CustomLogOwner`: no such group: `%s`", GetObjectType(), GetObjectName(), sErrorLogGroup.c_str());
			return -1;

		default:
			;;
	}

	// Buffer size for custom log
	iCustomLogBufferSize = ConfigHelper::ParseSize(oConfig["CustomLogBufferSize"]);
	if (iCustomLogBufferSize == 0) { oLogger.Info("Parameter `%s/%s/CustomLogBufferSize` not set", GetObjectType(), GetObjectName()); }

	// Try to open log files
	oLogger.Info("%s/%s: Try to open log file `%s`", GetObjectType(), GetObjectName(), sErrorLogFile.c_str());
	iErrorLogFileHandle = File::Open(sErrorLogFile.c_str(), OpenMode::WRITE | OpenMode::CREATE | OpenMode::APPEND);
	if (iErrorLogFileHandle == -1)
	{
		oLogger.Emerg("%s/%s: Can't open file `%s`: %s", GetObjectType(), GetObjectName(), sErrorLogFile.c_str(), Debug::PrintableError(SystemVars::ErrNo()).c_str());
		return -1;
	}
	// Change permissions and owner
	if (iErrorLogPerms != 0) { File::Chmod(iErrorLogFileHandle, iErrorLogPerms); }
	if (iErrorLogUID != 0) { File::Chown(iErrorLogFileHandle, iErrorLogUID, iErrorLogGID); }

	// Try to open log files
	oLogger.Info("%s/%s: Try to open log file `%s`", GetObjectType(), GetObjectName(), sCustomLogFile.c_str());
	iCustomLogFileHandle = File::Open(sCustomLogFile.c_str(), OpenMode::WRITE | OpenMode::CREATE | OpenMode::APPEND);
	if (iCustomLogFileHandle == -1)
	{
		oLogger.Emerg("%s/%s: Can't open file `%s`: %s", GetObjectType(), GetObjectName(), sCustomLogFile.c_str(), Debug::PrintableError(SystemVars::ErrNo()).c_str());
		File::Close(iErrorLogFileHandle);
		iErrorLogFileHandle = iCustomLogFileHandle = -1;
		return -1;
	}
	if (iCustomLogPerms != 0) { File::Chmod(iCustomLogFileHandle, iCustomLogPerms); }
	if (iCustomLogUID != 0) { File::Chown(iCustomLogFileHandle, iCustomLogUID, iCustomLogGID); }

	// Create writers
	pErrorLogWriter  = new LogWriter(iErrorLogFileHandle,  iErrorLogBufferSize);
	pCustomLogWriter = new LogWriter(iCustomLogFileHandle, iCustomLogBufferSize);

return 0;
}

//
// Close and open log files
//
void FileLogger::ReopenLog()
{
	using namespace IRIS;
	if (pErrorLogWriter != NULL)
	{
		CRITICAL
		{
			MutexLocker oMutexLocker(oErrorLogMutex);

			pErrorLogWriter -> Flush(true);
			File::Close(iErrorLogFileHandle);
			iErrorLogFileHandle = File::Open(sErrorLogFile.c_str(), OpenMode::WRITE | OpenMode::CREATE | OpenMode::APPEND);
			if (iErrorLogFileHandle != -1)
			{
				if (iErrorLogPerms != 0) { File::Chmod(iErrorLogFileHandle, iErrorLogPerms); }
				if (iErrorLogUID != 0) { File::Chown(iErrorLogFileHandle, iErrorLogUID, iErrorLogGID); }
			}
		}
	}

	if (pCustomLogWriter != NULL)
	{
		CRITICAL
		{
			MutexLocker oMutexLocker(oTransferLogMutex);

			pCustomLogWriter -> Flush(true);
			File::Close(iCustomLogFileHandle);
			iCustomLogFileHandle = File::Open(sCustomLogFile.c_str(), OpenMode::WRITE | OpenMode::CREATE | OpenMode::APPEND);
			if (iCustomLogFileHandle != -1)
			{
				if (iCustomLogPerms != 0) { File::Chmod(iCustomLogFileHandle, iCustomLogPerms); }
				if (iCustomLogUID != 0) { File::Chown(iCustomLogFileHandle, iCustomLogUID, iCustomLogGID); }
			}
		}
	}
}

//
// Switch priority from/to debug
//
void FileLogger::SwitchPriority()
{
	if (oActualPriority == IRIS::LogPriority::LOG_DEBUG)
	{
		oActualPriority = oBasePriority;
	}
	else
	{
		oActualPriority = IRIS::LogPriority::LOG_DEBUG;
	}
}

//
// Get error log
//
IRIS::Logger * FileLogger::GetErrorLog(const IRIS::LoggerContext * pLoggerContext)
{
	ParametrizedLogger * pErrorLog = new ParametrizedLogger(oActualPriority,
	                                                        *pErrorLogWriter,
	                                                        sErrorLogFormat,
	                                                        pLoggerContext,
	                                                        oErrorLogMutex);

return pErrorLog;
}

//
// Get custom log
//
IRIS::Logger * FileLogger::GetCustomLog(const IRIS::LoggerContext * pLoggerContext)
{
	ParametrizedLogger * pTransferLog = new ParametrizedLogger(oActualPriority,
	                                                           *pCustomLogWriter,
	                                                           sCustomLogFormat,
	                                                           pLoggerContext,
	                                                           oTransferLogMutex);

return pTransferLog;
}

//
// Destroy module
//
INT_32 FileLogger::DestroyModule(IRIS::GlobalContext  & oGlobalContext,
                                 IRIS::SignalHandler  & oSigHandler,
                                 IRIS::Logger         & oLogger)
{
	;;
return 0;
}

//
// Get object type
//
CCHAR_P FileLogger::GetObjectType() const { return "Logger"; }

//
// Get object name
//
CCHAR_P FileLogger::GetObjectName() const { return "FileLogger"; }

//
// A destructor
//
FileLogger::~FileLogger() throw()
{
	using namespace IRIS;

	if (pErrorLogWriter != NULL)
	{
		delete pErrorLogWriter;
		pErrorLogWriter = NULL;
	}

	if (pCustomLogWriter != NULL)
	{
		delete pCustomLogWriter;
		pCustomLogWriter = NULL;
	}

	if (iErrorLogFileHandle != -1)  { File::Close(iErrorLogFileHandle); }
	if (iCustomLogFileHandle != -1) { File::Close(iCustomLogFileHandle); }
}

} // namespace TEST
// End.
