/************************************************************************/
/* CharacterUtil.c                                                      */
/*                                                                      */
/* Unicode/GB2312(汉字)字符编码转换函数实现文件                         */
/*                                                                      */
/* Version   Author    Date           Modification                      */
/* ---------------------------------------------------------------------*/
/*  v0.1a    Matthew   07/11/29       Create                            */
/*                                                                      */
/************************************************************************/
#include "codecutil.h"
#include <string.h>

namespace CodecUtil
{
#define UNSUPPORTED		-1

#define HZCHAR_NUM		6763
#define SYMBOL_NUM		717
#define CODE_NUM		(HZCHAR_NUM+SYMBOL_NUM)


	/* extern convert table. see 'CharacterData.c' */
	extern const unsigned short datUnicode[CODE_NUM];
	extern const unsigned short GbkUnicode[CODE_NUM];
	extern const unsigned short datGbkcode[CODE_NUM];
	extern const unsigned short UnicodeGbk[CODE_NUM];

#ifdef __cplusplus
	extern "C" {
#endif
		/************************************************************************/
		/* LOCAL FUNCTIONS                                                      */
		/************************************************************************/


		/*************************************************************************
		*/
		static short SearchCode(const unsigned short* table,
			unsigned short num, unsigned short code)
		{
			unsigned short head, tail, middle;

			head = 0;
			tail = num-1;
			if( (code < table[head]) || (code > table[tail]) )
				return UNSUPPORTED;

			while( head <= tail )
			{
				middle = (head+tail)/2;
				if( code==table[middle] )
					return (middle);
				else if( code > table[middle] )
					head = middle+1;
				else
					tail = middle-1;
			}

			return UNSUPPORTED;
		}

		/*************************************************************************
		*/
		static int GetGbkcodeLength(const unsigned char* gstr)
		{
			int length=0;

			while( *gstr )
			{
				if( (*gstr) & 0x80 )
					gstr += 2;
				else
					gstr += 1;

				length++;
			}

			return length;
		}

		/*************************************************************************
		*/
		static int GetUnicodeLength(const unsigned short* ustr)
		{
			int length=0;

			while( *ustr )
			{
				if( (*ustr) >= 0x80 )
					length += 2;
				else
					length += 1;

				ustr++;
			}

			return length;
		}

		/*************************************************************************
		*/
		static unsigned short Gbkcode2Unicode(unsigned short gc)
		{
			int index = SearchCode(datGbkcode, CODE_NUM, gc);
			if( index==UNSUPPORTED )
			{
				return '?';
			}
			else
			{
				return UnicodeGbk[index];
			}
		}

		/*************************************************************************
		*/
		static unsigned short Unicode2Gbkcode(unsigned short uc)
		{
			int index = SearchCode(datUnicode, CODE_NUM, uc);
			if( index==UNSUPPORTED )
			{
				return 0x3F3F;
			}
			else
			{
				return GbkUnicode[index];
			}
		}

		/*************************************************************************
		*/
		static unsigned char WordHelper(unsigned short uc, unsigned char ho, unsigned char lo)
		{
			unsigned short mask;
			unsigned short interval;

			interval = ho - lo + 1;
			if( interval==16 )
			{
				return (unsigned char)uc;
			}

			uc  >>= lo;
			mask  = 1 << interval;
			uc   &= (mask-1);

			return (unsigned char)uc;
		}






		/************************************************************************/
		/* EXTREN FUNCTIONS                                                     */
		/************************************************************************/

		/*************************************************************************
		* ConvertGBKtoUCS2 - GBK编码转换为UCS2编码
		*
		* PARAM
		*   i_gstr    : 输入需要转换的GBK码地址
		*   o_ustr    : 输出转换后的Unicode码缓冲区地址
		*   i_ustrlen : 缓冲区长度
		*
		* RETURNS: 转换的编码个数
		*/
		int ConvertGBKtoUCS2(const unsigned char* i_gstr, unsigned short* o_ustr, int i_ustrlen)
		{
			int length, i;

			length = GetGbkcodeLength(i_gstr);
			if( !i_ustrlen || i_ustrlen < length )
			{
				return 0;
			}

			memset(o_ustr, 0, i_ustrlen);

			for(i=0; i<length; i++)
			{
				if( *i_gstr & 0x80 )
				{
					unsigned short gc = (i_gstr[0]<<8) + i_gstr[1];
					o_ustr[i] = Gbkcode2Unicode(gc);
					i_gstr += 2;
				}
				else
				{
					o_ustr[i] = *i_gstr;
					i_gstr++;
				}
			}

			return length;
		}

		/*************************************************************************
		* ConvertUCS2toGBK - UCS2编码转换为GBK编码
		*
		* PARAM
		*   i_ustr    : 输入需要转换的Unicode码地址
		*   o_gstr    : 输出转换后的GBK码缓冲区地址
		*   i_gstrlen : 缓冲区长度
		*
		* RETURNS: 转换的编码个数
		*/
		int ConvertUCS2toGBK(const unsigned short* i_ustr, unsigned char* o_gstr, int i_gstrlen)
		{
			int length, i;

			length = GetUnicodeLength(i_ustr);
			if( !i_gstrlen || i_gstrlen < length )
			{
				return 0;
			}

			memset(o_gstr, 0, i_gstrlen);

			for(i=0; i<length;)
			{
				if( *i_ustr >= 0x80 )
				{
					unsigned short uc = Unicode2Gbkcode(*i_ustr);
					o_gstr[i++] = (unsigned char)(uc >> 8);
					o_gstr[i++] = (unsigned char)(uc & 0xFF);
				}
				else
				{
					o_gstr[i++] = (unsigned char)(*i_ustr);
				}

				i_ustr++;
			}

			return length;
		}

		/*************************************************************************
		* ConvertUCS2toUTF8 - UCS2编码转换为UTF-8编码
		*
		* PARAM
		*   i_ucs    : 输入需要转换的Unicode码地址
		*   o_utf    : 输出转换后的UTF-8码缓冲区地址
		*   i_utflen : 缓冲区长度
		*
		* RETURNS: 转换的编码个数
		*
		**************************************************************************
		* 0000 - 007F 0xxxxxxx
		* 0080 - 07FF 110xxxxx 10xxxxxx
		* 0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
		*/

		int ConvertUCS2toUTF8(const unsigned short* i_ucs, unsigned char* o_utf, int i_utflen)
		{
			int utflength, ucslength;
			int maxlength = i_utflen - 3;
			unsigned short uc;

			memset(o_utf, 0, i_utflen);

			for(uc=i_ucs[0],utflength=0,ucslength=1; uc; ucslength++)
			{
				if( uc <= 0x7F )
				{
					o_utf[utflength++] = (unsigned char)uc;
				}
				else if( uc <= 0x7FF )
				{
					o_utf[utflength++] = 0xC0 | WordHelper(uc, 10, 6);
					o_utf[utflength++] = 0x80 | WordHelper(uc, 5,  0);
				}
				else
				{
					o_utf[utflength++] = 0xE0 | WordHelper(uc, 15, 12);
					o_utf[utflength++] = 0x80 | WordHelper(uc, 11, 6);
					o_utf[utflength++] = 0x80 | WordHelper(uc, 5,  0);
				}

				if( utflength > maxlength )
					break;

				uc = i_ucs[ucslength];
			}

			return utflength;
		}

		/*************************************************************************
		* ConvertUTF8toUCS2 - UTF-8编码转换为UCS2编码
		*
		* PARAM
		*   i_utf    : 输入需要转换的UTF-8码地址
		*   o_ucs    : 输出转换后的Unicode码缓冲区地址
		*   i_ucslen : 缓冲区长度
		*
		* RETURNS: 转换的编码个数
		*/
		int ConvertUTF8toUCS2(const unsigned char* i_utf, int i_utflen, unsigned short* o_ucs, int i_ucslen)
		{
			int utflength, ucslength;
			unsigned char uc;

			memset(o_ucs, 0, i_ucslen);

			for(uc=i_utf[0],utflength=1,ucslength=0; uc;)
			{
				if( (uc & 0x80)==0 )
				{
					o_ucs[ucslength] = uc;
				}
				else if( (uc & 0xE0)==0xC0 )
				{
					o_ucs[ucslength]  = (uc & 0x1F) << 6;

					uc = i_utf[utflength++];
					o_ucs[ucslength] |= (uc & 0x3F);
				}
				else if( (uc & 0xF0)==0xE0 )
				{
					o_ucs[ucslength]  = uc << 12;

					uc = i_utf[utflength++];
					o_ucs[ucslength] |= (uc & 0x3F) << 6;

					uc = i_utf[utflength++];
					o_ucs[ucslength] |= (uc & 0x3F);
				}

				if( ++ucslength > i_ucslen )
					break;

				uc = i_utf[utflength++];
			}

			return ucslength;
		}

#ifdef __cplusplus
	};
#endif
}



