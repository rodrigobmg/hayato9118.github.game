/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     SimplePlayer.cpp

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#include <nn.h>
#include <nn/os.h>
#include <nn/hid.h>
#include <nn/snd.h>

#include "SimplePlayer.h"

#include "WaveFileReader.h"
#include "WaveFileWriter.h"

// サウンド ファイル名
const int FILE_NUM = SOUND_NUM;
extern const char* FILE_NAME[];	// Sound.cppで定義

void SimplePlayer::OnInitialize()
{
	// dsp, snd の初期化
	NN_UTIL_PANIC_IF_FAILED(nn::dsp::Initialize());
	NN_UTIL_PANIC_IF_FAILED(nn::dsp::LoadDefaultComponent());
	NN_UTIL_PANIC_IF_FAILED(nn::snd::Initialize());

	m_CriticalSection.Initialize();
	for (int i = 0; i < VOICE_NUM; i++) {
		s_pVoice[i] = NULL;
		v_param[i].stat = 99;	//voice(音声再生トラック）はすべて使用していない。
	}

	// サウンドスレッドを起動
	m_SoundThreadFlag = true;
	m_SoundThread.StartUsingAutoStack(
		SoundThreadFunc,
		reinterpret_cast<uptr>(this),
		SOUND_THREAD_STACK_SIZE,
		SOUND_THREAD_PRIORITY
	);
}

void SimplePlayer::OnFinalize()
{
	// サウンドスレッドの破棄
	m_SoundThreadFlag = false;
	m_SoundThread.Join();
	m_SoundThread.Finalize();

	m_CriticalSection.Finalize();

	// dsp, snd の終了処理
	NN_UTIL_PANIC_IF_FAILED(nn::snd::Finalize());
	NN_UTIL_PANIC_IF_FAILED(nn::dsp::UnloadComponent());
	nn::dsp::Finalize();
}

void SimplePlayer::OnUpdate(void)
{

}

void SimplePlayer::SetSoundState(int voice_no, int s_st, int s_no, int lp)
{
	v_param[voice_no].stat = s_st + 1;
	v_param[voice_no].sound_no = s_no;
	v_param[voice_no].loop_flg = lp;
}

void SimplePlayer::SoundThreadFuncImpl()
{
	// サウンド出力をステレオに
	nn::snd::SetSoundOutputMode(nn::snd::OUTPUT_MODE_STEREO);

	// マスターボリュームを設定
	nn::snd::SetMasterVolume(1.0f);

	NN_LOG("Loading wav files...\n");

	u8* pMemory[FILE_NUM];
	nn::snd::WaveBuffer waveBuffer[FILE_NUM];
	nn::snd::WaveBuffer waveBuffer2[VOICE_NUM];
	// ファイルを開く
	for (int i = 0; i < FILE_NUM; i++) {
		WaveFileReader reader(FILE_NAME[i]);
		dataSize[i] = reader.GetDataSize();
		channelCount[i] = reader.GetChannelCount();
		sampleRate[i] = reader.GetSampleRate();
		quantumBits[i] = reader.GetQuantumBits();

		// デバイスメモリの取得、確認
		pMemory[i] = reinterpret_cast<u8*>(AllocateMemory(dataSize[i], 32));
		NN_ASSERT(pMemory[i]);

		// Wave データを読み込み、キャッシュを無効に
		reader.ReadData(pMemory[i], dataSize[i]);
		nn::snd::FlushDataCache(reinterpret_cast<uptr>(pMemory[i]), dataSize[i]);

		//サウンドの情報を記憶するwavebuffer構造体の内容を初期化
		nn::snd::InitializeWaveBuffer(&waveBuffer[i]);

		nn::snd::SampleFormat format = (quantumBits[i] == 8) ?
		nn::snd::SAMPLE_FORMAT_PCM8 : nn::snd::SAMPLE_FORMAT_PCM16;
		//サウンドの情報を記憶するwavebuffer構造体の内容をファイルから読み込んだ情報をもとに代入
		waveBuffer[i].bufferAddress = pMemory[i];
		waveBuffer[i].sampleLength  = nn::snd::GetSampleLength(dataSize[i], format, channelCount[i]);

		//音楽をループさせない（再生時設定されるので、ここでの設定は無意味）
		waveBuffer[i].loopFlag      = false;
	}
	for (int i = 0; i < VOICE_NUM; i++) {
		//具体的な音声再生用のメモリ（voice）を確保
		nn::snd::Voice* pVoice = nn::snd::AllocVoice(128, NULL, NULL);
		NN_ASSERT(pVoice);
		//確保したメモリを再生用のトラック0～7にそれぞれアタッチ
		s_pVoice[i] = pVoice;
	}

	// サウンドスレッドは、スリープ時には自動的に停止します
	// （nn::snd::WaitForDspSync() 内で Wait 状態になります）。
	// ただし、グラフィックスとの厳密な同期が必要な場合などは
	// アプリが制御する必要があります。
	while (m_SoundThreadFlag) {
		nn::snd::WaitForDspSync();		// DSP からのデータ受信を待つ。
		nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
		nn::snd::SendParameterToDsp();	// パラメータを DSP に送信。

		for (int i = 0; i < VOICE_NUM; i++) {
			switch (v_param[i].stat) {
			case 1:
				if (!s_pVoice[i]->IsPlaying()) {

					//サンプリングレートの選択
					nn::snd::SampleFormat format = (quantumBits[v_param[i].sound_no] == 8) ?
					nn::snd::SAMPLE_FORMAT_PCM8 : nn::snd::SAMPLE_FORMAT_PCM16;

					//ここからvoiceにパラメータをセット

					s_pVoice[i]->SetChannelCount(channelCount[v_param[i].sound_no]);
					s_pVoice[i]->SetSampleFormat(format);

					// 音量の設定
					nn::snd::MixParam mix;
					mix.mainBus[nn::snd::CHANNEL_INDEX_FRONT_LEFT ] = 0.707f; // メインボリューム (L)
					mix.mainBus[nn::snd::CHANNEL_INDEX_FRONT_RIGHT] = 0.707f; // メインボリューム (R)
					s_pVoice[i]->SetMixParam(mix);
					s_pVoice[i]->SetVolume(1.0f);

					// pitch の設定
					s_pVoice[i]->SetSampleRate(sampleRate[v_param[i].sound_no]);
					s_pVoice[i]->SetPitch(1.0f);

					//-----------------------------------------------------------

					waveBuffer2[i] = waveBuffer[v_param[i].sound_no];
					waveBuffer2[i].loopFlag = (v_param[i].loop_flg == 1);

					//nn::snd::Voice::State state = s_pVoice[i]->GetState();
					s_pVoice[i]->AppendWaveBuffer(&waveBuffer2[i]);
					s_pVoice[i]->SetState(nn::snd::Voice::STATE_PLAY);
					v_param[i].stat = 99;

				} else {
					s_pVoice[i]->SetState(nn::snd::Voice::STATE_STOP);
					//v_param[i].stat = 99;
				}
				break;
			case 2:
				s_pVoice[i]->SetState(nn::snd::Voice::STATE_STOP);
				v_param[i].stat = 99;
				break;
			case 3:
				{
					nn::snd::Voice::State state = s_pVoice[i]->GetState();
					if (state == nn::snd::Voice::STATE_PAUSE) {
						s_pVoice[i]->SetState(nn::snd::Voice::STATE_PLAY);
						v_param[i].stat = 99;
					} else if (state == nn::snd::Voice::STATE_PLAY) {
						s_pVoice[i]->SetState(nn::snd::Voice::STATE_PAUSE);
						v_param[i].stat = 99;
					}
				}
				break;
			}
		}
	}

	// 再生終了
	for (int i = 0; i < VOICE_NUM; i++) {
		if (s_pVoice[i]) {
			nn::snd::FreeVoice(s_pVoice[i]);
			s_pVoice[i] = NULL;
		}
	}
	for (int i = 0; i < FILE_NUM; i++) {
		FreeMemory(pMemory[i]);
	}
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
