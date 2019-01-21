// Image.h: interface for the CImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__646319CC_1D03_11D3_810B_52544C0472C0__INCLUDED_)
#define AFX_IMAGE_H__646319CC_1D03_11D3_810B_52544C0472C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32	// UNIX
#include <windows.h>
#endif
#include <stdlib.h>
#include <assert.h>
#include <vfw.h>

#ifdef USE_OPENCV
#include <cxcore.h>
#include <cv.h>
#endif

#ifndef max_image
#define max_image(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min_image
#define min_image(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#pragma comment (lib, "vfw32.lib")     /* link with Video for Windows lib */

// Definiciones básicas
///////////////////////////

// Tipo con el que se almacena una componente de color
#ifndef PIXEL_TYPE
#define PIXEL_TYPE unsigned int
#endif 

// Constantes sobre el tamaño de cada componente
const PIXEL_TYPE MAX_VALUE = (PIXEL_TYPE)255; // 8 bits por componente como máximo (0xff)
const PIXEL_TYPE HIGH_BIT = 128; // Bit más a la izquierda del valor 255 
const PIXEL_TYPE MAX_BITS = 16;
const PIXEL_TYPE CONVERSION = 1;

namespace image {
// Formatos posibles: Blanco y Negro, RGB, YUV y HLS
enum t_pixel {PIXEL_BW = 0, PIXEL_RGB = 1, PIXEL_YUV = 2, PIXEL_HLS = 3, PIXEL_RGBA};
// Métodos de acceso rapido al buffer
enum t_method {NO_FAST_METHOD = 0, BGR_24_BITS = 1, HLS_24_BITS = 2, GRAY_8_BITS = 3, GRAY_16_BITS = 4, BGRA_32_BITS = 5, RGB_24_BITS = 6};

// Máscara HLS Hue, Luminance, Saturation
// Las mascaras sirven para definir la forma de almacenar las componentes
typedef struct {
	PIXEL_TYPE h;
	PIXEL_TYPE s;
	PIXEL_TYPE l;
	UINT hBits;
	UINT sBits;
	UINT lBits;
} HLSMask;

// Máscara RGB
typedef struct {
	PIXEL_TYPE r;
	PIXEL_TYPE g;
	PIXEL_TYPE b;
	UINT rBits;
	UINT gBits;
	UINT bBits;
} RGBMask;

// Máscara RGBA
typedef struct {
	PIXEL_TYPE r;
	PIXEL_TYPE g;
	PIXEL_TYPE b;
	PIXEL_TYPE a;
	UINT rBits;
	UINT gBits;
	UINT bBits;
	UINT aBits;
} RGBAMask;

// Máscara YUV
typedef struct {
	PIXEL_TYPE y;
	PIXEL_TYPE u;
	PIXEL_TYPE v;
	UINT yBits;
	UINT uBits;
	UINT vBits;
} YUVMask;

// Valor de un pixel HLS
#ifdef WIN32 // Windows
class HLSValue  
#else // UNIX
class HLSValue
#endif
{
public:
	HLSValue() {h = l = s = 0;};
	HLSValue(PIXEL_TYPE hue, PIXEL_TYPE luminance, PIXEL_TYPE saturation) { 
		h = hue; l = luminance; s = saturation;
	}
	HLSValue(const HLSValue& hls) { h = hls.h; l = hls.l; s = hls.s;}
	HLSValue operator = (HLSValue hls) { 
		h = hls.h; l = hls.l; s = hls.s;
		return *this;
	}
	static HLSValue Distance(HLSValue hls1, HLSValue hls2) {
		PIXEL_TYPE h = abs((int)hls1.h-(int)hls2.h);
		h = min_image (h, MAX_VALUE-h);
		return HLSValue(h, abs((int)hls1.l-(int)hls2.l), abs((int)hls1.s-(int)hls2.s));
	}
public:
	PIXEL_TYPE h;
	PIXEL_TYPE l;
	PIXEL_TYPE s;
};

// Valor de un pixel RGB
#ifdef WIN32 // Windows
class RGBValue 
#else // UNIX
class RGBValue
#endif
{
public:
	// Colores predefinidos en RGB
	// blanco y negro
	static const RGBValue white;
	static const RGBValue black;

	// colores primarios
	static const RGBValue red;
	static const RGBValue green;
	static const RGBValue blue;

	// colores complementarios
	static const RGBValue magenta;
	static const RGBValue cyan;
	static const RGBValue yellow;

	// otros colores
	static const RGBValue aquamarine;
	static const RGBValue bakerschocolate;
	static const RGBValue blueviolet;
	static const RGBValue brass;
	static const RGBValue brightgold;
	static const RGBValue brown;
	static const RGBValue bronze;
	static const RGBValue bronzeii;
	static const RGBValue cadetblue;
	static const RGBValue coolcopper;
	static const RGBValue copper;
	static const RGBValue coral;
	static const RGBValue cornflowerblue;
	static const RGBValue darkbrown;
	static const RGBValue darkgreen;
	static const RGBValue darkgreencopper;
	static const RGBValue darkolivegreen;
	static const RGBValue darkorchid;
	static const RGBValue darkpurple;
	static const RGBValue darkslateblue;
	static const RGBValue darkslategrey;
	static const RGBValue darktan;
	static const RGBValue darkturquoise;
	static const RGBValue darkwood;
	static const RGBValue dimgrey;
	static const RGBValue dustyrose;
	static const RGBValue feldspar;
	static const RGBValue firebrick;
	static const RGBValue forestgreen;
	static const RGBValue gold;
	static const RGBValue goldenrod;
	static const RGBValue grey;
	static const RGBValue greencopper;
	static const RGBValue greenyellow;
	static const RGBValue huntergreen;
	static const RGBValue indianred;
	static const RGBValue khaki;
	static const RGBValue lightblue;
	static const RGBValue lightgrey;
	static const RGBValue lightsteelblue;
	static const RGBValue lightwood;
	static const RGBValue limegreen;
	static const RGBValue mandarianorange;
	static const RGBValue maroon;
	static const RGBValue mediumaquamarine;
	static const RGBValue mediumblue;
	static const RGBValue mediumforestgreen;
	static const RGBValue mediumgoldenrod;
	static const RGBValue mediumorchid;
	static const RGBValue mediumseagreen;
	static const RGBValue mediumslateblue;
	static const RGBValue mediumspringgreen;
	static const RGBValue mediumturquoise;
	static const RGBValue mediumvioletred;
	static const RGBValue mediumwood;
	static const RGBValue midnightblue;
	static const RGBValue navyblue;
	static const RGBValue neonblue;
	static const RGBValue neonpink;
	static const RGBValue newmidnightblue;
	static const RGBValue newtan;
	static const RGBValue oldgold;
	static const RGBValue orange;
	static const RGBValue orangered;
	static const RGBValue orchid;
	static const RGBValue palegreen;
	static const RGBValue pink;
	static const RGBValue plum;
	static const RGBValue quartz;
	static const RGBValue richblue;
	static const RGBValue salmon;
	static const RGBValue scarlet;
	static const RGBValue seagreen;
	static const RGBValue semisweetchocolate;
	static const RGBValue sienna;
	static const RGBValue silver;
	static const RGBValue skyblue;
	static const RGBValue slateblue;
	static const RGBValue spicypink;
	static const RGBValue springgreen;
	static const RGBValue steelblue;
	static const RGBValue summersky;
	static const RGBValue tan;
	static const RGBValue thistle;
	static const RGBValue turquoise;
	static const RGBValue verydarkbrown;
	static const RGBValue verylightgrey;
	static const RGBValue violet;
	static const RGBValue violetred;
	static const RGBValue wheat;
	static const RGBValue yellowgreen;

public:
	// Constructor
	RGBValue() {r = g = b = 0;};
	RGBValue(PIXEL_TYPE red, PIXEL_TYPE green, PIXEL_TYPE blue) { 
		r = red; g = green; b = blue;
	}
	RGBValue(const RGBValue& rgb) { r = rgb.r; g = rgb.g; b = rgb.b;}
	RGBValue operator = (RGBValue rgb) { 
		r = rgb.r; g = rgb.g; b = rgb.b;
		return *this;
	}
	bool operator == (RGBValue rgb) { 
		return (r == rgb.r) && (g == rgb.g) && (b == rgb.b);
	}
	static RGBValue Distance(RGBValue rgb1, RGBValue rgb2) {
		return RGBValue(
			abs((int)rgb1.r - (int)rgb2.r),
			abs((int)rgb1.g - (int)rgb2.g),
			abs((int)rgb1.b - (int)rgb2.b));
	}
public:
	PIXEL_TYPE r;
	PIXEL_TYPE g;
	PIXEL_TYPE b;
};

// Valor de un pixel RGBA
#ifdef WIN32 // Windows
class RGBAValue  
#else // UNIX
class RGBAValue
#endif
{
public:
	// Constructor
	RGBAValue() {r = g = b = a = 0;};
	RGBAValue(PIXEL_TYPE cr, PIXEL_TYPE cg, PIXEL_TYPE cb, PIXEL_TYPE ca) { 
		r = cr; g = cg; b = cb; a = ca;
	}
	RGBAValue(const RGBAValue& rgba) { r = rgba.r; g = rgba.g; b = rgba.b; a = rgba.a;}
	RGBAValue operator = (RGBAValue rgba) { 
		r = rgba.r; g = rgba.g; b = rgba.b; a = rgba.a;
		return *this;
	}
public:
	PIXEL_TYPE r;
	PIXEL_TYPE g;
	PIXEL_TYPE b;
	PIXEL_TYPE a;
};

// Valor de un pixel YUV
#ifdef WIN32 // Windows
class YUVValue  
#else // UNIX
class YUVValue
#endif
{
public:
	// Constructor
	YUVValue() {y = u = v = 0;};
	YUVValue(PIXEL_TYPE cy, PIXEL_TYPE cu, PIXEL_TYPE cv) { 
		y = cy; u = cu; v = cv;
	}
	YUVValue(const YUVValue& yuv) { y = yuv.y; u = yuv.u; v = yuv.v;}
	YUVValue operator = (YUVValue yuv) { 
		y = yuv.y; u = yuv.u; v = yuv.v;
		return *this;
	}
public:
	PIXEL_TYPE y;
	PIXEL_TYPE u;
	PIXEL_TYPE v;
};

// Máscara Blanco y Negro
typedef struct {
	PIXEL_TYPE bw;
	UINT bwBits;
} BWMask;

// Valor de un pixel Balnco y Negro
typedef PIXEL_TYPE BWValue;

// Estructura que contiene la informacion de la imagen
// Se almacena el formato
typedef struct {
	UINT bpp; // Debe ser un múltiple de 8 (bits per pixel)
	t_pixel pixel; // Tipo de formato RGB, HLS, YUV...
	union {
		RGBAMask rgba;
		HLSMask hls;
		RGBMask rgb;
		YUVMask yuv;
		BWMask  bw;
	};
	PIXEL_TYPE highBit;
} pixelFormat;

// Coordenada 2D de tipo float
typedef struct {
	float x;
	float y;
} t_fpoint;


// Clase para la definicion de un Blob
#ifdef WIN32 // Windows
class CBlob  
#else // UNIX
class CBlob
#endif
{
public:
	t_fpoint center;
	float area;
public:
	CBlob();
	virtual ~CBlob();

};

class CImage;

// Clase para la definicion de un grupo de Blobs
#ifdef WIN32 // Windows
class CBlobGroup
#else // UNIX
class CBlobGroup
#endif
{
private:
	UINT m_nNumBlobs;
	typedef struct t_blob_tag{
		CBlob blob;
		t_blob_tag* pNext;
	} t_blob_node;

	t_blob_node* m_pBlobs;
	int* m_pClass;
	UINT m_ancho;
	UINT m_alto;
	bool m_bAnalyzed;
	int m_iLastClass;
	UINT m_x, m_y;

	void ConvertClass(int fromClass, int toClass) {
		m_nNumBlobs--;
		UINT y;
		for (y = 0; y <= m_y; ++y){
			for (UINT x = 0; x <= m_ancho; ++x){
				if (GetClass(x, y) == fromClass) {
					m_pClass[x+y*m_ancho] = toClass;
				}
			}
		}
		for (UINT x = 0; x <= m_x; ++x){
			if (GetClass(x, y) == fromClass) {
				m_pClass[x+y*m_ancho] = toClass;
			}
		}
	}

	void SetClass(UINT x, UINT y) {
		assert(x < m_ancho);
		assert(y < m_alto);
		int cl, caux;
		cl = GetFriendClass(x, y);
		if (cl == 0) {
			cl = ++m_iLastClass;
			m_nNumBlobs++;
		}

		m_pClass[x+y*m_ancho] = cl;

		caux = GetClass(x-1, y-1);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);
		caux = GetClass(x, y-1);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);
		caux = GetClass(x+1, y-1);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);

		caux = GetClass(x-1, y);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);
		caux = GetClass(x+1, y);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);

		caux = GetClass(x-1, y+1);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);
		caux = GetClass(x, y+1);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);
		caux = GetClass(x+1, y+1);
		if ((caux != 0) && (caux != cl))
			ConvertClass(caux, cl);
	}

	int GetFriendClass(UINT x, UINT y) {
		int cl;
		cl =		 GetClass(x-1, y-1);
		cl = max_image(cl, GetClass(x-1, y));
		cl = max_image(cl, GetClass(x-1, y+1));
		cl = max_image(cl, GetClass(x, y+1));
		cl = max_image(cl, GetClass(x, y-1));
		cl = max_image(cl, GetClass(x+1, y-1));
		cl = max_image(cl, GetClass(x+1, y));
		cl = max_image(cl, GetClass(x+1, y+1));
		return cl;
	}

	int GetClass(UINT x, UINT y) {
		if ((x >= m_ancho) || (x < 0))
			return 0;
		if ((y >= m_alto) || (y < 0))
			return 0;
		return m_pClass[x+y*m_ancho];			
	}	
public:
	CBlobGroup();
	virtual ~CBlobGroup();
	void Add(CBlob& blob);
	CBlob& Get(UINT i);
	int GetNumBlobs() { return m_nNumBlobs;}
	void AnalizeBW(CImage& image, BWValue threshold, int iMinArea);
	CImage* GetImage();
};





// Clase Imagen

#ifdef WIN32 // Windows
class CImage
#else // UNIX
class CImage  
#endif
{
public:
	static PIXEL_TYPE GetMaxBits();
	static const pixelFormat s_formatRGB;
	static const pixelFormat s_formatBGR;
	static const pixelFormat s_formatBGRA;
	static const pixelFormat s_formatDEPTH;
public:
	enum t_spacecolor {BW, RGB, RGBA, YUV, HLS};
private:
	// Definicion de constantes para la lectura de BMPs
	#define BM_RGB				(DWORD)0
	#define BM_RLE8			(DWORD)1
	#define BM_RLE4			(DWORD)2
	#define BM_BITFIELDS		(DWORD)3

	// Tipos básicos
	typedef struct tagBMFILEHEADER {
		WORD	bfType;
		DWORD	bfSize;
		WORD	bfReserved1;
		WORD	bfReserved2;
		DWORD	bfOffBits;
	} BMFILEHEADER;

	typedef struct tagBMINFOHEADER{
		DWORD	biSize;
		LONG	biWidth;
		LONG	biHeight;
		WORD	biPlanes;
		WORD	biBitCount;
		DWORD	biCompression;
		DWORD	biSizeImage;
		LONG	biXPelsPerMeter;
		LONG	biYPelsPerMeter;
		DWORD	biClrUsed;
		DWORD	biClrImportant;
	} BMINFOHEADER;

	typedef struct tagBMCOREHEADER {
		DWORD   bcSize; 
		WORD    bcWidth; 
		WORD    bcHeight; 
		WORD    bcPlanes; 
		WORD    bcBitCount; 
	} BMCOREHEADER; 

 	typedef struct tagRGB_QUAD {
		BYTE    rgbBlue; 
		BYTE    rgbGreen; 
		BYTE    rgbRed; 
		BYTE    rgbReserved; 
	} RGB_QUAD; 

	typedef struct tagRGB_TRIPLE { 
		BYTE rgbBlue; 
		BYTE rgbGreen; 
		BYTE rgbRed; 
	} RGB_TRIPLE; 

	typedef struct tagBMINFO{
		BMINFOHEADER	bmiHeader;
		RGB_QUAD			bmiColors[1];
	} BMINFO;

	typedef struct tagBMCOREINFO { 
		BMCOREHEADER  bmciHeader; 
		RGB_TRIPLE     bmciColors[1]; 
	} BMCOREINFO; 

public:
	// Funciones de obtencion del formato
	pixelFormat GetFormat() { return m_format;}

	int GetWidth(){return m_width;}
	int GetHeight(){return m_height;}

	// Funciones de salvado y carga de imágenes
	bool	SaveRaw(const char* name);
	bool	LoadRaw(const char* name);

	bool	SavePPF(const char *name);
	bool	LoadPPF(const char *name);

	bool	SaveBMP(const char *name);
	bool	LoadBMP(const char *name);

	void Destroy(); // Libera la memoria reservada

	// Funciones de actualización y obtención de los pixeles
	HLSValue GetPixelHLS(int x, int y);
	HLSValue SetPixelHLS(int x, int y, HLSValue value);

	RGBValue GetPixelRGB(int x, int y);
	RGBValue SetPixelRGB(int x, int y, RGBValue value);

	RGBAValue GetPixelRGBA(int x, int y);
	RGBAValue SetPixelRGBA(int x, int y, RGBAValue value);

	YUVValue GetPixelYUV(int x, int y);
	YUVValue SetPixelYUV(int x, int y, YUVValue value);

	BWValue GetPixelBW(int x, int y);
	BWValue SetPixelBW(int x, int y, BWValue value);

	// Funciones de conversion entre los diferentes formatos de color
	static BWValue RGBtoBW (RGBValue value);
	static BWValue HLStoBW (HLSValue value);
	static BWValue YUVtoBW (YUVValue value);

	static RGBValue BWtoRGB (BWValue value);
	static HLSValue BWtoHLS (BWValue value);
	static YUVValue BWtoYUV (BWValue value);

	static HLSValue RGBtoHLS (RGBValue value);
	static RGBValue HLStoRGB (HLSValue value);

	static YUVValue RGBtoYUV (RGBValue value);
	static RGBValue YUVtoRGB (YUVValue value);

	static YUVValue HLStoYUV (HLSValue value);
	static HLSValue YUVtoHLS (YUVValue value);

	static RGBAValue RGBtoRGBA (RGBValue value);
	static RGBValue RGBAtoRGB (RGBAValue value);

public:
	CImage(void); // Constructor
	CImage(CImage& origen);
	~CImage(); // Destructor

	// Función para crear una imagen, se le pasa el formato deseado y crea el buffer
	bool Create(int width, int height, pixelFormat format);
	// Realiza una copia de la imagen con el mismo formato
	bool Create(CImage& origen);
	// Realiza una copia de la imagen origen al formato deseado
	bool Create(int width, int height, pixelFormat format, CImage& origen);
	// Crea una imagen de 640x480 con una muestra de la fuente de texto utilizada
	bool CreateTexto();
	// Copia una imagen
	bool Copy(CImage& origen); 
	// Determina si se ha creado
	bool IsCreated() { return m_bCreated; }

	BYTE* GetOpenGLTexture(int iSize = 512); // Devuelve una matriz en formato RGB para pasar a OpenGL, es trabajo del programa se encarga de liberar la memoria
	BYTE* GetOpenGLTexture(int width, int height); // Devuelve una matriz en formato RGB para pasar a OpenGL, es trabajo del programa se encarga de liberar la memoria
	void GetOpenGLTexture(int iSize, BYTE* pBuffer); // Pone en una matriz la imagen en formato OpenGL RGB
	void GetOpenGLTexture(int width, int height, BYTE* pBuffer); // Pone en una matriz la imagen en formato OpenGL RGB

	unsigned char* GetData() {return m_pData;} // Devuelve el puntero a la imagen tal y como se almacena, no se recomienda su utilización
	int GetSizeData() { return this->m_width*this->m_height*this->m_format.bpp/8;}
	// Devuelve la luminancia media de una imagen
	PIXEL_TYPE GetLuminance()
	{
		long double ret = 0;
		RGBValue rgb;
		for (int x = 0; x < GetWidth(); ++x)
		{
			for (int y = 0; y < GetHeight(); ++y)
			{
				rgb = this->GetPixelRGB(x, y);
				ret += rgb.r + rgb.g + rgb.b;
			}
		}
		return PIXEL_TYPE(ret/(3.0 * GetWidth() * GetHeight()));
	}
public:
	// Realiza una funcion espejo sobre el valor H de una imagen HLS
	// (H < (MAX_VALUE+1)/2)? H:MAX_VALUE+1-H
	void SymmetryH();

	// Borra la imagen del color especificado
	void Clear(RGBValue rgb = RGBValue::white);
	// Dibujado de primitivas sobre la imagen
	void DrawRectangle(float x1, float y1, float x2, float y2, RGBValue rgb);
	void DrawFillRectangle(float x1, float y1, float x2, float y2, RGBValue rgb);
	void DrawLine(float x1, float y1, float x2, float y2, RGBValue rgb);
	void DrawCircle(float x, float y, float radius, RGBValue rgb);
	void DrawFillCircle(float x, float y, float radius, RGBValue rgb);
	void Print(int posx, int posy, char* fmt, RGBValue rgb, ...);
	void DrawEllipse(float x, float y, float angulo, float mayor, float minor, RGBValue rgb);
	void DrawFillEllipse(float x, float y, float angulo, float mayor, float minor, RGBValue rgb);
	void DrawImage(float x, float y, CImage& source);
	// Operaciones de tratamiento sobre una imagen
	void Smooth();
	void Average();
	void Sharpen8();
	void Sharpen4();
	void Laplacian8();
	void Laplacian4();
	void HorzEdge();
	void VertEdge();
	CImage* Sobel2();
	void Sobel();
	void Prewitt();

	// Operación threshold
	void ThresholdRGB(RGBValue threshold, RGBValue lowValue, RGBValue highValue);
	void ThresholdBW(BWValue threshold, BWValue lowValue, BWValue highValue);
private:
	enum t_mode_combination {COMBINATION_SUM = 0, COMBINATION_MAX = 1};
	void Init();

	// Multiply by Matrix
	void MultiplyByMatrix(
		float fMatrix[9], float fDivide,
		bool bAbsolute = false,
		bool bSaturate = true);
	void MultiplyByMatrixRGB(float fMatrix[9], float fDivide, bool bAbsolute, bool bSaturate);
	void MultiplyByMatrixHLS(float fMatrix[9], float fDivide, bool bAbsolute, bool bSaturate);
	void MultiplyByMatrixBW (float fMatrix[9], float fDivide, bool bAbsolute, bool bSaturate);

	// Multiply by Two Matrix
	void MultiplyByTwoMatrix(
		float fMatrix1[9], float fDivide1,	bool bAbsolute1,
		float fMatrix2[9], float fDivide2,	bool bAbsolute2,
		float fDivideTotal,
		t_mode_combination mCombination,
		bool bSaturate = true);
	void MultiplyByTwoMatrixRGB(float fMatrix1[9], float fDivide1,	bool bAbsolute1, float fMatrix2[9], float fDivide2,	bool bAbsolute2, float fDivideTotal, t_mode_combination mCombination, bool bSaturate);
	void MultiplyByTwoMatrixHLS(float fMatrix1[9], float fDivide1,	bool bAbsolute1, float fMatrix2[9], float fDivide2,	bool bAbsolute2, float fDivideTotal, t_mode_combination mCombination, bool bSaturate);
	void MultiplyByTwoMatrixBW (float fMatrix1[9], float fDivide1,	bool bAbsolute1, float fMatrix2[9], float fDivide2,	bool bAbsolute2, float fDivideTotal, t_mode_combination mCombination, bool bSaturate);

protected:
	// Método rápido de acceso
	t_method method;

private:

	// Lee un bitmap de una estructura de memoria
	bool LoadBMPMemory(BYTE* pBitmap, DWORD bfOffBits = 0);

	// Actualiza en al posición x,y el valor value
	void SetValue(PIXEL_TYPE value, int x, int y);
	PIXEL_TYPE GetValue(int x, int y);

	// Pasa de máscara a componente
	PIXEL_TYPE GetResult(PIXEL_TYPE nMascara, PIXEL_TYPE nValue, UINT nTotalBits, UINT nBits);
	// Pasa de componente a máscara
	PIXEL_TYPE SetResult(PIXEL_TYPE nMascara, PIXEL_TYPE& nValue, UINT nTotalBits, UINT nBits);

	// Expande a más número de bits un valor, que esta en una escala de menos bits
	PIXEL_TYPE Expand(PIXEL_TYPE nValue, UINT nBits);

	// Formato de la imagen
	pixelFormat m_format;
	// Tamaño de la imagen
	int m_width;
	int m_height;
	// Indica si la imagen ha sido creada, o sea, si tiene información útil
	bool	m_bCreated;

private:
	// Puntero a la información de los pixeles
	BYTE*	m_pData;
	t_method m_method;
	static t_method computeMethod(pixelFormat format);

private:
#ifdef USE_OPENCV
	CvMat* m_matriz;
#else
    void*  m_matriz;	
#endif
	friend class CVideo;
};

// Expande un valor de nBits a MAX_BITS, sirve para corregir el error de precisión
inline PIXEL_TYPE CImage::Expand(PIXEL_TYPE nValue, UINT nBits) {
	PIXEL_TYPE auxValue;
	// Para cada bit que nos falta por poner
	for (UINT i = 0; i < MAX_BITS - nBits; ++i) {
		// Si no hay bits hacemos una copia del valor incial
		if ((i % nBits) == 0) {
			auxValue = nValue << (MAX_BITS - nBits);
		}
		// Multiplicamos por 2 el valor de salida
		nValue = nValue << 1;
		// Si el bit esta activo sumamos 1 
		if (auxValue & HIGH_BIT) {
			nValue++;
		}
		// Avanzamos en el valor inicial
		auxValue = auxValue << 1;
	}
	return nValue;
	// Ejemplo de expansion:
	// Valor 0x00003f (8 bits)  --> Resultado 0x3f3f3f (24 bits)
	// Valor 0x0000c1 (12 bits) --> Resultado 0x0c10c1 (24 bits)
	// Valor 0x000000 (8 bits)  --> Resultado 0x000000 (24 bits)
	// Valor 0x0000ff (8 bits)  --> Resultado 0xffffff (24 bits)
	// Lo que hace es colocarlo en un valor medio repitiendo los bits
	// de los que consta el valor inicial

}

// Obtiene una componente de un valor almacenado en el mismo formato que en el array
// Esta función se le pasa un valor recogido del array y la máscara que le quremos aplicar y 
//  nos devuelve el valor de esta componente en el rango definido para una componete.
//  nMascara.- máscara donde estan los bits de información de la componente
//  nResult.- valor del array del cual se quiere extraer la componente
//  nTotalBits.- número total de bits de la máscara
//  nBits.- número de bits de la componente
inline PIXEL_TYPE CImage::GetResult(PIXEL_TYPE nMascara, PIXEL_TYPE nResult, UINT nTotalBits, UINT nBits) {
	PIXEL_TYPE nValue = 0;
	// Para cada bit de la máscara
	for (UINT i = 0; i < nTotalBits; ++i) {
		// Si el bit pertenece a la componente
		if (nMascara & m_format.highBit) {
			// Multiplicamos por 2 el valor
			nValue = nValue << 1;
			// Si esta activo el bit en el valor
			if (nResult & m_format.highBit) {
				// Sumamos uno al valor
				nValue++;
			}
		}
		// Avanzamos tanto en la máscara como en el valor origen
		nResult = nResult << 1;
		nMascara = nMascara << 1;
	}
	// Expandimos el valor al número de bits que es una componente
	nValue = Expand(nValue, nBits);
	return nValue;
}

// Obtiene un valor alamcenado en el mismo formato del array a partir de una componente
// Esta función se le pasa una componente y nos devuelve un valor con los bits que le 
//  corresponden según la máscara
//  nMascara.- máscara donde estan los bits de información de la componente
//  nValue.- valor de la componente, devuelve el que realmente se almacena debido a
//		errores de precisión
//  nTotalBits.- número total de bits de la máscara
//  nBits.- número de bits de la componente
inline PIXEL_TYPE CImage::SetResult(PIXEL_TYPE nMascara, PIXEL_TYPE& nValue, UINT nTotalBits, UINT nBits) {
	PIXEL_TYPE nResult = 0;
	PIXEL_TYPE auxValue = nValue;
	// Truncamos el valor, al valor que realmente se guardará
	nValue = Expand(nValue >> (MAX_BITS - nBits), nBits);

	// Para cada bit de la máscara
	for (UINT i = 0; i < nTotalBits; ++i) {
		// Multiplicamos por 2 el valor de salida
		nResult = nResult << 1;
		// Si el bit esta activo en la máscara de la componente
		if (nMascara & m_format.highBit) {
			// Si el bit esta activo en la componente
			if (auxValue & HIGH_BIT) {
				// Sumamos uno al valor de salida
				nResult++;
			}
			// Avanzamos en el valor de la componente
			auxValue = auxValue << 1;
		}
		// Avanzamos en l amáscara
		nMascara = nMascara << 1;
	}
	return nResult;
}
}
#endif // !defined(AFX_IMAGE_H__646319CC_1D03_11D3_810B_52544C0472C0__INCLUDED_)

