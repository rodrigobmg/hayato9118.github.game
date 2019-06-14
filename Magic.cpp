// 魔法 実装 (Magic.cpp)

//==============================================================
// インクルード
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include "Magic.h"
#include "Texture.h"
#include "Main.h"
#include "GxCustm.h"
#include "Sound.h"

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define MAX_MAGIC		200		// 最大表示数
#define LIFE_MAGIC0		120		// 寿命
#define SPEED_MAGIC0	2.0f	// 速度
#define MAGIC0_W		12.0f	// 幅
#define MAGIC0_H		12.0f	// 高さ

//--------------------------------------------------------------
// 構造体
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TMagic {// 魔法
	nn::math::VEC2	pos;	// 位置
	nn::math::VEC2	vel;	// 速度
	s32				timer;	// タイマー
	s32				kind;	// 種類(0:ファイヤボール)
    s32				stat;   // 状態
    s32				pat;    // フレームNo.
    s32				cnt;    // フレームカウンタ
} TMagic;

//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
static ANIM_PAT g_animPat0[] =
{
	{15, 1}, {-1, -1}
};
static ANIM_PAT* g_animPat[] = {
	g_animPat0,
};
static TMagic g_magic[MAX_MAGIC];

//--------------------------------------------------------------
// 魔法描画
//--------------------------------------------------------------
void DrawMagic()
{
	TMagic* pEffect = g_magic;
	for (int i = 0; i < MAX_MAGIC; ++i, ++pEffect) {
		if (pEffect->stat == 0)
			continue;
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy2);
        CreateMatrix(pEffect->pos.x, pEffect->pos.y, 0.0f, 16.0f, 16.0f);	// ポリゴン表示位置セット
        SetFrame(g_animPat[pEffect->kind][pEffect->pat].frame);
        DrawPolygon(4, 0);							// ポリゴン表示（頂点数,頂点データ配列の要素番号）
    }
}

//--------------------------------------------------------------
// 魔法更新
//--------------------------------------------------------------
void UpdateMagic()
{
    for (int i = 0; i < MAX_MAGIC; ++i) {
		// 待機中？
        if (g_magic[i].stat == 0) {
			continue;	// 次へ
        }
		// 寿命？
		--g_magic[i].timer;
		if (g_magic[i].timer <= 0) {
			g_magic[i].stat = 0;
			continue;
		}
		// 移動
		g_magic[i].pos += g_magic[i].vel;
		if (g_magic[i].pos.x <=
nn::gx::DISPLAY0_HEIGHT * -0.5f - 16 ||
			g_magic[i].pos.x >=
nn::gx::DISPLAY0_HEIGHT * 0.5f + 16 ||
			g_magic[i].pos.y <=
nn::gx::DISPLAY0_WIDTH * -0.5f - 16 ||
			g_magic[i].pos.y >=
nn::gx::DISPLAY0_WIDTH * 0.5f + 16) {
			g_magic[i].stat = 0;// 待機
			continue;
		}
		// カウンタを減算
        --g_magic[i].cnt;
		// ゼロになったら
        if (g_magic[i].cnt <= 0) {
			// 次の絵へ
            ++g_magic[i].pat;
			// 絵の番号が-1なら
            if (g_animPat
[g_magic[i].kind][g_magic[i].pat].frame
< 0) {
				switch (g_magic[i].kind)
				{
				case 0:
					// 先頭に戻る
					g_magic[i].pat = 0;
					break;
				default:
					// アニメーション終了
					g_magic[i].stat = 0;
					continue;
				}
            }
			// カウンタを初期化
			g_magic[i].cnt = g_animPat
[g_magic[i].kind][g_magic[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// 魔法初期化
//--------------------------------------------------------------
void InitMagic()
{
	// 全て「待機中」に設定
    for (int i = 0; i < MAX_MAGIC; ++i) {
        g_magic[i].stat = 0;
    }
}

//--------------------------------------------------------------
// 魔法終了処理
//--------------------------------------------------------------
void FinMagic()
{
	// 全て「待機中」に設定
	for (int i = 0; i < MAX_MAGIC; ++i) {
		g_magic[i].stat = 0;
	}
}

//--------------------------------------------------------------
// 魔法攻撃
//--------------------------------------------------------------
void AttackMagic(nn::math::VEC2* pos,
	nn::math::VEC2* dir, s32 kind)
{
	for (int i = 0; i < MAX_MAGIC; ++i) {
		// 再生中？
		if (g_magic[i].stat > 0) {
			continue;	// 次へ
		}
		PlaySE(SE_FIREBALL);
		// 初期値を設定
		g_magic[i].stat = 1;
		g_magic[i].pos = *pos;
		g_magic[i].pat = 0;
		g_magic[i].cnt =
			g_animPat[kind][0].count;
		nn::math::VEC2Normalize(
			&g_magic[i].vel, dir);
		g_magic[i].kind = kind;
		switch (kind) {
		case 0:
			g_magic[i].vel *=
				SPEED_MAGIC0;
			g_magic[i].timer =
				LIFE_MAGIC0;
			break;
		}
		break;	// 繰り返しを終了
	}
}

// 当たり判定
int CollisionMagic(nn::math::VEC2* pos,
	nn::math::VEC2* rect,
	nn::math::VEC2* posMagic)
{
	// 全ての魔法との当たり判定
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		// 待機状態の魔法はスキップ
		if (g_magic[i].stat == 0)
			continue;
		// 矩形同士の当たり判定
		if (pos->x - rect->x <=
			g_magic[i].pos.x + MAGIC0_W &&
			g_magic[i].pos.x - MAGIC0_W <=
			pos->x + rect->x &&
			pos->y - rect->y <=
			g_magic[i].pos.y + MAGIC0_H &&
			g_magic[i].pos.y - MAGIC0_H <=
			pos->y + rect->y) {
			// 当たっている
			g_magic[i].stat = 0;
			*posMagic = g_magic[i].pos;
			return i;
		}
	}
	return -1;
}
