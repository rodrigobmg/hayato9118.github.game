// アイテム実装 (Item.cpp)

//==============================================================
// インクルード
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include "Item.h"
#include "Texture.h"
#include "Main.h"
#include "GxCustm.h"

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define MAX_ITEM	200		// 最大アイテム数
#define BLINK_TIME	180		// 消滅前の点滅時間
#define ERASE_TIME	60		// 高速点滅時間
#define POTION_LIFE	600		// 回復薬表示時間
#define ITEM0_W		8.0f	// 幅
#define ITEM0_H		12.0f	// 高さ

//--------------------------------------------------------------
// 構造体
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TItem {	// アイテム
	nn::math::VEC2	pos;	// 座標
	s32				stat;	// 状態
	s32				pat;	// フレームNo.
	s32				cnt;	// フレームカウンタ
	s32				item;	// アイテム種別
	s32				life;	// 消えるまでの時間
} TItem;

//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
static ANIM_PAT g_animPat[] =
{
	{ 3, 20}, { 7, 10}, {11, 20}, { 7, 10},
	{-1, -1}
};
static TItem g_item[MAX_ITEM];

//--------------------------------------------------------------
// アイテム描画
//--------------------------------------------------------------
void DrawItem()
{
	TItem* pItem = g_item;
	for (int i = 0; i < MAX_ITEM; ++i, ++pItem) {
		if (pItem->stat == 0)
			continue;
		// 点滅処理
		if (pItem->life > 0 && pItem->life < BLINK_TIME) {
			if (pItem->life < ERASE_TIME) {
				if (pItem->life & 2) continue;
			} else {
				if (pItem->life & 4) continue;
			}
		}
		// 描画
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
		if (pItem->item == ITEM_POTION)
	        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy2);
		else
			nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy);
        CreateMatrix(pItem->pos.x, pItem->pos.y, 0.0f, 16.0f, 16.0f);	// ポリゴン表示位置セット
        SetFrame(g_animPat[pItem->pat].frame);
        DrawPolygon(4, 0);							// ポリゴン表示（頂点数,頂点データ配列の要素番号）
    }
}

//--------------------------------------------------------------
// アイテム更新
//--------------------------------------------------------------
void UpdateItem()
{
    for (int i = 0; i < MAX_ITEM; ++i) {
		// 待機中？
        if (g_item[i].stat == 0) {
			continue;	// 次へ
        }
		// 自動消去アイテムの場合
		if (g_item[i].life > 0) {
			// 生存時間を減算
			--g_item[i].life;
			// 寿命なら消去
			if (g_item[i].life <= 0) {
				g_item[i].stat = 0;
				continue;
			}
		}
		// カウンタを減算
        --g_item[i].cnt;
		// ゼロになったら
        if (g_item[i].cnt <= 0) {
			// 次の絵へ
            ++g_item[i].pat;
			// 絵の番号が-1なら
            if (g_animPat[g_item[i].pat].frame < 0) {
				// 先頭の絵に戻る
				g_item[i].pat = 0;
            }
			// カウンタを初期化
			g_item[i].cnt =
				g_animPat[g_item[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// アイテム初期化
//--------------------------------------------------------------
void InitItem()
{
	// 全て「待機中」に設定
    for (int i = 0; i < MAX_ITEM; ++i) {
        g_item[i].stat = 0;
    }
}

//--------------------------------------------------------------
// アイテム終了処理
//--------------------------------------------------------------
void FinItem()
{
	// 全て「待機中」に設定
	for (int i = 0; i < MAX_ITEM; ++i) {
		g_item[i].stat = 0;
	}
}

//--------------------------------------------------------------
// アイテム開始
//--------------------------------------------------------------
void StartItem(nn::math::VEC2* pos, s32 item)
{
	for (int i = 0; i < MAX_ITEM; ++i) {
		// 再生中？
		if (g_item[i].stat > 0) {
			continue;	// 次へ
		}
		// 初期値を設定
		g_item[i].pos = *pos;
		g_item[i].pat = 0;
		g_item[i].cnt = g_animPat[0].count;
		g_item[i].stat = 1;
		g_item[i].item = item;
		switch (item) {
		case ITEM_POTION:
			g_item[i].life = POTION_LIFE;
			break;
		default:
			g_item[i].life = 0;	// 自動消去しない
			break;
		}
		break;	// 繰り返しを終了
	}
}

//--------------------------------------------------------------
// 当たり判定
//--------------------------------------------------------------
int CollisionItem(nn::math::VEC2* pos, nn::math::VEC2* rect)
{
	// 全てのアイテムとの当たり判定
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		// 待機状態のアイテムはスキップ
		if (g_item[i].stat == 0)
			continue;
		// 矩形同士の当たり判定
		if (pos->x - rect->x <= g_item[i].pos.x + ITEM0_W &&
			g_item[i].pos.x - ITEM0_W <= pos->x + rect->x &&
			pos->y - rect->y <= g_item[i].pos.y + ITEM0_H &&
			g_item[i].pos.y - ITEM0_H <= pos->y + rect->y) {
			// 当たっている
			g_item[i].stat = 0;

			return g_item[i].item;
		}
	}
	return -1;
}
