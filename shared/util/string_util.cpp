#include "string_util.h"
#include "ConvertUTF.h"

// #include <stdint.h>
// #include <stdio.h>
// #include <vector>

#ifndef COUNT_OF
#define COUNT_OF(array)			(sizeof(array)/sizeof(array[0]))
#endif

std::wstring UTF8ToUTF16(const std::string& utf8)
{
	std::wstring utf16;
	UTF16 output[4096];
	const UTF8 *src_begin = (const UTF8*)utf8.c_str();
	const UTF8 *src_end = src_begin + utf8.size();
	UTF16 *dst_begin = output;

	utf16.clear();
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF8toUTF16(&src_begin,
			src_end,
			&dst_begin,
			dst_begin + COUNT_OF(output),
			lenientConversion);

		utf16.append((wchar_t*)output, dst_begin - output);
		dst_begin = output;
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf16.clear();
			break;
		}
	}
	return utf16;
}

std::string UTF16ToUTF8(const std::wstring utf16)
{
	std::string utf8;
	UTF8 output[8192];
	const UTF16 *src_begin = (const UTF16*)utf16.c_str();
	const UTF16 *src_end = src_begin + utf16.size();
	UTF8 *dst_begin = output;

	utf8.clear();
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF16toUTF8(&src_begin,
			src_end,
			&dst_begin,
			dst_begin + COUNT_OF(output),
			lenientConversion);

		utf8.append((char*)output, dst_begin - output);
		dst_begin = reinterpret_cast<UTF8 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf8.clear();
			break;
		}
	}
	return utf8;
}