// プレイヤー定義 (Player.h)
#ifndef ___H_PLAYER_
#define ___H_PLAYER_

#include <nn/math.h>

// 関数プロトタイプ
void InitPlayer();	// 初期化
void UpdatePlayer();// 更新
void DrawPlayer();	// 描画
void FinPlayer();	// 終了処理
nn::math::VEC2 GetPlayerPos();
					// 位置取得
s32 GetPlayerHP();	// HP取得
bool GetPlayerKey();// 鍵所持状態取得

#endif
