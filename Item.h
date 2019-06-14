// アイテム定義 (Item.h)
#ifndef ___H_ITEM
#define ___H_ITEM

#include <nn/math.h>

// マクロ定義
#define ITEM_POTION		0	// 回復薬
#define ITEM_KEY		1	// 鍵

// 関数プロトタイプ
void InitItem();	// 初期化
void UpdateItem();	// 更新
void DrawItem();	// 描画
void FinItem();		// 終了処理
void StartItem(nn::math::VEC2* pos, s32 item);
					// 開始
int CollisionItem(
	nn::math::VEC2* pos,
	nn::math::VEC2* rect);
					// 当たり判定

#endif	// ___H_ITEM
