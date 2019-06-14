// �t�F�[�h�C���E�A�E�g���� (Fade.cpp)
#include <nn/gd.h>
#include <nn/snd.h>
#include "Fade.h"
#include "Main.h"
#include "GxCustm.h"

extern nn::gd::CombinerState* s_texComState;
extern nn::gd::VertexBufferResource* bufferCoord;
extern nn::gd::VertexBufferResource* bufferTexCoord;

static int			g_nFade = 0;	// -1:�t�F�[�h�C���A1:�t�F�[�h�A�E�g
static int			g_nAlpha = 255;	// �s�����x
static nn::os::Tick	g_tStart;		// �t�F�[�h�C���^�A�E�g�J�n����
static s64			g_sMilliSec;	// 
static u8			g_colorR = 0;
static u8			g_colorG = 0;
static u8			g_colorB = 0;

// �t�F�[�h�C���E�A�E�g�J���[�ݒ�
void SetFadeColor(u8 colorR, u8 colorG, u8 colorB)
{
	g_colorR = colorR;
	g_colorG = colorG;
	g_colorB = colorB;
}

// �t�F�[�h�C��
void FadeIn(f32 fSec)
{
	g_tStart = nn::os::Tick::GetSystemCurrent();
	g_sMilliSec = (s64)(fSec * 1000);
	g_nAlpha = 255;
	g_nFade = -1;
}

// �t�F�[�h�A�E�g
void FadeOut(f32 fSec)
{
	g_tStart = nn::os::Tick::GetSystemCurrent();
	g_sMilliSec = (s64)(fSec * 1000);
	g_nAlpha = 0;
	g_nFade = 1;
}

// �t�F�[�h�C���E�A�E�g�X�V
void UpdateFade()
{
	if (g_nFade) {
		nn::os::Tick t = nn::os::Tick::GetSystemCurrent() - g_tStart;
		s64 s = t.ToMilliSeconds();
		if (s <= 0) {
			s = 0;
		} else if (s >= g_sMilliSec) {
			if (g_nFade < 0) {
				s = 0;
			} else {
				s = g_sMilliSec;
			}
			g_nFade = 0;
		}
		if (g_nFade < 0) {
			s = g_sMilliSec - s;
		}
		g_nAlpha = s * 255 / g_sMilliSec;
		nn::snd::SetMasterVolume((255 - g_nAlpha) / 255.0f);
	}
}

// �t�F�[�h�C���E�A�E�g�`��(����)
void DrawFade()
{
	if (g_nAlpha) {
		CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f, 1.0f);
		FillRectMain(g_colorR, g_colorG, g_colorB, (u8)g_nAlpha);
	}
}

// �t�F�[�h�C���E�A�E�g�`��(�����)
void DrawFadeSub()
{
	if (g_nAlpha) {
		CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f, 1.0f);
		FillRectSub(g_colorR, g_colorG, g_colorB, (u8)g_nAlpha);
	}
}

// �t�F�[�h�C���E�A�E�g��
int IsFading()
{
	return (g_nFade != 0);
}

// �t�F�[�h�A�E�g��
int IsFadeOut()
{
	return g_nFade == 1;
}

// �t�F�[�h�C����
int IsFadeIn()
{
	return g_nFade == -1;
}

//--------------------------------------------------------------
// ��`�`�� ����
//--------------------------------------------------------------
void FillRectMain(u8 colorR, u8 colorG, u8 colorB, u8 colorA)
{
	//Texture combiner
	nn::gd::CombinerDescription combiner;
	combiner.ToDefault();

	// ���̏��@TextureColor + �萔 + �萔
	combiner.SetSourceRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT);
	// ���̏��@TextureAlpha + �萔 + �萔
	combiner.SetSourceAlpha(nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT);

	combiner.SetOperandRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR,
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR,
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR);

	combiner.SetOperandAlpha(nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA,
							nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA,
							nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA);

	combiner.SetCombineRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::COMBINE_RGB_REPLACE);

	combiner.SetCombineAlpha(nn::gd::CombinerStage::UNIT0,
							 nn::gd::CombinerStage::COMBINE_ALPHA_REPLACE);

	combiner.SetCombinerInUse(nn::gd::CombinerStage::UNIT0, GD_TRUE);

	nn::gd::CombinerState* comState = s_texComState;
	nn::gd::CombinerStage::CreateTextureCombinerState(&combiner, &s_texComState);
	nn::gd::CombinerStage::SetTextureCombinerUnitConstantColor(nn::gd::CombinerStage::UNIT0,
		colorR, colorG, colorB, colorA);
	nn::gd::CombinerStage::SetTextureCombinerState(s_texComState);
	if (comState) nn::gd::CombinerStage::ReleaseTextureCombinerState(comState);

	SetFrame();

	nn::gd::System::DrawIndexed(4, 0);
}

//--------------------------------------------------------------
// ��`�`�� �����
//--------------------------------------------------------------
void FillRectSub(u8 colorR, u8 colorG, u8 colorB, u8 colorA)
{
	nn::gd::CombinerDescription combiner;
	combiner.ToDefault();

	// ���̏��@TextureColor + �萔 + �萔
	combiner.SetSourceRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT);
	// ���̏��@TextureAlpha + �萔 + �萔
	combiner.SetSourceAlpha(nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT);

	combiner.SetOperandRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR,
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR,
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR);

	combiner.SetOperandAlpha(nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA,
							nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA,
							nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA);

	combiner.SetCombineRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::COMBINE_RGB_REPLACE);

	combiner.SetCombineAlpha(nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::COMBINE_ALPHA_REPLACE);

	combiner.SetCombinerInUse(nn::gd::CombinerStage::UNIT0, GD_TRUE);

	nn::gd::CombinerState* comState = s_texComState;
	nn::gd::CombinerStage::CreateTextureCombinerState(&combiner, &s_texComState);
	nn::gd::CombinerStage::SetTextureCombinerUnitConstantColor(nn::gd::CombinerStage::UNIT0,
		colorR, colorG, colorB, colorA);
	nn::gd::CombinerStage::SetTextureCombinerState(s_texComState);
	if (comState) nn::gd::CombinerStage::ReleaseTextureCombinerState(comState);

	nn::gd::VertexBufferResource* buffers[] = {bufferCoord, bufferTexCoord};
	u32 offsets[] = {0, 0};
	nn::gd::VertexInputStage::SetVertexBuffers(0, 2, buffers, offsets);

	nn::gd::System::DrawIndexed(4, 4);
}
