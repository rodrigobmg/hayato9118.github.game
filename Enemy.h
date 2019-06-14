// 敵 定義 (Enemy.h)
#ifndef ___H_ENEMY
#define ___H_ENEMY

#include <nn/math.h>

void InitEnemy();	// 初期化
void UpdateEnemy();	// 更新
void DrawEnemy();	// 描画
void FinEnemy();	// 終了処理
int CollisionEnemy(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage);
					// 当たり判定
int GetEnemyPos(int nEnemy,
	nn::math::VEC2* pos);
					// 敵位置取得

#endif	// ___H_ENEMY
