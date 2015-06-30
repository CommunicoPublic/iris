/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

/**
  @file IrisSyslogLogger.hpp
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
#include <syslog.h>

#ifndef C_LOGGER_START_MESSAGE_LEN
#define C_LOGGER_START_MESSAGE_LEN 512
#endif // C_LOGGER_START_MESSAGE_LEN

// http://wiki.opencsw.org/porting-faq#toc18
#ifdef sun
    #define LOG_PERROR 0x00
    #define LOG_AUTHPRIV (10<<3)
    #define LOG_FTP      (11<<3)
#endif

/**
  @namespace IRIS_MOD_SYSL
  @brief Iris file logger namespace
*/
namespace IRIS_MOD_SYSL
{

// FWD
class SyslogLogger;

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
	LoggerSignalHandler(SyslogLogger  & oILogger);

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
	SyslogLogger     & oLogger;
};

/**
  @class SyslogLogger IrisSyslogLogger.cpp <IrisSyslogLogger.cpp>
  @brief Loadable object; logger subsystem
*/
class SyslogLogger:
 public IRIS::LoggerObject
{
public:
	/**
	  @brief A constructor
	*/
	SyslogLogger();

	/**
	  @brief Switch priority from/to debug
	*/
	void SwitchPriority();

	/**
	  @brief A destructor
	*/
	~SyslogLogger() throw();
private:
	// Does not exist
	SyslogLogger(const SyslogLogger  & oRhs);
	SyslogLogger& operator=(const SyslogLogger  & oRhs);

	/** Base priority            */
	IRIS::LogPriority::LogPriorities  oBasePriority;
	/** Actual priority            */
	IRIS::LogPriority::LogPriorities  oActualPriority;
	/** Error log record format    */
	STLW::string           sErrorLogFormat;
	/** Custom log record format   */
	STLW::string           sCustomLogFormat;
	/** Logger signel handler     */
	LoggerSignalHandler    oLoggerSignalHandler;

	template <typename T> T ParseString(const STLW::string  & sSource,
	                                    CCHAR_P             * aKeys,
	                                    T                   * aVals,
	                                    const T             & oDefaultVal) const
	{
		while (*aKeys != NULL)
		{
			if (IRIS::Unicode::CompareIgnoreCase(sSource.data(), sSource.size(), *aKeys, strlen(*aKeys)) == 0)
			{
				return *aVals;
				break;
			}
			++aKeys;
			++aVals;
		}
	return oDefaultVal;
	}

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

EXPORT_HANDLER(SyslogLogger)

/**
  @class ParametrizedLogger IrisSyslogLogger.cpp <IrisSyslogLogger.cpp>
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
	                   const STLW::string                & sILogFormat,
	                   const IRIS::LoggerContext         * pILoggerContext);

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
	/** Parametrized string               */
	const STLW::string                & sLogFormat;
	/** Configuration data                */
	const IRIS::LoggerContext         * pLoggerContext;
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

//
// Constructor
//
LoggerSignalHandler::LoggerSignalHandler(SyslogLogger  & oILogger): oLogger(oILogger) { ;; }

//
// A signal handler
//
void LoggerSignalHandler::Handler(INT_32 iSignal)
{
	// Change priority
	if (iSignal == SIGUSR1)
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
                                       const STLW::string                & sILogFormat,
                                       const IRIS::LoggerContext         * pILoggerContext): oLoggerBasePriority(oILoggerBasePriority),
                                                                                             sLogFormat(sILogFormat),
                                                                                             pLoggerContext(pILoggerContext)
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
/*
	# $timegm.unix      - current GMT unixtime, seconds
	# $timegm.ascii     - current GMT time, ascii representation [Wkd Mmm DD HH:MM:SS YYYY]
	# $time.unix        - current LOCAL unixtime, seconds
	# $time.ascii       - current LOCAL time, ascii representation
*/
	SystemVars::Time oTimeGM = SystemVars::GetTime();

	IRIS::DataBuffer oDataBuffer;

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

	// Wrie log here
	syslog(INT_32(ePriority), "%.*s", INT_32(oDataBuffer.Size()), oDataBuffer.Data());

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
// Class SyslogLogger
//

//
// A constructor
//
SyslogLogger::SyslogLogger(): oLoggerSignalHandler(*this)
{
	;;
}

//
// Initialize module
//
INT_32 SyslogLogger::InitModule(IRIS::GlobalContext    & oGlobalContext,
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
	static LogPriority::LogPriorities oPriorities[] = { LogPriority::LogPriorities(LOG_EMERG),   LogPriority::LogPriorities(LOG_ALERT),
	                                                    LogPriority::LogPriorities(LOG_CRIT),    LogPriority::LogPriorities(LOG_ERR),
	                                                    LogPriority::LogPriorities(LOG_WARNING), LogPriority::LogPriorities(LOG_NOTICE),
	                                                    LogPriority::LogPriorities(LOG_INFO),    LogPriority::LogPriorities(LOG_DEBUG) };

/*	CCHAR_P      * aPriority = aPriorities;
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
*/
	oActualPriority = oBasePriority = ParseString(sTMP, aPriorities, oPriorities, LogPriority::LogPriorities(LOG_DEBUG));

	sErrorLogFormat.assign(oConfig["ErrorLogFormat"]);
	if (sErrorLogFormat.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter %s/%s/ErrorLogFormat not set", GetObjectType(), GetObjectName());
		return -1;
	}

	sCustomLogFormat.assign(oConfig["CustomLogFormat"]);
	if (sCustomLogFormat.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter %s/%s/CustomLogFormat not set", GetObjectType(), GetObjectName());
		return -1;
	}

	STLW::string sIdent = oConfig["LogIdent"].Str();

	static CCHAR_P  aOptions[] = { "LOG_CONS", "LOG_NDELAY", "LOG_NOWAIT", "LOG_ODELAY", "LOG_PERROR", "LOG_PID" };
	static INT_32   oOptions[] = { LOG_CONS, LOG_NDELAY, LOG_NOWAIT, LOG_ODELAY, LOG_PERROR, LOG_PID };

	INT_32 iOptions = 0;
	STLW::vector<STLW::string> vOptions = oConfig["LogOptions"];
	STLW::vector<STLW::string>::const_iterator itvOptions = vOptions.begin();
	while (itvOptions != vOptions.end())
	{
		iOptions |= ParseString(*itvOptions, aOptions, oOptions, LOG_PID);
		++itvOptions;
	}

	static CCHAR_P  aFacilities[] = { "LOG_AUTH", "LOG_AUTHPRIV", "LOG_CRON", "LOG_DAEMON", "LOG_FTP", "LOG_KERN", "LOG_LOCAL0", "LOG_LOCAL1", "LOG_LOCAL2", "LOG_LOCAL3", "LOG_LOCAL4", "LOG_LOCAL5", "LOG_LOCAL6", "LOG_LOCAL7", "LOG_LPR", "LOG_MAIL", "LOG_NEWS", "LOG_SYSLOG", "LOG_USER", "LOG_UUCP", NULL };
	static INT_32   oFacilities[] = { LOG_AUTH, LOG_AUTHPRIV, LOG_CRON, LOG_DAEMON, LOG_FTP, LOG_KERN, LOG_LOCAL0, LOG_LOCAL1, LOG_LOCAL2, LOG_LOCAL3, LOG_LOCAL4, LOG_LOCAL5, LOG_LOCAL6, LOG_LOCAL7, LOG_LPR, LOG_MAIL, LOG_NEWS, LOG_SYSLOG, LOG_USER, LOG_UUCP };

	sTMP = oConfig["LogFacility"].Str();

	INT_32 iFacility = ParseString(sTMP, aFacilities, oFacilities, LOG_USER);

	openlog(sIdent.c_str(), iOptions, iFacility);

return 0;
}

//
// Switch priority from/to debug
//
void SyslogLogger::SwitchPriority()
{
	if (oActualPriority == IRIS::LogPriority::LogPriorities(LOG_DEBUG))
	{
		oActualPriority = oBasePriority;
	}
	else
	{
		oActualPriority = IRIS::LogPriority::LogPriorities(LOG_DEBUG);
	}
}

//
// Get error log
//
IRIS::Logger * SyslogLogger::GetErrorLog(const IRIS::LoggerContext * pLoggerContext)
{
	ParametrizedLogger * pErrorLog = new ParametrizedLogger(oActualPriority,
	                                                        sErrorLogFormat,
	                                                        pLoggerContext);

return pErrorLog;
}

//
// Get custom log
//
IRIS::Logger * SyslogLogger::GetCustomLog(const IRIS::LoggerContext * pLoggerContext)
{
	ParametrizedLogger * pTransferLog = new ParametrizedLogger(oActualPriority,
	                                                           sCustomLogFormat,
	                                                           pLoggerContext);

return pTransferLog;
}

//
// Destroy module
//
INT_32 SyslogLogger::DestroyModule(IRIS::GlobalContext  & oGlobalContext,
                                   IRIS::SignalHandler  & oSigHandler,
                                   IRIS::Logger         & oLogger)
{
	;;
return 0;
}

//
// Get object type
//
CCHAR_P SyslogLogger::GetObjectType() const { return "Logger"; }

//
// Get object name
//
CCHAR_P SyslogLogger::GetObjectName() const { return "SyslogLogger"; }

//
// A destructor
//
SyslogLogger::~SyslogLogger() throw()
{
	closelog();
}

} // namespace TEST
// End.
