// ���l�\������ (Number.cpp)
#include "Number.h"
#include "Texture.h"
#include "Main.h"
#include "GxCustm.h"

// �}�N����`
#define FONT_WIDTH	24
#define FONT_HEIGHT	32

// ���l�\��
void DrawNumber(nn::math::VEC2* pos, s32 len, s32 num, const u8* pColor)
{
	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texNumber);
	nn::math::VEC2 v = *pos;
	v.x += FONT_WIDTH / 2;
	v.y -= FONT_HEIGHT / 2;
	v.x += FONT_WIDTH * (len - 1);
	for (s32 i = 0; i < len; ++i) {
		CreateMatrix(v.x, v.y, 0.0f, 16.0f, 16.0f);	// �|���S���\���ʒu�Z�b�g
		SetFrame(num % 10);
		DrawPolygon(4, 0, pColor);	// �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j
		v.x -= FONT_WIDTH;
		num /= 10;
	}
}
