// 消滅エフェクト 定義 (DeadEffect.h)
#ifndef ___H_DEADEFFECT
#define ___H_DEADEFFECT

#include <nn/math.h>

void InitDeadEffect();	// 初期化
void UpdateDeadEffect();// 更新
void DrawDeadEffect();	// 描画
void FinDeadEffect();	// 終了処理
void StartDeadEffect(nn::math::VEC2* pos);
						// 開始
void ClearDeadEffect();	// 全エフェクト消去

#endif	// ___H_DEADEFFECT
