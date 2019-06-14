// �^�C�g�����(Title.cpp)
#include <nn/gd.h>
#include "Title.h"
#include "GxCustm.h"
#include "Main.h"
#include "Texture.h"
#include "GxInit.h"
#include "Sound.h"
#include "Fade.h"
#include "Text.h"

static u8 prev_touch;
static bool fadeout;
static u32 blink = 0;
static u32 mask = 16;

// ������
void InitTitle() 
{
	PlayBGM(BGM_TITLE);
	SetFadeColor(255, 255, 255);
	FadeIn(1.0f);
	fadeout = false;
	mask = 16;
}

// �X�V
void UpdateTitle()
{
	++blink;			// �_�Ń^�C�~���O�p�^�C�}�X�V

	UpdateFade();		// �t�F�[�h�C���E�A�E�g�X�V
	if (IsFading())		// �t�F�[�h�C���E�A�E�g���H
		return;
	if (fadeout) {		// �t�F�[�h�A�E�g�����H
		SetScene(2);	// �Q�[����ʂ�
		return;
	}
	if ((padStatus.release & PAD_START)
	|| (prev_touch && !tpStatus.touch))
	{
		if (!fadeout) {
			FadeOut(1.0f);	// �t�F�[�h�A�E�g�J�n
			fadeout = true;	// �t�F�[�h�A�E�g�J�n
			mask = 4;
		}
	}
	prev_touch = tpStatus.touch;
}

// �`��
void DrawTitle()
{
	BeforeRender();     // �|���S���\���O����

	// Clear the render buffers
	u8 clearColor[] = { 25, 25, 122, 255 }; // �Z����
	nn::gd::Memory::ClearTargets(s_MainRenderTarget, s_MainDepthStencilTarget, clearColor, 1.f, 0); // ��ʃN���A

	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texTitle);
	CreateMatrix(0.0f, 0.0f, 0.0f, 256.0f, 128.0f);	// �|���S���\���ʒu�Z�b�g
	SetFrame();
	DrawPolygon(4, 0);	// �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j

	if (blink & mask) {
		static const u8 red[] = {255, 0, 0, 255};
		DrawText(-96.0f, -24.0f, "GAME STERT!", red,
								 "00000000000");
	}	//							   ��1�̂Ƃ��낪��������.

	DrawFade();			// �t�F�[�h�C���E�A�E�g

	AfterRender();		// �|���S���\���㏈��
}

// �I������
void FinTitle()
{
	StopBGM();
}
