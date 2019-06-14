// 文字列表示実装 (Text.cpp)
#include "Text.h"
#include "Texture.h"
#include "Main.h"
#include "GxCustm.h"

// マクロ定義
#define FONT_WIDTH	16
#define FONT_HEIGHT	16

// 文字表示
void DrawChar(const nn::math::VEC2* pos, u8 code, const u8* color)
{
	DrawChar(pos->x, pos->y, code, color);
}
void DrawChar(f32 fX, f32 fY, u8 code, const u8* color)
{
	CreateMatrix(fX, fY, 0.0f, 8.0f, 8.0f);
	u8 cx = code & 15;
	u8 cy = 15 - (code >> 4);
	SetFrame(16 + cx + (cy << 4));
	DrawPolygon(4, 0, color);
}

// 文字列表示
void DrawText(const nn::math::VEC2* pos, const char* text, const u8* color, const char* attr)
{
	DrawText(pos->x, pos->y, text, color, attr);
}

void DrawText(f32 fX, f32 fY, const char* text, const u8* color, const char* attr)
{
	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texFont);
	nn::math::VEC2 v(fX, fY);
	v.x += FONT_WIDTH / 2;
	v.y -= FONT_HEIGHT / 2;
	while (*text) {
		if (*text == '\n') {
			v.x = fX + FONT_WIDTH / 2;
			v.y += FONT_HEIGHT;
		} else {
			u8 code = (u8)*text;
			if (attr && (*attr & 1)) code ^= 0x20;
			DrawChar(&v, code, color);
			v.x += FONT_WIDTH;
		}
		++text;
		if (attr) ++attr;
	}
}

// 文字ウィンドウ表示
void DrawWindow(f32 fX, f32 fY, u32 w, u32 h)
{
	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texNumber);
	nn::math::VEC2 v(fX, fY);
	v.x += FONT_WIDTH / 2;
	v.y -= FONT_HEIGHT / 2;
	// 左上
	DrawChar(&v, 0);
	// 上
	nn::math::VEC2 p = v;
	for (u32 x = 1; x < w - 1; ++x) {
		p.x += FONT_WIDTH;
		DrawChar(&p, 2);
	}
	// 右上
	p.x += FONT_WIDTH;
	DrawChar(&p, 3);
	// 左下
	p.x = v.x;
	p.y = v.y - FONT_HEIGHT * (h - 1);
	DrawChar(&p, 16);
	// 下
	for (u32 x = 1; x < w - 1; ++x) {
		p.x += FONT_WIDTH;
		DrawChar(&p, 1);
	}
	// 右下
	p.x += FONT_WIDTH;
	DrawChar(&p, 19);
	// 左
	p = v;
	for (u32 y = 1; y < h - 1; ++y) {
		p.y -= FONT_HEIGHT;
		DrawChar(&p, 17);
	}
	// 右
	p.x = v.x + FONT_WIDTH * (w - 1);
	p.y = v.y;
	for (u32 y = 1; y < h - 1; ++y) {
		p.y -= FONT_HEIGHT;
		DrawChar(&p, 18);
	}
	// 中
	p.y = v.y;
	for (u32 y = 1; y < h - 1; ++y) {
		p.x = v.x;
		p.y -= FONT_HEIGHT;
		for (u32 x = 1; x < w - 1; ++x) {
			p.x += FONT_WIDTH;
			DrawChar(&p, 4);
		}
	}
}
