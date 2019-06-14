/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     WaveFileReader.cpp

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#include <nn/math.h>
#include "WaveFile.h"
#include "WaveFileReader.h"

WaveFileReader::WaveFileReader() :
    m_DataSize(0), m_DataOffset(0), m_ReadPosition(0),
    m_ChannelCount(0), m_SampleRate(0), m_QuantumBits(0)
{
}

WaveFileReader::WaveFileReader(const char* pPathName) :
    m_DataSize(0), m_DataOffset(0), m_ReadPosition(0),
    m_ChannelCount(0), m_SampleRate(0), m_QuantumBits(0)
{
    Initialize(pPathName);
}

WaveFileReader::~WaveFileReader()
{
    Finalize();
}

void WaveFileReader::Initialize(const char* pPathName)
{
    if (m_DataSize == 0 && m_DataOffset == 0)
    {
        m_Reader.Initialize(pPathName);
        ReadInfo();

        NN_LOG("%s (%1dch, %5dHz, %2d-bit wav file)\n",
               pPathName, m_ChannelCount, m_SampleRate, m_QuantumBits);
    }
}
void WaveFileReader::Finalize()
{
    if (m_DataSize != 0 && m_DataOffset != 0)
    {
        m_Reader.Finalize();
        m_DataSize = 0;
        m_DataOffset = 0;
        m_ChannelCount = 0;
        m_SampleRate = 0;
        m_QuantumBits = 0;
    }
}

void WaveFileReader::ReadInfo()
{
    m_Reader.SetPosition(0);

    WaveRiff riff;
    WaveChunk chunk;

    // RIFF ヘッダの確認
    m_Reader.Read(&riff, sizeof(riff));
    if (riff.tag != GetTagValue("RIFF"))
    {
        NN_PANIC("Not RIFF format.\n");
    }
    if (riff.type != GetTagValue("WAVE"))
    {
        NN_PANIC("Not Wave format.\n");
    }

    // fmt と data を探す
    bool isFormatChunkFound = false;
    bool isDataChunkFound = false;
    while (!isFormatChunkFound || !isDataChunkFound)
    {
        m_Reader.Read(&chunk, sizeof(WaveChunk));

        // "fmt "
        if (chunk.tag == GetTagValue("fmt "))
        {
            WaveFormat format;
            m_Reader.Read(&format, sizeof(format));
            m_ChannelCount = format.channel;
            m_SampleRate = format.sampleRate;
            m_QuantumBits = format.quantumBits;

            // 拡張領域があれば読み捨て
            if (chunk.size > sizeof(format))
            {
                u16 ext_size;
                m_Reader.Read(&ext_size, sizeof(ext_size));
                m_Reader.Seek(ext_size, nn::fs::POSITION_BASE_CURRENT);
            }
            isFormatChunkFound = true;
        }
        // "data"
        else if (chunk.tag == GetTagValue("data"))
        {
            m_DataSize = chunk.size;
            m_DataOffset = m_Reader.GetPosition();
            isDataChunkFound = true;
        }
        // それ以外は読み飛ばす
        else
        {
            m_Reader.Seek(chunk.size, nn::fs::POSITION_BASE_CURRENT);
        }
    }
}

s64 WaveFileReader::ReadData(void* pData, s64 size)
{
    NN_ASSERT(pData);
    size = nn::math::Min(size, m_DataSize - m_ReadPosition);
    m_Reader.SetPosition(m_DataOffset + m_ReadPosition);
    m_Reader.Read(pData, size);
    // 8-bit wav ファイルの場合は unsigned -> signed の変換が必要
    if (GetQuantumBits() == 8)
    {
        s8* p = reinterpret_cast<s8*>(pData);
        for (int i = 0; i < size; i++)
        {
            p[i] -= 128;
        }
    }
    m_ReadPosition += size;
    return size;
}
