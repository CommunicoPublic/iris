/*
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

%%{
machine JSON_Parser;
alphtype unsigned char;

action SYNTAX_ERROR
{
	fprintf(stderr, "SYNTAX_ERROR %s", p);
}

action TRUE_END
{
	pCurrVal -> operator=(INT_64(1));
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
}

action FALSE_END
{
	pCurrVal -> operator=(INT_64(0));
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
}

action NULL_END
{
	pCurrVal -> operator=(T());
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
}

action NUMBER_START
{
	oStringBuffer.Commit();
	sTMP.erase();
	bFoundFrac = false;
}

action NUMBER_FOUND_REAL { bFoundFrac = true; }

action NUMBER { oStringBuffer.Store(fc); }

action NUMBER_END
{
	oStringBuffer.Commit();
	if (bFoundFrac) { pCurrVal -> operator=(W_FLOAT(atof(sTMP.c_str()))); }
	else            { pCurrVal -> operator=(INT_64(atoll(sTMP.c_str()))); }
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
}

action UNESCAPE_CHAR
{
	switch(fc)
	{
		case '\'':
			oStringBuffer.Store('\'');
			break;
		case '"':
			oStringBuffer.Store('"');
			break;
		case '\\':
			oStringBuffer.Store('\\');
			break;
		case '/':
			oStringBuffer.Store('/');
			break;
		case 'b':
			oStringBuffer.Store('\b');
			break;
		case 'f':
			oStringBuffer.Store('\f');
			break;
		case 'n':
			oStringBuffer.Store('\n');
			break;
		case 'r':
			oStringBuffer.Store('\r');
			break;
		case 't':
			oStringBuffer.Store('\t');
			break;
		case 'v':
			oStringBuffer.Store('\v');
			break;
	}
}

action UNESCAPE_XDIGIT_START { iXDigit = 0; }

action UNESCAPE_XDIGIT
{
	iXDigit <<= 4;
	if      (fc >= '0' && fc <= '9') { iXDigit += fc - '0'; }
	else if (fc >= 'a' && fc <= 'f') { iXDigit += fc - 'a' + 10; }
	else if (fc >= 'A' && fc <= 'F') { iXDigit += fc - 'A' + 10; }
}

action UNESCAPE_XDIGIT_END
{
	// Convert From UCS to UTF-8
	CHAR_8 sBuffer[10];
	CHAR_P sTMP = sBuffer;
	const INT_32 iCharLen = Unicode::WideToUTF8(iXDigit, sTMP);
	if (iCharLen > 0) { oStringBuffer.Store(sBuffer, iCharLen); }
}

action CHAR { oStringBuffer.Store(fc); }

action STRING_START
{
	oStringBuffer.Commit();
	sTMP.erase();
}

action KEY_END
{
	oStringBuffer.Commit();
}

action VALUE_STRING_END
{
	oStringBuffer.Commit();
	pCurrVal -> operator=(sTMP);
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
}

action MAP_VALUE_START
{
	T * pTMP = &(pCurrVal -> operator[](sTMP));
	vStack.push_back(pCurrVal);
	pCurrVal = pTMP;
}

action ARRAY_VALUE_START
{
	pCurrVal -> PushBack(T());
	T * pTMP = &(pCurrVal -> operator[](pCurrVal -> Size() - 1));

	vStack.push_back(pCurrVal);
	pCurrVal = pTMP;
}

action ARRAY_END
{
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
	stack.pop_back();
	fret;
}

action MAP_END
{
	pCurrVal = *vStack.rbegin();
	vStack.pop_back();
	stack.pop_back();
	fret;
}

WSP               = [ \t\r\n]*;

TRUE              = 'true'  @TRUE_END;
FALSE             = 'false' @FALSE_END;
NULL              = 'null'  @NULL_END;

INTEGER           = '0' | ([1-9] digit**);
EXPONENT          = ([eE] [\-+]? digit++) >NUMBER_FOUND_REAL;
FRACTIONAL        = ('.' digit++) >NUMBER_FOUND_REAL;
NUMBER            = (('-'? INTEGER FRACTIONAL? EXPONENT?) >NUMBER_START $NUMBER) %NUMBER_END;

UNESCAPED_CHARS   = (0x20 | 0x21 | (0x23 .. 0x5b) | (0x5d .. 0xFF)) @CHAR;

ESCAPED_CHAR      = ( '\"' | '\\' | '/' | 'b' | 'f' | 'n' | 'r' | 't' | 'v') @UNESCAPE_CHAR;
ESCAPED_XDIGIT    = ('u' xdigit{4}) >UNESCAPE_XDIGIT_START $UNESCAPE_XDIGIT %UNESCAPE_XDIGIT_END;
ESCAPED_CHARS     = '\\' (ESCAPED_CHAR | ESCAPED_XDIGIT);

CHARS             = UNESCAPED_CHARS | ESCAPED_CHARS;

STRING            = '"' @STRING_START CHARS** '"';
KEY               = '"' @STRING_START CHARS** '"' @KEY_END;

VALUE_STRING      = STRING %VALUE_STRING_END;

ARRAY             = '[' @{ stack.push_back(0); fcall ARRAY_ENTRY; };
MAP               = '{' @{ stack.push_back(0); fcall MAP_ENTRY; };

VALUE             = (TRUE | FALSE | NULL | NUMBER | VALUE_STRING | ARRAY | MAP);
VALUES            = VALUE >ARRAY_VALUE_START (WSP ',' WSP VALUE >ARRAY_VALUE_START)*;

PAIR              = (KEY WSP ':' WSP VALUE >MAP_VALUE_START);
PAIRS             = PAIR (WSP ',' WSP PAIR)*;

ARRAY_ENTRY      := (WSP VALUES? WSP ']' @ARRAY_END);
MAP_ENTRY        := (WSP PAIRS? WSP '}' @MAP_END);

main             := (VALUE);

}%%

%% write data;

//
// Constructor
//
template <typename T>JSONParser<T>::JSONParser(T  & oIResult): oResult(oIResult), oStringBuffer(sTMP)
{
	pCurrVal = &oResult;
	vStack.push_back(pCurrVal);
}

//
// Initialize parser
//
template <typename T>void JSONParser<T>::Init()
{
	%% write init;
}

//
// Parse data
//
template <typename T>INT_32 JSONParser<T>::Parse(CCHAR_P        szString,
                                                 const UINT_64  iStringLen)
{
	Init();

return ParseChunk(szString, iStringLen, true);
}

//
// Parse data
//
template <typename T>INT_32 JSONParser<T>::ParseChunk(CCHAR_P        szString,
                                                      const UINT_64  iStringLen,
                                                      const bool     bIsLastChunk)
{
	const char *p   = szString;
	const char *pe  = szString + iStringLen;
	const char *eof = bIsLastChunk ? pe : 0;

	%% write exec;

	if ( cs == JSON_Parser_error )
	{
		fprintf(stderr, "JSON_Parser_error: p = %s", p);
		return -1;
	}
	if ( cs >= JSON_Parser_first_final ) { return 1; }

return 0;
}

//
// Destructor
//
template <typename T>JSONParser<T>::~JSONParser() throw()
{
	;;
}
// End.
