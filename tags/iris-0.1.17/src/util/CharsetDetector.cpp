/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "CharsetDetector.hpp"
#include "UnicodeUtils.hpp"

namespace MAIL // Charset detector
{

static UCHAR_8     UNK = 0x00;
static UCHAR_8     SPC = 0x20;

static UCHAR_8 a7BitUTF8Chars[] =
{
	 UNK, UNK,   UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK, 0x09, 0x0A,  UNK,  UNK, 0x0D,  UNK,  UNK,  // 0
	 UNK, UNK,   UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK, UNK,   UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,  // 2
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,  // 3
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  // 4
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,  // 5
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  // 6
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,  UNK,  // 7

	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 8
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 9
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // A
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // B
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // C
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // D
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // E
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK   // F
};

//
// Latin1 translation map
//
static UCHAR_8 aMapLatin1[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  // 4
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  // 6
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,  SPC,  SPC,  SPC,  SPC,  UNK,  // 7

	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 8
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 9
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // A
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // B
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // C
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // D
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // E
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK   // F
};

//
// CP1251 -> CP1251 translation map
//
static UCHAR_8 aMapCP1251[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 4
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 6
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 7

	0x80, 0x81,  SPC, 0x83,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC, 0x8A,  SPC, 0x8C, 0x8D, 0x8E, 0x8F,  // 8
	0x90,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC, 0x9A,  SPC,  SPC,  SPC,  SPC,  SPC,  // 9
	 SPC, 0xA1, 0xA2, 0xA3,  SPC,  SPC,  SPC,  SPC, 0xA8,  SPC, 0xAA,  SPC,  SPC,  SPC,  SPC, 0xAF,  // A
	 SPC,  SPC, 0xB2, 0xB3,  SPC, 0xB5,  SPC,  SPC, 0xB8,  SPC, 0xBA,  SPC, 0xBC, 0xBD, 0xBE, 0xBF,  // B
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,  // C
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,  // D
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,  // E
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF   // F
};

//
// CP855 -> CP1251 translation map
//
static UCHAR_8 aMapCP855[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 4
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 6
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 7

	0x90, 0x80, 0x83, 0x81, 0xB8, 0xA8, 0xBA, 0xAA, 0xBE, 0xBD, 0xB3, 0xB2, 0xBF, 0xAF, 0xBC, 0xA3,  // 8
	0x9A, 0x8A,  SPC, 0x8C,  SPC, 0x8E,  SPC, 0x8D, 0xA2, 0xA1,  SPC, 0x8F, 0x9E, 0xDE, 0xFA, 0xDA,  // 9
	0xE0, 0xC0, 0xE1, 0xC1, 0xF6, 0xD6, 0xE4, 0xC4, 0xE5, 0xC5, 0xF4, 0xD4, 0xE3, 0xC3,  SPC,  SPC,  // A
	 SPC,  SPC,  SPC,  SPC,  SPC, 0xF5, 0xD5, 0xE8, 0xC8,  SPC,  SPC,  SPC,  SPC, 0xE9, 0xC9,  SPC,  // B
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC, 0xEA, 0xCA,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // C
	0xEB, 0xCB, 0xEC, 0xCC, 0xED, 0xCD, 0xEE, 0xCE, 0xEF,  SPC,  SPC,  SPC,  SPC, 0xCF, 0xFF,  SPC,  // D
	0x9F, 0xF0, 0xD0, 0xF1, 0xD1, 0xF2, 0xD2, 0xF3, 0xD3, 0xE6, 0xC6, 0xE2, 0xC2, 0x9C, 0xDC,  SPC,  // E
	 SPC, 0xFB, 0xDB, 0xE7, 0xC7, 0xF8, 0xD8, 0xFD, 0x9D, 0xF9, 0xD9, 0xF7, 0xD7,  SPC,  SPC,  SPC   // F
};

//
// CP866 -> CP1251 translation map
//
static UCHAR_8 aMapCP866[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 4
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 6
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 7

	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,  // 8
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0x9D, 0xDE, 0x9F,  // 9
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,  // A
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // B
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // C
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // D
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0x9C, 0xFD, 0x9E, 0xFF,  // E
	0xA8, 0xB8, 0xAA, 0xBA, 0xAF, 0xBF, 0xA1, 0xA2,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC   // F

};

//
// KOI8-U -> CP1251 translation map
//
static UCHAR_8 aMapKOI8_U[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 4
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 6
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 7

	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 8
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 9
	 SPC,  SPC,  SPC, 0xB8, 0xBA,  SPC, 0xB3, 0xBF,  SPC,  SPC,  SPC,  SPC,  SPC, 0x20,  SPC,  SPC,  // A
	 SPC,  SPC,  SPC, 0xA8, 0xAA,  SPC, 0xB2, 0xAF,  SPC,  SPC,  SPC,  SPC,  SPC, 0x20,  SPC,  SPC,  // B
	0x9E, 0xE0, 0xE1, 0xF6, 0xE4, 0xE5, 0xF4, 0xE3, 0xF5, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE,  // C
	0xEF, 0xFF, 0xF0, 0xF1, 0xF2, 0xF3, 0xE6, 0xE2, 0x9C, 0xFB, 0xE7, 0xF8, 0xFD, 0xF9, 0xF7, 0xFA,  // D
	0xDE, 0xC0, 0xC1, 0xD6, 0xC4, 0xC5, 0xD4, 0xC3, 0xD5, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE,  // E
	0xCF, 0x9F, 0xD0, 0xD1, 0xD2, 0xD3, 0xC6, 0xC2, 0xDC, 0xDB, 0xC7, 0xD8, 0x9D, 0xD9, 0xD7, 0xDA   // F

};

//
// ISO-8859-5 -> CP1251 translation map
//
static UCHAR_8 aMapISO_8859_5[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 4
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 6
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 7

	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 8
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 9
	 SPC, 0xA8, 0x80, 0x81, 0xAA, 0xBD, 0xB2, 0xAF, 0xA3, 0x8A, 0x8C, 0x8E, 0x8D,  SPC, 0xA1, 0x8F,  // A
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,  // B
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0x9D, 0xDE, 0x9F,  // C
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,  // D
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0x9C, 0xFD, 0x9E, 0xFF,  // E
	 SPC, 0xB8, 0x90, 0x83, 0xBA, 0xBE, 0xB3, 0xBF, 0xBC, 0x9A,  SPC,  SPC,  SPC,  SPC, 0xA2,  SPC   // F

};
/*
//
// MACCYRILLIC -> CP1251 translation map
//
static UCHAR_8 aMapMacCyrillic[256] =
{
	// 0    1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  UNK,  UNK,  SPC,  UNK,  UNK,  // 0
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 1
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 2
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  // 3
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 4
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 5
	 SPC,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  // 6
	 UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  UNK,  SPC,  SPC,  SPC,  SPC,  SPC,  // 7

	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,  // 8
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0x9D, 0xDE, 0x9F,  // 9
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC, 0xB2,  SPC,  SPC,  SPC, 0x80, 0x90,  SPC, 0x81, 0x83,  // A
	 SPC,  SPC,  SPC,  SPC, 0xB3, 0xB5,  SPC, 0xA3, 0xAA, 0xBA, 0xAF, 0xBF, 0x8A, 0x9A, 0x8C,  SPC,  // B
	0xBC, 0xBD,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC, 0x8E,  SPC, 0x8D,  SPC, 0xBE,  // C
	 SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC,  SPC, 0xA1, 0xA2, 0x8F,  SPC,  SPC, 0xA8, 0xB8, 0xFF,  // D
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,  // E
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0x9C, 0xFD, 0x9E,  SPC   // F

};
*/
//
// Bigramms table
//
static UCHAR_8 aBigrammsTable[] =
{
	#include "BigrammsFull.cpp"
};

//
// Character set record
//
struct CharsetRec
{
	UCHAR_8  * charset_map;
	UCHAR_8  * table_bigramms;
	CCHAR_P    charset_name;
};

//
// Cyrillic character sets
//
CharsetRec aCyrRec[] =
{
	{ aMapCP1251,      aBigrammsTable, "CP1251" },
	{ aMapCP855,       aBigrammsTable, "CP855" },
	{ aMapCP866,       aBigrammsTable, "CP866" },
	{ aMapKOI8_U,      aBigrammsTable, "KOI8-U" },
	{ aMapISO_8859_5,  aBigrammsTable, "ISO-8859-5" },
//	{ aMapMacCyrillic, aBigrammsTable, "MACCYRILLIC" },
	{ NULL, NULL, NULL }
};

static void PrepareString(UCHAR_8        * aSource,
                          const UINT_32    iSrcLen,
                          UINT_32        & iSrcPos,

                          UCHAR_8        * aDest,
                          const UINT_32    iDstLen,
                          UINT_32        & iDstPos,

                          UCHAR_8          chPrevChar,
                          UCHAR_8        * aMap)
{
	for (; iSrcPos < iSrcLen; ++iSrcPos)
	{
		const UCHAR_8 chCurrChar = aMap[ aSource[iSrcPos] ];
		if (chCurrChar != UNK && chCurrChar != SPC)
		{
			if (chPrevChar == SPC || chPrevChar == UNK)
			{
				aDest[iDstPos++] = SPC;
			}

			aDest[iDstPos++] = chCurrChar;
		}
		chPrevChar = chCurrChar;

		if (iDstPos >= iDstLen) { break; }
	}
}

//
// Detect Latin1 charset
//
CCHAR_P CharsetDetector::DetectCharsetLatin1(CCHAR_P szString, const UINT_32 iStringLen)
{
	// Okay, let's check charsets
	for (UINT_32 iPos = 0; iPos < iStringLen; ++iPos)
	{
		const UCHAR_8 chTMP = szString[iPos];
		const UCHAR_8 chCurrChar = aMapLatin1[ chTMP ];

		if (chCurrChar == UNK) { return NULL; }
	}

return "LATIN1";
}

//
// Detect set of cyrillic charsets (CP1251, KOI8-U, ISO-8859-5, CP855, CP866, MACCYRILLIC)
//
CCHAR_P CharsetDetector::DetectCharsetCyr(CCHAR_P szString, const UINT_32 iStringLen)
{
	CharsetRec  * pRec             = aCyrRec;
	CharsetRec  * pMaxScore        = NULL;
	W_FLOAT       dPrevScore       = 0;
	bool          bFirstIteration  = true;

	while (pRec -> charset_map != NULL)
	{
		UINT_32 iNegativeSequenceCount = 0;
		UINT_32 iPositiveSequenceCount = 0;
		UINT_32 iSequenceCount         = 0;

		UCHAR_8 aBuffer[1024 + 2];
		UCHAR_8 chPrevChar = ' ';
		UINT_32 iStringPos = 0;

		for (;;)
		{
			UINT_32 iDstPos = 0;
			PrepareString((UCHAR_8 *)szString, iStringLen, iStringPos,
			              aBuffer,             1024,       iDstPos,
			              chPrevChar,
			              pRec -> charset_map);
			if (iDstPos == 0) { break; }

			iSequenceCount += iDstPos;

			// Okay, let's check charsets
			for (UINT_32 iPos = 0; iPos < iDstPos; ++iPos)
			{
				const UCHAR_8 chCurrChar = aBuffer[iPos];
				UINT_32 iValue = pRec -> table_bigramms[chPrevChar * 256 + chCurrChar];

				if (iValue == 0) { ++iNegativeSequenceCount; }
				else             { ++iPositiveSequenceCount; }
				chPrevChar = chCurrChar;
			}
		}
		W_FLOAT dScore = 1.0 * iSequenceCount + iPositiveSequenceCount - iNegativeSequenceCount;

		if (bFirstIteration)
		{
			dPrevScore = dScore;
			pMaxScore  = pRec;
			bFirstIteration = false;
		}

		if (dScore > dPrevScore)
		{
			dPrevScore = dScore;
			pMaxScore  = pRec;
		}

		++pRec;
	}

	if (pMaxScore == NULL) { return NULL; }

return pMaxScore -> charset_name;
}

//
// Detect UTF-8 charset
//
CCHAR_P CharsetDetector::DetectCharsetUTF8(CCHAR_P szString, const UINT_32 iStringLen)
{
	UINT_32 iBytesLeft = iStringLen;
	UINT_32 iBrokenChars = 0;
	UINT_32 iUTF8Chars   = 0;
	for(;;)
	{
		UINT_32 iUCS = 0;
		INT_32 iCharLen = Unicode::UTF8ToWide(szString, iBytesLeft, iUCS);
		if (iCharLen < 0)
		{
			++iBrokenChars;
			++szString;
			iCharLen = 1;
		}
		// Ignore LATIN1 subset
		else if (iCharLen == 1)
		{
			if (a7BitUTF8Chars[iUCS] != UNK) { ++iUTF8Chars;   }
			else                             { ++iBrokenChars; }
		}
		else if (iCharLen > 1) { ++iUTF8Chars; }

		iBytesLeft -= iCharLen;

		if (iBytesLeft == 0) { break; }
	}

	if (iBrokenChars > iUTF8Chars) { return NULL; }

return "UTF-8";
}

//
// Detect charset
//
CCHAR_P CharsetDetector::DetectCharset(CCHAR_P szString, const UINT_32 iStringLen)
{
	// Check first, because Latin1 is subset of UTF-8
	CCHAR_P szCharsetName = DetectCharsetLatin1(szString, iStringLen);
	if (szCharsetName != NULL) { return szCharsetName; }

	// Check UTF-8
	szCharsetName = DetectCharsetUTF8(szString, iStringLen);
	if (szCharsetName != NULL) { return szCharsetName; }

	// Check cyrillic charsets
	szCharsetName = DetectCharsetCyr(szString, iStringLen);
	if (szCharsetName != NULL) { return szCharsetName; }

return "UTF-8";
}

} // namespace IRIS
// End.
