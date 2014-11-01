/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CONFIG_FILE_PARSER_HPP__
#define _CONFIG_FILE_PARSER_HPP__ 1

#include "DataBuffer.hpp"
#include "Variant.hpp"
#include "STLSet.hpp"

namespace IRIS
{
/**
  @class ConfigIncludeCallback ConfigFileParser.hpp <ConfigFileParser.hpp>
  @brief Include file configuration callback
*/
class ConfigIncludeCallback
{
public:
	enum State { OK, ERROR };
	/**
	  @brief Include new file callback
	*/
	virtual State IncludeFile(CCHAR_P szStart, CCHAR_P szEnd) = 0;

	/**
	  @brief Virtual destructor
	*/
	virtual ~ConfigIncludeCallback() throw();
};

/**
  @class ConfigFileParser ConfigFileParser.hpp <ConfigFileParser.hpp>
  @brief Main configuration file parser
*/
class ConfigFileParser
{
public:
	/**
	  @brief Constructor
	*/
	ConfigFileParser(STLW::set<STLW::string>  & sISections);

	/**
	  @brief Init parser
	*/
	INT_32 Init();

	/**
	  @brief Parse chunk of data
	  @param szString - source data
	  @param iStringLen - source data length
	  @param oVariant - parsed configuration [out]
	  @param bIsLastChunk - true, if this is last chunk of data
	*/
	INT_32 ParseData(CCHAR_P                  szString,
	                 const UINT_32            iStringLen,
	                 VariantNC              & oVariant,
	                 ConfigIncludeCallback  * pCallback,
	                 const bool               bIsLastChunk);

	/**
	  @brief Finish parser
	*/
	INT_32 Finish();

	/**
	  @brief A destructor
	*/
	~ConfigFileParser() throw();

protected:
	// Does not exist
	ConfigFileParser(const ConfigFileParser & oRhs);
	ConfigFileParser &operator=(const ConfigFileParser & oRhs);

	/** Start token position      */
	CCHAR_P              sStartPos;
	/** End token position        */
	CCHAR_P              sEndPos;

	/** Name of section of config */
	STLW::string         sSectionName;
	/** Section config Id         */
	STLW::string         sSectionId;
	/** Configuration key name    */
	STLW::string         sKey;
	/** Configuration value       */
	STLW::string         sValue;
	/** Temp. data buffer         */
	DataBuffer           oData;
	/** Key name                  */
	DataBuffer           oKey;
	/** Subkey name               */
	DataBuffer           oValue;
	/** Set of processed  sections      */
	STLW::set<STLW::string>  & sSections;
	/** Ragel variables                 */
	int cs, top, stack[1];
};

} // namespace IRIS
#endif // _CONFIG_FILE_PARSER_HPP__
// End.
