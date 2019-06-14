// タイトル画面(Title.cpp)
#include <nn/gd.h>
#include "Title.h"
#include "GxCustm.h"
#include "Main.h"
#include "Texture.h"
#include "GxInit.h"
#include "Sound.h"
#include "Fade.h"
#include "Text.h"

static u8 prev_touch;
static bool fadeout;
static u32 blink = 0;
static u32 mask = 16;

// 初期化
void InitTitle() 
{
	PlayBGM(BGM_TITLE);
	SetFadeColor(255, 255, 255);
	FadeIn(1.0f);
	fadeout = false;
	mask = 16;
}

// 更新
void UpdateTitle()
{
	++blink;			// 点滅タイミング用タイマ更新

	UpdateFade();		// フェードイン・アウト更新
	if (IsFading())		// フェードイン・アウト中？
		return;
	if (fadeout) {		// フェードアウト完了？
		SetScene(2);	// ゲーム画面へ
		return;
	}
	if ((padStatus.release & PAD_START)
	|| (prev_touch && !tpStatus.touch))
	{
		if (!fadeout) {
			FadeOut(1.0f);	// フェードアウト開始
			fadeout = true;	// フェードアウト開始
			mask = 4;
		}
	}
	prev_touch = tpStatus.touch;
}

// 描画
void DrawTitle()
{
	BeforeRender();     // ポリゴン表示前処理

	// Clear the render buffers
	u8 clearColor[] = { 25, 25, 122, 255 }; // 濃い青
	nn::gd::Memory::ClearTargets(s_MainRenderTarget, s_MainDepthStencilTarget, clearColor, 1.f, 0); // 画面クリア

	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texTitle);
	CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f);	// ポリゴン表示位置セット
	SetFrame();
	DrawPolygon(4, 0);	// ポリゴン表示（頂点数,頂点データ配列の要素番号）

	if (blink & mask) {
		static const u8 red[] = {255, 0, 0, 255};
		DrawText(-96.0f, -24.0f, "GAME STERT!", red,
								 "00000000000");
	}	//							   ↑1のところが平仮名に.

	DrawFade();			// フェードイン・アウト

	AfterRender();		// ポリゴン表示後処理
}

// 終了処理
void FinTitle()
{
	StopBGM();
}
