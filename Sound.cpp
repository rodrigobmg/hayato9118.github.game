#include "SimplePlayer.h"

// サウンド ファイル名
const char* FILE_NAME[SOUND_NUM] = {					// 音楽No.
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

// サウンド プレイヤー インスタンス
static SimplePlayer app;

// サウンド初期化
void InitSound()
{
	app.Initialize();
}

// サウンド開始
void StartSound()
{
	app.Start();
}

// サウンド終了
void EndSound()
{
	app.End();
}

// サウンド開放
void FinSound()
{
	app.Finalize();
}

// SE再生
void PlaySE(int nSE)
{
	app.SetSoundState(nSE % (VOICE_NUM - 1), 0, nSE, 0);
}

// SE再生停止
void StopSE(int nSE)
{
	app.SetSoundState(nSE % (VOICE_NUM - 1), 1, nSE, 0);
}

// BGM再生開始
void PlayBGM(int nBGM)
{
	app.SetSoundState(VOICE_NUM - 1, 0, nBGM, 1);
}

// BGM再生停止
void StopBGM()
{
	app.SetSoundState(VOICE_NUM - 1, 1, 0, 0);
}

// BGM一時停止/再開
void PauseBGM()
{
	app.SetSoundState(VOICE_NUM - 1, 3, 0, 0);
}
