#ifndef TEXUTILS_H__
#define TEXUTILS_H__

/*
 * image format
 */

#define IMAGE_RGB                   0x1907
#define IMAGE_RGBA                  0x1908

#define IMAGE_BGR                   0x80E0
#define IMAGE_BGRA                  0x80E1

/*
 * Data types
 */
#define IMAGE_BYTE                  0x1400
#define IMAGE_UNSIGNED_BYTE         0x1401
#define IMAGE_SHORT                 0x1402
#define IMAGE_UNSIGNED_SHORT        0x1403
#define IMAGE_INT                   0x1404
#define IMAGE_UNSIGNED_INT          0x1405
#define IMAGE_FLOAT                 0x1406

/*
 * Orientation
 */
#define IMAGE_LOWER_LEFT            0x8CA1
#define IMAGE_UPPER_LEFT            0x8CA2


u8* GetTextureData_BMP(const char* filename, u32& bmpWidth, u32& bmpHeight, u32& textureWidth, u32& textureHeight, bool makeResolutionPowerOf2, nn::fnd::ExpHeap* p_appHeap);
u8* GetTextureData_BMP(const wchar_t* filename, u32& bmpWidth, u32& bmpHeight, u32& textureWidth, u32& textureHeight, bool makeResolutionPowerOf2, nn::fnd::ExpHeap* p_appHeap);
u8* GetTextureData_TGA(const char* filename, u32& width, u32& height, u32& format, u32& type, nn::fnd::ExpHeap* p_appHeap);
u8* GetTextureData_TGA(const wchar_t* filename, u32& width, u32& height, u32& format, u32& type, nn::fnd::ExpHeap* p_appHeap);

#endif //TEXUTILS_H__
