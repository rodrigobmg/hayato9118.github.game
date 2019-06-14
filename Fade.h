// �t�F�[�h�C���E�A�E�g��` (Fade.h)
#ifndef ___H_FADE
#define ___H_FADE
#include <nn/os.h>

// �t�F�[�h�C���E�A�E�g�`��(����)
void DrawFade();
// �t�F�[�h�C���E�A�E�g�`��(�����)
void DrawFadeSub();
// �t�F�[�h�C���E�A�E�g�J���[�ݒ�
void SetFadeColor(u8 colorR, u8 colorG, u8 colorB);
// �t�F�[�h�C��
void FadeIn(f32 fSec);
// �t�F�[�h�A�E�g
void FadeOut(f32 fSec);
// �t�F�[�h�C���E�A�E�g�X�V
void UpdateFade();
// �t�F�[�h�C���E�A�E�g��
int IsFading();
// �t�F�[�h�A�E�g��
int IsFadeOut();
// �t�F�[�h�C����
int IsFadeIn();

void FillRectMain(u8 colorR, u8 colorG, u8 colorB, u8 colorA);
void FillRectSub(u8 colorR, u8 colorG, u8 colorB, u8 colorA);

#endif // ___H_FADE
