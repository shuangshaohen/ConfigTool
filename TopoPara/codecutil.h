/************************************************************************/
/* codecutil.h                                                      */
/*                                                                      */
/* Unicode/GB2312(����)�ַ�����ת�����������ļ�                         */
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
		* ConvertGBKtoUCS2 - GBK����ת��ΪUCS2����
		*
		* PARAM
		*   i_gstr    : ������Ҫת����GBK���ַ
		*   o_ustr    : ���ת�����Unicode�뻺������ַ
		*   i_ustrlen : ����������
		*
		* RETURNS: ת���ı������
		*/
		SyspubDllExport int ConvertGBKtoUCS2(const unsigned char* i_gstr, unsigned short* o_ustr, int i_ustrlen);

		/*************************************************************************
		* ConvertUCS2toGBK - UCS2����ת��ΪGBK����
		*
		* PARAM
		*   i_ustr    : ������Ҫת����Unicode���ַ
		*   o_gstr    : ���ת�����GBK�뻺������ַ
		*   i_gstrlen : ����������
		*
		* RETURNS: ת���ı������
		*/
		SyspubDllExport int ConvertUCS2toGBK(const unsigned short* i_ustr, unsigned char* o_gstr, int i_gstrlen);

		/*************************************************************************
		* ConvertUCS2toUTF8 - UCS2����ת��ΪUTF-8����
		*
		* PARAM
		*   i_ucs    : ������Ҫת����Unicode���ַ
		*   o_utf    : ���ת�����UTF-8�뻺������ַ
		*   i_utflen : ����������
		*
		* RETURNS: ת���ı������
		*/
		SyspubDllExport int ConvertUCS2toUTF8(const unsigned short* i_ucs, unsigned char* o_utf, int i_utflen);

		/*************************************************************************
		* ConvertUTF8toUCS2 - UTF-8����ת��ΪUCS2����
		*
		* PARAM
		*   i_utf    : ������Ҫת����UTF-8���ַ
		*   o_ucs    : ���ת�����Unicode�뻺������ַ
		*   i_ucslen : ����������
		*
		* RETURNS: ת���ı������
		*/
		SyspubDllExport int ConvertUTF8toUCS2(const unsigned char* i_utf, int i_utflen, unsigned short* o_ucs, int i_ucslen);

#ifdef	__cplusplus
	};
#endif
};


#endif	/* _CODEC_UTIL_INCLUDE__ */
