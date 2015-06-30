/*
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "ConfigFileParser.hpp"

#include <string.h>
#include <time.h>

namespace IRIS
{

%%{
machine ConfigFileParserScanner;

CR             = '\r';
LF             = '\n';
CRLF           = (CR ? LF);
SP             = ' ';
TAB            = '\t';

action STRING_START
{
	sStartPos = p;
}

action STRING_END
{
	sEndPos = p;
}

action SECTION_NAME_END
{
	sSectionName.assign(sStartPos, p - sStartPos);
}

action SECTION_ID_END
{
	sSectionId.assign(sStartPos, p - sStartPos);

	if (sSections.find(sSectionName + "/" + sSectionId) != sSections.end())
	{
		fprintf(stderr, "Duplicate section found: %s/%s\n", sSectionName.c_str(), sSectionId.c_str());
		return -1;
	}
}

action KEY_END
{
	sKey.assign(sStartPos, sEndPos - sStartPos);
}

action VALUE_END
{
	sValue.assign(sStartPos, sEndPos - sStartPos);

	VariantNC & oSection = oVariant[sSectionName][sSectionId];

	VariantNC & oValue = oSection[sKey];
	if (oValue.Size() == 0) { oValue = sValue; }
	else
	{
		if (oValue.Type() == VariantNC::STRING)
		{
			STLW::string sTMP = oValue.Str();
			oValue.PushBack(sTMP);
		}

		oValue.PushBack(sValue);
	}
}

action SYNTAX_ERROR
{
	fprintf(stderr, "Syntax error at: `%.*s`\n", INT_32(pe - p), p);
}

action SECTION_END
{
	sSections.insert(sSectionName + "/" + sSectionId);
}

action INCLUDE_FILE_END
{
	pCallback -> IncludeFile(sStartPos, sEndPos);
}

action EXECUTE_END
{
	pCallback -> ExecuteScript(sStartPos, sEndPos);
}

# Single Quote.
SINGLE_QUOTED_STRING = ("'" ( [^'\\\n] | /\\./ )* >STRING_START %STRING_END "'" );
#"
DOUBLE_QUOTED_STRING = ('"' ( [^"\\\n] | /\\./ )* >STRING_START %STRING_END '"' );
#'
UNQUOTED_STRING      = (([a-zA-Z0-9._:*\[\]/\\\-])+) >STRING_START %STRING_END;
#
STRING               = UNQUOTED_STRING | SINGLE_QUOTED_STRING | DOUBLE_QUOTED_STRING;

SECTION_NAME         = (([a-zA-Z0-9_])+) >STRING_START %SECTION_NAME_END;
SECTION_ID           = (([a-zA-Z0-9_])+) >STRING_START %SECTION_ID_END;

COMMENT              = ('#' [^\n]* LF);
WSP                  = ([ \r\t] | LF | COMMENT)+;

KEY                  = STRING >STRING_START %KEY_END;
VALUE                = STRING >STRING_START %VALUE_END;
VALUES               = VALUE (WSP+ VALUE)*;
PAIR                 = WSP* KEY WSP+ VALUES WSP* ';' WSP*;

INCLUDE_FILE         = STRING >STRING_START %INCLUDE_FILE_END;
INCLUDE_FILES        = INCLUDE_FILE (WSP+ INCLUDE_FILE)*;
INCLUDE              = WSP* 'include'i WSP+ INCLUDE_FILES WSP* ';' WSP*;

EXECUTE_FILE         = STRING >STRING_START %EXECUTE_END;
EXECUTE_FILES        = EXECUTE_FILE (WSP+ EXECUTE_FILE)*;
EXECUTE              = WSP* 'execute'i WSP+ EXECUTE_FILES WSP* ';' WSP*;

SECTION              = ((INCLUDE) | (EXECUTE)) | ((SECTION_NAME WSP+ SECTION_ID WSP* '{' PAIR* '}' WSP* ';' WSP*) %SECTION_END);

SECTIONS             = SECTION*;

main := WSP* (SECTIONS)? WSP* $!SYNTAX_ERROR;

}%%

%% write data;

//
// Virtual destructor
//
ConfigIncludeCallback::~ConfigIncludeCallback() throw() { ;; }

//
// Constructor
//
ConfigFileParser::ConfigFileParser(STLW::set<STLW::string>  & sISections): sSections(sISections)
{
	;;
}

//
// Initialize parser
//
INT_32 ConfigFileParser::Init()
{
	%% write init;

return 0;
}

INT_32 ConfigFileParser::ParseData(CCHAR_P                  szString,
                                   const UINT_32            iStringLen,
                                   VariantNC              & oVariant,
                                   ConfigIncludeCallback  * pCallback,
                                   const bool               bIsLastChunk)
{
	const char *p  = szString;
	const char *pe = szString + iStringLen;
	const char *eof = bIsLastChunk ? pe : 0;

	%% write exec;

	if ( cs == ConfigFileParserScanner_error )
	{
		fprintf(stderr, "SettingsScanner_error: p = %s", p);
		return -1;
	}
	if ( cs >= ConfigFileParserScanner_first_final ) { return 1; }

return 0;
}

//
// Finish parsing
//
INT_32 ConfigFileParser::Finish()
{
	if ( cs == ConfigFileParserScanner_error ) { return -1; }
	if ( cs >= ConfigFileParserScanner_first_final ) { return 1; }

return 0;
}

//
// Destructor
//
ConfigFileParser::~ConfigFileParser() throw()
{
	;;
}

} // namespace IRIS
// End.
