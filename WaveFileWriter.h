/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     WaveFileWriter.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_WRITER_H_
#define NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_WRITER_H_

#include <nn/fs.h>
#include <nn/hio.h>

class WaveFileWriter
{
private:
    nn::fs::FileWriter m_Writer;
    s64 m_DataSize;
    s32 m_ChannelCount;
    s32 m_SampleRate;
    s32 m_QuantumBits;
    NN_PADDING4;
private:
    void WriteHeader();
public:
    /*!
        @brief      コンストラクタです。ファイルは開きません。
     */
    WaveFileWriter();
    /*!
        @brief      指定されたファイルを開くコンストラクタです。
        @param[in]  pPathName   ファイルパス
     */
    WaveFileWriter(const char* pPathName);
    /*!
        @brief      デストラクタです。
                    Finalize() が呼ばれていない場合は内部で自動的に呼び出します。
     */
    ~WaveFileWriter();
    /*!
        @brief      指定されたファイルを開きます。
                    既にファイルが開かれている場合は何も行いません。
        @param[in]  pPathName   ファイルパス
     */
    void Initialize(const char* pPathName);
    /*!
        @brief      ファイルを閉じます。
                    既にファイルが閉じられている場合は何も行いません。
        @param[in]  pPathName   ファイルパス
     */
    void Finalize();
    /*!
        @brief      チャンネル数を設定します。
        @param[in]  channelCount    チャンネル数
     */
    void SetChannelCount(s32 channelCount) { m_ChannelCount = channelCount; }
    /*!
        @brief      サンプルレートを設定します。
        @param[in]  sampleRate  サンプルレート
     */
    void SetSampleRate(s32 sampleRate) { m_SampleRate = sampleRate; }
    /*!
        @brief      量子化ビット数を設定します。
        @param[in]  quantumBits 量子化ビット数
     */
    void SetQuantumBits(s32 quantumBits) { m_QuantumBits = quantumBits; }
    /*!
        @brief      波形データを書き込みます。
        @param[in]  pData       書き込む波形データバッファ
        @param[in]  size        波形データのサイズ (bytes)
     */
    void WriteData(const void* pData, s64 size);
};

#ifdef SND_DEMO_HOST_IO
class HostIoWaveFileWriter
{
private:
    nn::hio::HostFile m_Writer;
    NN_PADDING4;
    s64 m_DataSize;
    s32 m_ChannelCount;
    s32 m_SampleRate;
    s32 m_QuantumBits;
    NN_PADDING4;
private:
    void WriteHeader();
public:
    /*!
        @brief      コンストラクタです。ファイルは開きません。
    */
    HostIoWaveFileWriter();
    /*!
        @brief      指定されたファイルを開くコンストラクタです。
        @param[in]  pPathName   ファイルパス
     */
    HostIoWaveFileWriter(const char* pPathName);
    /*!
        @brief      デストラクタです。
                    Finalize() が呼ばれていない場合は内部で自動的に呼び出します。
     */
    ~HostIoWaveFileWriter();
    /*!
        @brief      指定されたファイルを開きます。
                    既にファイルが開かれている場合は何も行いません。
        @param[in]  pPathName   ファイルパス
     */
    void Initialize(const char* pPathName);
    /*!
        @brief      ファイルを閉じます。
                    既にファイルが閉じられている場合は何も行いません。
        @param[in]  pPathName   ファイルパス
     */
    void Finalize();
    /*!
        @brief      チャンネル数を設定します。
        @param[in]  channelCount    チャンネル数
     */
    void SetChannelCount(s32 channelCount) { m_ChannelCount = channelCount; }
    /*!
        @brief      サンプルレートを設定します。
        @param[in]  sampleRate  サンプルレート
     */
    void SetSampleRate(s32 sampleRate) { m_SampleRate = sampleRate; }
    /*!
        @brief      量子化ビット数を設定します。
        @param[in]  quantumBits 量子化ビット数
     */
    void SetQuantumBits(s32 quantumBits) { m_QuantumBits = quantumBits; }
    /*!
        @brief      波形データを書き込みます。
        @param[in]  pData       書き込む波形データバッファ
        @param[in]  size        波形データのサイズ (bytes)
     */
    void WriteData(const void* pData, s64 size);
};
#endif  // SND_DEMO_HOST_IO

#endif  // NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_WRITER_H_
