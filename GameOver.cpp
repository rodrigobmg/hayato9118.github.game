// �Q�[���I�[�o�[���(GameOver.cpp)
#include <nn/gd.h>
#include "GameOver.h"
#include "GxCustm.h"
#include "Main.h"
#include "Texture.h"
#include "GxInit.h"
#include "Number.h"
#include "Sound.h"
#include "Fade.h"

static u8 prev_touch;
static u32 timer;
static bool fadeout;

// ������
void InitGameOver() 
{
	PlaySE(BGM_GAMEOVER);

	timer = 600;	// 10�b
	prev_touch = tpStatus.touch;

	FadeIn(1.0f);
	fadeout = false;
}

// �X�V
void UpdateGameOver()
{
	UpdateFade();	// �t�F�[�h�C���E�A�E�g�X�V
	if (IsFading())
		return;
	if (fadeout) {
		SetScene(1);
		return;
	}
	if ((padStatus.release & PAD_START)
	|| (prev_touch && !tpStatus.touch))
	{
		if (!fadeout) {
			FadeOut(1.0f);	// �t�F�[�h�A�E�g�J�n
			fadeout = true;	// �t�F�[�h�A�E�g�J�n
		}
		return;
	}
	prev_touch = tpStatus.touch;
	if (!timer) {
		if (!fadeout) {
			FadeOut(1.0f);	// �t�F�[�h�A�E�g�J�n
			fadeout = true;	// �t�F�[�h�A�E�g�J�n
		}
		return;
	}
	--timer;
}

// �`��
void DrawGameOver()
{
	BeforeRender();     // �|���S���\���O����

						// Clear the render buffers
	u8 clearColor[] = { 25, 25, 122, 255 }; // �Z����
	nn::gd::Memory::ClearTargets(s_MainRenderTarget, s_MainDepthStencilTarget, clearColor, 1.f, 0); // ��ʃN���A

	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texGameOver);
	CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f);    // �|���S���\���ʒu�Z�b�g
	SetFrame();
	DrawPolygon(4, 0);                      // �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j

	s32 sc = GetScore();
	nn::math::VEC2 pos;
	pos.x = -(24 * 2.5f);
	pos.y = nn::gx::DISPLAY0_WIDTH / -2 + (32 + 2) * 5;
	bool blink = false;
	for (s32 i = 0; i < MAX_HISCORE; ++i) {
		s32 hisc = GetHiScore(i);
		if (sc > 0 && hisc == sc && !(timer & 8))
			blink = true;
		else
			DrawNumber(&pos, 5, hisc);
		pos.y -= (32 + 2);
	}
	if (!blink) {
		pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - 24 * 5;
		pos.y = nn::gx::DISPLAY0_WIDTH / 2;
		DrawNumber(&pos, 5, sc);
	}

	DrawFade();			// �t�F�[�h�C���E�A�E�g

	AfterRender();      // �|���S���\���㏈��
}

// �I������
void FinGameOver()
{
	StopSE(BGM_GAMEOVER);
}
