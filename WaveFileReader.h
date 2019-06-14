/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     WaveFileReader.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_READER_H_
#define NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_READER_H_

#include <nn/fs.h>

class WaveFileReader
{
private:
    nn::fs::FileReader m_Reader;
    s64 m_DataSize;
    s64 m_DataOffset;
    s64 m_ReadPosition;
    s32 m_ChannelCount;
    s32 m_SampleRate;
    s32 m_QuantumBits;
    NN_PADDING4;
private:
    void ReadInfo();
public:
    /*!
        @brief      コンストラクタです。ファイルは開きません。
     */
    WaveFileReader();
    /*!
        @brief      指定されたファイルを開くコンストラクタです。
        @param[in]  pPathName   ファイルパス
     */
    WaveFileReader(const char* pPathName);
    /*!
        @brief      デストラクタです。
                    Finalize() が呼ばれていない場合は内部で自動的に呼び出します。
     */
    ~WaveFileReader();
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
        @brief      波形データのサイズ (bytes) を取得します。
                    ファイルが開かれていなければ 0 を返します。
        @return     波形データのサイズ
     */
    s64 GetDataSize() const { return m_DataSize; }
    /*!
        @brief      チャンネル数を取得します。
                    ファイルが開かれていなければ 0 を返します。
        @return     チャンネル数
     */
    s32 GetChannelCount() const { return m_ChannelCount; }
    /*!
        @brief      サンプルレートを取得します。
                    ファイルが開かれていなければ 0 を返します。
        @return     サンプルレート
     */
    s32 GetSampleRate() const { return m_SampleRate; }
    /*!
        @brief      量子化ビット数を取得します。
                    ファイルが開かれていなければ 0 を返します。
        @return     量子化ビット数
     */
    s32 GetQuantumBits() const { return m_QuantumBits; }
    /*!
        @brief      波形データを取得します。
        @param[out] pData       波形データを格納するバッファ
        @param[in]  size        波形データのサイズ (bytes)
        @return     実際に取得したデータのサイズ (bytes) を返します。
     */
    s64 ReadData(void* pData, s64 size);
};

#endif  // NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_READER_H_
