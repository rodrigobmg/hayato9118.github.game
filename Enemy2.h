// 敵2 定義 (Enemy2.h)
#ifndef ___H_ENEMY2
#define ___H_ENEMY2

#include <nn/math.h>

void InitEnemy2();	// 初期化
void UpdateEnemy2();// 更新
void DrawEnemy2();	// 描画
void FinEnemy2();	// 終了処理
int CollisionEnemy2(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage);
					// 当たり判定
int GetEnemy2Pos(int nEnemy2,
	nn::math::VEC2* pos);
					// 敵位置取得

#endif	// ___H_ENEMY2
