/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "ConfigFileParser.hpp"
#include "ConfigHelper.hpp"
#include "FileUtils.hpp"
#include "Logger.hpp"
#include "MainConfig.hpp"
#include "SystemVars.hpp"
#include "UnicodeUtils.hpp"

namespace IRIS
{
/**
  @class ConfigIncludeCallback ConfigFileParser.hpp <ConfigFileParser.hpp>
  @brief Include file configuration callback
*/
class FileIncludeCallback:
  public ConfigIncludeCallback
{
public:
	/**
	  @brief Constructor
	*/
	FileIncludeCallback(STLW::vector<STLW::string>  & vIIncludeFiles,
	                    STLW::vector<STLW::string>  & vIConfigs);

	/**
	  @brief Include new file callback
	*/
	State IncludeFile(CCHAR_P szStart, CCHAR_P szEnd);

	/**
	  @brief Execute script and get output
	*/
	State ExecuteScript(CCHAR_P szStart, CCHAR_P szEnd);

	/**
	  @brief A destructor
	*/
	~FileIncludeCallback() throw();

private:
	/** List of include directorues */
	STLW::vector<STLW::string>  & vIncludeFiles;
	/** List of configurations      */
	STLW::vector<STLW::string>  & vConfigs;
};

//
// Constructor
//
FileIncludeCallback::FileIncludeCallback(STLW::vector<STLW::string>  & vIIncludeFiles,
                                         STLW::vector<STLW::string>  & vIConfigs): vIncludeFiles(vIIncludeFiles),
                                                                                   vConfigs(vIConfigs)
{
	;;
}

//
// Include new file callback
//
ConfigIncludeCallback::State FileIncludeCallback::IncludeFile(CCHAR_P szStart, CCHAR_P szEnd)
{
	const STLW::string sFileMask(szStart, szEnd - szStart);
	vIncludeFiles.push_back(sFileMask);

return OK;
}

//
// Include new file callback
//
ConfigIncludeCallback::State FileIncludeCallback::ExecuteScript(CCHAR_P szStart, CCHAR_P szEnd)
{
	const STLW::string sConfigFile(szStart, szEnd - szStart);
	vConfigs.push_back(sConfigFile);

return OK;
}

//
// A destructor
//
FileIncludeCallback::~FileIncludeCallback() throw()
{
	;;
}

//
// Parse bool flag
//
bool ConfigHelper::ParseBool(const STLW::string  & sFlag,
                             const bool            bDefaultValue)
{
	if (sFlag.empty()) { return bDefaultValue; }

	return ((Unicode::CompareIgnoreCase(sFlag.data(), sFlag.size(), "On", sizeof("On") - 1) == 0) ||
	        (Unicode::CompareIgnoreCase(sFlag.data(), sFlag.size(), "Yes", sizeof("Yes") - 1) == 0) ||
	        (Unicode::CompareIgnoreCase(sFlag.data(), sFlag.size(), "Enabled", sizeof("Enabled") - 1) == 0));
}

//
// Parse timeout
//
INT_64 ConfigHelper::ParseTimeout(const STLW::string  & sTimeout)
{
	if (sTimeout.empty()) { return -1; }

	// 1ms 1s 1min 1hr
	INT_64 iTimeout = 0;
	bool bParseOk = false;
	STLW::string::const_iterator itsTimeout = sTimeout.begin();
	while(itsTimeout != sTimeout.end())
	{
		const UCHAR_8 ucTMP = *itsTimeout;
		if (!(ucTMP >= '0' && ucTMP <= '9')) { break; }

		bParseOk = true;
		iTimeout = iTimeout * 10 + ucTMP - '0';
		++itsTimeout;
	}
	if (!bParseOk) { return -1; }

	if (itsTimeout == sTimeout.end()) { return iTimeout; }

	switch(*itsTimeout)
	{
		// ms min
		case 'm':
		case 'M':
			{
				++itsTimeout;
				if (itsTimeout != sTimeout.end())
				{
					if (*itsTimeout != 's' && *itsTimeout != 'S') { iTimeout *= 60 * 1000; }
				}
			}
			break;
		// hr
		case 'h':
		case 'H':
			iTimeout *= 3600 * 1000;
			break;
		// s
		case 's':
		case 'S':
			iTimeout *= 1000;
			break;
	}

return iTimeout;
}

//
// Parse size
//
INT_64 ConfigHelper::ParseSize(const STLW::string  & sSize)
{
	if (sSize.empty()) { return -1; }

	// 1k 1m 1g 1t
	INT_64 iSize = 0;
	bool bParseOk = false;
	STLW::string::const_iterator itsSize = sSize.begin();
	while(itsSize != sSize.end())
	{
		const UCHAR_8 ucTMP = *itsSize;
		if (!(ucTMP >= '0' && ucTMP <= '9')) { break; }

		bParseOk = true;
		iSize = iSize * 10 + ucTMP - '0';
		++itsSize;
	}
	if (!bParseOk) { return -1; }

	if (itsSize == sSize.end()) { return iSize; }

	switch(*itsSize)
	{
		case 't':
		case 'T':
			iSize <<= 10;
		case 'g':
		case 'G':
			iSize <<= 10;
		case 'm':
		case 'M':
			iSize <<= 10;
		case 'k':
		case 'K':
			iSize <<= 10;
			break;
	}

return iSize;
}

//
// Parse IPv6, IPv4 and local network address
//
ConfigHelper::State ConfigHelper::ParseAddress(const STLW::string        & sSource,
                                               TCPSocket::AddressFamily  & oAF,
                                               STLW::string              & sAddress,
                                               UINT_64                   & iPort)
{
	// inet:ipv4address:port
	// inet6:[ipv6address]:port
	// unix:/path/to/the/socket:mode
	size_t iAFPos = sSource.find(':');
	if (iAFPos == STLW::string::npos) { return CONFIG_ERROR; }

	if (Unicode::CompareIgnoreCase(sSource.data(), iAFPos, "inet6", sizeof("inet6") - 1) == 0)
	{
		++iAFPos;
		if (sSource[iAFPos] != '[') { return ERROR; }
		size_t iAddrPos = sSource.find(']', iAFPos + 1);
		if (iAddrPos == STLW::string::npos) { return CONFIG_ERROR; }
		++iAddrPos;
		if (sSource[iAddrPos] != ':') { return ERROR; }

		sAddress.assign(sSource, iAFPos + 1, iAddrPos - iAFPos - 2);
		iPort = atoi(sSource.data() + iAddrPos + 1);
		oAF = TCPSocket::INET6;
	}
	else if (Unicode::CompareIgnoreCase(sSource.data(), iAFPos, "inet", sizeof("inet") - 1) == 0)
	{
		size_t iAddrPos = sSource.find(':', iAFPos + 1);
		if (iAddrPos == STLW::string::npos) { return CONFIG_ERROR; }

		sAddress.assign(sSource, iAFPos + 1, iAddrPos - iAFPos - 1);
		iPort = atoi(sSource.data() + iAddrPos + 1);
		oAF = TCPSocket::INET;
	}
	else if (Unicode::CompareIgnoreCase(sSource.data(), iAFPos, "unix", sizeof("unix") - 1) == 0)
	{
		size_t iAddrPos = sSource.find(':', iAFPos + 1);
		if (iAddrPos == STLW::string::npos) { return CONFIG_ERROR; }

		sAddress.assign(sSource, iAFPos + 1, iAddrPos - iAFPos - 1);
		iPort = strtol(sSource.data() + iAddrPos + 1, (char **) NULL, 8);
		oAF = TCPSocket::UNIX;
	}
	else { return CONFIG_ERROR; }

return OK;
}

//
// Parse user and group name
//
ConfigHelper::State ConfigHelper::ParseUserGroup(const VariantNC  & oUserGroup,
                                                 STLW::string     & sUser,
                                                 INT_64           & iUID,
                                                 STLW::string     & sGroup,
                                                 INT_64           & iGID)
{
	// User and group
	// Not need if runs not from super-user
	iUID = iGID = 0;
	if (SystemVars::GetUID() != 0) { return NOT_NEED; }

	if (oUserGroup.Type() == VariantNC::STRING && oUserGroup.Size() != 0)
	{
		sUser.assign(oUserGroup);
	}
	else if (oUserGroup.Type() == VariantNC::ARRAY && oUserGroup.Size() == 2)
	{
		sUser.assign(oUserGroup[0]);
		sGroup.assign(oUserGroup[1]);
	}
	else
	{
		return CONFIG_ERROR;
	}

	SystemVars::SystemUser oSystemUser = SystemVars::GetSystemUserData(sUser.c_str());
	if (oSystemUser.uid == -1) { return NO_SUCH_USER; }
	iUID = oSystemUser.uid;
	iGID = oSystemUser.gid;

	if (sGroup.size())
	{
		iGID = SystemVars::GetGID(sGroup.c_str());
		if (iGID == -1) { return NO_SUCH_GROUP; }
	}
	else
	{
		sGroup = SystemVars::GetGroupNam(iGID);
	}

return OK;
}

//
// Parse configuration data
//
ConfigHelper::State ConfigHelper::ParseConfigData(const STLW::string          & sSource,
                                                  const STLW::string          & sConfigData,
                                                  STLW::vector<STLW::string>  & vIncludeFiles,
                                                  STLW::vector<STLW::string>  & vConfigs,
                                                  STLW::set<STLW::string>     & sSections,
                                                  VariantNC                   & oConfigData,
                                                  Logger                      & oLogger)
{
	FileIncludeCallback oFileIncludeCallback(vIncludeFiles, vConfigs);

	ConfigFileParser oParser(sSections);
	oParser.Init();
	INT_32 iRC = oParser.ParseData(sConfigData.data(), sConfigData.size(), oConfigData, &oFileIncludeCallback, true);
	oParser.Finish();

	if (iRC == -1)
	{
		oLogger.Emerg("Can't parse config from `%s`", sSource.c_str());
		return ERROR;
	}

return OK;
}

//
// Parse configuration file
//
ConfigHelper::State ConfigHelper::ParseConfigFile(const STLW::string          & sConfigFile,
                                                  STLW::vector<STLW::string>  & vIncludeFiles,
                                                  STLW::vector<STLW::string>  & vConfigs,
                                                  STLW::set<STLW::string>     & sSections,
                                                  VariantNC                   & oConfigData,
                                                  Logger                      & oLogger)
{
	const INT_64 iConfigFileHandle = File::Open(sConfigFile.c_str(), OpenMode::READ);
	if (iConfigFileHandle == -1)
	{
		oLogger.Emerg("Can't open `%s` for reading", sConfigFile.c_str());
		return ERROR;
	}

	File::Stat oConfigFileStat;
	INT_32 iRC = File::GetStat(iConfigFileHandle, oConfigFileStat);
	if (iRC == -1)
	{
		oLogger.Emerg("Can't stat `%s`", sConfigFile.c_str());
		return ERROR;
	}

	// Empty file
	if (oConfigFileStat.size == 0)
	{
		oLogger.Emerg("Config file `%s` is empty", sConfigFile.c_str());
		File::Close(iConfigFileHandle);
		return CONFIG_ERROR;
	}

	// Map file region to memory
	CCHAR_P vBuffer = (CCHAR_P)File::MapRegion(iConfigFileHandle, 0, oConfigFileStat.size);
	if (vBuffer == NULL)
	{
		oLogger.Emerg("Can't map config file `%s` to memory", sConfigFile.c_str());
		File::Close(iConfigFileHandle);
		return ERROR;
	}

	FileIncludeCallback oFileIncludeCallback(vIncludeFiles, vConfigs);

	ConfigFileParser oParser(sSections);
	oParser.Init();
	iRC = oParser.ParseData(vBuffer, oConfigFileStat.size, oConfigData, &oFileIncludeCallback, true);
	File::UnmapRegion((void*)vBuffer, 0, oConfigFileStat.size);
	oParser.Finish();
	File::Close(iConfigFileHandle);

	if (iRC == -1)
	{
		oLogger.Emerg("Can't parse config file `%s`", sConfigFile.c_str());
		return ERROR;
	}

return OK;
}

//
// Get list of directories from config file
//
void ConfigHelper::GetDirList(const VariantNC             & oVariantData,
                              STLW::vector<STLW::string>  & vDirList)
{
	if (oVariantData.Type() == VariantNC::STRING)
	{
		const STLW::string sData = oVariantData.Str();
		if (sData.size())
		{
			CHAR_P szRealPath = File::GetRealPath(sData.c_str(), NULL);
			if (szRealPath != NULL && *szRealPath) { vDirList.push_back(szRealPath); }
			free(szRealPath);
		}
	}
	else
	{
		for (UINT_64 iPos = 0; iPos < oVariantData.Size(); ++iPos)
		{
			const STLW::string sData = oVariantData[iPos].Str();
			if (sData.size())
			{
				CHAR_P szRealPath = File::GetRealPath(sData.c_str(), NULL);
				if (szRealPath != NULL && *szRealPath) { vDirList.push_back(szRealPath); }
				free(szRealPath);
			}
		}
	}
}

//
// Get list of strings from config file
//
void ConfigHelper::GetList(const VariantNC             & oVariantData,
                           STLW::vector<STLW::string>  & vStringList)
{
	if (oVariantData.Type() == VariantNC::STRING)
	{
		const STLW::string sData = oVariantData.Str();
		if (sData.size()) { vStringList.push_back(sData); }
	}
	else
	{
		for (UINT_64 iPos = 0; iPos < oVariantData.Size(); ++iPos)
		{
			const STLW::string sData = oVariantData[iPos].Str();
			if (sData.size()) { vStringList.push_back(sData); }
		}
	}
}

} // namespace IRIS
// End.
