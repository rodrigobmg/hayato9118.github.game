// 消滅エフェクト 実装 (DeadEffect.cpp)

//==============================================================
// インクルード
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include "DeadEffect.h"
#include "Sound.h"
#include "GxCustm.h"
#include "Main.h"

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define MAX_DEADEFFECT	200

//--------------------------------------------------------------
// 構造体
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TDeadEffect {   // 消滅エフェクト
    f32 x;      // X座標
    f32 y;      // Y座標
    s32 stat;   // 状態
    s32 pat;    // フレームNo.
    s32 cnt;    // フレームカウンタ
} TDeadEffect;

extern nn::gd::Texture2D* s_texDeadEffect;
extern nn::gd::SamplerState* s_samplerDefault;

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
static ANIM_PAT g_animPat[] =
{
	{12, 4}, {13, 4}, {14, 4}, {15, 4},
	{ 8, 4}, { 9, 4}, {10, 4}, {11, 4},
	{ 4, 4}, { 5, 4}, { 6, 4}, { 7, 4},
	{ 0, 4}, { 1, 4}, { 2, 4}, { 3, 4},
	{-1, -1}
};
static TDeadEffect g_deadEffect[MAX_DEADEFFECT];

//--------------------------------------------------------------
// 消滅エフェクト描画
//--------------------------------------------------------------
void DrawDeadEffect()
{
	TDeadEffect* pEffect = g_deadEffect;
	for (int i = 0; i < MAX_DEADEFFECT; ++i, ++pEffect) {
		if (pEffect->stat == 0)
			continue;
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texDeadEffect);
        CreateMatrix(pEffect->x, pEffect->y, 0.0f, 16.0f, 16.0f);	// ポリゴン表示位置セット
        SetFrame(g_animPat[pEffect->pat].frame);
        DrawPolygon(4, 0);							// ポリゴン表示（頂点数,頂点データ配列の要素番号）
    }
}

//--------------------------------------------------------------
// 消滅エフェクト更新
//--------------------------------------------------------------
void UpdateDeadEffect()
{
    for (int i = 0; i < MAX_DEADEFFECT; ++i) {
		// 待機中？
        if (g_deadEffect[i].stat == 0) {
			continue;	// 次へ
        }
		// カウンタを減算
        --g_deadEffect[i].cnt;
		// ゼロになったら
        if (g_deadEffect[i].cnt <= 0) {
			// 次の絵へ
            ++g_deadEffect[i].pat;
			// 絵の番号が-1なら
            if (g_animPat
[g_deadEffect[i].pat].frame < 0) {
				// アニメーション終了
				g_deadEffect[i].stat = 0;
				continue;
            }
			// カウンタを初期化
			g_deadEffect[i].cnt =
g_animPat[g_deadEffect[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// 消滅エフェクト初期化
//--------------------------------------------------------------
void InitDeadEffect()
{
	// 全て「待機中」に設定
    for (int i = 0; i < MAX_DEADEFFECT; ++i) {
        g_deadEffect[i].stat = 0;
    }
}

//--------------------------------------------------------------
// 消滅エフェクト終了処理
//--------------------------------------------------------------
void FinDeadEffect()
{
	ClearDeadEffect();	// 全エフェクト消去
}

//--------------------------------------------------------------
// 消滅エフェクト開始
//--------------------------------------------------------------
void StartDeadEffect(nn::math::VEC2* pos)
{
	for (int i = 0; i < MAX_DEADEFFECT; ++i) {
		// 再生中？
		if (g_deadEffect[i].stat > 0) {
			continue;	// 次へ
		}
		PlaySE(SE_DEADEFFECT);
		// 初期値を設定
		g_deadEffect[i].x = pos->x;
		g_deadEffect[i].y = pos->y;
		g_deadEffect[i].pat = 0;
		g_deadEffect[i].cnt =
			g_animPat[0].count;
		g_deadEffect[i].stat = 1;
		break;	// 繰り返しを終了
	}
}

//--------------------------------------------------------------
// 全エフェクト消去
//--------------------------------------------------------------
void ClearDeadEffect()
{
	// 全て「待機中」に設定
	for (int i = 0; i < MAX_DEADEFFECT; ++i) {
		g_deadEffect[i].stat = 0;
	}
}
