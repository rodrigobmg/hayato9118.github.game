// 魔法 定義 (Magic.h)
#ifndef ___H_MAGIC
#define ___H_MAGIC

#include <nn/math.h>

void InitMagic();	// 初期化
void UpdateMagic();	// 更新
void DrawMagic();	// 描画
void FinMagic();	// 終了処理
void AttackMagic(
	nn::math::VEC2* pos,
	nn::math::VEC2* dir,
	s32 kind);		// 攻撃
int CollisionMagic(
	nn::math::VEC2* pos,
	nn::math::VEC2* rect,
	nn::math::VEC2* posMagic);
					// 当たり判定

#endif	// ___H_MAGIC
