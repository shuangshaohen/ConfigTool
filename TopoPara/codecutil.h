/************************************************************************/
/* codecutil.h                                                      */
/*                                                                      */
/* Unicode/GB2312(汉字)字符编码转换函数定义文件                         */
/*                                                                      */
/* Version   Author    Date           Modification                      */
/* ---------------------------------------------------------------------*/
/*  v0.1a    Matthew   07/11/29       Create                            */
/*                                                                      */
/************************************************************************/

#ifndef	_CODEC_UTIL_INCLUDE__
#define	_CODEC_UTIL_INCLUDE__

//#include "syspublib.h"

#define SyspubDllExport

namespace CodecUtil
{
#ifdef	__cplusplus
	extern	"C" {
#endif

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
		SyspubDllExport int ConvertGBKtoUCS2(const unsigned char* i_gstr, unsigned short* o_ustr, int i_ustrlen);

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
		SyspubDllExport int ConvertUCS2toGBK(const unsigned short* i_ustr, unsigned char* o_gstr, int i_gstrlen);

		/*************************************************************************
		* ConvertUCS2toUTF8 - UCS2编码转换为UTF-8编码
		*
		* PARAM
		*   i_ucs    : 输入需要转换的Unicode码地址
		*   o_utf    : 输出转换后的UTF-8码缓冲区地址
		*   i_utflen : 缓冲区长度
		*
		* RETURNS: 转换的编码个数
		*/
		SyspubDllExport int ConvertUCS2toUTF8(const unsigned short* i_ucs, unsigned char* o_utf, int i_utflen);

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
		SyspubDllExport int ConvertUTF8toUCS2(const unsigned char* i_utf, int i_utflen, unsigned short* o_ucs, int i_ucslen);

#ifdef	__cplusplus
	};
#endif
};


#endif	/* _CODEC_UTIL_INCLUDE__ */
