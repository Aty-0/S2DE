#pragma once
#define S2DE_BUILD_YEAR_0 (__DATE__[ 7])
#define S2DE_BUILD_YEAR_1 (__DATE__[ 8])
#define S2DE_BUILD_YEAR_2 (__DATE__[ 9])
#define S2DE_BUILD_YEAR_3 (__DATE__[10])


#define S2DE_BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define S2DE_BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define S2DE_BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define S2DE_BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define S2DE_BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define S2DE_BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define S2DE_BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define S2DE_BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define S2DE_BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define S2DE_BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define S2DE_BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define S2DE_BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define S2DE_BUILD_MONTH_0 \
    ((S2DE_BUILD_MONTH_IS_OCT || S2DE_BUILD_MONTH_IS_NOV || S2DE_BUILD_MONTH_IS_DEC) ? '1' : '0')

#define S2DE_BUILD_MONTH_1 \
    ( \
        (S2DE_BUILD_MONTH_IS_JAN) ? '1' : \
        (S2DE_BUILD_MONTH_IS_FEB) ? '2' : \
        (S2DE_BUILD_MONTH_IS_MAR) ? '3' : \
        (S2DE_BUILD_MONTH_IS_APR) ? '4' : \
        (S2DE_BUILD_MONTH_IS_MAY) ? '5' : \
        (S2DE_BUILD_MONTH_IS_JUN) ? '6' : \
        (S2DE_BUILD_MONTH_IS_JUL) ? '7' : \
        (S2DE_BUILD_MONTH_IS_AUG) ? '8' : \
        (S2DE_BUILD_MONTH_IS_SEP) ? '9' : \
        (S2DE_BUILD_MONTH_IS_OCT) ? '0' : \
        (S2DE_BUILD_MONTH_IS_NOV) ? '1' : \
        (S2DE_BUILD_MONTH_IS_DEC) ? '2' : \
        /* error default */    '?' \
    )

#define S2DE_BUILD_DAY_0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define S2DE_BUILD_DAY_1 (__DATE__[ 5])

#define S2DE_BUILD_VER '0'

namespace S2DE::Core
{
	static const wchar_t S2DE_BUILD_DATE_W[]
	{
		(wchar_t)S2DE_BUILD_YEAR_0, (wchar_t)S2DE_BUILD_YEAR_1, (wchar_t)S2DE_BUILD_YEAR_2, (wchar_t)S2DE_BUILD_YEAR_3, (wchar_t)S2DE_BUILD_MONTH_0, (wchar_t)S2DE_BUILD_MONTH_1, (wchar_t)S2DE_BUILD_DAY_0, (wchar_t)S2DE_BUILD_DAY_1, '.', (wchar_t)S2DE_BUILD_VER,  L'\0'
	};

	static const char S2DE_BUILD_DATE[]
	{
		S2DE_BUILD_YEAR_0, S2DE_BUILD_YEAR_1, S2DE_BUILD_YEAR_2, S2DE_BUILD_YEAR_3, S2DE_BUILD_MONTH_0, S2DE_BUILD_MONTH_1, S2DE_BUILD_DAY_0, S2DE_BUILD_DAY_1, '.', S2DE_BUILD_VER, '\0'
	};
}