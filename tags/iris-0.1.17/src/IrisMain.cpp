/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef IRIS_ENV_CONFIG_FILE
#define IRIS_ENV_CONFIG_FILE "IRIS_CONFIG_FILE"
#endif // IRIS_ENV_CONFIG_FILE

#include <sysexits.h>

#include "ConfigHelper.hpp"
#include "GetOpt.hpp"
#include "GlobalContext.hpp"
#include "FileHandleCache.hpp"
#include "FileUtils.hpp"
#include "LoggerFile.hpp"
#include "MainSignalHandler.hpp"
#include "MainHelpers.hpp"
#include "MainProcess.hpp"
#include "SystemVars.hpp"

using namespace IRIS;

//
// Options
//
static const Options oOptions[] = {
                                    { 'f', "config",       C_HAS_ARG }, // Config file
                                    { 'd', "debug",        C_NO_ARG  }, // Print debug info at startup
                                    { 'h', "help",         C_NO_ARG  }, // Print usage info
                                    { 'v', "version",      C_NO_ARG  }, // Print version
                                    { 'V', "compile-info", C_NO_ARG  }, // Show compile settings
                                    { 'F', "foreground",   C_NO_ARG  }, // Run process in foreground
                                    { 0 }
                                  };

//
// Get parameters
//
static INT_32 GetParams(const INT_32    iArgc,
                        CHAR_P        * aArgv,
                        STLW::string  & sConfigFile,
                        bool          & bDebugInfo,
                        bool          & bPrintHelp,
                        bool          & bPrintVersion,
                        bool          & bPrintCompileInfo,
                        bool          & bDaemonize)
{
	bDebugInfo = bPrintHelp = bPrintVersion = bPrintCompileInfo = false;
	bDaemonize = true;
	sConfigFile.erase();

	UINT_32 iArgNum = 1;
	CCHAR_P szArgValue;
	for(;;)
	{
		INT_32 iOption = GetOpt(oOptions, iArgNum, iArgc, aArgv, szArgValue);
		if (iOption == -1) { break; }
		switch (iOption)
		{
			case 'f':
				sConfigFile = szArgValue;
				break;
			case 'd':
				bDebugInfo = true;
				break;
			case 'h':
				bPrintHelp = true;
				break;
			case 'v':
				bPrintVersion = true;
				break;
			case 'V':
				bPrintCompileInfo = true;
				break;
			case 'F':
				bDaemonize = false;
				break;
			case '?':
				fprintf(stderr, "Unknown option `%s` or option with missed argument\n", aArgv[iArgNum]);
				return EX_CONFIG;
				break;
		}
	}

	if (sConfigFile.empty())
	{
		// Check environment
		CCHAR_P szConfigFile = getenv(IRIS_ENV_CONFIG_FILE);
		if (szConfigFile != NULL && *szConfigFile != '\0') { sConfigFile = szConfigFile; }

		if (sConfigFile.empty()) { sConfigFile.assign(IRIS_CONFIG_FILE); }
	}

return EX_OK;
}

//
// Print usage info
//
static void Usage(CHAR_P szProgname)
{
	fprintf(stdout, "Usage: %s [-f <config file>] [-h] [-v] [-V] [-d] [-F]\n", szProgname);
}

//
// Print short help
//
static void Help(CHAR_P szProgname)
{
	Usage(szProgname);
	fprintf(stdout, "Options\n"
	                "  -f <config file> : specify alternate configuration file\n"
	                "  -h               : print short help\n"
	                "  -v               : show version number\n"
	                "  -V               : show compile settings\n"
	                "  -d               : run in debug mode\n"
	                "  -F               : run main process in foreground\n\n");
}

//
// Print version
//
static void Version()
{
	fprintf(stdout, "This is Iris server. Version " IRIS_VERSION "\n"
	                "For more information, refer to: " IRIS_MASTER_SITE_URL "\n\n");
}

//
// Print compile info
//
static void CompileInfo()
{
	fprintf(stdout, "Iris server compiled " __TIME__ " " __DATE__
#ifdef __GNUC__
	" with gcc "  __VERSION__ "\n"
#endif // __GNUC__
	);

	fprintf(stdout, "    Version....................: " IRIS_VERSION "\n");

#ifdef IRIS_TLS_SUPPORT
	fprintf(stdout, "    TLS support................: On\n");
#else
	fprintf(stdout, "    TLS support................: Off\n");
#endif // IRIS_TLS_SUPPORT

#ifdef IRIS_TLS_SUPPORT
	fprintf(stdout, "    Default configuration file.: " IRIS_CONFIG_FILE "\n");
#endif // IRIS_TLS_SUPPORT

#ifdef IRIS_BIG_SECURITY_HOLE
	fprintf(stdout, "\n\n");
	fprintf(stdout, "!!!! ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION !!!!\n"
	                "!!!!    Iris was compiled with IRIS_BIG_SECURITY_HOLE option.    !!!!\n"
	                "!!!!   This option is potentially harmful and you should use it  !!!!\n"
	                "!!!!                   ONLY AT YOUR OWN RISK.                    !!!!\n"
	                "!!!! ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION !!!!\n");
#endif // IRIS_BIG_SECURITY_HOLE

	fprintf(stdout, "\n");
}

/**
  @class DirectoryCallback FileUtils.hpp <FileUtils.hpp>
  @brief Directory callback
*/
class IncludeFileCallback:
  public GlobCallback
{
public:
	/**
	  @brief Constructor
	*/
	IncludeFileCallback(STLW::vector<STLW::string>   & vIIncludeFiles);

	/**
	  @brief Callback
	*/
	void Entry(CCHAR_P szName);

	/**
	  @brief Destructor
	*/
	~IncludeFileCallback() throw();
private:
	// Does not exist
	IncludeFileCallback(const IncludeFileCallback  & oRhs);
	IncludeFileCallback& operator=(const IncludeFileCallback  & oRhs);

	/** Include files */
	STLW::vector<STLW::string> & vIncludePatterns;
};

//
// Constructor
//
IncludeFileCallback::IncludeFileCallback(STLW::vector<STLW::string>   & vIIncludeFiles): vIncludePatterns(vIIncludeFiles) { ;; }

//
// Callback
//
void IncludeFileCallback::Entry(CCHAR_P szName) { vIncludePatterns.push_back(szName); }

//
// Destructor
//
IncludeFileCallback::~IncludeFileCallback() throw() { ;; }

//
// Recursive parse includes
//
static ConfigHelper::State ParseIncludes(const STLW::vector<STLW::string>  & vIncludeDirs,
                                         const STLW::vector<STLW::string>  & vIncludePatterns,
                                         STLW::set<STLW::string>           & sSections,
                                         VariantNC                         & oConfigData,
                                         STLW::set<STLW::string>           & sFiles,
                                         Logger                            & oLogger)
{
	STLW::vector<STLW::string>::const_iterator itvIncludeDirs = vIncludeDirs.begin();
	while (itvIncludeDirs != vIncludeDirs.end())
	{
		STLW::vector<STLW::string>::const_iterator itvIncludePatterns = vIncludePatterns.begin();
		while (itvIncludePatterns != vIncludePatterns.end())
		{
			STLW::vector<STLW::string> vFiles;
			IncludeFileCallback oIncludeFileCallback(vFiles);
			const STLW::string sPattern = *itvIncludeDirs + '/' + *itvIncludePatterns;
			File::Glob(sPattern.c_str(), &oIncludeFileCallback);

			STLW::vector<STLW::string>::const_iterator itvFiles = vFiles.begin();
			while (itvFiles != vFiles.end())
			{
				CHAR_P szRealPath = File::GetRealPath(itvFiles -> c_str(), NULL);
				STLW::string sRealPath = szRealPath;
				free(szRealPath);
				if (sFiles.find(sRealPath) != sFiles.end())
				{
					oLogger.Info("Skipping duplicate configuraton file %s", sRealPath.c_str());
				}
				else
				{
					sFiles.insert(sRealPath);
					oLogger.Info("Loading configuraton file %s", sRealPath.c_str());
					STLW::vector<STLW::string> vIncludePatterns2;
					ConfigHelper::State oConfigState = ConfigHelper::ParseConfigFile(sRealPath, vIncludePatterns2, sSections, oConfigData, oLogger);
					if (oConfigState != ConfigHelper::OK) { return oConfigState; }

					if (!vIncludePatterns2.empty())
					{
						oConfigState = ParseIncludes(vIncludeDirs, vIncludePatterns2, sSections,  oConfigData, sFiles, oLogger);
						if (oConfigState != ConfigHelper::OK) { return oConfigState; }
					}
				}

				++itvFiles;
			}
			++itvIncludePatterns;
		}
		++itvIncludeDirs;
	}
return ConfigHelper::OK;
}

int main(int argc, char ** argv)
{
	STLW::string  sConfigFile;
	bool  bPrintHelp        = false;
	bool  bPrintVersion     = false;
	bool  bPrintCompileInfo = false;
	bool  bDaemonize        = true;
	bool  bDebugInfo        = false;

	SystemVars::Time oTime = SystemVars::GetTime();
	srandom(oTime.sec);

	// Temp. logger
	LoggerFile oLogger(stderr);
	oLogger.SetPriority(LogPriority::LOG_WARNING);

	// Read and parse command-line options
	INT_32 iRC = GetParams(argc, argv, sConfigFile, bDebugInfo, bPrintHelp, bPrintVersion, bPrintCompileInfo, bDaemonize);
	if (iRC != EX_OK) { Usage(argv[0]); return EX_USAGE; }

	if (bDebugInfo) { oLogger.SetPriority(LogPriority::LOG_DEBUG); }

	// Just print help and exit
	if (bPrintHelp) { Help(argv[0]); return EX_OK; }

	// Print version
	if (bPrintVersion) { Version(); return EX_OK; }

	// Print compiler settings
	if (bPrintCompileInfo) { CompileInfo(); return EX_OK; }

	// Parse configuration
	VariantNC oConfigData;
	STLW::vector<STLW::string> vIncludePatterns;
	STLW::set<STLW::string>    sSections;
	ConfigHelper::State oConfigState = ConfigHelper::ParseConfigFile(sConfigFile, vIncludePatterns, sSections, oConfigData, oLogger);
	if (oConfigState != ConfigHelper::OK)
	{
		if (argc == 1) { oLogger.Info("\nTry `%s --help' for more information.\n", argv[0]); }

		if (oConfigState == ConfigHelper::ERROR)        { return EX_SOFTWARE; }
		if (oConfigState == ConfigHelper::CONFIG_ERROR) { return EX_CONFIG; }
	}

	bool bLoggerStarted = false;

	// Global execution context
	GlobalContext oGlobalContext;
	oGlobalContext.error_log    = &oLogger;
	oGlobalContext.transfer_log = &oLogger;

	STLW::set<STLW::string> sFiles;
	CHAR_P szRealPath = File::GetRealPath(sConfigFile.c_str(), NULL);
	sFiles.insert(szRealPath);
	free(szRealPath);

	sFiles.insert(sConfigFile);

	STLW::vector<STLW::string> vIncludeDirs;
	// Get include dirs
	ConfigHelper::GetDirList(oConfigData["Module"]["Main"]["IncludeDirs"], vIncludeDirs);
	// Current directory
	vIncludeDirs.push_back(".");
	// Directory with config file
	CHAR_P szConfigDir = File::Dirname(sConfigFile.c_str());
	if (szConfigDir != NULL)
	{
		vIncludeDirs.push_back(szConfigDir);
		free(szConfigDir);
	}

	// Load and parse includes, first pass
	oConfigState = ParseIncludes(vIncludeDirs, vIncludePatterns, sSections, oConfigData, sFiles, oLogger);
	if (oConfigState != ConfigHelper::OK)
	{
		if (oConfigState == ConfigHelper::ERROR)        { return EX_SOFTWARE; }
		if (oConfigState == ConfigHelper::CONFIG_ERROR) { return EX_CONFIG; }
	}

	// Get configuration
	MainConfig::State oMainConfigState = MainConfig::GetConfig(oConfigData, oGlobalContext.config, oLogger);
	if (oMainConfigState == MainConfig::ERROR)        { return EX_SOFTWARE; }
	if (oMainConfigState == MainConfig::CONFIG_ERROR) { return EX_CONFIG;   }

	// File handle cache
	oGlobalContext.file_handle_cache = new FileHandleCache(oGlobalContext.config.file_cache_size);

	// Signal handler
	SignalHandler & oSigHandler = SignalHandler::Instance();

	/*
	 * Need scope here to avoid "pure virtual method called" after destroying loggers
	 */
	{
		// Set main signal handler.
		MainSignalHandler oMainSignalHandler(oGlobalContext);
		oSigHandler.RegisterHandler(SIGPIPE, &oMainSignalHandler);
		oSigHandler.RegisterHandler(SIGINT,  &oMainSignalHandler);
		oSigHandler.RegisterHandler(SIGTERM, &oMainSignalHandler);
		oSigHandler.RegisterHandler(SIGUSR1, &oMainSignalHandler);

		// Create main process
		MainProcess oMainProcess(oSigHandler, oGlobalContext);

		// Daemonize
		if (bDaemonize)
		{
			iRC = MainHelpers::Daemonize(oGlobalContext.config.pid_file.c_str(), oLogger);
			if (iRC != EX_OK)
			{
				delete oGlobalContext.file_handle_cache;
				return iRC;
			}
		}

		// Load modules, initialize loggers, etc
		// This is last point where we need to use this logger
		if (oMainProcess.Setup(oLogger, bLoggerStarted) != 0)
		{
			// Shutdown server
			oMainProcess.Shutdown();
			delete oGlobalContext.file_handle_cache;
			return EX_SOFTWARE;
		}

		// Setup network connections, etc
		if (oMainProcess.SetupNetwork() == 0)
		{
			// Switch to unprivileged user and daemonize
			iRC = MainHelpers::UnixSetup(oGlobalContext.config, *oGlobalContext.error_log);
			if (iRC != EX_OK)
			{
				// Shutdown server
				oMainProcess.Shutdown();

				delete oGlobalContext.file_handle_cache;
				return iRC;
			}

			// Initialize handlers
			iRC = oMainProcess.InitHandlers();
			if (iRC != EX_OK)
			{
				// Shutdown server
				oMainProcess.Shutdown();

				delete oGlobalContext.file_handle_cache;
				return iRC;
			}

			// Main loop
			oMainProcess.Run();
		}

		// Shutdown server
		oMainProcess.Shutdown();
	}

	if (bLoggerStarted)
	{
		delete oGlobalContext.transfer_log;
		delete oGlobalContext.error_log;
	}

	delete oGlobalContext.file_handle_cache;

return EX_OK;
}
// End.

