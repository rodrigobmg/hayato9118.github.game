//==============================================================
//  Texture.cpp
//
//--------------------------------------------------------------
//                                    Author : Nakagawa Hiroki
//                                             Michiba  Hiroaki
//                                             HAL OSAKA
//==============================================================

//==============================================================
// インクルード
//==============================================================
#include <nn/gx.h>
#include <nn/math.h>
#include <nn/fs.h>
#include <nn/os.h>
#include <nn/init.h>
#include <nn/applet.h>
#include <nn/fnd/fnd_ExpHeap.h>
#include <nn/gd.h>
#include <stdio.h>

#include "TexUtils.h" //テクスチャユーティリティ
#include "demo.h"

//Linear
nn::gd::SamplerState* s_samplerDefault;
nn::gd::SamplerState* s_samplerLinear;
nn::gd::Texture2DResource* s_texture2D_res;
nn::gd::Texture2D* s_texture2D_TGA;
nn::gd::Texture2D* s_texEnemy;
nn::gd::Texture2D* s_texEnemy2;
nn::gd::Texture2D* s_texTitle;
nn::gd::Texture2D* s_texGameOver;
nn::gd::Texture2D* s_texGameClear;
nn::gd::Texture2D* s_texDeadEffect;
nn::gd::Texture2D* s_texNumber;
nn::gd::Texture2D* s_texMapChip;
nn::gd::Texture2D* s_texBoss1;
nn::gd::Texture2D* s_texFlare;
nn::gd::Texture2D* s_texFont;

//Texture combiner
extern nn::gd::CombinerState* s_texComState;
extern nn::fnd::ExpHeap s_AppHeap;

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
static const char* g_dir = "rom:/resources/%s";

//--------------------------------------------------------------
// 関数プロトタイプ
//--------------------------------------------------------------
static nn::gd::Texture2D* LoadBMP(const char* fname);
static nn::gd::Texture2D* LoadTGA(const char* fname);

//==============================================================
// プログラム本編
//==============================================================
void Initialize_Tex(void)
{
	//----------  TGA  -----------------
	s_texture2D_TGA = LoadTGA("player001.tga");
	if (!s_texture2D_TGA) {NN_PANIC("\n");}
	s_texEnemy = LoadTGA("enemy001.tga");
	if (!s_texEnemy) { NN_PANIC("\n"); }
	s_texEnemy2 = LoadTGA("enemy002.tga");
	if (!s_texEnemy2) { NN_PANIC("\n"); }
	s_texDeadEffect = LoadTGA("effect001.tga");
	if (!s_texDeadEffect) { NN_PANIC("\n"); }
	s_texNumber = LoadTGA("number001.tga");
	if (!s_texNumber) { NN_PANIC("\n"); }
	s_texBoss1 = LoadTGA("boss.tga");
	if (!s_texBoss1) { NN_PANIC("\n"); }
	s_texFont = LoadTGA("PressStart2Ph.tga");
	if (!s_texFont) { NN_PANIC("\n"); }
	//----------   BMP   ----------------
	s_texTitle = LoadBMP("title.bmp");
	if (!s_texTitle) { NN_PANIC("\n"); }
	s_texGameOver = LoadBMP("gameover.bmp");
	if (!s_texGameOver) { NN_PANIC("\n"); }
	s_texGameClear = LoadBMP("gameclear.bmp");
	if (!s_texGameClear) { NN_PANIC("\n"); }
	s_texMapChip = LoadBMP("sozai.bmp");
	if (!s_texMapChip) { NN_PANIC("\n"); }
	s_texFlare = LoadBMP("led_lens_flare.bmp");
	if (!s_texFlare) { NN_PANIC("\n"); }

	//Sampler states
    nn::gd::SamplerStateDescription descDefault;
    descDefault.ToDefault();
    nn::gd::TextureStage::CreateSamplerState(&descDefault, &s_samplerDefault);

    nn::gd::SamplerStateDescription descSamplerState;
    descSamplerState.SetMinFilter(nn::gd::TextureStage::SAMPLER_MIN_FILTER_LINEAR);
    descSamplerState.SetMagFilter(nn::gd::TextureStage::SAMPLER_MAG_FILTER_LINEAR);
    descSamplerState.SetWrapS(nn::gd::TextureStage::SAMPLER_WRAP_REPEAT);
    descSamplerState.SetWrapT(nn::gd::TextureStage::SAMPLER_WRAP_REPEAT);
    descSamplerState.SetBorderColor(0.0f, 1.0f, 0.0f, 1.0f);
    nn::gd::TextureStage::CreateSamplerState(&descSamplerState, &s_samplerLinear);
}

nn::gd::Texture2D* LoadBMP(const char* fname)
{
	char filename[260];
	sprintf(filename, g_dir, fname);

	u32 width = 0; u32 height = 0; u32 textureWidth = 0; u32 textureHeight = 0;
	nn::gd::Texture2D* pTex = NULL;

	u8* textureDataPtr = GetTextureData_BMP(filename, width, height,
		textureWidth, textureHeight, false, &s_AppHeap);

	nn::gd::Texture2DResourceDescription Text2DResDesc2 = {textureWidth, textureHeight,
		1, nn::gd::Resource::NATIVE_FORMAT_RGB_888, nn::gd::Memory::LAYOUT_BLOCK_8,
		nn::gd::Memory::VRAMA};
	nnResult res = nn::gd::Resource::CreateTexture2DResource(&Text2DResDesc2,
		textureDataPtr, GD_FALSE, &s_texture2D_res);
	s_AppHeap.Free(textureDataPtr);
	if (!GD_NNRESULT_IS_FAILURE(res)) {
		nn::gd::Texture2DDescription tex2DDesc = {0, -1};
		nn::gd::TextureStage::CreateTexture2D(s_texture2D_res, &tex2DDesc, &pTex);
		if (GD_NNRESULT_IS_FAILURE(res)) {
			pTex = NULL;
		}
	}

	return pTex;
}

nn::gd::Texture2D* LoadTGA(const char* fname)
{
	u32 type = 2;
	u32 format = 0;
	u32 width = 0; u32 height = 0; u32 bpp = 4;
	nn::gd::Resource::Format fmt = nn::gd::Resource::FORMAT_RGBA_8888;
	nn::gd::Resource::NativeFormat nf = nn::gd::Resource::NATIVE_FORMAT_RGBA_8888;
	nn::gd::Texture2D* pTex = NULL;

	char filename[260];
	sprintf(filename, g_dir, fname);
	u8* textureDataPtr = GetTextureData_TGA(filename, width, height, format, type, &s_AppHeap);
	if (format == GL_RGB) {
		bpp = 3;
		fmt = nn::gd::Resource::FORMAT_RGB_888;
		nf = nn::gd::Resource::NATIVE_FORMAT_RGB_888;
	}
	u8* textureDataConvert = static_cast<u8*>(s_AppHeap.Allocate(width * height * bpp));

	nn::gd::Resource::Helper::ConvertTextureResourceToNativeFormat(
		fmt, width, height, textureDataPtr, textureDataConvert, NULL);
	s_AppHeap.Free(textureDataPtr);

	nn::gd::Texture2DResourceDescription Text2DResDesc = {width, height,
		1, nf, nn::gd::Memory::LAYOUT_BLOCK_8, nn::gd::Memory::VRAMA};
	nnResult res = nn::gd::Resource::CreateTexture2DResource(&Text2DResDesc,
		textureDataConvert, GD_FALSE, &s_texture2D_res);
	s_AppHeap.Free(textureDataConvert);
	if (!GD_NNRESULT_IS_FAILURE(res)) {
		nn::gd::Texture2DDescription tex2DDesc = {0, -1};
		res = nn::gd::TextureStage::CreateTexture2D(s_texture2D_res, &tex2DDesc, &pTex);
		if (GD_NNRESULT_IS_FAILURE(res)) {
			pTex = NULL;
		}
	}

	return pTex;
}
