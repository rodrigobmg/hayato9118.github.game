#include "SimplePlayer.h"

// �T�E���h �t�@�C����
const char* FILE_NAME[SOUND_NUM] = {					// ���yNo.
	"rom:/voice/se_maoudamashii_battle17.wav",			//    0
	"rom:/voice/se_maoudamashii_magical14.wav",			//    1
	"rom:/voice/se_maoudamashii_element_fire09.wav",	//    2
	"rom:/voice/se_maoudamashii_magical25.wav",			//    3
	"rom:/voice/se_maoudamashii_battle12.wav",			//    4
	"rom:/voice/se_maoudamashii_magical02.wav",			//    5
	"rom:/voice/se_maoudamashii_magical01.wav",			//    6
	"rom:/voice/se_maoudamashii_magical05.wav",			//    7
	"rom:/voice/se_maoudamashii_system37.wav",			//    8
	"rom:/voice/se_maoudamashii_effect04.wav",
	"rom:/voice/se_maoudamashii_voice_monster02.wav",
	"rom:/voice/se_maoudamashii_battle08.wav",
	"rom:/voice/se_maoudamashii_explosion04.wav",
	"rom:/voice/game_maoudamashii_3_theme14.wav",		//    9
	"rom:/voice/game_maoudamashii_1_battle22.wav",		//   10
	"rom:/voice/game_maoudamashii_9_jingle05.wav",		//   11
	"rom:/voice/game_maoudamashii_9_jingle07.wav",		//   12
	"rom:/voice/game_maoudamashii_2_boss01.wav",		//   13
};

// �T�E���h �v���C���[ �C���X�^���X
static SimplePlayer app;

// �T�E���h������
void InitSound()
{
	app.Initialize();
}

// �T�E���h�J�n
void StartSound()
{
	app.Start();
}

// �T�E���h�I��
void EndSound()
{
	app.End();
}

// �T�E���h�J��
void FinSound()
{
	app.Finalize();
}

// SE�Đ�
void PlaySE(int nSE)
{
	app.SetSoundState(nSE % (VOICE_NUM - 1), 0, nSE, 0);
}

// SE�Đ���~
void StopSE(int nSE)
{
	app.SetSoundState(nSE % (VOICE_NUM - 1), 1, nSE, 0);
}

// BGM�Đ��J�n
void PlayBGM(int nBGM)
{
	app.SetSoundState(VOICE_NUM - 1, 0, nBGM, 1);
}

// BGM�Đ���~
void StopBGM()
{
	app.SetSoundState(VOICE_NUM - 1, 1, 0, 0);
}

// BGM�ꎞ��~/�ĊJ
void PauseBGM()
{
	app.SetSoundState(VOICE_NUM - 1, 3, 0, 0);
}
