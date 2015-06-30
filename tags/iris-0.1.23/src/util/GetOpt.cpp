/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "GetOpt.hpp"

namespace IRIS
{

//
// Compare strings.
//
CCHAR_P StrStr(CCHAR_P sX, CCHAR_P sY)
{
	while (*sX == *sY)
	{
		++sY; ++sX;
		if (*sX == '\0') { return sY; }
	}
	return NULL;
}

//
// Parse command-line options
//
INT_32 GetOpt(const Options   * aOptions,
              UINT_32         & iArgNum,
              const UINT_32     iMaxArgNum,
              CHAR_P          * aArgList,
              CCHAR_P         & szArgValue)
{
	szArgValue = NULL;
	// Error?
	if (aOptions == NULL || aArgList == NULL || iArgNum >= iMaxArgNum || aArgList[iArgNum] == NULL) { return -1; }

	// Not an option
	CHAR_P szOption = aArgList[iArgNum];
	if (*szOption != '-') { return '?'; }

	// Empty option
	++szOption;
	if (*szOption == '\0') { return '?'; }

	// Short option
	if (*szOption != '-')
	{
		while (aOptions -> short_opt != '\0')
		{
			// Match found
			if (aOptions -> short_opt == *szOption)
			{
				switch (aOptions -> has_arg)
				{
					case C_NO_ARG:
						++szOption;
						if (*szOption != '\0') { return '?'; }
						++iArgNum;
						return aOptions -> short_opt;

					case C_HAS_ARG:
						++szOption;
						++iArgNum;
						// Get argument
						if (*szOption != '\0')
						{
							szArgValue = szOption;
							return aOptions -> short_opt;
						}
						// Get next argument
						else
						{
							// Error
							if (iArgNum == iMaxArgNum) { return '?'; }
							szArgValue = aArgList[iArgNum];
							++iArgNum;
							return aOptions -> short_opt;
						}

					case C_OPT_ARG:
						++iArgNum;
						szArgValue = ++szOption;
						return aOptions -> short_opt;

					// Error?
					default:
						return '?';
				}
			}

			// Check next option
			++aOptions;
		}
	}
	// Long Option
	else
	{
		// Empty option
		++szOption;
		if (*szOption == '\0') { return '?'; }

		// Check options
		while (aOptions -> short_opt != '\0')
		{
			CCHAR_P szSubstring = StrStr(aOptions -> long_opt, szOption);
			if (szSubstring != NULL)
			{
				switch (aOptions -> has_arg)
				{
					case C_NO_ARG:
						if (*szSubstring != '\0') { return '?'; }
						++iArgNum;
						return aOptions -> short_opt;

					case C_HAS_ARG:
						++szOption;
						// Get argument
						if (*szSubstring == '=')
						{
							++iArgNum;
							++szSubstring;
							szArgValue = szSubstring;
							return aOptions -> short_opt;
						}
						// Get next argument
						else
						{
							// Error
							if (iArgNum + 1 == iMaxArgNum) { return '?'; }
							++iArgNum;
							szArgValue = aArgList[iArgNum];
							++iArgNum;
							return aOptions -> short_opt;
						}

					case C_OPT_ARG:
						// Check option
						if (*szSubstring == '=')
						{
							++iArgNum;
							++szSubstring;
							szArgValue = szSubstring;
							return aOptions -> short_opt;
						}
					// Error?
					default:
						return '?';
				}
			}

			// Check next option
			++aOptions;
		}
	}

return '?';
}

} // namespace CAS
// End.
