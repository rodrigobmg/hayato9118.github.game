// フェードイン・アウト処理 (Fade.cpp)
#include <nn/gd.h>
#include <nn/snd.h>
#include "Fade.h"
#include "Main.h"
#include "GxCustm.h"

extern nn::gd::CombinerState* s_texComState;
extern nn::gd::VertexBufferResource* bufferCoord;
extern nn::gd::VertexBufferResource* bufferTexCoord;

static int			g_nFade = 0;	// -1:フェードイン、1:フェードアウト
static int			g_nAlpha = 255;	// 不透明度
static nn::os::Tick	g_tStart;		// フェードイン／アウト開始時間
static s64			g_sMilliSec;	// 
static u8			g_colorR = 0;
static u8			g_colorG = 0;
static u8			g_colorB = 0;

// フェードイン・アウトカラー設定
void SetFadeColor(u8 colorR, u8 colorG, u8 colorB)
{
	g_colorR = colorR;
	g_colorG = colorG;
	g_colorB = colorB;
}

// フェードイン
void FadeIn(f32 fSec)
{
	g_tStart = nn::os::Tick::GetSystemCurrent();
	g_sMilliSec = (s64)(fSec * 1000);
	g_nAlpha = 255;
	g_nFade = -1;
}

// フェードアウト
void FadeOut(f32 fSec)
{
	g_tStart = nn::os::Tick::GetSystemCurrent();
	g_sMilliSec = (s64)(fSec * 1000);
	g_nAlpha = 0;
	g_nFade = 1;
}

// フェードイン・アウト更新
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

// フェードイン・アウト描画(上画面)
void DrawFade()
{
	if (g_nAlpha) {
		CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f, 1.0f);
		FillRectMain(g_colorR, g_colorG, g_colorB, (u8)g_nAlpha);
	}
}

// フェードイン・アウト描画(下画面)
void DrawFadeSub()
{
	if (g_nAlpha) {
		CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f, 1.0f);
		FillRectSub(g_colorR, g_colorG, g_colorB, (u8)g_nAlpha);
	}
}

// フェードイン・アウト中
int IsFading()
{
	return (g_nFade != 0);
}

// フェードアウト中
int IsFadeOut()
{
	return g_nFade == 1;
}

// フェードイン中
int IsFadeIn()
{
	return g_nFade == -1;
}

//--------------------------------------------------------------
// 矩形描画 上画面
//--------------------------------------------------------------
void FillRectMain(u8 colorR, u8 colorG, u8 colorB, u8 colorA)
{
	//Texture combiner
	nn::gd::CombinerDescription combiner;
	combiner.ToDefault();

	// 下の情報　TextureColor + 定数 + 定数
	combiner.SetSourceRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT);
	// 下の情報　TextureAlpha + 定数 + 定数
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
// 矩形描画 下画面
//--------------------------------------------------------------
void FillRectSub(u8 colorR, u8 colorG, u8 colorB, u8 colorA)
{
	nn::gd::CombinerDescription combiner;
	combiner.ToDefault();

	// 下の情報　TextureColor + 定数 + 定数
	combiner.SetSourceRGB(	nn::gd::CombinerStage::UNIT0,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT,
							nn::gd::CombinerStage::SOURCE_CONSTANT);
	// 下の情報　TextureAlpha + 定数 + 定数
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
