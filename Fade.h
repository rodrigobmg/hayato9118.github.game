// フェードイン・アウト定義 (Fade.h)
#ifndef ___H_FADE
#define ___H_FADE
#include <nn/os.h>

// フェードイン・アウト描画(上画面)
void DrawFade();
// フェードイン・アウト描画(下画面)
void DrawFadeSub();
// フェードイン・アウトカラー設定
void SetFadeColor(u8 colorR, u8 colorG, u8 colorB);
// フェードイン
void FadeIn(f32 fSec);
// フェードアウト
void FadeOut(f32 fSec);
// フェードイン・アウト更新
void UpdateFade();
// フェードイン・アウト中
int IsFading();
// フェードアウト中
int IsFadeOut();
// フェードイン中
int IsFadeIn();

void FillRectMain(u8 colorR, u8 colorG, u8 colorB, u8 colorA);
void FillRectSub(u8 colorR, u8 colorG, u8 colorB, u8 colorA);

#endif // ___H_FADE
