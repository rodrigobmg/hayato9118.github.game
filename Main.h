// メイン定義 (Main.h)
#ifndef ___MAIN_H
#define ___MAIN_H

#include <nn/hid.h>

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define PAD_LEFT    nn::hid::BUTTON_LEFT
#define PAD_RIGHT   nn::hid::BUTTON_RIGHT
#define PAD_UP      nn::hid::BUTTON_UP
#define PAD_DOWN    nn::hid::BUTTON_DOWN
#define PAD_L       nn::hid::BUTTON_L
#define PAD_R       nn::hid::BUTTON_R
#define PAD_A       nn::hid::BUTTON_A
#define PAD_B       nn::hid::BUTTON_B
#define PAD_X       nn::hid::BUTTON_X
#define PAD_Y       nn::hid::BUTTON_Y
#define PAD_START   nn::hid::BUTTON_START

#define MAX_HISCORE	5

//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------
void CreateMatrix(f32 tx, f32 ty, f32 tz, f32 sx = 1.0f, f32 sy = 1.0f, f32 sz = 1.0f, f32 angle = 0.0f);
void SetScene(s32 scene);
s32 random(s32 range);
void AddScore(s32 score);	// スコア加算
s32 GetScore();				// スコア取得
s32 GetHiScore(s32 i);		// ハイスコア取得
s32 GetFadeOutFlag();
void SetFadeOutFlag();

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
extern nn::hid::PadStatus          padStatus;      // キー情報
extern nn::hid::TouchPanelStatus   tpStatus;       // タッチパネル情報
extern nn::hid::AccelerationFloat  acceleration;   // 加速度センサ情報

#endif // ___MAIN_H
