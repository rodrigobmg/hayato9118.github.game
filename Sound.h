#ifndef _H_SOUND
#define _H_SOUND

void InitSound();		// �T�E���h������
void StartSound();		// �T�E���h�g�p�J�n
void EndSound();		// �T�E���h�g�p�I��
void FinSound();		// �T�E���h���
void PlaySE(int nSE);	// SE�Đ�
void StopSE(int nSE);	// SE��~
void PlayBGM(int nBGM);	// BGM�Đ�
void StopBGM();			// BGM��~
void PauseBGM();		// BGM�ꎞ��~/�ĊJ

// �ȃf�[�^�ɑΉ�����萔
//const int SOUND_NUM = 9;	// �t�@�C����
enum {
	SE_SWORD = 0,
	SE_BEAM,
	SE_FIREBALL,
	SE_DEADEFFECT,
	SE_PLAYERDAMAGE,
	SE_PLAYERDEAD,
	SE_ITEMOK,
	SE_ITEMNG,
	SE_PAUSE,
	SE_DOOROPEN,
	SE_MONSTER,
	SE_DAMAGE,
	SE_EXPLOSION,
	BGM_TITLE,
	BGM_STAGE1,
	BGM_GAMECLEAR,
	BGM_GAMEOVER,
	BGM_BOSS1,

	SOUND_NUM
};

#endif
