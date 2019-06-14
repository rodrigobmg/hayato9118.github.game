#ifndef _H_SOUND
#define _H_SOUND

void InitSound();		// サウンド初期化
void StartSound();		// サウンド使用開始
void EndSound();		// サウンド使用終了
void FinSound();		// サウンド解放
void PlaySE(int nSE);	// SE再生
void StopSE(int nSE);	// SE停止
void PlayBGM(int nBGM);	// BGM再生
void StopBGM();			// BGM停止
void PauseBGM();		// BGM一時停止/再開

// 曲データに対応する定数
//const int SOUND_NUM = 9;	// ファイル数
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
