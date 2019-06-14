// ゲームオーバー画面(GameOver.cpp)
#include <nn/gd.h>
#include "GameOver.h"
#include "GxCustm.h"
#include "Main.h"
#include "Texture.h"
#include "GxInit.h"
#include "Number.h"
#include "Sound.h"
#include "Fade.h"

static u8 prev_touch;
static u32 timer;
static bool fadeout;

// 初期化
void InitGameOver() 
{
	PlaySE(BGM_GAMEOVER);

	timer = 600;	// 10秒
	prev_touch = tpStatus.touch;

	FadeIn(1.0f);
	fadeout = false;
}

// 更新
void UpdateGameOver()
{
	UpdateFade();	// フェードイン・アウト更新
	if (IsFading())
		return;
	if (fadeout) {
		SetScene(1);
		return;
	}
	if ((padStatus.release & PAD_START)
	|| (prev_touch && !tpStatus.touch))
	{
		if (!fadeout) {
			FadeOut(1.0f);	// フェードアウト開始
			fadeout = true;	// フェードアウト開始
		}
		return;
	}
	prev_touch = tpStatus.touch;
	if (!timer) {
		if (!fadeout) {
			FadeOut(1.0f);	// フェードアウト開始
			fadeout = true;	// フェードアウト開始
		}
		return;
	}
	--timer;
}

// 描画
void DrawGameOver()
{
	BeforeRender();     // ポリゴン表示前処理

						// Clear the render buffers
	u8 clearColor[] = { 25, 25, 122, 255 }; // 濃い青
	nn::gd::Memory::ClearTargets(s_MainRenderTarget, s_MainDepthStencilTarget, clearColor, 1.f, 0); // 画面クリア

	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texGameOver);
	CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f);    // ポリゴン表示位置セット
	SetFrame();
	DrawPolygon(4, 0);                      // ポリゴン表示（頂点数,頂点データ配列の要素番号）

	s32 sc = GetScore();
	nn::math::VEC2 pos;
	pos.x = -(24 * 2.5f);
	pos.y = nn::gx::DISPLAY0_WIDTH / -2 + (32 + 2) * 5;
	bool blink = false;
	for (s32 i = 0; i < MAX_HISCORE; ++i) {
		s32 hisc = GetHiScore(i);
		if (sc > 0 && hisc == sc && !(timer & 8))
			blink = true;
		else
			DrawNumber(&pos, 5, hisc);
		pos.y -= (32 + 2);
	}
	if (!blink) {
		pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - 24 * 5;
		pos.y = nn::gx::DISPLAY0_WIDTH / 2;
		DrawNumber(&pos, 5, sc);
	}

	DrawFade();			// フェードイン・アウト

	AfterRender();      // ポリゴン表示後処理
}

// 終了処理
void FinGameOver()
{
	StopSE(BGM_GAMEOVER);
}
