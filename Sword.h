// 剣による攻撃定義(Sword.h)
#ifndef ___SWORD_H
#define ___SWORD_H

#include <nn/math.h>

void InitSword();	// 初期化
void UpdateSword();	// 更新
void DrawSword();	// 描画
void FinSword();	// 終了処理
void AttackSword(nn::math::VEC2* pos,
	s32 dir, s32 beam);	// 攻撃

#endif	// ___SWORD_H
