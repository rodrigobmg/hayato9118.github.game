// テクスチャ定義(Texture.h)
#ifndef ___TEXTURE_H
#define ___TEXTURE_H

#include <nn/gd.h>

// グローバル変数
extern nn::gd::SamplerState* s_samplerDefault;
extern nn::gd::SamplerState* s_samplerLinear;
extern nn::gd::Texture2DResource* s_texture2D_res;
extern nn::gd::Texture2D* s_texture2D_TGA;
extern nn::gd::Texture2D* s_texEnemy;
extern nn::gd::Texture2D* s_texEnemy2;
extern nn::gd::Texture2D* s_texTitle;
extern nn::gd::Texture2D* s_texGameOver;
extern nn::gd::Texture2D* s_texGameClear;
extern nn::gd::Texture2D* s_texNumber;
extern nn::gd::Texture2D* s_texMapChip;
extern nn::gd::Texture2D* s_texBoss1;
extern nn::gd::Texture2D* s_texFlare;
extern nn::gd::Texture2D* s_texFont;

// プロトタイプ
void Initialize_Tex(void);

#endif // ___TEXTURE_H
