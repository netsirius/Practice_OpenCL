// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef WIN32	// UNIX
typedef unsigned long       DWORD; 
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int        UINT;
typedef int					bool;
typedef	long				LONG;
#define true				1
#define false				0
#define assert(x)			NULL
#define throw(x)			NULL
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#endif

#ifdef USE_OPENCV
#pragma comment (lib, "cxcore.lib")
#pragma comment (lib, "cv.lib")
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "Image.h"

#ifdef WIN32
#ifndef TRACE
#define TRACE(x) printf(x)
#endif
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif
#endif

// MACROS utilizadas para leer BMP
#define Lee(x, tipo)	x = *(tipo*)(&pBitmap[index]); index += sizeof(tipo)

#define Read(x)			fread (&x, sizeof(x), 1, file)*sizeof(x)
#define Escribe(x)		fwrite(&x, sizeof(x), 1, file)

namespace image {

//////////////////////////////////////////////////////////////////////
// Colores RGB predefinidos
//////////////////////////////////////////////////////////////////////
const RGBValue RGBValue::white              ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::red                ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::green              ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::blue               ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::magenta            ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::cyan               ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::yellow             ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::black              ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::aquamarine         ((PIXEL_TYPE)(MAX_VALUE*0.43921568627451), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.576470588235294));
const RGBValue RGBValue::bakerschocolate    ((PIXEL_TYPE)(MAX_VALUE*0.36078431372549), (PIXEL_TYPE)(MAX_VALUE*0.2), (PIXEL_TYPE)(MAX_VALUE*0.0901960784313725));
const RGBValue RGBValue::blueviolet         ((PIXEL_TYPE)(MAX_VALUE*0.623529411764706), (PIXEL_TYPE)(MAX_VALUE*0.372549019607843), (PIXEL_TYPE)(MAX_VALUE*0.623529411764706));
const RGBValue RGBValue::brass              ((PIXEL_TYPE)(MAX_VALUE*0.709803921568627), (PIXEL_TYPE)(MAX_VALUE*0.650980392156863), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765));
const RGBValue RGBValue::brightgold         ((PIXEL_TYPE)(MAX_VALUE*0.850980392156863), (PIXEL_TYPE)(MAX_VALUE*0.850980392156863), (PIXEL_TYPE)(MAX_VALUE*0.0980392156862745));
const RGBValue RGBValue::brown              ((PIXEL_TYPE)(MAX_VALUE*0.650980392156863), (PIXEL_TYPE)(MAX_VALUE*0.164705882352941), (PIXEL_TYPE)(MAX_VALUE*0.164705882352941));
const RGBValue RGBValue::bronze             ((PIXEL_TYPE)(MAX_VALUE*0.549019607843137), (PIXEL_TYPE)(MAX_VALUE*0.470588235294118), (PIXEL_TYPE)(MAX_VALUE*0.325490196078431));
const RGBValue RGBValue::bronzeii           ((PIXEL_TYPE)(MAX_VALUE*0.650980392156863), (PIXEL_TYPE)(MAX_VALUE*0.490196078431373), (PIXEL_TYPE)(MAX_VALUE*0.23921568627451));
const RGBValue RGBValue::cadetblue          ((PIXEL_TYPE)(MAX_VALUE*0.372549019607843), (PIXEL_TYPE)(MAX_VALUE*0.623529411764706), (PIXEL_TYPE)(MAX_VALUE*0.623529411764706));
const RGBValue RGBValue::coolcopper         ((PIXEL_TYPE)(MAX_VALUE*0.850980392156863), (PIXEL_TYPE)(MAX_VALUE*0.529411764705882), (PIXEL_TYPE)(MAX_VALUE*0.0980392156862745));
const RGBValue RGBValue::copper             ((PIXEL_TYPE)(MAX_VALUE*0.72156862745098), (PIXEL_TYPE)(MAX_VALUE*0.450980392156863), (PIXEL_TYPE)(MAX_VALUE*0.2));
const RGBValue RGBValue::coral              ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::cornflowerblue     ((PIXEL_TYPE)(MAX_VALUE*0.258823529411765), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765), (PIXEL_TYPE)(MAX_VALUE*0.435294117647059));
const RGBValue RGBValue::darkbrown          ((PIXEL_TYPE)(MAX_VALUE*0.36078431372549), (PIXEL_TYPE)(MAX_VALUE*0.250980392156863), (PIXEL_TYPE)(MAX_VALUE*0.2));
const RGBValue RGBValue::darkgreen          ((PIXEL_TYPE)(MAX_VALUE*0.184313725490196), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627), (PIXEL_TYPE)(MAX_VALUE*0.184313725490196));
const RGBValue RGBValue::darkgreencopper    ((PIXEL_TYPE)(MAX_VALUE*0.290196078431373), (PIXEL_TYPE)(MAX_VALUE*0.462745098039216), (PIXEL_TYPE)(MAX_VALUE*0.431372549019608));
const RGBValue RGBValue::darkolivegreen     ((PIXEL_TYPE)(MAX_VALUE*0.309803921568627), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627), (PIXEL_TYPE)(MAX_VALUE*0.184313725490196));
const RGBValue RGBValue::darkorchid         ((PIXEL_TYPE)(MAX_VALUE*0.6), (PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.803921568627451));
const RGBValue RGBValue::darkpurple         ((PIXEL_TYPE)(MAX_VALUE*0.529411764705882), (PIXEL_TYPE)(MAX_VALUE*0.12156862745098), (PIXEL_TYPE)(MAX_VALUE*0.470588235294118));
const RGBValue RGBValue::darkslateblue      ((PIXEL_TYPE)(MAX_VALUE*0.419607843137255), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.556862745098039));
const RGBValue RGBValue::darkslategrey      ((PIXEL_TYPE)(MAX_VALUE*0.184313725490196), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627));
const RGBValue RGBValue::darktan            ((PIXEL_TYPE)(MAX_VALUE*0.592156862745098), (PIXEL_TYPE)(MAX_VALUE*0.411764705882353), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627));
const RGBValue RGBValue::darkturquoise      ((PIXEL_TYPE)(MAX_VALUE*0.43921568627451), (PIXEL_TYPE)(MAX_VALUE*0.576470588235294), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765));
const RGBValue RGBValue::darkwood           ((PIXEL_TYPE)(MAX_VALUE*0.52156862745098), (PIXEL_TYPE)(MAX_VALUE*0.368627450980392), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765));
const RGBValue RGBValue::dimgrey            ((PIXEL_TYPE)(MAX_VALUE*0.329411764705882), (PIXEL_TYPE)(MAX_VALUE*0.329411764705882), (PIXEL_TYPE)(MAX_VALUE*0.329411764705882));
const RGBValue RGBValue::dustyrose          ((PIXEL_TYPE)(MAX_VALUE*0.52156862745098), (PIXEL_TYPE)(MAX_VALUE*0.388235294117647), (PIXEL_TYPE)(MAX_VALUE*0.388235294117647));
const RGBValue RGBValue::feldspar           ((PIXEL_TYPE)(MAX_VALUE*0.819607843137255), (PIXEL_TYPE)(MAX_VALUE*0.572549019607843), (PIXEL_TYPE)(MAX_VALUE*0.458823529411765));
const RGBValue RGBValue::firebrick          ((PIXEL_TYPE)(MAX_VALUE*0.556862745098039), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784));
const RGBValue RGBValue::forestgreen        ((PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.556862745098039), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784));
const RGBValue RGBValue::gold               ((PIXEL_TYPE)(MAX_VALUE*0.803921568627451), (PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*0.196078431372549));
const RGBValue RGBValue::goldenrod          ((PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.43921568627451));
const RGBValue RGBValue::grey               ((PIXEL_TYPE)(MAX_VALUE*0.752941176470588), (PIXEL_TYPE)(MAX_VALUE*0.752941176470588), (PIXEL_TYPE)(MAX_VALUE*0.752941176470588));
const RGBValue RGBValue::greencopper        ((PIXEL_TYPE)(MAX_VALUE*0.32156862745098), (PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*0.462745098039216));
const RGBValue RGBValue::greenyellow        ((PIXEL_TYPE)(MAX_VALUE*0.576470588235294), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.43921568627451));
const RGBValue RGBValue::huntergreen        ((PIXEL_TYPE)(MAX_VALUE*0.129411764705882), (PIXEL_TYPE)(MAX_VALUE*0.368627450980392), (PIXEL_TYPE)(MAX_VALUE*0.129411764705882));
const RGBValue RGBValue::indianred          ((PIXEL_TYPE)(MAX_VALUE*0.305882352941176), (PIXEL_TYPE)(MAX_VALUE*0.184313725490196), (PIXEL_TYPE)(MAX_VALUE*0.184313725490196));
const RGBValue RGBValue::khaki              ((PIXEL_TYPE)(MAX_VALUE*0.623529411764706), (PIXEL_TYPE)(MAX_VALUE*0.623529411764706), (PIXEL_TYPE)(MAX_VALUE*0.372549019607843));
const RGBValue RGBValue::lightblue          ((PIXEL_TYPE)(MAX_VALUE*0.752941176470588), (PIXEL_TYPE)(MAX_VALUE*0.850980392156863), (PIXEL_TYPE)(MAX_VALUE*0.850980392156863));
const RGBValue RGBValue::lightgrey          ((PIXEL_TYPE)(MAX_VALUE*0.658823529411765), (PIXEL_TYPE)(MAX_VALUE*0.658823529411765), (PIXEL_TYPE)(MAX_VALUE*0.658823529411765));
const RGBValue RGBValue::lightsteelblue     ((PIXEL_TYPE)(MAX_VALUE*0.56078431372549), (PIXEL_TYPE)(MAX_VALUE*0.56078431372549), (PIXEL_TYPE)(MAX_VALUE*0.741176470588235));
const RGBValue RGBValue::lightwood          ((PIXEL_TYPE)(MAX_VALUE*0.913725490196078), (PIXEL_TYPE)(MAX_VALUE*0.76078431372549), (PIXEL_TYPE)(MAX_VALUE*0.650980392156863));
const RGBValue RGBValue::limegreen          ((PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.803921568627451), (PIXEL_TYPE)(MAX_VALUE*0.196078431372549));
const RGBValue RGBValue::mandarianorange    ((PIXEL_TYPE)(MAX_VALUE*0.894117647058824), (PIXEL_TYPE)(MAX_VALUE*0.470588235294118), (PIXEL_TYPE)(MAX_VALUE*0.2));
const RGBValue RGBValue::maroon             ((PIXEL_TYPE)(MAX_VALUE*0.556862745098039), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.419607843137255));
const RGBValue RGBValue::mediumaquamarine   ((PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.803921568627451), (PIXEL_TYPE)(MAX_VALUE*0.6));
const RGBValue RGBValue::mediumblue         ((PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.803921568627451));
const RGBValue RGBValue::mediumforestgreen  ((PIXEL_TYPE)(MAX_VALUE*0.419607843137255), (PIXEL_TYPE)(MAX_VALUE*0.556862745098039), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784));
const RGBValue RGBValue::mediumgoldenrod    ((PIXEL_TYPE)(MAX_VALUE*0.917647058823529), (PIXEL_TYPE)(MAX_VALUE*0.917647058823529), (PIXEL_TYPE)(MAX_VALUE*0.682352941176471));
const RGBValue RGBValue::mediumorchid       ((PIXEL_TYPE)(MAX_VALUE*0.576470588235294), (PIXEL_TYPE)(MAX_VALUE*0.43921568627451), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765));
const RGBValue RGBValue::mediumseagreen     ((PIXEL_TYPE)(MAX_VALUE*0.258823529411765), (PIXEL_TYPE)(MAX_VALUE*0.435294117647059), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765));
const RGBValue RGBValue::mediumslateblue    ((PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::mediumspringgreen  ((PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::mediumturquoise    ((PIXEL_TYPE)(MAX_VALUE*0.43921568627451), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765));
const RGBValue RGBValue::mediumvioletred    ((PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.43921568627451), (PIXEL_TYPE)(MAX_VALUE*0.576470588235294));
const RGBValue RGBValue::mediumwood         ((PIXEL_TYPE)(MAX_VALUE*0.650980392156863), (PIXEL_TYPE)(MAX_VALUE*0.501960784313725), (PIXEL_TYPE)(MAX_VALUE*0.392156862745098));
const RGBValue RGBValue::midnightblue       ((PIXEL_TYPE)(MAX_VALUE*0.184313725490196), (PIXEL_TYPE)(MAX_VALUE*0.184313725490196), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627));
const RGBValue RGBValue::navyblue           ((PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.556862745098039));
const RGBValue RGBValue::neonblue           ((PIXEL_TYPE)(MAX_VALUE*0.301960784313725), (PIXEL_TYPE)(MAX_VALUE*0.301960784313725), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::neonpink           ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0.431372549019608), (PIXEL_TYPE)(MAX_VALUE*0.780392156862745));
const RGBValue RGBValue::newmidnightblue    ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0.611764705882353));
const RGBValue RGBValue::newtan             ((PIXEL_TYPE)(MAX_VALUE*0.92156862745098), (PIXEL_TYPE)(MAX_VALUE*0.780392156862745), (PIXEL_TYPE)(MAX_VALUE*0.619607843137255));
const RGBValue RGBValue::oldgold            ((PIXEL_TYPE)(MAX_VALUE*0.811764705882353), (PIXEL_TYPE)(MAX_VALUE*0.709803921568627), (PIXEL_TYPE)(MAX_VALUE*0.231372549019608));
const RGBValue RGBValue::orange             ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::orangered          ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0.141176470588235), (PIXEL_TYPE)(MAX_VALUE*0));
const RGBValue RGBValue::orchid             ((PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.43921568627451), (PIXEL_TYPE)(MAX_VALUE*0.858823529411765));
const RGBValue RGBValue::palegreen          ((PIXEL_TYPE)(MAX_VALUE*0.56078431372549), (PIXEL_TYPE)(MAX_VALUE*0.737254901960784), (PIXEL_TYPE)(MAX_VALUE*0.56078431372549));
const RGBValue RGBValue::pink               ((PIXEL_TYPE)(MAX_VALUE*0.737254901960784), (PIXEL_TYPE)(MAX_VALUE*0.56078431372549), (PIXEL_TYPE)(MAX_VALUE*0.56078431372549));
const RGBValue RGBValue::plum               ((PIXEL_TYPE)(MAX_VALUE*0.917647058823529), (PIXEL_TYPE)(MAX_VALUE*0.67843137254902), (PIXEL_TYPE)(MAX_VALUE*0.917647058823529));
const RGBValue RGBValue::quartz             ((PIXEL_TYPE)(MAX_VALUE*0.850980392156863), (PIXEL_TYPE)(MAX_VALUE*0.850980392156863), (PIXEL_TYPE)(MAX_VALUE*0.952941176470588));
const RGBValue RGBValue::richblue           ((PIXEL_TYPE)(MAX_VALUE*0.349019607843137), (PIXEL_TYPE)(MAX_VALUE*0.349019607843137), (PIXEL_TYPE)(MAX_VALUE*0.670588235294118));
const RGBValue RGBValue::salmon             ((PIXEL_TYPE)(MAX_VALUE*0.435294117647059), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765));
const RGBValue RGBValue::scarlet            ((PIXEL_TYPE)(MAX_VALUE*0.549019607843137), (PIXEL_TYPE)(MAX_VALUE*0.0901960784313725), (PIXEL_TYPE)(MAX_VALUE*0.0901960784313725));
const RGBValue RGBValue::seagreen           ((PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.556862745098039), (PIXEL_TYPE)(MAX_VALUE*0.407843137254902));
const RGBValue RGBValue::semisweetchocolate ((PIXEL_TYPE)(MAX_VALUE*0.419607843137255), (PIXEL_TYPE)(MAX_VALUE*0.258823529411765), (PIXEL_TYPE)(MAX_VALUE*0.149019607843137));
const RGBValue RGBValue::sienna             ((PIXEL_TYPE)(MAX_VALUE*0.556862745098039), (PIXEL_TYPE)(MAX_VALUE*0.419607843137255), (PIXEL_TYPE)(MAX_VALUE*0.137254901960784));
const RGBValue RGBValue::silver             ((PIXEL_TYPE)(MAX_VALUE*0.901960784313726), (PIXEL_TYPE)(MAX_VALUE*0.909803921568627), (PIXEL_TYPE)(MAX_VALUE*0.980392156862745));
const RGBValue RGBValue::skyblue            ((PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.6), (PIXEL_TYPE)(MAX_VALUE*0.8));
const RGBValue RGBValue::slateblue          ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*0.498039215686275), (PIXEL_TYPE)(MAX_VALUE*1));
const RGBValue RGBValue::spicypink          ((PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0.109803921568627), (PIXEL_TYPE)(MAX_VALUE*0.682352941176471));
const RGBValue RGBValue::springgreen        ((PIXEL_TYPE)(MAX_VALUE*0), (PIXEL_TYPE)(MAX_VALUE*1), (PIXEL_TYPE)(MAX_VALUE*0.498039215686275));
const RGBValue RGBValue::steelblue          ((PIXEL_TYPE)(MAX_VALUE*0.137254901960784), (PIXEL_TYPE)(MAX_VALUE*0.419607843137255), (PIXEL_TYPE)(MAX_VALUE*0.556862745098039));
const RGBValue RGBValue::summersky          ((PIXEL_TYPE)(MAX_VALUE*0.219607843137255), (PIXEL_TYPE)(MAX_VALUE*0.690196078431373), (PIXEL_TYPE)(MAX_VALUE*0.870588235294118));
const RGBValue RGBValue::tan                ((PIXEL_TYPE)(MAX_VALUE*0.858823529411765), (PIXEL_TYPE)(MAX_VALUE*0.576470588235294), (PIXEL_TYPE)(MAX_VALUE*0.43921568627451));
const RGBValue RGBValue::thistle            ((PIXEL_TYPE)(MAX_VALUE*0.847058823529412), (PIXEL_TYPE)(MAX_VALUE*0.749019607843137), (PIXEL_TYPE)(MAX_VALUE*0.847058823529412));
const RGBValue RGBValue::turquoise          ((PIXEL_TYPE)(MAX_VALUE*0.67843137254902), (PIXEL_TYPE)(MAX_VALUE*0.917647058823529), (PIXEL_TYPE)(MAX_VALUE*0.917647058823529));
const RGBValue RGBValue::verydarkbrown      ((PIXEL_TYPE)(MAX_VALUE*0.36078431372549), (PIXEL_TYPE)(MAX_VALUE*0.250980392156863), (PIXEL_TYPE)(MAX_VALUE*0.2));
const RGBValue RGBValue::verylightgrey      ((PIXEL_TYPE)(MAX_VALUE*0.803921568627451), (PIXEL_TYPE)(MAX_VALUE*0.803921568627451), (PIXEL_TYPE)(MAX_VALUE*0.803921568627451));
const RGBValue RGBValue::violet             ((PIXEL_TYPE)(MAX_VALUE*0.309803921568627), (PIXEL_TYPE)(MAX_VALUE*0.184313725490196), (PIXEL_TYPE)(MAX_VALUE*0.309803921568627));
const RGBValue RGBValue::violetred          ((PIXEL_TYPE)(MAX_VALUE*0.8), (PIXEL_TYPE)(MAX_VALUE*0.196078431372549), (PIXEL_TYPE)(MAX_VALUE*0.6));
const RGBValue RGBValue::wheat              ((PIXEL_TYPE)(MAX_VALUE*0.847058823529412), (PIXEL_TYPE)(MAX_VALUE*0.847058823529412), (PIXEL_TYPE)(MAX_VALUE*0.749019607843137));
const RGBValue RGBValue::yellowgreen        ((PIXEL_TYPE)(MAX_VALUE*0.6), (PIXEL_TYPE)(MAX_VALUE*0.8), (PIXEL_TYPE)(MAX_VALUE*0.196078431372549));

//////////////////////////////////////////////////////////////////////
// Funciones de dibujado sobre la imagen
//   no estan optimizadas
//////////////////////////////////////////////////////////////////////

void CImage::Clear(RGBValue rgb)
{
	int x, y;

	if (method == BGR_24_BITS)
	{
		if (rgb == RGBValue::white)
		{
			memset(m_pData, 0xFF, GetHeight()*GetWidth()*3);
		}
		else if (rgb == RGBValue::black)
		{
			memset(m_pData, 0x00, GetHeight()*GetWidth()*3);
		}
		else
		{
			BYTE r = rgb.r / CONVERSION;
			BYTE g = rgb.g / CONVERSION;
			BYTE b = rgb.b / CONVERSION;
			BYTE* pEnd = &m_pData[GetHeight()*GetWidth()*3 + 1];
			for (BYTE* p = m_pData; p < pEnd; ++p)
			{
				*(p++) = b;
				*(p++) = g;
				*(p++) = r;
			}
		}
	} 
	else if (method == RGB_24_BITS)
	{
		if (rgb == RGBValue::white)
		{
			memset(m_pData, 0xFF, GetHeight()*GetWidth()*3);
		}
		else if (rgb == RGBValue::black)
		{
			memset(m_pData, 0x00, GetHeight()*GetWidth()*3);
		}
		else
		{
			BYTE r = rgb.r / CONVERSION;
			BYTE g = rgb.g / CONVERSION;
			BYTE b = rgb.b / CONVERSION;
			BYTE* pEnd = &m_pData[GetHeight()*GetWidth()*3 + 1];
			for (BYTE* p = m_pData; p < pEnd; ++p)
			{
				*(p++) = r;
				*(p++) = g;
				*(p++) = b;
			}
		}
	}
	else if (method == BGRA_32_BITS)
	{
		if (rgb == RGBValue::white)
		{
			memset(m_pData, 0xFF, GetHeight()*GetWidth()*4);
		}
		else if (rgb == RGBValue::black)
		{
			memset(m_pData, 0x00, GetHeight()*GetWidth()*4);
		}
		else
		{
			BYTE r = rgb.r / CONVERSION;
			BYTE g = rgb.g / CONVERSION;
			BYTE b = rgb.b / CONVERSION;
			BYTE a = 0;
			BYTE* pEnd = &m_pData[GetHeight()*GetWidth()*4 + 1];
			for (BYTE* p = m_pData; p < pEnd; ++p)
			{
				*(p++) = b;
				*(p++) = g;
				*(p++) = r;
				*(p++) = a;
			}
		}
	}
	else
	{
		int width = (int)GetWidth();
		int height = (int)GetHeight();
		for (x = 0; x < width; ++x)
			for (y = 0; y < height; ++y)
				SetPixelRGB(x, y, rgb);
	}
}

void CImage::DrawRectangle(float x1, float y1, float x2, float y2, RGBValue rgb)
{
	DrawLine(x1, y1, x2, y1, rgb);
	DrawLine(x1, y2, x2, y2, rgb);
	DrawLine(x1, y1, x1, y2, rgb);
	DrawLine(x2, y1, x2, y2, rgb);
}

void CImage::DrawFillRectangle(float x1, float y1, float x2, float y2, RGBValue rgb)
{
	for (float y = y1; y <= y2; y += 1)
		DrawLine(x1, y, x2, y, rgb);
}

void CImage::DrawLine(float x1, float y1, float x2, float y2, RGBValue rgb)
{
	float num = sqrtf( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) + 1;
	for (int i = 0; i < num; ++i)
	{
		int x, y;
		x = (int)(x1 + (x2-x1)*(i/num));
		y = (int)(y1 + (y2-y1)*(i/num));
		if (x >= 0 && x < (int)m_width && y >= 0 && y < (int)m_height)
			this->SetPixelRGB(x, y, rgb);
	}
	if (x2 >= 0 && x2 <= (float)m_width-1 && y2 >= 0 && y2 <= (float)m_height-1)
		this->SetPixelRGB((int)x2, (int)y2, rgb);
}

void CImage::DrawEllipse(float x, float y, float angulo, float mayor, float minor, RGBValue rgb)
{
	int parts = (int)(max(mayor, minor) * 3.14159275f*2 + 1);
	float posx, posy, tx, ty;
	float cos_angulo = cosf(angulo);
	float sin_angulo = sinf(angulo);
	for (int p = 0; p < parts; ++p)
	{
		float ang = p / (float)parts * 2 * 3.141592f;
		tx = cosf(ang)*minor;
		ty = sinf(ang)*mayor;
		posx = x + tx*cos_angulo - ty*sin_angulo;
		posy = y + tx*sin_angulo + ty*cos_angulo;

		if ((posx >= 0) && (posx < this->m_width) && 
			 (posy >= 0) && (posy < this->m_height))
			SetPixelRGB((int)posx, (int)posy, rgb);
	}
}

void CImage::DrawImage(float x, float y, CImage& source)
{
	int ini_x = (int) x;
	int ini_y = (int) y;
	for (int yy = 0; yy < source.GetHeight(); ++yy) {
		for (int xx = 0; xx < source.GetWidth(); ++xx) {
			RGBValue rgb = source.GetPixelRGB(xx, yy);
			this->SetPixelRGB(xx + ini_x, yy + ini_y, rgb);
		}
	}
}

void CImage::DrawFillEllipse(float x, float y, float angulo, float mayor, float minor, RGBValue rgb)
{
#ifdef USE_OPENCV
	cvEllipse( m_matriz, cvPoint(x, m_height-y-1), cvSize(minor, mayor), angulo*360/(2*3.141592f),
                0, 360, cvScalar(rgb.r, rgb.g, rgb.b),
                -1, 8, 0 );
#else
	float posx, posy, tx, ty;
	float cos_angulo = cosf(angulo);
	float sin_angulo = sinf(angulo);

	float radius = max(minor, mayor);
	for (float r = radius; r >= 0; r -= 1.5)
	{
		int parts = (int)(r * M_PI*2) + 1;
		for (int p = 0; p < parts; ++p)
		{
			float ang = p / (float)parts * 2 * (float)M_PI;
			tx = cosf(ang)*minor*(r/radius);
			ty = sinf(ang)*mayor*(r/radius);
			posx = x + tx*cos_angulo - ty*sin_angulo;
			posy = y + tx*sin_angulo + ty*cos_angulo;

			if ((posx >= 0) && (posx < this->m_width) && 
				(posy >= 0) && (posy < this->m_height))
				SetPixelRGB((int)posx, (int)posy, rgb);
		}
	}
#endif
}
void CImage::DrawCircle(float x, float y, float radius, RGBValue rgb)
{
	int parts = (int)(radius * M_PI*2) + 1;
	int posx, posy;
	for (int p = 0 ; p < parts; ++p)
	{
		float ang = p / (float)parts * 2 * (float) M_PI;
		posx = (int)(x + radius * cosf(ang));
		posy = (int)(y + radius * sinf(ang));

		if ((posx >= 0) && (posx < (int)m_width) && 
			 (posy >= 0) && (posy < (int)m_height))
			SetPixelRGB((int)posx, (int)posy, rgb);
	}
}
void CImage::DrawFillCircle(float x, float y, float radius, RGBValue rgb)
{
	int posx, posy;
	for (float r = 0 ; r <= radius; r += 0.5)
	{
		int parts = (int)(r*2*M_PI* 1.1);
		for (int p = 0 ; p < parts; ++p)
		{
			posx = (int)(x + r * cos(p/(float)parts*M_PI*2));
			posy = (int)(y + r * sin(p/(float)parts*M_PI*2));
			if ((posx >= 0) && (posx < (int)m_width) &&
				 (posy >= 0) && (posy < (int)m_height))
				SetPixelRGB(posx, posy, rgb);
		}
	}
}

const pixelFormat CImage::s_formatBGR = {24, PIXEL_RGB,  
		0x0000FF, 0x00FF00, 0xFF0000, 
		8, 8, 8};

const pixelFormat CImage::s_formatRGB = {24, PIXEL_RGB,  
		0xFF0000, 0x00FF00, 0x0000FF, 
		8, 8, 8};

const pixelFormat CImage::s_formatBGRA = {32, PIXEL_RGBA,  
		0x0000FF, 0x00FF00, 0xFF0000, 0xFF000000,
		8, 8, 8, 8};

const pixelFormat CImage::s_formatDEPTH = {16, PIXEL_BW,  
		0x00FFFF,
		16};

bool CImage::CreateTexto()
{
	if (m_bCreated)
		return false;

	pixelFormat format;
	format.pixel = PIXEL_RGB;
	format.bpp = 24;
	format.rgb.r = 0x0000FF;
	format.rgb.rBits = 8;
	format.rgb.g = 0x00FF00;
	format.rgb.gBits = 8;
	format.rgb.b = 0xFF0000;
	format.rgb.bBits = 8;
	Create(640, 480, format);

	Clear(RGBValue::white);
	Print(10, 10, "0123456789", RGBValue::black);
	Print(10, 20, "ABCDEFGHIJKLMNÑOPQRSTUVWXYZ", RGBValue::black);
	Print(10, 30, "abcdefghijklmnñopqrstuvwxyz", RGBValue::black);
	Print(10, 40, ".-,;:!\"@#$%&/()=", RGBValue::black);

	return true;
}

// Dibuja letras sobre la imagen, esta en desarrollo, no estan todos los caracteres
void CImage::Print(int posx, int posy, char* fmt, RGBValue rgb, ...)
{
unsigned char car_0[] ={56, 68, 100, 84, 76, 68, 56, 0};
unsigned char car_1[] ={16, 48, 80, 16, 16, 16, 124, 0};
unsigned char car_2[] ={56, 68, 68, 8, 16, 32, 124, 0};
unsigned char car_3[] ={56, 68, 4, 24, 4, 68, 56, 0};
unsigned char car_4[] ={68, 68, 68, 60, 4, 4, 4, 0};
unsigned char car_5[] ={124, 64, 64, 120, 4, 4, 120, 0};
unsigned char car_6[] ={56, 68, 64, 120, 68, 68, 56, 0};
unsigned char car_7[] ={124, 4, 4, 8, 8, 16, 16, 0};
unsigned char car_8[] ={56, 68, 68, 56, 68, 68, 56, 0};
unsigned char car_9[] ={56, 68, 68, 60, 4, 68, 56, 0};
unsigned char car_dot[] ={0, 0, 0, 0, 0, 24, 24, 0};
unsigned char car_guion[] ={0, 0, 0, 124, 0, 0, 0, 0};
unsigned char car_A[] ={16, 40, 40, 68, 124, 68, 68, 0};
unsigned char car_B[] ={120, 68, 68, 120, 68, 68, 120, 0};
unsigned char car_C[] ={60, 64, 64, 64, 64, 64, 60, 0};
unsigned char car_D[] ={120, 68, 68, 68, 68, 68, 120, 0};
unsigned char car_E[] ={124, 64, 64, 120, 64, 64, 124, 0};
unsigned char car_F[] ={124, 64, 64, 112, 64, 64, 64, 0};
unsigned char car_G[] ={56, 68, 64, 76, 68, 68, 56, 0};
unsigned char car_H[] ={68, 68, 68, 124, 68, 68, 68, 0};
unsigned char car_I[] ={56, 16, 16, 16, 16, 16, 56, 0};
unsigned char car_J[] ={8, 8, 8, 8, 8, 72, 48, 0};
unsigned char car_K[] ={68, 72, 80, 96, 80, 72, 68, 0};
unsigned char car_L[] ={64, 64, 64, 64, 64, 64, 124, 0};
unsigned char car_M[] ={68, 108, 84, 68, 68, 68, 68, 0};
unsigned char car_N[] ={68, 100, 100, 84, 76, 76, 68, 0};
unsigned char car_ENE[] ={124, 0, 68, 100, 84, 76, 68, 0};
unsigned char car_O[] ={56, 68, 68, 68, 68, 68, 56, 0};
unsigned char car_P[] ={120, 68, 68, 120, 64, 64, 64, 0};
unsigned char car_Q[] ={56, 68, 68, 68, 68, 76, 60, 2};
unsigned char car_R[] ={120, 68, 68, 120, 80, 72, 68, 0};
unsigned char car_S[] ={60, 64, 64, 56, 4, 4, 120, 0};
unsigned char car_T[] ={124, 16, 16, 16, 16, 16, 16, 0};
unsigned char car_U[] ={68, 68, 68, 68, 68, 68, 56, 0};
unsigned char car_V[] ={68, 68, 68, 40, 40, 40, 16, 0};
unsigned char car_W[] ={146, 146, 146, 84, 84, 40, 40, 0};
unsigned char car_X[] ={68, 68, 40, 16, 40, 68, 68, 0};
unsigned char car_Y[] ={68, 68, 40, 16, 16, 16, 16, 0};
unsigned char car_Z[] ={124, 4, 8, 16, 32, 64, 124, 0};
unsigned char car_a[] ={0, 0, 56, 4, 60, 68, 60, 0};
unsigned char car_b[] ={64, 64, 120, 68, 68, 68, 120, 0};
unsigned char car_c[] ={0, 0, 60, 64, 64, 64, 60, 0};
unsigned char car_d[] ={4, 4, 60, 68, 68, 68, 60, 0};
unsigned char car_e[] ={0, 0, 56, 68, 120, 64, 56, 0};
unsigned char car_f[] ={12, 16, 16, 56, 16, 16, 16, 0};
unsigned char car_g[] ={0, 0, 52, 76, 76, 52, 4, 56};
unsigned char car_h[] ={64, 64, 88, 100, 68, 68, 68, 0};
unsigned char car_i[] ={16, 0, 16, 16, 16, 16, 16, 0};
unsigned char car_j[] ={8, 0, 8, 8, 8, 8, 8, 48};
unsigned char car_k[] ={64, 64, 68, 72, 80, 104, 68, 0};
unsigned char car_l[] ={16, 16, 16, 16, 16, 16, 16, 0};
unsigned char car_m[] ={0, 0, 104, 84, 84, 84, 84, 0};
unsigned char car_n[] ={0, 0, 88, 100, 68, 68, 68, 0};
unsigned char car_ene[] ={56, 0, 88, 100, 68, 68, 68, 0};
unsigned char car_o[] ={0, 0, 56, 68, 68, 68, 56, 0};
unsigned char car_p[] ={0, 0, 88, 100, 100, 88, 64, 64};
unsigned char car_q[] ={0, 0, 52, 76, 76, 52, 4, 4};
unsigned char car_r[] ={0, 0, 76, 80, 96, 64, 64, 0};
unsigned char car_s[] ={0, 0, 60, 64, 56, 4, 120, 0};
unsigned char car_t[] ={16, 16, 56, 16, 16, 16, 8, 0};
unsigned char car_u[] ={0, 0, 68, 68, 68, 68, 60, 0};
unsigned char car_v[] ={0, 0, 68, 68, 40, 40, 16, 0};
unsigned char car_w[] ={0, 0, 84, 84, 84, 40, 40, 0};
unsigned char car_x[] ={0, 0, 68, 40, 16, 40, 68, 0};
unsigned char car_y[] ={0, 0, 68, 68, 68, 60, 4, 56};
unsigned char car_z[] ={0, 0, 124, 8, 16, 32, 124, 0};

//	unsigned char car_0[] = {0, 255, 255, 255, 255, 255, 255, 255};

	va_list  vlist;
	char str[4096];

	// Get output string
	va_start(vlist, fmt);
	vsprintf_s(str, fmt, vlist);

	int i = 0;
	while (str[i] != 0)
	{
		unsigned char* caracter = NULL;
		switch (str[i])
		{
			case '0':caracter = car_0;break;
			case '1':caracter = car_1;break;
			case '2':caracter = car_2;break;
			case '3':caracter = car_3;break;
			case '4':caracter = car_4;break;
			case '5':caracter = car_5;break;
			case '6':caracter = car_6;break;
			case '7':caracter = car_7;break;
			case '8':caracter = car_8;break;
			case '9':caracter = car_9;break;
			case '.':caracter = car_dot;break;
			case '-':caracter = car_guion;break;
			case 'A':caracter = car_A;break; 
			case 'B':caracter = car_B;break;
			case 'C':caracter = car_C;break;
			case 'D':caracter = car_D;break;
			case 'E':caracter = car_E;break;
			case 'F':caracter = car_F;break;
			case 'G':caracter = car_G;break;
			case 'H':caracter = car_H;break;
			case 'I':caracter = car_I;break;
			case 'J':caracter = car_J;break;
			case 'K':caracter = car_K;break;
			case 'L':caracter = car_L;break;
			case 'M':caracter = car_M;break;
			case 'N':caracter = car_N;break;
			case 'Ñ':caracter = car_ENE;break;
			case 'O':caracter = car_O;break;
			case 'P':caracter = car_P;break;
			case 'Q':caracter = car_Q;break;
			case 'R':caracter = car_R;break;
			case 'S':caracter = car_S;break;
			case 'T':caracter = car_T;break;
			case 'U':caracter = car_U;break;
			case 'V':caracter = car_V;break;
			case 'W':caracter = car_W;break;
			case 'X':caracter = car_X;break;
			case 'Y':caracter = car_Y;break;
			case 'Z':caracter = car_Z;break;
			case 'a':caracter = car_a;break;
			case 'b':caracter = car_b;break;
			case 'c':caracter = car_c;break;
			case 'd':caracter = car_d;break;
			case 'e':caracter = car_e;break;
			case 'f':caracter = car_f;break;
			case 'g':caracter = car_g;break;
			case 'h':caracter = car_h;break;
			case 'i':caracter = car_i;break;
			case 'j':caracter = car_j;break;
			case 'k':caracter = car_k;break;
			case 'l':caracter = car_l;break;
			case 'm':caracter = car_m;break;
			case 'n':caracter = car_n;break;
			case 'ñ':caracter = car_ene;break;
			case 'o':caracter = car_o;break;
			case 'p':caracter = car_p;break;
			case 'q':caracter = car_q;break;
			case 'r':caracter = car_r;break;
			case 's':caracter = car_s;break;
			case 't':caracter = car_t;break;
			case 'u':caracter = car_u;break;
			case 'v':caracter = car_v;break;
			case 'w':caracter = car_w;break;
			case 'x':caracter = car_x;break;
			case 'y':caracter = car_y;break;
			case 'z':caracter = car_z;break;
		}

		if (caracter != NULL)
		{
			for (int y = 0; y < 8; ++y)
			{
				unsigned char carac = caracter[y];
				for (int x = 0; x < 8; ++x)
				{
					if ((carac % 2) == 1)
					{
						if ( (posx + (7-x) < (int)m_width) &&
							  (posy+y < (int)m_height)
							)
						SetPixelRGB(posx + (7-x), posy+y, rgb);
					}
					carac = carac >> 1;
				}
			}
		}
		posx += 8;
		i++;
	}
	va_end(vlist);
}

t_method CImage::computeMethod(pixelFormat format)
{
	switch (format.pixel) {
	case PIXEL_RGB:
		if ((format.rgb.r == 0x0000FF) &&
			(format.rgb.g == 0x00FF00) &&
			(format.rgb.b == 0xFF0000) &&
			(MAX_BITS == 16))
			return BGR_24_BITS;
		if ((format.rgb.r == 0xFF0000) &&
			(format.rgb.g == 0x00FF00) &&
			(format.rgb.b == 0x0000FF) &&
			(MAX_BITS == 16))
			return RGB_24_BITS;
		break;
	case PIXEL_RGBA:
		if ((format.rgba.r == 0x000000FF) &&
			(format.rgba.g == 0x0000FF00) &&
			(format.rgba.b == 0x00FF0000) &&
			(format.rgba.a == 0xFF000000) &&
			(MAX_BITS == 16))
			return BGRA_32_BITS;
		break;
	case PIXEL_HLS:
		if ((format.hls.h == 0xFF0000) &&
			(format.hls.l == 0x00FF00) &&
			(format.hls.s == 0x0000FF) &&
			(MAX_BITS == 16))
			return HLS_24_BITS;
		break;
	case PIXEL_BW:
		if ((format.bw.bw == 0xFF) &&
			(format.bw.bwBits == 8) && 
			(MAX_BITS == 16))
			return GRAY_8_BITS;
		if ((format.bw.bw == 0xFFFF) &&
			(format.bw.bwBits == 16) && 
			(MAX_BITS == 16))
			return GRAY_16_BITS;
		break;		
	}
	return NO_FAST_METHOD;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor de la clase CImage
CImage::CImage()
{
	Init();
}

CImage::CImage(CImage& origen)
{
	Init();
	Create(origen);
}

void CImage::Init() {
	// Por defecto la imagen no contine nada, no esta creada
	m_bCreated = false;
	m_pData = NULL;
	// Inicializamos el formato a un formato con todo a cero
	m_width = 0;
	m_height = 0;
	m_format.bpp = 0;
	m_format.rgb.r = 0;
	m_format.rgb.rBits = 0;
	m_format.rgb.g = 0;
	m_format.rgb.gBits = 0;
	m_format.rgb.b = 0;
	m_format.rgb.bBits = 0;
	m_format.pixel = PIXEL_BW;
	m_format.highBit = 0;
}


// Destructor
CImage::~CImage()
{
	Destroy();
}

void CImage::Destroy()
{
	// Eliminamos la memoria reservada
	if (m_bCreated)
		delete m_pData;
	m_bCreated = false;
	m_pData = NULL;
}

// Funcion para crear la imagen, se le pasa el formato deseado y la imagen origen
bool CImage::Create(int width, int height, pixelFormat format, CImage& origen)
{
	if (origen.m_bCreated == false)
		// La imagen origen no esta creada
		return false;
	if (m_bCreated)
		// La imagen ya ha sido creada
		return false;
	if ((height == 0) || (width == 0)) {
		height = origen.m_height;
		width = origen.m_width;
	}

	method = computeMethod(format);
	int xOrigen, yOrigen;
	if (Create(width, height, format)) {
		for (int y=0; y < m_height; ++y) {
			for (int x=0; x < m_width; ++x) {
				xOrigen = (int)(origen.m_width  *(x/float(m_width)));
				yOrigen = (int)(origen.m_height *(y/float(m_height)));

				switch (format.pixel) {
				case PIXEL_RGB:
					SetPixelRGB(x, y, origen.GetPixelRGB(xOrigen,yOrigen));
					break;
				case PIXEL_YUV:
					SetPixelYUV(x, y, origen.GetPixelYUV(xOrigen,yOrigen));
					break;
				case PIXEL_HLS:
					SetPixelHLS(x, y, origen.GetPixelHLS(xOrigen,yOrigen));
					break;
				case PIXEL_RGBA:
					SetPixelRGBA(x, y, origen.GetPixelRGBA(xOrigen,yOrigen));
					break;
				}

			}
		}
		return true;
	}
	return false;
}

// Funcion para crear la imagen, se le pasa la imagen origen y realliza una copia
bool CImage::Create(CImage& origen)
{
	if (origen.m_bCreated == false)
		// La imagen origen no esta creada
		return false;
	if (m_bCreated)
		// La imagen ya ha sido creada
		return false;

	if (Create(origen.m_width, origen.m_height, origen.m_format)) {
		memcpy(m_pData, origen.m_pData, m_width * m_height * (m_format.bpp/8));
		return true;
	}
	return false;
}

// Funcion para copiar una imagen
bool CImage::Copy(CImage& origen)
{
	if (origen.m_bCreated == false)
		// La imagen origen no esta creada
		return false;

	// Si la imagen destino esta creada
	if (m_bCreated)
	{
		// Si la memoria reservada para datos no es la misma
		if ((origen.m_height*origen.m_width*origen.m_format.bpp) !=
			(m_height*m_width*m_format.bpp))
		{
			// Liberamos memoria
			Destroy();
		}
		else
		{
			// Sino basta con copiar el formato y los datos
			m_width = origen.m_width;
			m_height = origen.m_height;
			m_format = origen.m_format;
			memcpy(m_pData, origen.m_pData, m_width * m_height * (m_format.bpp/8));
			return true;
		}
	}
	// Imagen no creada o destruida, llamamos al Create pasandole la imagen
	return Create(origen);
}

// Funcion para crear la imagen, se le pasa el formato deseado
bool CImage::Create(int width, int height, pixelFormat format)
{
	// Si ya esta creada no la podemos volver a crear
	if (m_bCreated)
		return false;

	assert(format.bpp <= sizeof(PIXEL_TYPE)*8);
	switch (format.pixel) {
	case PIXEL_BW:
		assert (format.bw.bwBits <= MAX_BITS);
		break;
	case PIXEL_RGB:
		assert (format.rgb.rBits <= MAX_BITS);
		assert (format.rgb.gBits <= MAX_BITS);
		assert (format.rgb.bBits <= MAX_BITS);
		break;
	case PIXEL_YUV:
		assert (format.yuv.yBits <= MAX_BITS);
		assert (format.yuv.uBits <= MAX_BITS);
		assert (format.yuv.vBits <= MAX_BITS);
		break;
	case PIXEL_HLS:
		assert (format.yuv.yBits <= MAX_BITS);
		assert (format.yuv.uBits <= MAX_BITS);
		assert (format.yuv.vBits <= MAX_BITS);
	case PIXEL_RGBA:
		assert (format.rgba.rBits <= MAX_BITS);
		assert (format.rgba.gBits <= MAX_BITS);
		assert (format.rgba.bBits <= MAX_BITS);
		assert (format.rgba.aBits <= MAX_BITS);
		break;
	default:
		throw ("Formato de pixel desconocido");
		break;
	}
	
	// Calculamos la memoria necesaria
	long size = (long)width * height * format.bpp / 8;

	// Reservamos memoria
	m_pData = new BYTE[size];
	if (m_pData != NULL) {
		method = computeMethod(format);
		// Se ha podido crear la imagen
		m_bCreated = true;
		m_height = height;
		m_width = width;
		m_format = format;
		m_format.highBit = 1 << (m_format.bpp - 1);
#ifdef USE_OPENCV
		m_matriz = new CvMat();
		if (method == BGR_24_BITS)
			*m_matriz = cvMat( m_height, m_width, CV_8UC3, m_pData );
		else if (method == RGB_24_BITS)
			*m_matriz = cvMat( m_height, m_width, CV_8UC3, m_pData );
		else if (method == GRAY_8_BITS)
			*m_matriz = cvMat( m_height, m_width, CV_8U, m_pData );
		else if (method == GRAY_16_BITS)
			*m_matriz = cvMat( m_height, m_width, CV_16S, m_pData );
#endif
	}
	return m_bCreated;
}

//////////////////////////////////////////////////////////////////////
// Funciones de acceso al array
//////////////////////////////////////////////////////////////////////

// Obtiene el valor de la posicion x,y
PIXEL_TYPE CImage::GetValue(int x, int y)
{
	// Obtenemos el valor en un única variable
	PIXEL_TYPE value = 0;
	BYTE* pValue = (BYTE*)&value;
	assert (x < m_width);
	assert (y < m_height);
	int position = (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
	assert (m_format.bpp/8 <= sizeof(PIXEL_TYPE));
	int inc = m_format.bpp / 8;
	for (UINT i=0; i < m_format.bpp/8; ++i) {
		pValue[inc - 1] = m_pData[position];
		++position;
		--inc;
	}
	return value;
}

// Actualiza en x,y el valor value
void CImage::SetValue(PIXEL_TYPE value, int x, int y)
{
	BYTE* pValue = (BYTE*)&value;
	assert (x < m_width);
	assert (y < m_height);
	int position = (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
	assert (m_format.bpp/8 <= sizeof(PIXEL_TYPE));
	int inc = m_format.bpp / 8;
	for (UINT i=0; i < m_format.bpp/8; ++i) {
		m_pData[position] = pValue[inc - 1];
		++position;
		--inc;
	}
}

//////////////////////////////////////////////////////////////////////
// Funciones de tratamiento a nivel de pixel
//////////////////////////////////////////////////////////////////////

// Obtiene el valor de un pixel 
YUVValue CImage::GetPixelYUV(int x, int y)
{
	YUVValue ret;
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_YUV:
			{
				PIXEL_TYPE value = GetValue(x, y);

				// Red, Green and Blue
				ret.y = GetResult(m_format.yuv.y, value, m_format.bpp, m_format.yuv.yBits);
				ret.u = GetResult(m_format.yuv.u, value, m_format.bpp, m_format.yuv.uBits);
				ret.v = GetResult(m_format.yuv.v, value, m_format.bpp, m_format.yuv.vBits);
				return ret;
			}
		case PIXEL_RGB:
			return RGBtoYUV(GetPixelRGB(x, y));
		case PIXEL_HLS:
			return RGBtoYUV(HLStoRGB(GetPixelHLS(x, y)));
		default:
			throw ("No existe la funcion de conversion a YUV");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return ret;
}

// Actualiza el valor de un pixel 		
YUVValue CImage::SetPixelYUV(int x, int y, YUVValue value)
{
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_YUV:
			{
				PIXEL_TYPE yuvValue;
				// Unimos los valores de las tres componentes
				// Red, Green and Blue
				PIXEL_TYPE yValue = SetResult(m_format.yuv.y, value.y, m_format.bpp, m_format.yuv.yBits);
				PIXEL_TYPE uValue = SetResult(m_format.yuv.u, value.u, m_format.bpp, m_format.yuv.uBits);
				PIXEL_TYPE vValue = SetResult(m_format.yuv.v, value.v, m_format.bpp, m_format.yuv.vBits);

				// Colocamos el valor en un única variable y actualizamos
				yuvValue = yValue | uValue | vValue;
				SetValue (yuvValue, x, y);
				return value;
			}
		case PIXEL_RGB:
			return RGBtoYUV(SetPixelRGB(x, y, YUVtoRGB(value)));
		case PIXEL_HLS:
			return RGBtoYUV(HLStoRGB(SetPixelHLS(x, y, RGBtoHLS(YUVtoRGB(value)))));
		default:
			throw ("No existe la funcion de conversion a YUV");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return value;
}

// Obtiene el valor de un pixel 
HLSValue CImage::GetPixelHLS(int x, int y)
{
	HLSValue ret;
	// Se tiene que haber creado la imagen
	if (m_bCreated) {

		switch (m_format.pixel) {
		case PIXEL_HLS:
			{
				assert ((x >= 0) && (x < m_width));
				assert ((y >= 0) && (y < m_height));
				if (method == HLS_24_BITS)	{
					// Optimizacion para formato "s[8]l[8]h[8]"
					int position = (x + (m_height-y-1) * m_width) * 3;
					ret.s = m_pData[position];   // ret.s = ret.s*256 + ret.s;
					ret.l = m_pData[position+1]; // ret.l = ret.l*256 + ret.l;
					ret.h = m_pData[position+2]; // ret.h = ret.h*256 + ret.h;
					return ret;
				} else {
					PIXEL_TYPE value = GetValue(x, y);
					// Red, Green and Blue
					ret.h = GetResult(m_format.hls.h, value, m_format.bpp, m_format.hls.hBits);
					ret.s = GetResult(m_format.hls.s, value, m_format.bpp, m_format.hls.sBits);
					ret.l = GetResult(m_format.hls.l, value, m_format.bpp, m_format.hls.lBits);
				}
				return ret;
			}
		case PIXEL_RGB:
			return RGBtoHLS(GetPixelRGB(x, y));
		case PIXEL_YUV:
			return RGBtoHLS(YUVtoRGB(GetPixelYUV(x, y)));
		default:
			throw ("No existe la funcion de conversion a HLS");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return ret;
}

// Actualiza el valor de un pixel 		
HLSValue CImage::SetPixelHLS(int x, int y, HLSValue value)
{
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_HLS:
			{
				assert ((x >= 0) && (x < m_width));
				assert ((y >= 0) && (y < m_height));
				if (method == HLS_24_BITS) {
					int position = (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
					m_pData[position]   = value.s / CONVERSION;
					m_pData[position+1] = value.l / CONVERSION;
					m_pData[position+2] = value.h / CONVERSION;
				} else {
					// Unimos los valores de las tres componentes
					// Red, Green and Blue
					PIXEL_TYPE hValue = SetResult(m_format.hls.h, value.h, m_format.bpp, m_format.hls.hBits);
					PIXEL_TYPE sValue = SetResult(m_format.hls.s, value.s, m_format.bpp, m_format.hls.sBits);
					PIXEL_TYPE lValue = SetResult(m_format.hls.l, value.l, m_format.bpp, m_format.hls.lBits);

					// Colocamos el valor en un única variable y actualizamos
					PIXEL_TYPE HLSValue = hValue | sValue | lValue;
					SetValue (HLSValue, x, y);
				}
				return value;
			}
		case PIXEL_RGB:
			return RGBtoHLS(SetPixelRGB(x, y, HLStoRGB(value)));
		case PIXEL_YUV:
			return RGBtoHLS(YUVtoRGB(SetPixelYUV(x, y, RGBtoYUV(HLStoRGB(value)))));
		default:
			throw ("No existe la funcion de conversion a HLS");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return value;
}

// Obtiene el valor de un pixel 
RGBValue CImage::GetPixelRGB(int x, int y)
{
	RGBValue ret;
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_RGB:
			{
				assert ((x >= 0) && (x < m_width));
				assert ((y >= 0) && (y < m_height));
				if (method == BGR_24_BITS) {
					// Optimizacion para formato "b[8]g[8]r[8]"
					int position = (x + (m_height-y-1) * m_width) * 3;
					ret.b = m_pData[position];   // ret.g = ret.g*256 + ret.g;
					ret.g = m_pData[position+1]; // ret.b = ret.b*256 + ret.b;
					ret.r = m_pData[position+2]; // ret.r = ret.r*256 + ret.r;
					return ret;
				} else if (method == RGB_24_BITS) {
					// Optimizacion para formato "r[8]g[8]b[8]"
					int position = (x + (m_height-y-1) * m_width) * 3;
					ret.r = m_pData[position];   // ret.g = ret.g*256 + ret.g;
					ret.g = m_pData[position+1]; // ret.b = ret.b*256 + ret.b;
					ret.b = m_pData[position+2]; // ret.r = ret.r*256 + ret.r;
					return ret;
				} else {
					PIXEL_TYPE value = GetValue(x, y);

					// Red, Green and Blue
					ret.r = GetResult(m_format.rgb.r, value, m_format.bpp, m_format.rgb.rBits);
					ret.g = GetResult(m_format.rgb.g, value, m_format.bpp, m_format.rgb.gBits);
					ret.b = GetResult(m_format.rgb.b, value, m_format.bpp, m_format.rgb.bBits);
				}
				return ret;
			}
		case PIXEL_RGBA:
			return RGBAtoRGB(GetPixelRGBA(x, y));
		case PIXEL_YUV:
			return YUVtoRGB(GetPixelYUV(x, y));
		case PIXEL_HLS:
			return HLStoRGB(GetPixelHLS(x, y));
		default:
			throw ("No existe la funcion de conversion a RGB");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return ret;
}

// Actualiza el valor de un pixel 		
RGBValue CImage::SetPixelRGB(int x, int y, RGBValue value)
{
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_RGB:
			{
				assert ((x >= 0) && (x < m_width));
				assert ((y >= 0) && (y < m_height));
				if (method == BGR_24_BITS) {
					BYTE* pData = m_pData + (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
					*pData = value.b / CONVERSION; pData++;
					*pData = value.g / CONVERSION; pData++;
					*pData = value.r / CONVERSION; 
				} else 	if (method == RGB_24_BITS) {
					BYTE* pData = m_pData + (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
					*pData = value.r / CONVERSION; pData++;
					*pData = value.g / CONVERSION; pData++;
					*pData = value.b / CONVERSION; 
				} else {
					// Unimos los valores de las tres componentes
					// Red, Green and Blue
					PIXEL_TYPE rValue = SetResult(m_format.rgb.r, value.r, m_format.bpp, m_format.rgb.rBits);
					PIXEL_TYPE gValue = SetResult(m_format.rgb.g, value.g, m_format.bpp, m_format.rgb.gBits);
					PIXEL_TYPE bValue = SetResult(m_format.rgb.b, value.b, m_format.bpp, m_format.rgb.bBits);

					// Colocamos el valor en un única variable y actualizamos
					PIXEL_TYPE rgbValue = rValue | gValue | bValue;
					SetValue (rgbValue, x, y);
				}
			return value;
			}
		case PIXEL_RGBA:
			return RGBAtoRGB(SetPixelRGBA(x, y, RGBtoRGBA(value)));
		case PIXEL_YUV:
			return YUVtoRGB(SetPixelYUV(x, y, RGBtoYUV(value)));
		case PIXEL_HLS:
			return HLStoRGB(SetPixelHLS(x, y, RGBtoHLS(value)));
		default:
			throw ("No existe la funcion de conversion a RGB");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return value;
}

// Obtiene el valor de un pixel 
RGBAValue CImage::GetPixelRGBA(int x, int y)
{
	RGBAValue ret;
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_RGBA:
			{
				assert ((x >= 0) && (x < m_width));
				assert ((y >= 0) && (y < m_height));
				if (method == BGRA_32_BITS) {
					// Optimizacion para formato "b[8]g[8]r[8]"
/*					BYTE* pData = m_pData + (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
					ret.b = *pData; pData++;
					ret.g = *pData; pData++;
					ret.r = *pData; 
*/
					int position = (x + (m_height-y-1) * m_width) * 4;
					ret.b = m_pData[position];   // ret.g = ret.g*256 + ret.g;
					ret.g = m_pData[position+1]; // ret.b = ret.b*256 + ret.b;
					ret.r = m_pData[position+2]; // ret.r = ret.r*256 + ret.r;
					ret.a = m_pData[position+3]; // ret.a = ret.a*256 + ret.a;
					return ret;
//*/
				} else {
					PIXEL_TYPE value = GetValue(x, y);

					// Red, Green, Blue and Alpha
					ret.r = GetResult(m_format.rgba.r, value, m_format.bpp, m_format.rgba.rBits);
					ret.g = GetResult(m_format.rgba.g, value, m_format.bpp, m_format.rgba.gBits);
					ret.b = GetResult(m_format.rgba.b, value, m_format.bpp, m_format.rgba.bBits);
					ret.a = GetResult(m_format.rgba.a, value, m_format.bpp, m_format.rgba.aBits);
				}
				return ret;
			}
		case PIXEL_RGB:
			return RGBtoRGBA(GetPixelRGB(x, y));
		default:
			throw ("No existe la funcion de conversion a RGBA");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return ret;
}

// Actualiza el valor de un pixel 		
RGBAValue CImage::SetPixelRGBA(int x, int y, RGBAValue value)
{
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		switch (m_format.pixel) {
		case PIXEL_RGBA:
			{
				assert ((x >= 0) && (x < m_width));
				assert ((y >= 0) && (y < m_height));
				if (method == BGRA_32_BITS) {
/*					int position = (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
					m_pData[position]   = value.b / CONVERSION;
					m_pData[position+1] = value.g / CONVERSION;
					m_pData[position+2] = value.r / CONVERSION;
*/
					BYTE* pData = m_pData + (x + (m_height-y-1) * m_width) * m_format.bpp / 8;
					*pData = value.b / CONVERSION; pData++;
					*pData = value.g / CONVERSION; pData++;
					*pData = value.r / CONVERSION; pData++;
					*pData = value.a / CONVERSION; 
//*/
				} else {
					// Unimos los valores de las tres componentes
					// Red, Green and Blue
					PIXEL_TYPE rValue = SetResult(m_format.rgba.r, value.r, m_format.bpp, m_format.rgba.rBits);
					PIXEL_TYPE gValue = SetResult(m_format.rgba.g, value.g, m_format.bpp, m_format.rgba.gBits);
					PIXEL_TYPE bValue = SetResult(m_format.rgba.b, value.b, m_format.bpp, m_format.rgba.bBits);
					PIXEL_TYPE aValue = SetResult(m_format.rgba.a, value.a, m_format.bpp, m_format.rgba.aBits);

					// Colocamos el valor en un única variable y actualizamos
					PIXEL_TYPE rgbaValue = rValue | gValue | bValue | aValue;
					SetValue (rgbaValue, x, y);
				}
			return value;
			}
		default:
			throw ("No existe la funcion de conversion a RGB");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return value;
}

// Obtiene el valor del pixel
BWValue CImage::GetPixelBW(int x, int y)
{
	BWValue ret;
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		RGBValue rgb;
		PIXEL_TYPE value;
		switch (m_format.pixel) {
		case PIXEL_RGB:
			rgb = GetPixelRGB(x, y);
			ret = (rgb.r + rgb.g + rgb.b)/3;
			break;
		case PIXEL_BW:
			if (method == GRAY_16_BITS) {
				int position = (x + (m_height-y-1) * m_width) * 2;
				ret = *(unsigned short*)(&m_pData[position]);
				return ret;
			} else {
				value = GetValue(x, y);
				ret = GetResult(m_format.bw.bw, value, m_format.bpp, m_format.bw.bwBits);
			}
			break;
		default:
			throw ("No existe la funcion de conversion a BW");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return ret;
}

// Actualiza el valor de un pixel 
BWValue CImage::SetPixelBW(int x, int y, BWValue value)
{
	// Se tiene que haber creado la imagen
	if (m_bCreated) {
		RGBValue rgb;
		switch (m_format.pixel) {
		case PIXEL_RGB:
			rgb.r = rgb.g = rgb.b = value;
			rgb = SetPixelRGB(x, y, rgb);
			return ((rgb.r+rgb.g+rgb.b)/3);
			break;
		case PIXEL_BW:
			{
				PIXEL_TYPE bwValue= SetResult(m_format.bw.bw, value, m_format.bpp, m_format.bw.bwBits);

				// Actualizamos
				SetValue (bwValue, x, y);
				return value;
			}
			break;
		default:
			throw ("No existe la funcion de conversion de BW");
		}
	} else {
		throw ("No se ha creado la imagen");
	}
	return value;
}

//////////////////////////////////////////////////////////////////////
// Funciones de grabar y leer imágenes
//////////////////////////////////////////////////////////////////////

// Función para grabar la imagen, no contendrá cabecera!!!!!!
bool CImage::SaveRaw(const char *name)
{
	if (m_bCreated) {
		FILE* file;
		fopen_s(&file, name, "w+b");
		if (file == NULL)
			return false;
		size_t writed = fwrite(m_pData, sizeof(BYTE), m_width * m_height, file);
		if (writed != m_width * m_height) {
			fclose(file);
			return false;
		}
		fclose(file);
		return true;
	} else 
		return false;
}

// Función para cargar una imagen en RAW data, primero se debe haber creado con la
//   función de Create o Load, ya que no tenemos información del formato
bool CImage::LoadRaw(const char *name)
{
	if (m_bCreated) {
		FILE* file;
		fopen_s(&file, name, "rb");
		if (file == NULL)
			return false;
		size_t writed = fread(m_pData, sizeof(BYTE), m_width * m_height, file);
		if (writed != m_width * m_height) {
			fclose(file);
			return false;
		}
		fclose(file);
		return true;
	} else 
		return false;
}

bool CImage::SavePPF(const char *name)
{
	if (m_bCreated) {
		FILE *file;
		fopen_s(&file, name,"wb+");
		if( file == NULL)
		  return false;
		/*
		P6
		# CREATOR: The GIMP's PNM Filter Version 1.0
		256 256
		255
		data
			*/
		fprintf(file, "P6\n");
		fprintf(file, "# CREATOR: Image Library U.I.B.\n");
		fprintf(file, "%d %d\n%d\n", m_width, m_height, 255);

		HLSValue value;
		for (int y=0; y < m_height; ++y) {
			for (int x=0; x < m_width; ++x) {
				BYTE aux;
				value = GetPixelHLS(x, y);
				aux = value.h >> (MAX_BITS - 8);
				Escribe (aux);
				aux = value.l >> (MAX_BITS - 8);
				Escribe (aux);
				aux = value.s >> (MAX_BITS - 8);
				Escribe (aux);
			}
		}

		fclose (file);
		return true;

	} else {
		return false;
	}
}

bool CImage::LoadPPF(const char *name)
{
	if (!m_bCreated) {
 
		FILE *file;
		UINT uBytesReaded = 0;

		char comments[255], formato[10], c; int quantization;
		fopen_s(&file, name,"rb");
		if(file == NULL)
		  return false;
		/*
		P6
		# CREATOR: The GIMP's PNM Filter Version 1.0
		256 256
		255
		data
		*/

		pixelFormat format;

		fscanf_s(file,"%s",formato);
		fread_s(&c,1,1,1, file); /*Advance on last return */
		fscanf_s(file,"%[^\n]", comments);
		fscanf_s(file,"%d %d", &m_width, &m_height);
		fscanf_s(file,"%d", &quantization);

		// Creamos la imagen
		format.pixel = PIXEL_HLS;
		format.bpp = 24;
		format.hls.hBits = 8;
		format.hls.h = 0x0000FF;
		format.hls.lBits = 8;
		format.hls.l = 0x00FF00;
		format.hls.sBits = 8;
		format.hls.s = 0xFF0000;
		Create(m_width, m_height, format);
		
		HLSValue value;
		for (int y=0; y < m_height; ++y) {
			for (int x=0; x < m_width; ++x) {
				BYTE aux;
				uBytesReaded += Read (aux);
				value.h = Expand(aux, MAX_BITS - 8);
				uBytesReaded += Read (aux);
				value.l = Expand(aux, MAX_BITS - 8);
				uBytesReaded += Read (aux);
				value.s = Expand(aux, MAX_BITS - 8);
				SetPixelHLS(x, y, value);
			}
		}
		fclose(file);
		if (uBytesReaded != m_height * m_width * 3)
			return false;
		return true;
	} else {
		return false;
	}
}


bool CImage::SaveBMP(const char *name)
{
	// Miramos que la imagen ya este creada
	if (!m_bCreated)
		return false;
	BMFILEHEADER cabecera;
	// Los dos primeros chars ha de ser "BM"=19778
	cabecera.bfType = 19778; 
	//     Cabecera del fichero + cabecera informacion + imagen
	cabecera.bfSize = 14 + 40 + m_width * m_height * 3;
	if ((m_width % 4) != 0)
		cabecera.bfSize = 14 + 40 + (m_width*3 + m_width%4) * m_height;
	cabecera.bfReserved1 = 0;
	cabecera.bfReserved2 = 0;
	cabecera.bfOffBits = 54; // Cabecera del fihcero + cabecera informacion

	BMINFO info;
	info.bmiHeader.biSize = 40;
	info.bmiHeader.biHeight = m_height;
	info.bmiHeader.biWidth = m_width;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 24;
	info.bmiHeader.biCompression = BM_RGB;
	info.bmiHeader.biSizeImage = m_width * m_height * 3;
	if ((m_width % 4) != 0)
		info.bmiHeader.biSizeImage = (m_width*3 + m_width%4) * m_height;
	info.bmiHeader.biXPelsPerMeter = 1700;
	info.bmiHeader.biYPelsPerMeter = 1700;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;

	FILE* file;
	
	fopen_s(&file, name, "wb+");
	if (file == NULL)
		return false;

	// Escribimos la cabecera
	Escribe(cabecera.bfType);
	Escribe(cabecera.bfSize);
	Escribe(cabecera.bfReserved1);
	Escribe(cabecera.bfReserved2);
	Escribe(cabecera.bfOffBits);

	// Escribimos la cabecera
	Escribe(info.bmiHeader.biSize);
	Escribe(info.bmiHeader.biWidth);
	Escribe(info.bmiHeader.biHeight);
	Escribe(info.bmiHeader.biPlanes);
	Escribe(info.bmiHeader.biBitCount);
	Escribe(info.bmiHeader.biCompression);
	Escribe(info.bmiHeader.biSizeImage);
	Escribe(info.bmiHeader.biXPelsPerMeter);
	Escribe(info.bmiHeader.biYPelsPerMeter);
	Escribe(info.bmiHeader.biClrUsed);
	Escribe(info.bmiHeader.biClrImportant);

	BYTE aux;
	if (method == GRAY_16_BITS) {
		BWValue value;
		for (int y=info.bmiHeader.biHeight-1; y >= 0 ; --y) {
			for (int x=0; x < info.bmiHeader.biWidth; x++) {
				value = GetPixelBW(x,y);
				aux = (BYTE)(value/256);
				Escribe(aux);
				aux = (BYTE)(value%256);
				Escribe(aux);
				aux = (BYTE)(0);
				Escribe(aux);
			}

			// Insertamos pixeles imaginarios para que el numero de pixeles por fila sea
			//  múltiplo de 4
			for (int i = 0; i <  m_width % 4; ++i) {
				Escribe(aux);//Escribe(aux);Escribe(aux);
			}
		}
	} else {
		RGBValue value;
		for (int y=info.bmiHeader.biHeight-1; y >= 0 ; --y) {
			for (int x=0; x < info.bmiHeader.biWidth; x++) {
				value = GetPixelRGB(x,y);
				aux = (BYTE)(value.b / ((MAX_VALUE +1) >> (MAX_BITS-8)));
				Escribe(aux);
				aux = (BYTE)(value.g / ((MAX_VALUE +1) >> (MAX_BITS-8)));
				Escribe(aux);
				aux = (BYTE)(value.r / ((MAX_VALUE +1) >> (MAX_BITS-8)));
				Escribe(aux);
			}

			// Insertamos pixeles imaginarios para que el numero de pixeles por fila sea
			//  múltiplo de 4
			for (int i = 0; i <  m_width % 4; ++i) {
				Escribe(aux);//Escribe(aux);Escribe(aux);
			}
		}
	}

	fclose (file);

	return true;
}

bool CImage::LoadBMPMemory(BYTE* pBitmap, DWORD bfOffBits)
{
	BMINFO info;
	int index = 0;
	RGB_QUAD* paleta = NULL;
	bool bIndexed = false;

	// Leemos BMINFO
	// Leemos BMINFOHEADER

	info.bmiHeader = *((BMINFOHEADER*)pBitmap);
	index = info.bmiHeader.biSize;
	assert (info.bmiHeader.biCompression == BM_RGB);

	// Creamos la imagen
	pixelFormat format;
	UINT height = info.bmiHeader.biHeight;
	UINT width = info.bmiHeader.biWidth;
	format.pixel = PIXEL_RGB;
	format.bpp = info.bmiHeader.biBitCount;
	switch (info.bmiHeader.biBitCount) {
		UINT i;
	case 32:
		format.bpp = 24;
		format.rgb.rBits = 8;
		format.rgb.r = 0x0000FF;
		format.rgb.gBits = 8;
		format.rgb.g = 0x00FF00;
		format.rgb.bBits = 8;
		format.rgb.b = 0xFF0000;
		break;
	case 24:
		format.rgb.rBits = 8;
		format.rgb.r = 0x0000FF;
		format.rgb.gBits = 8;
		format.rgb.g = 0x00FF00;
		format.rgb.bBits = 8;
		format.rgb.b = 0xFF0000;
		break;
	case 16:
		// Fichero con indice de colores
		format.bpp = 24;
		format.rgb.rBits = 8;
		format.rgb.r = 0x0000FF;
		format.rgb.gBits = 8;
		format.rgb.g = 0x00FF00;
		format.rgb.bBits = 8;
		format.rgb.b = 0xFF0000;

		bIndexed =true;
		paleta = (RGB_QUAD*)malloc(sizeof(RGB_QUAD)*info.bmiHeader.biClrUsed);
		for (i = 0; i < info.bmiHeader.biClrUsed; ++i) {
			Lee (paleta[i].rgbBlue, BYTE);
			Lee (paleta[i].rgbGreen, BYTE);
			Lee (paleta[i].rgbRed, BYTE);
			Lee (paleta[i].rgbReserved, BYTE);
		}
		break;
	case 8:
		// Fichero con indice de colores
		format.bpp = 24;
		format.rgb.rBits = 8;
		format.rgb.r = 0x0000FF;
		format.rgb.gBits = 8;
		format.rgb.g = 0x00FF00;
		format.rgb.bBits = 8;
		format.rgb.b = 0xFF0000;

		bIndexed =true;
		paleta = (RGB_QUAD*)malloc(sizeof(RGB_QUAD)*info.bmiHeader.biClrUsed);
		for (i = 0; i < info.bmiHeader.biClrUsed; ++i) {
			Lee (paleta[i].rgbBlue, BYTE);
			Lee (paleta[i].rgbGreen, BYTE);
			Lee (paleta[i].rgbRed, BYTE);
			Lee (paleta[i].rgbReserved, BYTE);
		}
		break;
	case 4:
		// Fichero con indice de colores
		format.bpp = 24;
		format.rgb.rBits = 8;
		format.rgb.r = 0x0000FF;
		format.rgb.gBits = 8;
		format.rgb.g = 0x00FF00;
		format.rgb.bBits = 8;
		format.rgb.b = 0xFF0000;

		bIndexed =true;
		paleta = (RGB_QUAD*)malloc(sizeof(RGB_QUAD)*info.bmiHeader.biClrUsed);
		for (i = 0; i < info.bmiHeader.biClrUsed; ++i) {
			Lee (paleta[i].rgbBlue, BYTE);
			Lee (paleta[i].rgbGreen, BYTE);
			Lee (paleta[i].rgbRed, BYTE);
			Lee (paleta[i].rgbReserved, BYTE);
		}
		break;
	case 1:
		// Fichero con indice de colores
		format.bpp = 24;
		format.rgb.rBits = 8;
		format.rgb.r = 0x0000FF;
		format.rgb.gBits = 8;
		format.rgb.g = 0x00FF00;
		format.rgb.bBits = 8;
		format.rgb.b = 0xFF0000;

		bIndexed =true;
		paleta = (RGB_QUAD*)malloc(sizeof(RGB_QUAD)*info.bmiHeader.biClrUsed);
		for (i = 0; i < info.bmiHeader.biClrUsed; ++i) {
			Lee (paleta[i].rgbBlue, BYTE);
			Lee (paleta[i].rgbGreen, BYTE);
			Lee (paleta[i].rgbRed, BYTE);
			Lee (paleta[i].rgbReserved, BYTE);
		}
		break;
	default:
		throw ("Caso no tratado");
		break;
	}
	// Miramos que la imagen no este ya creada
	if (!m_bCreated) {
		if (Create(width, height, format) == false) {
			if (paleta != NULL)
				delete paleta;
			return false;
		}
	} else {
		assert (width == GetWidth());
		assert (height == GetHeight());
		assert (format.bpp == m_format.bpp);
	}

	if (bfOffBits != 0)
		index = bfOffBits - 14;
	else
		index = info.bmiHeader.biSize + info.bmiHeader.biClrUsed * 4;

	if (info.bmiHeader.biBitCount == 24) {
		int iDesp = 0;
		if (info.bmiHeader.biSizeImage != 0) {
			UINT over = (UINT)(info.bmiHeader.biSizeImage - info.bmiHeader.biHeight*info.bmiHeader.biWidth*(float(info.bmiHeader.biBitCount)/8));
			iDesp = over / info.bmiHeader.biHeight;
		}
		// Optimizacion de lectura para 24 bits
		if (iDesp > 0) {
			BYTE*	pAux = m_pData;
			int		iBytes = sizeof(BYTE)*3*info.bmiHeader.biWidth;
			for (int y=info.bmiHeader.biHeight-1; y >= 0 ; --y) {
				memcpy(pAux, &pBitmap[index], iBytes);
				index += iBytes + iDesp;
				pAux += iBytes;
			}
		} else {
			int		iBytes = sizeof(BYTE)*3*info.bmiHeader.biWidth*info.bmiHeader.biHeight;
			memcpy(m_pData, &pBitmap[index], iBytes);
			index += iBytes;
		}
	} else {
		PIXEL_TYPE value;
		for (int y=info.bmiHeader.biHeight-1; y >= 0 ; --y) {
			for (int x=0; x < info.bmiHeader.biWidth; x++) {
				switch (info.bmiHeader.biBitCount) {
				case 32:
					{
						WORD aux1;
						WORD aux2;
						Lee (aux1, WORD);
						Lee (aux2, WORD);

						// Insertamos el pixel
						// b = aux1%256
						// g = aux1/256 
						// r = aux2%256

						int aux = aux1/256;
						value = (((aux1%256)*256+(aux1/256))*256)+(aux2%256);

						SetValue(value, x, y);
					}
					break;
				case 24:
					// NO debería llegar aquí ya que este caso esta optimizado
				case 16:
					{
						WORD aux;
						Lee (aux,  WORD);

						if (aux >= info.bmiHeader.biClrUsed) {
							if (paleta!=NULL)
								free(paleta);
							return false;
						}

						// Insertamos el pixel
						value = paleta[aux].rgbRed + 
							(paleta[aux].rgbGreen +
							paleta[aux].rgbBlue * 256)*256;

						SetValue(value, x, y);
					}
					break;
				case 8:
					{
						BYTE aux;
						Lee (aux,  BYTE);

						if (aux >= info.bmiHeader.biClrUsed) {
							if (paleta!=NULL)
								free(paleta);
							return false;
						}

						// Insertamos el pixel
						value = paleta[aux].rgbRed + 
							(paleta[aux].rgbGreen +
							paleta[aux].rgbBlue * 256)*256;

						SetValue(value, x, y);
					}
					break;
				case 4:
					{
						BYTE aux, aux2;
						Lee (aux,  BYTE);

						// Insertamos el primer pixel
						aux2 = aux >> 4;
						aux2 = (BYTE)(aux2 % info.bmiHeader.biClrUsed);
						if (aux2 >= info.bmiHeader.biClrUsed) {
							if (paleta!=NULL)
								free(paleta);
							return false;
						}
						value = paleta[aux2].rgbRed + 
							(paleta[aux2].rgbGreen +
							paleta[aux2].rgbBlue * 256)*256;
						SetValue(value, x, y);

						x++;

						// Insertamos el segundo pixel
						aux2 = aux & 0x0F;
						aux2 = (BYTE)(aux2 % info.bmiHeader.biClrUsed);
						if (aux2 >= info.bmiHeader.biClrUsed) {
							if (paleta!=NULL)
								free(paleta);
							return false;
						}
						value = paleta[aux2].rgbRed + 
							(paleta[aux2].rgbGreen +
							paleta[aux2].rgbBlue * 256)*256;
						SetValue(value, x, y);

					}
					break;
				case 1:
					{
						BYTE aux;
						Lee (aux,  BYTE);

						// Insertamos el pixel
						for (int j=0; j <8; ++j,x++) {
							if ((aux & 128) != 0 ) {
								value = paleta[1].rgbRed + 
									(paleta[1].rgbGreen +
									paleta[1].rgbBlue * 256)*256;
							} else {
								value = paleta[0].rgbRed + 
									(paleta[0].rgbGreen +
									paleta[0].rgbBlue * 256)*256;
							}
							aux = aux << 1;

							if ( x < info.bmiHeader.biWidth)
								SetValue(value, x, y);
						}
						x--;
					}
					break;
				default:
					throw ("Caso no tratado");
					break;
				}
			}
		}

		// Avanzamos la sobra de cada fila
		if (info.bmiHeader.biSizeImage != 0) {
			UINT over = (UINT)(info.bmiHeader.biSizeImage - info.bmiHeader.biHeight*info.bmiHeader.biWidth*(float(info.bmiHeader.biBitCount)/8));
			index += over / info.bmiHeader.biHeight;
		}
	}
		
	if (paleta!=NULL)
		free(paleta);
	return true;
}

bool CImage::LoadBMP(const char *name)
{
	BMFILEHEADER cabecera;

	FILE* file;

	fopen_s(&file, name, "rb");
	if (file == NULL)
		return false;

	fread (&cabecera.bfType, sizeof(cabecera.bfType), 1, file);
	fread (&cabecera.bfSize, sizeof(cabecera.bfSize), 1, file);
	fread (&cabecera.bfReserved1, sizeof(cabecera.bfReserved1), 1, file);
	fread (&cabecera.bfReserved2, sizeof(cabecera.bfReserved2), 1, file);
	fread (&cabecera.bfOffBits, sizeof(cabecera.bfOffBits), 1, file);

	if (cabecera.bfType != 19778)
		// Los dos primeros chars ha de ser "BM"=19778
		return false;

	// Leemos el resto del fichero
	BYTE* stream = new BYTE[cabecera.bfSize-sizeof(cabecera) + 1];
	if (fread (stream, sizeof(BYTE), cabecera.bfSize-sizeof(cabecera), file) !=  cabecera.bfSize-sizeof(cabecera)) {
		delete stream;
		return false;
	}
	if (feof(file)) {
		delete stream;
		return false;
	}
	bool bLoaded = LoadBMPMemory (stream, cabecera.bfOffBits);
	delete stream;
	fclose (file);
	return bLoaded;
}
	
//////////////////////////////////////////////////////////////////////
// Funciones de conversión entre formatos de color
//////////////////////////////////////////////////////////////////////

// Funcion de conversion de RGB a BW
BWValue CImage::RGBtoBW (RGBValue rgb)
{
	return (rgb.r+rgb.g+rgb.b)/3;
}

// Funcion de conversion de HLS a BW
BWValue CImage::HLStoBW (HLSValue hls)
{
	return hls.l;
}

// Funcion de conversion de YUV a BW
BWValue CImage::YUVtoBW (YUVValue yuv)
{
	return yuv.y;
}

RGBValue CImage::BWtoRGB (BWValue value)
{
	RGBValue rgb;
	rgb.r = rgb.g = rgb.b = value;
	return rgb;
}

HLSValue CImage::BWtoHLS (BWValue value)
{
	HLSValue hls;
	hls.l = value;
	hls.h = hls.s = 0;
	return hls;
}

YUVValue CImage::BWtoYUV (BWValue value)
{
	RGBValue rgb;
	rgb.r = rgb.g = rgb.b = value;
	return CImage::RGBtoYUV(rgb);
}

// Funcion de conversion de RGB a hls
HLSValue CImage::RGBtoHLS (RGBValue rgb)
{
	HLSValue ret;
	float h, l, s;
	float i, j;
	float maxval, minval;

	const float SQRT_3 = 1.73205776481628f;
	const float SQRT_2 = 1.41421358169861f;
	const float PI = 3.1415927410125731f;

	// Calculo de HLS del frame
	l = (rgb.r + rgb.g + rgb.b) / 3.0f;
	if ((rgb.r == rgb.b) && (rgb.r == rgb.g)) {
		h = 0;
		s = 0;
	} else {
		i = (2.0f*rgb.r - rgb.b - rgb.g);
		j = ((rgb.g - float(rgb.b))*SQRT_3);
		h = atanf(j/i)*(MAX_VALUE+1)/(2*PI);
		if (i < 0)
			h = (MAX_VALUE+1)/2+h;

		maxval = float(max(rgb.r, max(rgb.g, rgb.b)));
		minval = float(min(rgb.r, min(rgb.g, rgb.b)));

		s = (1 - minval/maxval)*MAX_VALUE;
	}

	if (h < 0)
		h += MAX_VALUE+1;

	ret.h = (UINT) h;
	ret.l = (UINT) l;
	ret.s = (UINT) s;

/*	float r = value.r / float(MAX_VALUE);
	float g = value.g / float(MAX_VALUE);
	float b = value.b / float(MAX_VALUE);

	double h, s, l;
	// If s == 0, then h is undefined.
  
	// The procedure described in Foley & van Dam places red at 0 degrees of
	// hue angle, whereas DIGITAL VT3xx terminals place blue at 0 degrees.
	// This routine has blue at 0 degrees.

	#define ML_MIN(a,b) (((a)<(b))?(a):(b))
	#define ML_MAX(a,b) (((a)>(b))?(a):(b))

	// Reference: figure 13.36, page 595 of Foley & van Dam 
    float maxval;
    float minval;
    float delta;

    maxval = ML_MAX (r, ML_MAX (g, b));
    minval = ML_MIN (r, ML_MIN (g, b));

    l = (maxval + minval) / 2.0f; // This is the lightness value.

    if (maxval == minval) {
		// Achromatic case, because r==g==b
		s = 0.0f;
		h = 0.0f; // The Hue is UNDEFINED in this case.
    } else {
		// Chromatic case
		// Calculate the saturation.
		if (l <= 0.5f) {
			s = (maxval - minval) / (maxval + minval);
		} else {
			s = (maxval - minval) / (2.0f - maxval - minval);
		}
		// Calculate the hue. 
		delta = maxval - minval;
		if (r == maxval) {
			// The resulting color is between yellow and magenta.
			h = (g - b) / delta;
		} else {
			if (g == maxval) {
				// The resulting color is between cyan and yellow.
				h = 2.0f + (b - r) / delta;
			} else {
				if (b == maxval) {
					// The resulting color is between magenta and cyan. 
					h = 4.0f + (r - g) / delta;
				}
			}
		}
		h = h * 60.0f; // Convert to degrees
						 // zero hue to Blue at zero hue.
		while (h < 0.0f) // Be sure 0.0 <= hue <= 360.0
			h = h + 360.0f;

		while (h > 360.0f)
			h = h - 360.0f;
    } // End else Chromatic case.

	// Ahora tenemos el formato hls, debemos convertirlos a nuestro posible rango
	// H [0.0, 360.0] -> [0, MAX_VALUE]
	// S [0.0, 1.0] -> [0, MAX_VALUE]
	// L [0.0, 1.0] -> [0, MAX_VALUE]
	HLSValue ret;
	ret.h = (PIXEL_TYPE)(h / 360.0f * MAX_VALUE);
	ret.s = (PIXEL_TYPE)(s * MAX_VALUE);
	ret.l = (PIXEL_TYPE)(l * MAX_VALUE);
*/
	return ret;
}

// Funcion de conversion de HLS a YUV
YUVValue CImage::HLStoYUV (HLSValue value)
{
	return RGBtoYUV(HLStoRGB(value));
}

HLSValue CImage::YUVtoHLS (YUVValue value)
{
	return RGBtoHLS(YUVtoRGB(value));
}

// Funcion de conversion de HLS a RGB
RGBValue CImage::HLStoRGB (HLSValue value)
{
	double h = (double) value.h / MAX_VALUE;
	double s = (double) value.s / MAX_VALUE;
	double l = (double) value.l / MAX_VALUE;

	double r, g , b;
  
	// The procedure described in Foley & van Dam not work correctly
	// downloaded from Internet

    double v;

    v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
    if (v <= 0) {
		r = g = b = 0.0;
    } else {
		double m;
		double sv;
		int sextant;
		double fract, vsf, mid1, mid2;

		m = l + l - v;
		sv = (v - m ) / v;
		h *= 6.0;
		sextant = int(h);	
		fract = h - sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant) {
			case 0: r = v;		g = mid1;	b = m; break;
			case 1: r = mid2;	g = v;		b = m; break;
			case 2: r = m;		g = v;		b = mid1; break;
			case 3: r = m;		g = mid2;	b = v; break;
			case 4: r = mid1;	g = m;		b = v; break;
			case 5: r = v;		g = m;		b = mid2; break;
		}
    }

	RGBValue ret;

	ret.r = (PIXEL_TYPE) (r * MAX_VALUE);
	ret.g = (PIXEL_TYPE) (g * MAX_VALUE);
	ret.b = (PIXEL_TYPE) (b * MAX_VALUE);

	return ret;
}

YUVValue CImage::RGBtoYUV (RGBValue value)
{
	YUVValue ret;
	double aux;
	double r, g, b;
	double fact1, fact2, fact3;
	r = value.r;
	g = value.g;
	b = value.b;
	fact1 = 255/double(235-16);
	fact2 = double(128-16)*MAX_VALUE/255;
	fact3 = 255/double(240-16);

	aux = ( (0.257*r) + (0.504*g) + (0.098*b))*fact1;
	if (aux < 0)
		aux = 0;
	if (aux > MAX_VALUE)
		aux = MAX_VALUE;
	ret.y = (PIXEL_TYPE)aux;

	aux = ( (0.439*r) - (0.368*g) - (0.071*b) + fact2)*fact3;
	if (aux < 0)
		aux = 0;
	if (aux > MAX_VALUE)
		aux = MAX_VALUE;
	ret.u = (PIXEL_TYPE)aux;

	aux = (-(0.148*r) - (0.291*g) + (0.439*b) + fact2)*fact3;
	if (aux < 0)
		aux = 0;
	if (aux > MAX_VALUE)
		aux = MAX_VALUE;
	ret.v = (PIXEL_TYPE)aux;
	return ret;
}

RGBValue CImage::YUVtoRGB (YUVValue value)
{
	RGBValue ret;
	double y = (value.y/double(MAX_VALUE))*double(235-16)+16;
	double u = (value.u/double(MAX_VALUE))*double(240-16)+16;
	double v = (value.v/double(MAX_VALUE))*double(240-16)+16;

	double fact = MAX_VALUE/double(255);

	double aux;
	aux = (1.164 *(y - double(16))                            + 2.018 *(v - double(128)))*fact;
	if (aux < 0)
		aux = 0;
	if (aux > MAX_VALUE)
		aux = MAX_VALUE;
	ret.b = (PIXEL_TYPE)aux;

	aux = (1.164 *(y - double(16)) - 0.813 *(u - double(128)) - 0.391 *(v - double(128)))*fact;
	if (aux < 0)
		aux = 0;
	if (aux > MAX_VALUE)
		aux = MAX_VALUE;
	ret.g = (PIXEL_TYPE)aux;

	aux = (1.164 *(y - double(16)) + 1.596 *(u - double(128)))*fact;
	if (aux < 0)
		aux = 0;
	if (aux > MAX_VALUE)
		aux = MAX_VALUE;
	ret.r = (PIXEL_TYPE)aux;
	return ret;
}

// Funcion de conversion de RGBA a RGB
RGBValue CImage::RGBAtoRGB (RGBAValue rgba)
{
	RGBValue ret;
	ret.r = rgba.r;
	ret.g = rgba.g;
	ret.b = rgba.b;
	return ret;
}

// Funcion de conversion de RGB a RGBA
RGBAValue CImage::RGBtoRGBA (RGBValue rgb)
{
	RGBAValue ret;
	ret.r = rgb.r;
	ret.g = rgb.g;
	ret.b = rgb.b;
	ret.a = MAX_VALUE;
	return ret;
}
	
//////////////////////////////////////////////////////////////////////
// Funciones para obtener una textura OpenGL
//////////////////////////////////////////////////////////////////////

// Devuelve un buffer con una imagen lista para utilizar como textura OpenGL
BYTE* CImage::GetOpenGLTexture(int iSize)
{
	BYTE* pBuffer = new BYTE[iSize*iSize*3];
	GetOpenGLTexture(iSize, pBuffer);
	return pBuffer;
}

BYTE* CImage::GetOpenGLTexture(int width, int height)
{
	BYTE* pBuffer = new BYTE[width*height*3];
	GetOpenGLTexture(width, height, pBuffer);
	return pBuffer;
}


void CImage::GetOpenGLTexture(int iSize, BYTE* pBuffer)
{
	int x, y;
	switch (method) {
	case BGR_24_BITS:
		{
			int posDest, posSource;
			posDest = 0;
			float fAspX = (m_width - 1) / float (iSize-1);
			float fAspY = (m_height - 1) / float (iSize-1);
			for (y = iSize-1; y >= 0; --y) {
				for (x = 0; x < iSize; ++x) {
					posSource = (int(x*fAspX) + (m_height-(int(y*fAspY)+1)) * m_width) * 3;
					pBuffer[posDest++] = m_pData[posSource+2];
					pBuffer[posDest++] = m_pData[posSource+1];
					pBuffer[posDest++] = m_pData[posSource];
				}
			}
		} break;
	case RGB_24_BITS:
		{
			int posDest, posSource;
			posDest = 0;
			float fAspX = (m_width - 1) / float (iSize-1);
			float fAspY = (m_height - 1) / float (iSize-1);
			for (y = iSize-1; y >= 0; --y) {
				for (x = 0; x < iSize; ++x) {
					posSource = (int(x*fAspX) + (m_height-(int(y*fAspY)+1)) * m_width) * 3;
					pBuffer[posDest++] = m_pData[posSource];
					pBuffer[posDest++] = m_pData[posSource+1];
					pBuffer[posDest++] = m_pData[posSource+2];
				}
			}
		} break;
	case BGRA_32_BITS:
		{
			int posDest, posSource;
			posDest = 0;
			float fAspX = (m_width - 1) / float (iSize-1);
			float fAspY = (m_height - 1) / float (iSize-1);
			for (y = iSize-1; y >= 0; --y) {
				for (x = 0; x < iSize; ++x) {
					posSource = (int(x*fAspX) + (m_height-(int(y*fAspY)+1)) * m_width) * 4;
					pBuffer[posDest++] = m_pData[posSource+2];
					pBuffer[posDest++] = m_pData[posSource+1];
					pBuffer[posDest++] = m_pData[posSource];
				}
			}
		} break;
	case NO_FAST_METHOD:
		{
			int posDest = 0;
			RGBValue rgb;
			float fAspX = (m_width - 1) / float (iSize-1);
			float fAspY = (m_height - 1) / float (iSize-1);
			for (y = iSize-1; y >= 0; --y) {
				for (x = 0; x < iSize; ++x) {
					rgb = GetPixelRGB((int)(x*fAspX), (int)(y*fAspY));
					pBuffer[posDest++] = rgb.b / CONVERSION;
					pBuffer[posDest++] = rgb.g / CONVERSION;
					pBuffer[posDest++] = rgb.r / CONVERSION;
				}
			}
		} break;
	}
}

void CImage::GetOpenGLTexture(int width, int height, BYTE* pBuffer)
{
	int x, y;
	switch (method) {
	case BGR_24_BITS:
		{
			int posDest, posSource;
			posDest = 0;
			float fAspX = (m_width - 1) / float (width-1);
			float fAspY = (m_height - 1) / float (height-1);
			for (y = height-1; y >= 0; --y) {
				for (x = 0; x < width; ++x) {
					posSource = (int(x*fAspX) + (m_height-(int(y*fAspY)+1)) * m_width) * 3;
					pBuffer[posDest++] = m_pData[posSource+2];
					pBuffer[posDest++] = m_pData[posSource+1];
					pBuffer[posDest++] = m_pData[posSource];
				}
			}
		} break;
	case RGB_24_BITS:
		{
			int posDest, posSource;
			posDest = 0;
			float fAspX = (m_width - 1) / float (width-1);
			float fAspY = (m_height - 1) / float (height-1);
			for (y = height-1; y >= 0; --y) {
				for (x = 0; x < width; ++x) {
					posSource = (int(x*fAspX) + (m_height-(int(y*fAspY)+1)) * m_width) * 3;
					pBuffer[posDest++] = m_pData[posSource];
					pBuffer[posDest++] = m_pData[posSource+1];
					pBuffer[posDest++] = m_pData[posSource+2];
				}
			}
		} break;
	case BGRA_32_BITS:
		{
			int posDest, posSource;
			posDest = 0;
			float fAspX = (m_width - 1) / float (width-1);
			float fAspY = (m_height - 1) / float (height-1);
			for (y = height-1; y >= 0; --y) {
				for (x = 0; x < width; ++x) {
					posSource = (int(x*fAspX) + (m_height-(int(y*fAspY)+1)) * m_width) * 4;
					pBuffer[posDest++] = m_pData[posSource+2];
					pBuffer[posDest++] = m_pData[posSource+1];
					pBuffer[posDest++] = m_pData[posSource];
				}
			}
		} break;
	case NO_FAST_METHOD:
		{
			int posDest = 0;
			RGBValue rgb;
			float fAspX = (m_width - 1) / float (width-1);
			float fAspY = (m_height - 1) / float (height-1);
			for (y = height-1; y >= 0; --y) {
				for (x = 0; x < width; ++x) {
					rgb = GetPixelRGB((int)(x*fAspX), (int)(y*fAspY));
					pBuffer[posDest++] = rgb.b / CONVERSION;
					pBuffer[posDest++] = rgb.g / CONVERSION;
					pBuffer[posDest++] = rgb.r / CONVERSION;
				}
			}
		} break;
	}
}



//////////////////////////////////////////////
//                                          //
// Funciones de procesamiento de imagenes   //
//                                          //
//////////////////////////////////////////////


//////////////////////////////////////////////
// SMOOTH                                   //
//////////////////////////////////////////////
// Suaviza la imagen, distingue entre los distintos tipos de formato de imagen
void CImage::Smooth() {
	float matrix[9]; float divide;
	matrix[0] = 1;	matrix[1] = 2;	matrix[2] = 1;
	matrix[3] = 2;	matrix[4] = 4;	matrix[5] = 2;
	matrix[6] = 1;	matrix[7] = 2;	matrix[8] = 1;
	divide = 16;
	MultiplyByMatrix(matrix, divide);
}

//////////////////////////////////////////////
// AVERAGE                                  //
//////////////////////////////////////////////
// Realiza la media 
void CImage::Average() {
	float matrix[9]; float divide;
	matrix[0] = 1;	matrix[1] = 1;	matrix[2] = 1;
	matrix[3] = 1;	matrix[4] = 1;	matrix[5] = 1;
	matrix[6] = 1;	matrix[7] = 1;	matrix[8] = 1;
	divide = 9;
	MultiplyByMatrix(matrix, divide);
}

//////////////////////////////////////////////
// SHARPEN8                                 //
//////////////////////////////////////////////
// Realiza la media 
void CImage::Sharpen8(){
	float matrix[9]; float divide;
	matrix[0] = -1;	matrix[1] = -1;	matrix[2] = -1;
	matrix[3] = -1;	matrix[4] =  9;	matrix[5] = -1;
	matrix[6] = -1;	matrix[7] = -1;	matrix[8] = -1;
	divide = 1;
	MultiplyByMatrix(matrix, divide);
}

//////////////////////////////////////////////
// SHARPEN4                                 //
//////////////////////////////////////////////
// Realiza la media 
void CImage::Sharpen4(){
	float matrix[9]; float divide;
	matrix[0] =  0;	matrix[1] = -1;	matrix[2] = 0;
	matrix[3] = -1;	matrix[4] =  5;	matrix[5] = -1;
	matrix[6] =  0;	matrix[7] = -1;	matrix[8] = 0;
	divide = 1;
	MultiplyByMatrix(matrix, divide);
}

//////////////////////////////////////////////
// LAPLACIAN8                               //
//////////////////////////////////////////////
// Realiza la media 
void CImage::Laplacian8(){
	float matrix[9]; float divide;
	matrix[0] = -1;	matrix[1] = -1;	matrix[2] = -1;
	matrix[3] = -1;	matrix[4] =  8;	matrix[5] = -1;
	matrix[6] = -1;	matrix[7] = -1;	matrix[8] = -1;
	divide = 1;
	MultiplyByMatrix(matrix, divide);
}

//////////////////////////////////////////////
// LAPLACIAN4                               //
//////////////////////////////////////////////
// Realiza la media 
void CImage::Laplacian4(){
	float matrix[9]; float divide;
	matrix[0] =  0;	matrix[1] = -1;	matrix[2] = 0;
	matrix[3] = -1;	matrix[4] =  4;	matrix[5] = -1;
	matrix[6] =  0;	matrix[7] = -1;	matrix[8] = 0;
	divide = 1;
	MultiplyByMatrix(matrix, divide);
}

//////////////////////////////////////////////
// VERTEDGE                                 //
//////////////////////////////////////////////
// Realiza la media 
void CImage::VertEdge(){
	float matrix[9]; float divide;
	matrix[0] = -2;	matrix[1] =  0;	matrix[2] = +2;
	matrix[3] = -2;	matrix[4] =  0;	matrix[5] = +2;
	matrix[6] = -2;	matrix[7] =  0;	matrix[8] = +2;
	divide = 1;
	MultiplyByMatrix(matrix, divide, true);
}

//////////////////////////////////////////////
// HORZEDGE                                 //
//////////////////////////////////////////////
// Realiza la media 
void CImage::HorzEdge(){
	float matrix[9]; float divide;
	matrix[0] = -2;	matrix[1] = -2;	matrix[2] = -2;
	matrix[3] =  0;	matrix[4] =  0;	matrix[5] =  0;
	matrix[6] = +2;	matrix[7] = +2;	matrix[8] = +2;
	divide = 1;
	MultiplyByMatrix(matrix, divide, true);
}

//////////////////////////////////////////////
// SOBEL                                    //
//////////////////////////////////////////////
// Operador Sobel
CImage* CImage::Sobel2()
{
#ifdef USE_OPENCV
	pixelFormat format;
	format = this->GetFormat();
	CImage* ret = new CImage();
	ret->Create(m_width, m_height, format);

	format.pixel = PIXEL_BW;
	format.bw.bwBits = 8;
	format.bw.bw = 0xFF;
	CImage temp;
	temp.Create(m_width, m_height, format);

	CImage temp2;
//	format.bw.bwBits = 16;
//	format.bw.bw = 0xFFFF;
	temp2.Create(m_width, m_height, format);

	cvCvtColor(m_matriz, temp.m_matriz, CV_RGB2GRAY);

//	cvSobel( temp.m_matriz, temp2.m_matriz, 1, 0, 3 );	
//	cvLaplace( temp.m_matriz, temp2.m_matriz, 3 );	
	cvCanny( temp.m_matriz, temp.m_matriz, 33, 100, 3 );	

//	cvConvert( temp2.m_matriz, temp.m_matriz );
//	cvCvtColor( temp.m_matriz, ret->m_matriz, CV_GRAY2RGB );
	cvCvtColor( temp.m_matriz, ret->m_matriz, CV_GRAY2RGB );
	return ret;
#else 
	return NULL;
#endif
}

void CImage::Sobel(){
#ifdef USE_OPENCV
//	pixelFormat format;
//	format = this->GetFormat();
//	format.pixel = PIXEL_BW;
//	format.bw.bwBits = 8;
//	format.bw.bw = 0xFF;
//
//	CImage temp;
//	temp.Create(format);
//	cvCvtColor(m_matriz, temp.m_matriz, CV_RGB2GRAY);
//	cvSobel( m_matriz, temp.m_matriz, 1, 1, 3 );	
//	cvCvtColor(temp.m_matriz, m_matriz, CV_GRAY2RGB);
//#else
	float matrix1[9]; float divide1;
	float matrix2[9]; float divide2;
	float fDivideTotal;

	matrix1[0] =  1;	matrix1[1] =  2;	matrix1[2] =  1;
	matrix1[3] =  0;	matrix1[4] =  0;	matrix1[5] =  0;
	matrix1[6] = -1;	matrix1[7] = -2;	matrix1[8] = -1;
	divide1 = 1;

	matrix2[0] = -1;	matrix2[1] =  0;	matrix2[2] =  1;
	matrix2[3] = -2;	matrix2[4] =  0;	matrix2[5] =  2;
	matrix2[6] = -1;	matrix2[7] =  0;	matrix2[8] =  1;
	divide2 = 1;
	fDivideTotal = 2;

	MultiplyByTwoMatrix(matrix1, divide1, true,
		matrix2, divide2, true,
		fDivideTotal, COMBINATION_SUM, true);
#endif
}

//////////////////////////////////////////////
// PREWITT                                  //
//////////////////////////////////////////////
// Operador Prewitt
void CImage::Prewitt(){
	float matrix1[9]; float divide1;
	float matrix2[9]; float divide2;
	float fDivideTotal;

	matrix1[0] =  1;	matrix1[1] =  1;	matrix1[2] =  1;
	matrix1[3] =  0;	matrix1[4] =  0;	matrix1[5] =  0;
	matrix1[6] = -1;	matrix1[7] = -1;	matrix1[8] = -1;
	divide1 = 1;

	matrix2[0] = -1;	matrix2[1] =  0;	matrix2[2] =  1;
	matrix2[3] = -1;	matrix2[4] =  0;	matrix2[5] =  1;
	matrix2[6] = -1;	matrix2[7] =  0;	matrix2[8] =  1;
	divide2 = 1;
	fDivideTotal = 2;

	MultiplyByTwoMatrix(matrix1, divide1, true,
		matrix2, divide2, true,
		fDivideTotal, COMBINATION_SUM, true);
}

//////////////////////////////////////////////
// MultiplyByTwoMatrix                         //
//////////////////////////////////////////////
void CImage::MultiplyByTwoMatrix(
	float fMatrix1[9], float fDivide1,	bool bAbsolute1,
	float fMatrix2[9], float fDivide2,	bool bAbsolute2,
	float fDivideTotal,
	t_mode_combination mCombination,
	bool bSaturate)
{
	switch(m_format.pixel) {
	case PIXEL_RGB:
		MultiplyByTwoMatrixRGB(fMatrix1, fDivide1, bAbsolute1,
			fMatrix2, fDivide2,bAbsolute2, fDivideTotal,
			mCombination, bSaturate);
		break;
	case PIXEL_HLS:
		MultiplyByTwoMatrixHLS(fMatrix1, fDivide1, bAbsolute1,
			fMatrix2, fDivide2,bAbsolute2, fDivideTotal,
			mCombination, bSaturate);
		break;
	case PIXEL_BW:
		MultiplyByTwoMatrixBW(fMatrix1, fDivide1, bAbsolute1,
			fMatrix2, fDivide2,bAbsolute2, fDivideTotal,
			mCombination, bSaturate);
		break;
	}
}

void CImage::MultiplyByTwoMatrixRGB(
	float fMatrix1[9], float fDivide1,	bool bAbsolute1,
	float fMatrix2[9], float fDivide2,	bool bAbsolute2,
	float fDivideTotal,
	t_mode_combination mCombination,
	bool bSaturate)
{
	CImage temporal;
	pixelFormat format = GetFormat();

	assert(format.pixel == PIXEL_RGB);
	temporal.Create(m_width, m_height, format);

	int x, y;

	// Copiamos los bordes derecho e izquierdo de la imagen
	for (y = 0; y < m_height; ++y) {
		temporal.SetPixelRGB(0, y, GetPixelRGB(0, y));
		temporal.SetPixelRGB(m_width-1, y, GetPixelRGB(m_width-1, y));
	}
	// Copiamos los bordes superior e inferior de la imagen
	for (x = 0; x < m_width; ++x) {
		temporal.SetPixelRGB(x, 0, GetPixelRGB(x, 0));
		temporal.SetPixelRGB(x, m_height-1, GetPixelRGB(x, m_height-1));
	}

	// Tratamos el resto de la imagen
	RGBValue ret;
	RGBValue rgb1, rgb2, rgb3,
			 rgb4, rgb5, rgb6,
			 rgb7, rgb8, rgb9;
	int aux1, aux2;
	for (x = 1; x < m_width-1; ++x) {
		y = 1;
		// Obtenemos los puntos
		rgb1 = GetPixelRGB(x-1, y-1);
		rgb2 = GetPixelRGB(x  , y-1);
		rgb3 = GetPixelRGB(x+1, y-1);
		rgb4 = GetPixelRGB(x-1, y);
		rgb5 = GetPixelRGB(x, y);
		rgb6 = GetPixelRGB(x+1, y);
		for (; y < m_height-1; ++y) {
			// Avanzamos la nueva linea horizontal para este punto
			rgb7 = GetPixelRGB(x-1, y+1);
			rgb8 = GetPixelRGB(x  , y+1);
			rgb9 = GetPixelRGB(x+1, y+1);

			// Calculamos la componente R
			aux1 = int((
					rgb1.r*fMatrix1[0] + rgb2.r*fMatrix1[1] + rgb3.r*fMatrix1[2] +
					rgb4.r*fMatrix1[3] + rgb5.r*fMatrix1[4] + rgb6.r*fMatrix1[5] +
					rgb7.r*fMatrix1[6] + rgb8.r*fMatrix1[7] + rgb9.r*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					rgb1.r*fMatrix2[0] + rgb2.r*fMatrix2[1] + rgb3.r*fMatrix2[2] +
					rgb4.r*fMatrix2[3] + rgb5.r*fMatrix2[4] + rgb6.r*fMatrix2[5] +
					rgb7.r*fMatrix2[6] + rgb8.r*fMatrix2[7] + rgb9.r*fMatrix2[8])/fDivide2);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret.r = UINT(aux1);

			// Calculamos la componente G
			aux1 = int((
					rgb1.g*fMatrix1[0] + rgb2.g*fMatrix1[1] + rgb3.g*fMatrix1[2] +
					rgb4.g*fMatrix1[3] + rgb5.g*fMatrix1[4] + rgb6.g*fMatrix1[5] +
					rgb7.g*fMatrix1[6] + rgb8.g*fMatrix1[7] + rgb9.g*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					rgb1.g*fMatrix2[0] + rgb2.g*fMatrix2[1] + rgb3.g*fMatrix2[2] +
					rgb4.g*fMatrix2[3] + rgb5.g*fMatrix2[4] + rgb6.g*fMatrix2[5] +
					rgb7.g*fMatrix2[6] + rgb8.g*fMatrix2[7] + rgb9.g*fMatrix2[8])/fDivide2);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret.g = UINT(aux1);

			// Calculamos la componente B
			aux1 = int((
					rgb1.b*fMatrix1[0] + rgb2.b*fMatrix1[1] + rgb3.b*fMatrix1[2] +
					rgb4.b*fMatrix1[3] + rgb5.b*fMatrix1[4] + rgb6.b*fMatrix1[5] +
					rgb7.b*fMatrix1[6] + rgb8.b*fMatrix1[7] + rgb9.b*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					rgb1.b*fMatrix2[0] + rgb2.b*fMatrix2[1] + rgb3.b*fMatrix2[2] +
					rgb4.b*fMatrix2[3] + rgb5.b*fMatrix2[4] + rgb6.b*fMatrix2[5] +
					rgb7.b*fMatrix2[6] + rgb8.b*fMatrix2[7] + rgb9.b*fMatrix2[8])/fDivide2);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret.b = UINT(aux1);

			temporal.SetPixelRGB(x, y, ret);
			// Aqui avanzamos una linea hacia abajo
			rgb1 = rgb4;
			rgb2 = rgb5;
			rgb3 = rgb6;
			rgb4 = rgb7;
			rgb5 = rgb8;
			rgb6 = rgb9;
		}
	}
	memcpy(this->m_pData, temporal.m_pData, format.bpp/8*m_width*m_height);
}

void CImage::MultiplyByTwoMatrixHLS(
	float fMatrix1[9], float fDivide1,	bool bAbsolute1,
	float fMatrix2[9], float fDivide2,	bool bAbsolute2,
	float fDivideTotal,
	t_mode_combination mCombination,
	bool bSaturate)
{
	CImage temporal;
	pixelFormat format = GetFormat();

	assert(format.pixel == PIXEL_HLS);
	temporal.Create(m_width, m_height, format);

	int x, y;

	// Copiamos los bordes derecho e izquierdo de la imagen
	for (y = 0; y < m_height; ++y) {
		temporal.SetPixelHLS(0, y, GetPixelHLS(0, y));
		temporal.SetPixelHLS(m_width-1, y, GetPixelHLS(m_width-1, y));
	}
	// Copiamos los bordes superior e inferior de la imagen
	for (x = 0; x < m_width; ++x) {
		temporal.SetPixelHLS(x, 0, GetPixelHLS(x, 0));
		temporal.SetPixelHLS(x, m_height-1, GetPixelHLS(x, m_height-1));
	}

	// Tratamos el resto de la imagen
	HLSValue ret;
	HLSValue hls1, hls2, hls3,
			 hls4, hls5, hls6,
			 hls7, hls8, hls9;
	int aux1, aux2;
	for (x = 1; x < m_width-1; ++x) {
		y = 1;
		// Obtenemos los puntos
		hls1 = GetPixelHLS(x-1, y-1);
		hls2 = GetPixelHLS(x  , y-1);
		hls3 = GetPixelHLS(x+1, y-1);
		hls4 = GetPixelHLS(x-1, y);
		hls5 = GetPixelHLS(x, y);
		hls6 = GetPixelHLS(x+1, y);
		for (; y < m_height-1; ++y) {
			// Avanzamos la nueva linea horizontal para este punto
			hls7 = GetPixelHLS(x-1, y+1);
			hls8 = GetPixelHLS(x  , y+1);
			hls9 = GetPixelHLS(x+1, y+1);

			// Calculamos la componente H
			aux1 = int((
					hls1.h*fMatrix1[0] + hls2.h*fMatrix1[1] + hls3.h*fMatrix1[2] +
					hls4.h*fMatrix1[3] + hls5.h*fMatrix1[4] + hls6.h*fMatrix1[5] +
					hls7.h*fMatrix1[6] + hls8.h*fMatrix1[7] + hls9.h*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					hls1.h*fMatrix2[0] + hls2.h*fMatrix2[1] + hls3.h*fMatrix2[2] +
					hls4.h*fMatrix2[3] + hls5.h*fMatrix2[4] + hls6.h*fMatrix2[5] +
					hls7.h*fMatrix2[6] + hls8.h*fMatrix2[7] + hls9.h*fMatrix2[8])/fDivide2);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret.h = UINT(aux1);

			// Calculamos la componente L
			aux1 = int((
					hls1.l*fMatrix1[0] + hls2.l*fMatrix1[1] + hls3.l*fMatrix1[2] +
					hls4.l*fMatrix1[3] + hls5.l*fMatrix1[4] + hls6.l*fMatrix1[5] +
					hls7.l*fMatrix1[6] + hls8.l*fMatrix1[7] + hls9.l*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					hls1.l*fMatrix2[0] + hls2.l*fMatrix2[1] + hls3.l*fMatrix2[2] +
					hls4.l*fMatrix2[3] + hls5.l*fMatrix2[4] + hls6.l*fMatrix2[5] +
					hls7.l*fMatrix2[6] + hls8.l*fMatrix2[7] + hls9.l*fMatrix2[8])/fDivide2);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret.l = UINT(aux1);

			// Calculamos la componente B
			aux1 = int((
					hls1.s*fMatrix1[0] + hls2.s*fMatrix1[1] + hls3.s*fMatrix1[2] +
					hls4.s*fMatrix1[3] + hls5.s*fMatrix1[4] + hls6.s*fMatrix1[5] +
					hls7.s*fMatrix1[6] + hls8.s*fMatrix1[7] + hls9.s*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					hls1.s*fMatrix2[0] + hls2.s*fMatrix2[1] + hls3.s*fMatrix2[2] +
					hls4.s*fMatrix2[3] + hls5.s*fMatrix2[4] + hls6.s*fMatrix2[5] +
					hls7.s*fMatrix2[6] + hls8.s*fMatrix2[7] + hls9.s*fMatrix2[8])/fDivide2);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret.s = UINT(aux1);

			temporal.SetPixelHLS(x, y, ret);
			// Aqui avanzamos una linea hacia abajo
			hls1 = hls4;
			hls2 = hls5;
			hls3 = hls6;
			hls4 = hls7;
			hls5 = hls8;
			hls6 = hls9;
		}
	}
	memcpy(this->m_pData, temporal.m_pData, format.bpp/8*m_width*m_height);
}

void CImage::MultiplyByTwoMatrixBW(
	float fMatrix1[9], float fDivide1,	bool bAbsolute1,
	float fMatrix2[9], float fDivide2,	bool bAbsolute2,
	float fDivideTotal,
	t_mode_combination mCombination,
	bool bSaturate)
{
	CImage temporal;
	pixelFormat format = GetFormat();

	assert(format.pixel == PIXEL_BW);
	temporal.Create(m_width, m_height, format);

	int x, y;

	// Copiamos los bordes derecho e izquierdo de la imagen
	for (y = 0; y < m_height; ++y) {
		temporal.SetPixelBW(0, y, GetPixelBW(0, y));
		temporal.SetPixelBW(m_width-1, y, GetPixelBW(m_width-1, y));
	}
	// Copiamos los bordes superior e inferior de la imagen
	for (x = 0; x < m_width; ++x) {
		temporal.SetPixelBW(x, 0, GetPixelBW(x, 0));
		temporal.SetPixelBW(x, m_height-1, GetPixelBW(x, m_height-1));
	}

	// Tratamos el resto de la imagen
	BWValue ret;
	BWValue bw1, bw2, bw3, 
			bw4, bw5, bw6,
			bw7, bw8, bw9;
	int aux1, aux2;
	for (x = 1; x < m_width-1; ++x) {
		y = 1;
		// Obtenemos los puntos
		bw1 = GetPixelBW(x-1, y-1);
		bw2 = GetPixelBW(x  , y-1);
		bw3 = GetPixelBW(x+1, y-1);
		bw4 = GetPixelBW(x-1, y);
		bw5 = GetPixelBW(x, y);
		bw6 = GetPixelBW(x+1, y);
		for (; y < m_height-1; ++y) {
			// Avanzamos la nueva linea horizontal para este punto
			bw7 = GetPixelBW(x-1, y+1);
			bw8 = GetPixelBW(x  , y+1);
			bw9 = GetPixelBW(x+1, y+1);

			// Calculamos la componente
			aux1 = int((
					bw1*fMatrix1[0] + bw2*fMatrix1[1] + bw3*fMatrix1[2] +
					bw4*fMatrix1[3] + bw5*fMatrix1[4] + bw6*fMatrix1[5] +
					bw7*fMatrix1[6] + bw8*fMatrix1[7] + bw9*fMatrix1[8])/fDivide1);
			if (bAbsolute1)
				aux1 = abs(aux1);

			aux2 = int((
					bw1*fMatrix2[0] + bw2*fMatrix2[1] + bw3*fMatrix2[2] +
					bw4*fMatrix2[3] + bw5*fMatrix2[4] + bw6*fMatrix2[5] +
					bw7*fMatrix2[6] + bw8*fMatrix2[7] + bw9*fMatrix2[8])/fDivide1);
			if (bAbsolute2)
				aux2 = abs(aux2);

			switch(mCombination) {
			case COMBINATION_SUM:
				aux1 +=aux2;
				break;
			case COMBINATION_MAX:
				aux1 = max(aux1, aux2);
				break;
			}

			aux1 = int(aux1/fDivideTotal);

			if (bSaturate) {
				if (aux1 < 0)
					aux1 = 0;
				if (aux1 > MAX_VALUE)
					aux1 = MAX_VALUE;
			}
			ret = UINT(aux1);

			temporal.SetPixelBW(x, y, ret);
			// Aqui avanzamos una linea hacia abajo
			bw1 = bw4;
			bw2 = bw5;
			bw3 = bw6;
			bw4 = bw7;
			bw5 = bw8;
			bw6 = bw9;
		}
	}
	memcpy(this->m_pData, temporal.m_pData, format.bpp/8*m_width*m_height);
}

//////////////////////////////////////////////
// MultiplyByMatrix                         //
//////////////////////////////////////////////
void CImage::MultiplyByMatrix(float fMatrix[9], float fDivide,
							  bool bAbsolute, bool bSaturate)
{
	switch (m_format.pixel) {
	case PIXEL_RGB:
		MultiplyByMatrixRGB(fMatrix, fDivide, bAbsolute, bSaturate);
		break;
	case PIXEL_HLS:
		MultiplyByMatrixHLS(fMatrix, fDivide, bAbsolute, bSaturate);
		break;
	case PIXEL_BW:
		MultiplyByMatrixBW(fMatrix, fDivide, bAbsolute, bSaturate);
		break;
	}
}

void CImage::MultiplyByMatrixRGB(float fMatrix[9], float fDivide,
							  bool bAbsolute, bool bSaturate)
{

	CImage temporal;
	pixelFormat format = GetFormat();

	assert(format.pixel == PIXEL_RGB);
	temporal.Create(m_width, m_height, format);

	int x, y;

	// Copiamos los bordes derecho e izquierdo de la imagen
	for (y = 0; y < m_height; ++y) {
		temporal.SetPixelRGB(0, y, GetPixelRGB(0, y));
		temporal.SetPixelRGB(m_width-1, y, GetPixelRGB(m_width-1, y));
	}
	// Copiamos los bordes superior e inferior de la imagen
	for (x = 0; x < m_width; ++x) {
		temporal.SetPixelRGB(x, 0, GetPixelRGB(x, 0));
		temporal.SetPixelRGB(x, m_height-1, GetPixelRGB(x, m_height-1));
	}

	// Tratamos el resto de la imagen
	RGBValue ret;
	RGBValue rgb1, rgb2, rgb3,
			 rgb4, rgb5, rgb6,
			 rgb7, rgb8, rgb9;
	int aux;
	for (x = 1; x < m_width-1; ++x) {
		y = 1;
		// Obtenemos los puntos
		rgb1 = GetPixelRGB(x-1, y-1);
		rgb2 = GetPixelRGB(x  , y-1);
		rgb3 = GetPixelRGB(x+1, y-1);
		rgb4 = GetPixelRGB(x-1, y);
		rgb5 = GetPixelRGB(x, y);
		rgb6 = GetPixelRGB(x+1, y);
		for (; y < m_height-1; ++y) {
			// Avanzamos la nueva linea horizontal para este punto
			rgb7 = GetPixelRGB(x-1, y+1);
			rgb8 = GetPixelRGB(x  , y+1);
			rgb9 = GetPixelRGB(x+1, y+1);

			// Calculamos la componente R
			aux = int((
					rgb1.r*fMatrix[0] + rgb2.r*fMatrix[1] + rgb3.r*fMatrix[2] +
					rgb4.r*fMatrix[3] + rgb5.r*fMatrix[4] + rgb6.r*fMatrix[5] +
					rgb7.r*fMatrix[6] + rgb8.r*fMatrix[7] + rgb9.r*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret.r = UINT(aux);

			// Calculamos la componente G
			aux = int((
					rgb1.g*fMatrix[0] + rgb2.g*fMatrix[1] + rgb3.g*fMatrix[2] +
					rgb4.g*fMatrix[3] + rgb5.g*fMatrix[4] + rgb6.g*fMatrix[5] +
					rgb7.g*fMatrix[6] + rgb8.g*fMatrix[7] + rgb9.g*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret.g = UINT(aux);

			// Calculamos la componente B
			aux = int((
					rgb1.b*fMatrix[0] + rgb2.b*fMatrix[1] + rgb3.b*fMatrix[2] +
					rgb4.b*fMatrix[3] + rgb5.b*fMatrix[4] + rgb6.b*fMatrix[5] +
					rgb7.b*fMatrix[6] + rgb8.b*fMatrix[7] + rgb9.b*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret.b = UINT(aux);

			temporal.SetPixelRGB(x, y, ret);
			// Aqui avanzamos una linea hacia abajo
			rgb1 = rgb4;
			rgb2 = rgb5;
			rgb3 = rgb6;
			rgb4 = rgb7;
			rgb5 = rgb8;
			rgb6 = rgb9;
		}
	}
	memcpy(this->m_pData, temporal.m_pData, format.bpp/8*m_width*m_height);
}

void CImage::MultiplyByMatrixHLS(float fMatrix[9], float fDivide, bool bAbsolute, bool bSaturate)
{

	CImage temporal;
	pixelFormat format = GetFormat();

	assert(format.pixel == PIXEL_HLS);
	temporal.Create(m_width, m_height, format);

	int x, y;

	// Copiamos los bordes derecho e izquierdo de la imagen
	for (y = 0; y < m_height; ++y) {
		temporal.SetPixelHLS(0, y, GetPixelHLS(0, y));
		temporal.SetPixelHLS(m_width-1, y, GetPixelHLS(m_width-1, y));
	}
	// Copiamos los bordes superior e inferior de la imagen
	for (x = 0; x < m_width; ++x) {
		temporal.SetPixelHLS(x, 0, GetPixelHLS(x, 0));
		temporal.SetPixelHLS(x, m_height-1, GetPixelHLS(x, m_height-1));
	}

	// Tratamos el resto de la imagen
	HLSValue ret;
	HLSValue hls1, hls2, hls3,
			 hls4, hls5, hls6,
			 hls7, hls8, hls9;
	int aux;
	for (x = 1; x < m_width-1; ++x) {
		y = 1;
		// Obtenemos los puntos
		hls1 = GetPixelHLS(x-1, y-1);
		hls2 = GetPixelHLS(x  , y-1);
		hls3 = GetPixelHLS(x+1, y-1);
		hls4 = GetPixelHLS(x-1, y);
		hls5 = GetPixelHLS(x, y);
		hls6 = GetPixelHLS(x+1, y);
		for (; y < m_height-1; ++y) {
			// Avanzamos la nueva linea horizontal para este punto
			hls7 = GetPixelHLS(x-1, y+1);
			hls8 = GetPixelHLS(x  , y+1);
			hls9 = GetPixelHLS(x+1, y+1);

			// Calculamos la componente H
			aux = int((
					hls1.h*fMatrix[0] + hls2.h*fMatrix[1] + hls3.h*fMatrix[2] +
					hls4.h*fMatrix[3] + hls5.h*fMatrix[4] + hls6.h*fMatrix[5] +
					hls7.h*fMatrix[6] + hls8.h*fMatrix[7] + hls9.h*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret.h = UINT(aux);

			// Calculamos la componente L
			aux = int((
					hls1.l*fMatrix[0] + hls2.l*fMatrix[1] + hls3.h*fMatrix[2] +
					hls4.l*fMatrix[3] + hls5.l*fMatrix[4] + hls6.h*fMatrix[5] +
					hls7.l*fMatrix[6] + hls8.l*fMatrix[7] + hls9.h*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret.l = UINT(aux);

			// Calculamos la componente S
			aux = int((
					hls1.s*fMatrix[0] + hls2.s*fMatrix[1] + hls3.s*fMatrix[2] +
					hls4.s*fMatrix[3] + hls5.s*fMatrix[4] + hls6.s*fMatrix[5] +
					hls7.s*fMatrix[6] + hls8.s*fMatrix[7] + hls9.s*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret.s = UINT(aux);

			temporal.SetPixelHLS(x, y, ret);
			// Aqui avanzamos una linea hacia abajo
			hls1 = hls4;
			hls2 = hls5;
			hls3 = hls6;
			hls4 = hls7;
			hls5 = hls8;
			hls6 = hls9;
		}
	}
	memcpy(this->m_pData, temporal.m_pData, format.bpp/8*m_width*m_height);
}

void CImage::MultiplyByMatrixBW(float fMatrix[9], float fDivide,
							  bool bAbsolute, bool bSaturate)
{

	CImage temporal;
	pixelFormat format = GetFormat();

	assert(format.pixel == PIXEL_BW);
	temporal.Create(m_width, m_height, format);

	int x, y;

	// Copiamos los bordes derecho e izquierdo de la imagen
	for (y = 0; y < m_height; ++y) {
		temporal.SetPixelBW(0, y, GetPixelBW(0, y));
		temporal.SetPixelBW(m_width-1, y, GetPixelBW(m_width-1, y));
	}
	// Copiamos los bordes superior e inferior de la imagen
	for (x = 0; x < m_width; ++x) {
		temporal.SetPixelBW(x, 0, GetPixelBW(x, 0));
		temporal.SetPixelBW(x, m_height-1, GetPixelBW(x, m_height-1));
	}

	// Tratamos el resto de la imagen
	BWValue ret;
	BWValue bw1, bw2, bw3,
			bw4, bw5, bw6,
			bw7, bw8, bw9;
	int aux;
	for (x = 1; x < m_width-1; ++x) {
		y = 1;
		// Obtenemos los puntos
		bw1 = GetPixelBW(x-1, y-1);
		bw2 = GetPixelBW(x  , y-1);
		bw3 = GetPixelBW(x+1, y-1);
		bw4 = GetPixelBW(x-1, y);
		bw5 = GetPixelBW(x, y);
		bw6 = GetPixelBW(x+1, y);
		for (; y < m_height-1; ++y) {
			// Avanzamos la nueva linea horizontal para este punto
			bw7 = GetPixelBW(x-1, y+1);
			bw8 = GetPixelBW(x  , y+1);
			bw9 = GetPixelBW(x+1, y+1);

			// Calculamos la componente 
			aux = int((
					bw1*fMatrix[0] + bw2*fMatrix[1] + bw3*fMatrix[2] +
					bw4*fMatrix[3] + bw5*fMatrix[4] + bw6*fMatrix[5] +
					bw7*fMatrix[6] + bw8*fMatrix[7] + bw9*fMatrix[8])/fDivide);
			if (bAbsolute)
				aux = abs(aux);
			if (bSaturate) {
				if (aux < 0)
					aux = 0;
				if (aux > MAX_VALUE)
					aux = MAX_VALUE;
			}
			ret = UINT(aux);

			temporal.SetPixelBW(x, y, ret);

			// Aqui avanzamos una linea hacia abajo
			bw1 = bw4;
			bw2 = bw5;
			bw3 = bw6;
			bw4 = bw7;
			bw5 = bw8;
			bw6 = bw9;
		}
	}
	memcpy(this->m_pData, temporal.m_pData, format.bpp/8*m_width*m_height);
}

void CImage::ThresholdRGB(RGBValue threshold, RGBValue lowValue, RGBValue highValue)
{
	int x, y;
	RGBValue rgb;
	for (x = 0; x < m_width; ++x) {
		for (y = 0; y < m_height; ++y) {
			rgb = GetPixelRGB(x, y);
			// Componente R
			if (rgb.r < threshold.r)
				rgb.r = lowValue.r;
			else 
				rgb.r = highValue.r;
			// Componente G
			if (rgb.g < threshold.g)
				rgb.g = lowValue.g;
			else 
				rgb.g = highValue.g;
			// Componente B
			if (rgb.b < threshold.b)
				rgb.b = lowValue.b;
			else 
				rgb.b = highValue.b;
			SetPixelRGB(x, y, rgb);
		}
	}
}

void CImage::ThresholdBW(BWValue threshold, BWValue lowValue, BWValue highValue)
{
	int x, y;
	BWValue bw;
	for (x = 0; x < m_width; ++x) {
		for (y = 0; y < m_height; ++y) {
			bw = GetPixelBW(x, y);
			// Componente BW
			if (bw < threshold)
				bw = lowValue;
			else 
				bw = highValue;
			SetPixelBW(x, y, bw);
		}
	}
}

// Realiza una funcion espejo sobre el valor H de una imagen HLS
// (H < (MAX_VALUE+1)/2)? H:MAX_VALUE+1-H
void CImage::SymmetryH(){
	assert(m_format.pixel == PIXEL_HLS);
	int x, y;
	HLSValue hls;
	for (x = 0; x < m_width; ++x) {
		for (y = 0; y < m_height; ++y) {
			hls = GetPixelHLS(x, y);
			if (hls.h > (MAX_VALUE+1)/2) {
				hls.h = (MAX_VALUE+1-hls.h);
			}
			SetPixelHLS(x, y, hls);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// CBlob Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlob::CBlob()
{

}

CBlob::~CBlob()
{

}


//////////////////////////////////////////////////////////////////////
// CBlobGroup Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlobGroup::CBlobGroup()
{
	m_nNumBlobs = 0;
	m_pBlobs = NULL;
	m_pClass = NULL;
	m_bAnalyzed = false;
}

CBlobGroup::~CBlobGroup()
{
	t_blob_node* pAux;
	while (m_pBlobs != NULL) {
		pAux = m_pBlobs->pNext;
		delete m_pBlobs;
		m_pBlobs = pAux;
	}
	if (m_pClass != NULL)
		delete m_pClass;
}


//////////////////////////////////////////////////////////////////////
// Añade nuevos elementos
//////////////////////////////////////////////////////////////////////

void CBlobGroup::Add(CBlob& blob)
{
	t_blob_node* pAux;
	pAux = m_pBlobs;
	m_pBlobs = new t_blob_node;
	m_pBlobs->blob = blob;
	m_pBlobs->pNext = pAux;
	m_nNumBlobs++;
}

//////////////////////////////////////////////////////////////////////
// Devuelve un blob
//////////////////////////////////////////////////////////////////////

CBlob& CBlobGroup::Get(UINT n)
{
	assert (n < m_nNumBlobs);
	UINT aux = 0;
	t_blob_node* pAux = m_pBlobs;

	while ( n != aux) {
		aux++;
		pAux = m_pBlobs->pNext;
	}

	return pAux->blob;
}

//////////////////////////////////////////////////////////////////////
// Analiza una imagen
//////////////////////////////////////////////////////////////////////


void CBlobGroup::AnalizeBW(CImage& image, BWValue threshold, int iMinArea)
{
	pixelFormat format;
	format = image.GetFormat();
	m_ancho = image.GetWidth();
	m_alto = image.GetHeight();
	m_pClass = new int[m_ancho*m_alto];
	// Inicializamos todos los pixeles pertenecen a la clase 0
	// Clase 0 quiere decir a ninguna
	memset(m_pClass, 0, sizeof(int)* m_ancho*m_alto);
	m_iLastClass = 0;
	m_nNumBlobs = 0;

	BWValue bw;
	// Detectamos todos los blobs
	for (m_y = 0; m_y < m_alto; ++m_y) {
		for (m_x = 0; m_x < m_ancho; ++m_x) {
			bw = image.GetPixelBW(m_x, m_y);
			if (bw >= threshold) {
				SetClass(m_x, m_y);
			}
		}
	}

	// Ahora debemos eliminar los que no alcanzan el area mínima
	int* pArea = new int[m_iLastClass+1];
	memset(pArea, 0, sizeof(int)*(m_iLastClass+1));
	for (m_y = 0; m_y < m_alto; ++m_y) {
		for (m_x = 0; m_x < m_ancho; ++m_x) {
			pArea[GetClass(m_x, m_y)]++;			
		}
	}

	for (m_y = 0; m_y < m_alto; ++m_y) {
		for (m_x = 0; m_x < m_ancho; ++m_x) {
			if (pArea[GetClass(m_x, m_y)] < iMinArea)
				m_pClass[m_x+m_y*m_ancho] = 0;
		}
	}

	m_bAnalyzed = true;
}

CImage* CBlobGroup::GetImage()
{
	assert (m_bAnalyzed);
	CImage* pImage = new CImage();
	pixelFormat format;

	format.bpp = 24;
	format.pixel = PIXEL_RGB;
	format.rgb.r = 0x0000FF;
	format.rgb.g = 0x00FF00;
	format.rgb.b = 0xFF0000;
	format.rgb.rBits = 8;
	format.rgb.gBits = 8;
	format.rgb.bBits = 8;
	pImage->Create(m_ancho, m_alto, format);
	RGBValue rgb;
	for (UINT x = 0; x < m_ancho; ++x) {
		for (UINT y = 0; y < m_alto; ++y) {
			rgb.r = rgb.g = rgb.b = 
				UINT(GetClass(x, y)/float(m_iLastClass)*MAX_VALUE);
			pImage->SetPixelRGB(x, y, rgb);
		}
	}
	return pImage;
}

PIXEL_TYPE CImage::GetMaxBits() 
{
	return MAX_BITS;
}
}