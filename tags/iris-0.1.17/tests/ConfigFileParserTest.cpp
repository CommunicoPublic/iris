/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "ConfigHelper.hpp"
#include "ConfigFileParser.hpp"
#include "FileHandleCache.hpp"
#include "FileUtils.hpp"
#include "LoggerFile.hpp"

#include "Variant.hpp"
#include "VariantDumper.hpp"
#include "STLSstream.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

static STLW::string EscapeString(const STLW::string  & sSource)
{
	if (sSource.empty()) { return "\"\""; }
	STLW::string::const_iterator itsSource = sSource.begin();
	while (itsSource != sSource.end())
	{
		const UCHAR_8 ucTMP = *itsSource;
		if (!((ucTMP >= 'a' && ucTMP <= 'z') ||
		      (ucTMP >= 'A' && ucTMP <= 'Z') ||
		      (ucTMP >= '0' && ucTMP <= '9') ||
		       ucTMP == '.' || ucTMP == '_' || ucTMP == ':' || ucTMP == '*' ||
		       ucTMP == '[' || ucTMP == ']' || ucTMP == '\\' || ucTMP == '/' || ucTMP == '-'))
		{
			return '"' + sSource + '"';
		}
		++itsSource;
	}
return sSource;
}

int main(int argc, char ** argv)
{
	if (argc != 3 && argc != 2)
	{
		fprintf(stderr, "usage: %s config_file.conf [output file]\n", argv[0]);
		return EX_SOFTWARE;
	}

	FILE * FOUT = stdout;
	if (argc == 3)
	{
		FOUT = fopen(argv[2], "w");
		if (FOUT == NULL)
		{
			fprintf(stderr, "FATAL: Can't open `%s` for writing\n", argv[2]);
			return EX_SOFTWARE;
		}
	}

	LoggerFile oLogger(stderr);

	VariantNC oVariant;
	STLW::vector<STLW::string>  vIncludeFiles;
	STLW::set<STLW::string>     sSections;
	ConfigHelper::State eState = ConfigHelper::ParseConfigFile(argv[1], vIncludeFiles, sSections, oVariant, oLogger);
	if (eState != ConfigHelper::OK) { return EX_SOFTWARE; }

	STLW::stringstream sStream;

	VariantNC::ConstIterator itoSections = oVariant.Begin();
	while (itoSections != oVariant.End())
	{
		VariantNC::ConstIterator itoSectionId = itoSections -> second.Begin();
		while (itoSectionId != itoSections -> second.End())
		{
			sStream << itoSections -> first << " " << itoSectionId -> first << "\n{\n";
			VariantNC::ConstIterator itoPairs = itoSectionId -> second.Begin();
			UINT_64 iMaxKeyLength = 0;
			while (itoPairs != itoSectionId -> second.End())
			{
				if (iMaxKeyLength < itoPairs -> first.size()) { iMaxKeyLength = itoPairs -> first.size(); }
				++itoPairs;
			}
			iMaxKeyLength += 1;

			itoPairs = itoSectionId -> second.Begin();
			while (itoPairs != itoSectionId -> second.End())
			{
				sStream << '\t' << EscapeString(itoPairs -> first) << STLW::string(iMaxKeyLength - itoPairs -> first.size(), ' ');
				if (itoPairs -> second.Type() == VariantNC::STRING)
				{
					sStream << ' ' << EscapeString(itoPairs -> second.Str()) << ";\n";
				}
				else
				{
					UINT_64 iLineLength = itoPairs -> first.size();
					for (UINT_64 iPos = 0; iPos < itoPairs -> second.Size(); ++iPos)
					{
						iLineLength += itoPairs -> second[iPos].Size();
					}

					for (UINT_64 iPos = 0; iPos < itoPairs -> second.Size(); ++iPos)
					{
						if (iPos == 0 || iLineLength < 80)
						{
							sStream << ' ' << EscapeString(itoPairs -> second[iPos].Str());
							if (iLineLength >= 80 && iPos != itoPairs -> second.Size() - 1) { sStream << '\n'; }
						}
						else
						{
							sStream << '\t' << STLW::string(iMaxKeyLength + 1, ' ') << EscapeString(itoPairs -> second[iPos].Str());

							if (iPos != itoPairs -> second.Size() - 1) { sStream << '\n'; }
						}
					}
					sStream << ";\n";
				}
				++itoPairs;
			}

			sStream << "};\n\n";
			++itoSectionId;
		}
		++itoSections;
	}

	STLW::vector<STLW::string>::const_iterator itvIncludeFiles = vIncludeFiles.begin();
	while (itvIncludeFiles != vIncludeFiles.end())
	{
		sStream << "Include " << EscapeString(*itvIncludeFiles) << ";\n";
		++itvIncludeFiles;
	}
	sStream << "\n";

	fprintf(stdout, "%s", sStream.str().c_str());

	if (argc == 3) { fclose(FOUT); }

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
