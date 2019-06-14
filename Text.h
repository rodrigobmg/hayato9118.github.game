// •¶Žš—ñ•\Ž¦’è‹` (Text.h)
#ifndef ___H_TEXT_
#define ___H_TEXT_

#include <nn/math.h>

void DrawChar(f32 fX, f32 fY, u8 code, const u8* color = NULL);
void DrawChar(const nn::math::VEC2* pos, u8 code, const u8* color = NULL);
void DrawText(const nn::math::VEC2* pos,
	const char* text, const u8* color = NULL,
	const char* attr = NULL);
void DrawText(f32 x, f32 y,
	const char* text, const u8* color = NULL,
	const char* attr = NULL);
void DrawWindow(f32 fX, f32 fY, u32 wcnt, u32 hcnt);

#endif // ___H_TEXT_
