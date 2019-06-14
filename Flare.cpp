// �t���A���� (Flare.cpp)
#include "Main.h"
#include "Flare.h"
#include "Texture.h"
#include "GxCustm.h"
#include "GxInit.h"

typedef struct _TFlare {	// �t���A
	f32	x;		// X���W
	f32	y;		// Y���W
	f32 angle;	// �p�x
	f32 zoom;	// �g�嗦
	s32	stat;	// ���
} TFlare;
static TFlare flare;

// �\������
void DrawFlare()
{
	if (flare.stat <= 0) {
		return;
	}
	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texFlare);
	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

	if (s_bd) {
		s_bd = NULL;
	}

	nn::gd::BlendState* bd = s_bd;
	nn::gd::BlendStateDescription desc;
	desc.ToDefault();
	desc.m_BlendType = nn::gd::OutputStage::BLEND_TYPE_BLENDING;
	desc.SetBlendFunc(nn::gd::OutputStage::BLEND_FUNCTION_SRC_ALPHA,
					  nn::gd::OutputStage::BLEND_FUNCTION_ONE,
					  nn::gd::OutputStage::BLEND_EQUATION_ADD);
	nn::gd::OutputStage::CreateBlendState(&desc, &s_bd);
	nn::gd::OutputStage::SetBlendState(s_bd);
	if (bd) nn::gd::OutputStage::ReleaseBlendState(bd);

	SetFrame();
	for (int i = 0; i < 3; ++i) {
		CreateMatrix(flare.x, flare.y, 0.0f,
			flare.zoom * 64.0f, flare.zoom * 64.0f, 1.0f,
			flare.angle + i * 60.0f);
		DrawPolygon(4, 0);
	}

	BlendStateDesc();
}

// �ړ�����
void UpdateFlare()
{
	if (flare.stat <= 0) {
		return;
	}
	flare.angle += 1.0f;
	flare.zoom -= 0.05f;
	if (flare.zoom <= 0.0f)
		flare.stat = 0;
}

// �J�n����
s32 StartFlare(f32 x, f32 y)
{
	flare.x = x;
	flare.y = y;
	flare.zoom = 6.0f;
	flare.angle = 30.0f;
	flare.stat = 1;
	return 1;
}

// ��������
void ClearFlare()
{
	flare.stat = 0;
}
