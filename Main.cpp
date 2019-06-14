//==============================================================
//  Main.cpp
//
//--------------------------------------------------------------
//                                    Author : Nakagawa Hiroki
//                                             Michiba  Hiroaki
//                                             HAL OSAKA
//==============================================================

//==============================================================
// �C���N���[�h
//==============================================================
#include <nn/gx.h>
#include <nn/math.h>
#include <nn/fs.h>
#include <nn/os.h>
#include <nn/init.h>
#include <nn/applet.h>
#include <nn/fnd/fnd_ExpHeap.h>
#include <nn/gd.h>
#include <math.h>

#include "Main.h"
#include "TexUtils.h"
#include "demo.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Title.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Sword.h"
#include "deadEffect.h"
#include "Player.h"
#include "Magic.h"
#include "Item.h"
#include "Number.h"
#include "Sound.h"
#include "Fade.h"
#include "GxCustm.h"
#include "GxInit.h"
#include "Texture.h"
#include "Map.h"
#include "Boss1.h"
#include "Flare.h"
#include "Text.h"

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define STAGE_TIMER		(3*60*60)
#define PAUSE_NONE		0	// �Q�[����
#define PAUSE_PAUSE		1	// �ꎞ��~��
#define PAUSE_TITLE		2	// �ꎞ��~��(�^�C�g���ɖ߂�?)
#define PAUSE_STAFF		3	// �X�^�b�t���[�� 

//--------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------
void DrawFrame(void);
int  Initialize(void);
void Finalize();

//--------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------
nn::hid::PadStatus          padStatus;      // �L�[���
nn::hid::TouchPanelStatus   tpStatus;       // �^�b�`�p�l�����
nn::hid::AccelerationFloat  acceleration;   // �����x�Z���T���

static nn::math::RandomGenerator	rg;		// ����������

s32 g_scene = 0;			// �V�[��

s32 g_score = 0;			// �X�R�A
s32 g_timer = STAGE_TIMER;	// �^�C�}�[
s32 g_hiscore[MAX_HISCORE] = {0};

static s32 fadeout;
static u32 g_pause = PAUSE_NONE;
static u32 g_blink = 0;

//==============================================================
// �v���O�����{��
//==============================================================

// �t�F�[�h�A�E�g�t���O�擾
s32 GetFadeOutFlag()
{
	return fadeout;
}

// �t�F�[�h�A�E�g�t���O�ݒ�
void SetFadeOutFlag()
{
	fadeout = 1;
}

// ��������
s32 random(s32 range)
{
	return (s32)rg.Generate((u32)range);
}

// ����������
void init_random()
{
	rg.SetSeed((s64)nn::os::Tick::GetSystemCurrent());
}

// �X�R�A���Z
void AddScore(s32 score)
{
	g_score += score;
}

// �X�R�A�擾
s32 GetScore()
{
	return g_score;
}

// �n�C�X�R�A�擾
s32 GetHiScore(s32 i)
{
	if (i >= 0 && i < MAX_HISCORE)
		return g_hiscore[i];
	return 0;
}

//--------------------------------------------------------------
// �Q�[��������
//--------------------------------------------------------------
void InitGame()
{
	PlayBGM(BGM_STAGE1);

	InitEnemy();
	InitEnemy2();
	InitSword();
	InitDeadEffect();
	InitPlayer();
	InitMagic();
	InitItem();
	ResetBoss1();
	g_timer = STAGE_TIMER;
	g_score = 0;

	FadeIn(1.0f);
	fadeout = 0;

	g_pause = PAUSE_NONE;
	SetMapDoor(false);
}

//--------------------------------------------------------------
// �Q�[���I������
//--------------------------------------------------------------
void FinGame()
{
	FinEnemy();
	FinEnemy2();
	FinSword();
	FinDeadEffect();
	FinPlayer();
	FinMagic();
	FinItem();

	// �n�C�X�R�A�X�V
	for (int i = 0; i < MAX_HISCORE; ++i) {
		if (g_hiscore[i] <= g_score) {
			if (g_hiscore[i] < g_score) {
				for (int j = MAX_HISCORE - 1; j > i; --j)
					g_hiscore[j] = g_hiscore[j - 1];
				g_hiscore[i] = g_score;
			}
			break;
		}
	}

	StopBGM();
}

// �V�[���ݒ�
void SetScene(s32 scene)
{
	if (g_scene == scene) return;
	// ���݂̃V�[���̏I������
	switch (g_scene) {
	case 1:	// �^�C�g��
		FinTitle();
		break;
	case 2:	// �Q�[��
		FinGame();
		break;
	case 3:	// �Q�[���I�[�o�[
		FinGameOver();
		break;
	case 4:	// �Q�[���N���A
		FinGameClear();
		break;
	}
	// �V�[���؂�ւ�
	g_scene = scene;
	// ���̃V�[���̏���������
	switch (g_scene) {
	case 1:	// �^�C�g��
		InitTitle();
		break;
	case 2:	// �Q�[��
		InitGame();
		break;
	case 3:	// �Q�[���I�[�o�[
		InitGameOver();
		break;
	case 4:	// �Q�[���N���A
		InitGameClear();
		break;
	}
}

//--------------------------------------------------------------
// �}�g���N�X�쐬
//--------------------------------------------------------------
void CreateMatrix(f32 vec_tx, f32 vec_ty, f32 vec_tz, f32 scl_x, f32 scl_y, f32 scl_z, f32 angle)
{
    nn::math::MTX34 trans;                      // ���s�ړ��p�s��̕ϐ�
    nn::math::MTX34 scale;                      // �g�k�p�s��̕ϐ�
    nn::math::MTX34 eye;                        // �J�������W�n�ϊ��s��p
    
    /*
      3DS�ɂ�����LCD�̔z�u����(���ʒu)�͒Z�ӂ���(x)�A���ӂ��c(y)�Őݒ肳��Ă���B
      �J�����𔽎��v����90��(-1.0f)�X���邱�ƂŒZ�ӂ��c(y)�A���ӂ���(x)�ɂȂ�悤�ɐݒ肵�Ă���B
    */
    nn::math::VEC3  camPos(0.0f, 0.4f, 9.5f);   // �J�����̈ʒu(X,Y,Z)
    nn::math::VEC3  camUp(-1.0f, 0.0f, 0.0f);   // �J�����̏���������߂�x�N�g��(X,Y,Z)
    nn::math::VEC3  target(0.0f, 0.0f, 0.0f);   // �J���������Ă���_(X,Y,Z)
    
    nn::math::VEC3  transVec(vec_tx, vec_ty, vec_tz);       // �ړ������̃x�N�g��(x,y,z�̏��ŃZ�b�g)
    nn::math::VEC3  scaleVec(scl_x, scl_y, scl_z);          // �g��k���̃x�N�g��(x,y,z�̏��ŃZ�b�g)

    //---------------------------------------------------------------------------   
    nn::math::MTX34Identity(&eye);                              // �J�������W�n�ɒP�ʍs����Z�b�g
    nn::math::MTX34LookAt(&eye, &camPos, &camUp, &target);      // ����ϊ��s������o��

    nn::math::MTX34Translate(&trans, &transVec);                // �ړ������x�N�g�����畽�s�ړ��s����쐬
    nn::math::MTX34Mult(&eye, &eye, &trans);                    // ����ϊ��s��ƍ��W�ϊ��s��i�ړ��j������
    nn::math::MTX34Scale(&scale, &scaleVec);                    // �g�k�x�N�g������g�k�s����쐬
    nn::math::MTX34Mult(&eye, &eye, &scale);                    // ����ϊ��s��ƍ��W�ϊ��s��i�g�k�j������

	if (angle != 0.0f) {
		nn::math::MTX34 rotate;
		nn::math::MTX34RotXYZDeg(&rotate, 0.0f, 0.0f, angle);
		nn::math::MTX34Mult(&eye, &eye, &rotate);
	}

	nn::math::MTX44 mv(eye);                                    // ��������3*4�s���4*4�s��ɃZ�b�g

    nn::gd::ShaderStage::SetShaderPipelineConstantFloat(shaderPipeline, s_shaderVariable_view, mv.a);
    // ����ϊ��~���[�J�����W�ϊ��s����V�F�[�_�p�C�v���C���ɃZ�b�g
}

void DrawPause()
{
	if (g_pause != PAUSE_PAUSE && g_pause != PAUSE_TITLE && g_pause != PAUSE_STAFF) return;
	f32 x = -144.0f;
	f32 y = 72.0f;
	switch (g_pause) {
	case PAUSE_PAUSE:
		DrawWindow(-52.0f, 24.0f, 7, 3);
		if (!(g_blink & 8)) return;
		DrawText(-36.0f, 8.0f, "PAUSE");
		break;
	case PAUSE_TITLE:
		DrawWindow(-72.0f, 32.0f, 9, 4);
		if ((g_blink & 63) < 8) return;
		DrawText(-36.0f, 16.0f, "EXIT?");
		DrawText(-56.0f, 0.0f, "(Y/���)", NULL,
							   "0001110");
		break;
	case PAUSE_STAFF:
		DrawWindow(x - 16.0f, y + 3.0f, 20, 11);
		DrawText(x, y, "       STAFF");
		y -=16.0f;
		DrawText(x, y, "���      :��� ���", NULL,
			           "11100000000000000");
		y -= 16.0f;
		DrawText(x, y, "��۸���  :��� ���", NULL,
			           "00000000000000000");
		y -= 16.0f;
		DrawText(x, y, "���̨��  :FSM(��)", NULL,
			           "00000000000000110");
		y -= 16.0f;
		DrawText(x, y, "�����    :ϵ���ϼ�", NULL,
			           "000000000011110111");
		y -= 16.0f;
		DrawText(x, y, "��ص     :��� ���", NULL,
                       "00000000000000000");
		y -= 16.0f;
		DrawText(x, y, "�ð�޺���:��� ���", NULL,
                       "00000111100000000");
		y -= 16.0f;
		DrawText(x, y, "�ިڸ��  :��� ���", NULL,
					   "00000000000000000");
		y -= 16.0f;
		DrawText(x, y, "����ޭ���:��� ���", NULL,
					   "00000000000000000");
		y -= 16.0f;
		DrawText(x, y, "�ݾ�     :��� ��˺", NULL,
		               "000000000000000000");
		break;
	}
}


//--------------------------------------------------------------
// �Q�[���`�揈��
//--------------------------------------------------------------
void DrawGame(void)
{
    BeforeRender();     // �|���S���\���O����

    // Clear the render buffers
    u8 clearColor[] = {25, 25, 122, 255}; // �Z����
    nn::gd::Memory::ClearTargets(s_MainRenderTarget, s_MainDepthStencilTarget, clearColor, 1.f, 0); // ��ʃN���A

	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texMapChip);
	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
	CreateMatrix(0.0f, 0.0f, 0.0f);
	DrawMap();
	SetMapChip();

    DrawEnemy();        // �G�`��
	DrawEnemy2();       // �G2�`��
	DrawBoss1();		// �{�X1�`��
	DrawPlayer();		// �v���C���[�`��
	DrawItem();			// �A�C�e���`��
	DrawSword();		// ���`��
	DrawMagic();		// ���@�`��
	DrawFlare();		// �t���A�G�t�F�N�g�`��
	DrawDeadEffect();	// ���ŃG�t�F�N�g�`��

	// ���l�\��
	nn::math::VEC2 pos;
	pos.x = nn::gx::DISPLAY0_HEIGHT / -2;
	pos.y = nn::gx::DISPLAY0_WIDTH / 2;
	DrawNumber(&pos, 3, g_timer / 60);
	pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - 24 * 5;
	DrawNumber(&pos, 5, g_score);
	pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - 24;
	pos.y = nn::gx::DISPLAY0_WIDTH / -2 + 32;
	DrawNumber(&pos, 1, GetPlayerHP());

	DrawBoss1HP(-124.0f, -88.0f);

	DrawPause();

	DrawFade();			// �t�F�[�h�C���E�A�E�g

    AfterRender();		// �|���S���\���㏈��
}

//--------------------------------------------------------------
// ���͏���
//--------------------------------------------------------------
void UpdateKey(void)
{
    // Pad
    static nn::hid::PadReader padReader;
    padReader.ReadLatest(&padStatus);
    // Touch
    static nn::hid::TouchPanelReader tpReader;
    tpReader.ReadLatest(&tpStatus);
    // Acceleration
    static nn::hid::AccelerometerReader accReader;
    nn::hid::AccelerometerStatus accStatus;
    accReader.ReadLatest(&accStatus);
    accReader.ConvertToAcceleration(&acceleration, 1, &accStatus);
}

//--------------------------------------------------------------
// �Q�[���X�V
//--------------------------------------------------------------
void UpdateGame()
{
	++g_blink;

	UpdateFade();		// �t�F�[�h�C���E�A�E�g�X�V
	if (IsFading())
		return;
	if (fadeout) {
		SetScene(fadeout + 2);
		return;
	}
	switch (g_pause) {
	case PAUSE_PAUSE:
		if (padStatus.trigger & PAD_START) {
			PlaySE(SE_PAUSE);
			PauseBGM();
			g_pause = PAUSE_NONE;
			break;
		}
		if (padStatus.trigger & PAD_X) {
			PlaySE(SE_PAUSE);
			g_pause = PAUSE_TITLE;
			return;
		}
		if (padStatus.trigger & PAD_R) {
			PlaySE(SE_PAUSE);
			g_pause = PAUSE_STAFF;
			return;
		}
		return;
	case PAUSE_TITLE:
		if (padStatus.trigger & PAD_Y) {
			StopBGM();
			PlaySE(SE_PAUSE);
			FadeOut(1.0f);	// �t�F�[�h�A�E�g�J�n
			fadeout = -1;	// �^�C�g���Ńt�F�[�h�A�E�g
			return;
		}
		if (padStatus.trigger) {
			PauseBGM();
			PlaySE(SE_PAUSE);
			g_pause = PAUSE_PAUSE;
			return;
		}
		return;
	case PAUSE_STAFF:
		if (padStatus.trigger) {
			PlaySE(SE_PAUSE);
			g_pause = PAUSE_PAUSE;
			return;
		}
		return;
	default:
		if (padStatus.trigger & PAD_START) {
			PlaySE(SE_PAUSE);
			g_pause = PAUSE_PAUSE;
			return;
		}
		break;
	}
	UpdateEnemy();		// �G�X�V����
	UpdateEnemy2();		// �G2�X�V����
	UpdatePlayer();		// �v���C���[�X�V����
	UpdateSword();		// ���U���X�V����
	UpdateDeadEffect();	// ���ŃG�t�F�N�g�X�V����
	UpdateMagic();		// ���@�X�V����
	UpdateItem();		// �A�C�e���X�V����
	UpdateBoss1();		// �{�X1�X�V
	UpdateFlare();		// �t���A�G�t�F�N�g�X�V
	--g_timer;
	if (g_timer <= 0 || GetBoss1Stat() == BOSS1_STAT_HIDE) {
		g_timer = 0;
		if (!fadeout) {
			FadeOut(1.0f);	// �t�F�[�h�A�E�g�J�n
			if (GetBoss1Stat() == BOSS1_STAT_HIDE)

				fadeout = 2;// �N���A�Ńt�F�[�h�A�E�g
			else
				fadeout = 1;// �Q�[���I�[�o�[�Ńt�F�[�h�A�E�g
		}
	}
}

//--------------------------------------------------------------
// �ړ�����
//--------------------------------------------------------------
void PolyMove()
{
	switch (g_scene) {
	case 0:	// ������
		InitTitle();		// �^�C�g��������
		g_scene = 1;
		// THRU
	case 1:	// �^�C�g��
		UpdateTitle();		// �^�C�g���X�V����
		break;
	case 2:	// �Q�[��
		UpdateGame();		// �Q�[���X�V����
		break;
	case 3:	// �Q�[���I�[�o�[
		UpdateGameOver();	// �Q�[���I�[�o�[�X�V����
		break;
	case 4:	// �Q�[���N���A
		UpdateGameClear();	// �Q�[���N���A�X�V����
		break;
	}
}

//--------------------------------------------------------------
// ���ʕ`��
//--------------------------------------------------------------
void DrawFrame(void)
{
	switch (g_scene) {
	case 0:	// ������
		break;
	case 1:	// �^�C�g��
		DrawTitle();
		break;
	case 2:	// �Q�[��
		DrawGame();
		break;
	case 3:	// �Q�[���I�[�o�[
		DrawGameOver();
		break;
	case 4:	// �Q�[���N���A
		DrawGameClear();
		break;
	}
}

//--------------------------------------------------------------
// �Q�[�����C��
//--------------------------------------------------------------
void GameMain(void)
{
    UpdateKey();    // ���͏���
    PolyMove();     // �ړ�����
    DrawFrame();    // �`�揈��
}

//--------------------------------------------------------------
// �G���g���|�C���g
//--------------------------------------------------------------
void nnMain(void)
{
	// nn::applet::Enable() �͌Ă΂Ȃ� (SimplePlayer::Initialize()�ɂ܂�����)
	//nn::applet::Enable();

    nngxStartLcdDisplay();

	InitSound();
	Initialize();

    // hid(���͑��u)�̏�����
    nn::hid::Initialize();

	init_random();	// ����������

	StartSound();
	InitMap();
	while (1)
    {
        GameMain();
        nngxWaitVSync(NN_GX_DISPLAY_BOTH); // VSync��҂�
    }
	FinMap();
	EndSound();

    Finalize();
	FinSound();

    /* shutdown_display */
    s_AppHeap.Free(reinterpret_cast<void*>(s_HeapForGx));
    s_AppHeap.Finalize();
}
