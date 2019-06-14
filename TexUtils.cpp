
#include <nn/gx.h>
#include <nn/math.h>
#include <nn/fs.h>
#include <nn/os.h>
#include <nn/init.h>
#include <nn/fnd/fnd_ExpHeap.h>

#include <nn/gd.h>

#include "TexUtils.h"

typedef __packed struct BitmapFileHeader
{
    u8 bfType[2];
    u16 bfSize[2];
    u16 bfReserved1;
    u16 bfReserved2;
    u16 bfOffBits[2];    
} BitmapFileHeader;

typedef __packed struct BitmapInfoHeader
{
    u32 biSize;
    s32 biWidth;
    s32 biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImage;
    s32 biXPixPerMeter;
    s32 biYPixPerMeter;
    u32 biClrUsed;
    u32 biClrImportant;    
} BitmapInfoHeader;

u8* GetBmpFileData(u8* rawDataBuffer, u32& bmpWidth, u32& bmpHeight)
{
    BitmapInfoHeader* bmp_info_header_ptr = (BitmapInfoHeader*)(rawDataBuffer + sizeof(BitmapFileHeader));
    bmpWidth = bmp_info_header_ptr->biWidth;
    bmpHeight = bmp_info_header_ptr->biHeight;
    u8* bmpDataBuffer = rawDataBuffer + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    return bmpDataBuffer;
}

u32 GetTextureLength(const u32& imageLength)
{        
    u32 textureLength = 8;

    // 8, 16, 32, 64, 128, 256, 512, 1024
    for (u32 i = 0; i < 7; i++)
    {
        if ( imageLength > textureLength )
        {
            textureLength *= 2;
        }
        else
        {
            return textureLength;
        }
    }

    return 1024;
}

#define CONVERT_TEXTURE_TO_NATIVE_FORMAT_USING_GD
u8* GetTextureDataFromBmpFileData(const u32& bmpWidth, const u32& bmpHeight, u8* bmpDataBuffer, u32& textureWidth, u32& textureHeight, bool makeResolutionPowerOf2, nn::fnd::ExpHeap* p_appHeap)
{
    // Convert BMP to OpenGL RGB format
    textureWidth = bmpWidth;
    textureHeight = bmpHeight;
    if (makeResolutionPowerOf2)
    {
        textureWidth = GetTextureLength(bmpWidth);
        textureHeight = GetTextureLength(bmpHeight);
    }
    u8* textureGLDataBuffer = reinterpret_cast<u8*>( p_appHeap->Allocate(3 * textureWidth * textureHeight) );

    for (u32 y = 0; y < textureHeight; y++)
    {        
        for (u32 x = 0; x < textureWidth; x++)
        {
            u8* textureDataPtr = textureGLDataBuffer;
            textureDataPtr += 3 * ((static_cast<u32>(textureWidth) * y) + x);
            if ( ( x >= bmpWidth ) || ( y >= bmpHeight ) )
            {
                (*textureDataPtr) = 0x00;
                textureDataPtr += 1;
                
                (*textureDataPtr) = 0x00;
                textureDataPtr += 1;
                
                (*textureDataPtr) = 0x00;
            }
            else
            {
                u8* bmpDataPtr = bmpDataBuffer;
                bmpDataPtr += 3 * (bmpWidth * y + x);
                
                (*textureDataPtr) = (*(bmpDataPtr + 2));
                textureDataPtr += 1;
                (*textureDataPtr) = (*(bmpDataPtr + 1));
                textureDataPtr += 1;
                
                (*textureDataPtr) = (*(bmpDataPtr + 0));
            }
        }
    }

    u8* textureDataBuffer = reinterpret_cast<u8*>( p_appHeap->Allocate(3 * textureWidth * textureHeight) );

#ifdef CONVERT_TEXTURE_TO_NATIVE_FORMAT_USING_GD
    nn::gd::Resource::Helper::ConvertTextureResourceToNativeFormat(nn::gd::Resource::FORMAT_RGB_888, textureWidth, textureHeight, textureGLDataBuffer, textureDataBuffer);
#else
    // Convert OpenGL RGB format to PICA Native RGB format
    GLenum format = GL_RGB_NATIVE_DMP;
    bool result = demo::ConvertGLTextureToNative(format, textureWidth, textureHeight, textureGLDataBuffer, textureDataBuffer);
    if ( result )
    {
        NN_LOG("  Convert to GL_RGB_NATIVE_DMP succeeded.\n");
    }
    else
    {
        NN_PANIC("  Convert to GL_RGB_NATIVE_DMP failed.\n");
    }
#endif

    p_appHeap->Free(textureGLDataBuffer);

    return textureDataBuffer;
}

u8* GetTextureData_BMP(const char* filename, u32& bmpWidth, u32& bmpHeight, u32& textureWidth, u32& textureHeight, bool makeResolutionPowerOf2, nn::fnd::ExpHeap* p_appHeap)
{   
    wchar_t* wfilename = static_cast<wchar_t*>(p_appHeap->Allocate((strlen(filename) + 1) * 2));
    std::mbstowcs(wfilename, filename, strlen(filename) + 1);
	u8* pixels = GetTextureData_BMP(wfilename, bmpWidth, bmpHeight, textureWidth, textureHeight, makeResolutionPowerOf2, p_appHeap);
    p_appHeap->Free(wfilename);
	return pixels;
}

u8* GetTextureData_BMP(const wchar_t* filename, u32& bmpWidth, u32& bmpHeight, u32& textureWidth, u32& textureHeight, bool makeResolutionPowerOf2, nn::fnd::ExpHeap* p_appHeap)
{   
    //NN_LOG("\nReading data form ROMFS...\n");

    /* open */
	nn::fs::FileReader file(filename);
    
    size_t fileSize = file.GetSize();
    //NN_LOG("  fileSize = %d (Byte)\n", fileSize);
    if ( fileSize == 0 )
    {
        NN_PANIC("Failed to open bmp file.\n");
        return NULL;
    }

    void* buf = p_appHeap->Allocate(fileSize);

    s32 readSize = file.Read(buf, fileSize);
    //NN_LOG("  file readSize = %d (Byte)\n", readSize);
    if ( readSize == 0 )
    {
        NN_PANIC("Failed to open bmp file.\n");
        return NULL;
    }

    u8* bmpDataBuffer = NULL;
    bmpDataBuffer = GetBmpFileData((u8*)buf, bmpWidth, bmpHeight);
    //NN_LOG("  bmpWidth = %d, bmpHeight = %d\n", bmpWidth, bmpHeight);

    u8* textureDataBuffer = NULL;
    textureDataBuffer = GetTextureDataFromBmpFileData(bmpWidth, bmpHeight, bmpDataBuffer, textureWidth, textureHeight, makeResolutionPowerOf2, p_appHeap);
    //NN_LOG("  textureWidth = %d, textureHeight = %d\n", textureWidth, textureHeight);

    file.Finalize();
    p_appHeap->Free(buf);

    return textureDataBuffer;
}

/********************************************************************************************************************/
/***************************************************** TGA LOADER ***************************************************/
/********************************************************************************************************************/
#define _TGA_HEADER_SIZE    18

/*
 * TGA image header
 */
typedef struct {
    unsigned char   id_len;
    unsigned char   colormap_type;
    unsigned char   image_type;
    unsigned char   padding0;

    unsigned short  colormap_first;
    unsigned short  colormap_len;
    unsigned char   colormap_entry;
    unsigned char   padding1;

    unsigned short  x, y;
    unsigned short  width, height;
    unsigned char   depth;
    unsigned char   descriptor;

    /* */

    unsigned int    bpp;
} _TGA_HEADER;

static void _setTGAHeader(
    unsigned char data[_TGA_HEADER_SIZE],
    _TGA_HEADER *tga)
{
    tga->id_len         = data[0];
    tga->colormap_type  = data[1];
    tga->image_type     = data[2];

    tga->colormap_first = data[4] * 256 + data[3];
    tga->colormap_len   = data[6] * 256 + data[5];
    tga->colormap_entry = data[7];

    tga->x              = data[9] * 256 + data[8];
    tga->y              = data[11] * 256 + data[10];
    tga->width          = data[13] * 256 + data[12];
    tga->height         = data[15] * 256 + data[14];
    tga->depth          = data[16];
    tga->descriptor     = data[17];

    tga->bpp = tga->depth / 8;
}

static unsigned int _LoadTGAScanline(
    _TGA_HEADER *tga,
    unsigned char *dst, 
    unsigned char *src)
{
    int x;
    unsigned int l;

    l = 0;
    for(x = 0; x < tga->width; x++) {
        if (tga->bpp == 3) {
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
        } else { 
            /* 32BIT */
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = src[3];
        }
        src += tga->bpp;
        dst += tga->bpp;
        l += tga->bpp;
    }

    return l;
}

static unsigned int _LoadTGAScanlineRLE(
    _TGA_HEADER *tga,
    unsigned char *dst, 
    unsigned char *src)
{
    int x;
    unsigned int l;
    int errFlag;


    l = 0;
    x = 0;
    errFlag = 0;
    while(x < tga->width && !errFlag) {
        int count = (*src & 0x7f) + 1;
        if (*src & 0x80) {
            int i;

            src++;
            l++;
            for(i = 0; i < count; i++) {
                if (x >= tga->width) {
                    errFlag = 1;
                    break;
                }
                if (tga->bpp == 3) {
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                } else { 
                    /* 32BIT */
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                    dst[3] = src[3];
                }
                dst += tga->bpp;
                x++;
            }
            src += tga->bpp;
            l += tga->bpp;
        } else {
            int i;

            src++;
            l++;
            for (i = 0; i < count; i++) {
                if (x >= tga->width) {
                    errFlag = 1;
                    break;
                }
                if (tga->bpp == 3) {
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                } else { 
                    /* 32BIT */
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                    dst[3] = src[3];
                }
                src += tga->bpp;
                l += tga->bpp;
                dst += tga->bpp;
                x++;
            }
        }
    }

    if (errFlag) {
        return 0;
    }

    return l;
}

// コンパイラバグ回避
#pragma Ospace
void *dmpLoadTGA(
    const wchar_t *filename,
    unsigned int *width,
    unsigned int *height,
    unsigned int *format,
    unsigned int *type,
    unsigned int *orientation,
    nn::fnd::ExpHeap* p_appHeap)
{
    unsigned char *header = NULL;
    unsigned char *pixels = NULL;
    long long int filesize = 0;
    _TGA_HEADER tga;
    unsigned int _rle = 0;
    unsigned int _upper = 0;
    int y;
    unsigned char *src, *srchead, *dst;
    nn::fs::FileReader file;
    
    src = srchead = dst = NULL;
    
    /* init */
    if (width) {
        *width = 0;     /* unknown */
    }
    if (height) {
        *height = 0;    /* unknown */
    }
    if (format) {
        *format = 0;  /* unknown */
    }
    if (type) {
        *type = IMAGE_UNSIGNED_BYTE;
    }
    if (orientation) {
        *orientation = IMAGE_LOWER_LEFT;
    }

    /* open */
    file.Initialize(filename);

    filesize = file.GetSize();
    if (filesize < _TGA_HEADER_SIZE) {
        /* ERROR */
        return NULL;
    }

    header = static_cast<unsigned char*>(p_appHeap->Allocate(filesize));
    file.Read(header, _TGA_HEADER_SIZE);

    _setTGAHeader(header, &tga);
    
    p_appHeap->Free(header);

    _upper = tga.descriptor & 0x20 ? 1 : 0;

    if (tga.depth == 24 && (tga.descriptor & 0x0f) == 0x00) {
        /* 24BIT */
    } else if (tga.depth == 32 && (tga.descriptor & 0x0f) == 0x08) {
        /* 32BIT */
    } else if (tga.depth == 32 && (tga.descriptor & 0x0f) == 0x00) {
        /* 32BIT */
    } else {
        /* ERROR */
        file.Finalize();
        return NULL;
    }

    if (tga.colormap_type == 0x00 && tga.image_type == 0x02) {
        /* TRUE-COLOR IMAGE */
        _rle = 0;
    } else if (tga.colormap_type == 0x00 && tga.image_type == 0x0a) {
        /* RLE TRUE-COLOR IMAGE */
        _rle = 1;
    } else {
        /* ERROR */
        file.Finalize();
        return NULL;
    }


    if ((pixels = static_cast<unsigned char*>(p_appHeap->Allocate(tga.width * tga.height * tga.bpp))) == NULL) {
        /* ERROR */
        file.Finalize();
        return NULL;
    }

    if ((srchead = static_cast<unsigned char *>(p_appHeap->Allocate(filesize - _TGA_HEADER_SIZE))) == NULL)
    {
        file.Finalize();
        return NULL;
    }
    
    src = srchead;
    file.Read(src, filesize - _TGA_HEADER_SIZE);
    for(y = 0; y < tga.height; y++) {
        unsigned int l;

        dst = pixels + tga.width * tga.bpp * 
                (_upper ? tga.height - y - 1 : y);

        if (!_rle) {
            if ((l = _LoadTGAScanline(&tga, dst, src)) == 0) {
                /* ERROR */
                file.Finalize();
                p_appHeap->Free(srchead);
                p_appHeap->Free(pixels);
                return NULL;
            }
        } else {
            if ((l = _LoadTGAScanlineRLE(&tga, dst, src)) == 0) {
                /* ERROR */
                file.Finalize();
                p_appHeap->Free(srchead);
                p_appHeap->Free(pixels);
                return NULL;
            }
        }
        src += l;
    }

    file.Finalize();
    p_appHeap->Free(srchead);

    if (width) {
        *width = tga.width;
    }
    if (height) {
        *height = tga.height;
    }
    if (format) {
        if (tga.depth == 24) {
            *format = IMAGE_RGB;
        } else {
            *format = IMAGE_RGBA;
        }
    }

    return (void *)pixels;
}

u8* GetTextureData_TGA(const char* filename, u32& width, u32& height, u32& format, u32& type, nn::fnd::ExpHeap* p_appHeap)
{
    wchar_t* wfilename = static_cast<wchar_t*>(p_appHeap->Allocate((strlen(filename) + 1) * 2));
    std::mbstowcs(wfilename, filename, strlen(filename) + 1);
	u8* pixels = GetTextureData_TGA(wfilename, width, height, format, type, p_appHeap);
    p_appHeap->Free(wfilename);
	return pixels;
}

u8* GetTextureData_TGA(const wchar_t* filename, u32& width, u32& height, u32& format, u32& type, nn::fnd::ExpHeap* p_appHeap)
{
    unsigned char* pixels;
    unsigned orientation;
    
    pixels = (unsigned char*)dmpLoadTGA(filename, &width, &height, &format, &type, &orientation, p_appHeap);
    if (!pixels) return NULL;
    
    switch (format)
    {
        case IMAGE_RGB:     format = GL_RGB;    break;
        case IMAGE_RGBA:    format = GL_RGBA;   break;
        default: p_appHeap->Free(pixels); return NULL;
    }
    
    /* force to convert format */
    /*if (_forceformat != 0 && format != _forceformat)
    {
        unsigned i;
        unsigned char* newpixels = 0;
        
        switch (format)
        {
            case GL_RGB:
                switch (_forceformat)
                {
                    case GL_RGBA:
                        newpixels = (unsigned char*)malloc(width * height * 4);
                        if (!newpixels)
                        {
                            p_appHeap->Free(pixels);
                            return;
                        }
                        for (i = 0; i < width * height; i++)
                        {
                            newpixels[i * 4 + 0] = pixels[i * 3 + 0];
                            newpixels[i * 4 + 1] = pixels[i * 3 + 1];
                            newpixels[i * 4 + 2] = pixels[i * 3 + 2];
                            newpixels[i * 4 + 3] = 0xff;
                        }
                        break;
                }
                break;
            case GL_RGBA:
                switch (_forceformat)
                {
                    case GL_RGB:
                        newpixels = (unsigned char*)malloc(width * height * 3);
                        if (!newpixels)
                        {
                            p_appHeap->Free(pixels);
                            return;
                        }
                        for (i = 0; i < width * height; i++)
                        {
                            newpixels[i * 3 + 0] = pixels[i * 4 + 0];
                            newpixels[i * 3 + 1] = pixels[i * 4 + 1];
                            newpixels[i * 3 + 2] = pixels[i * 4 + 2];
                        }
                        break;
                }
                break;
        }
        
        if (newpixels)
        {
            format = _forceformat;
            p_appHeap->Free(pixels);
            pixels = newpixels;
        }
    }*/
    
    return pixels;
}
