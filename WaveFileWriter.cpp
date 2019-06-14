/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     WaveFileWriter.cpp

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#include "WaveFile.h"
#include "WaveFileWriter.h"

namespace
{
    static const size_t HEADER_SIZE = sizeof(WaveRiff) + sizeof(WaveFormat) + sizeof(WaveChunk) * 2;
}


WaveFileWriter::WaveFileWriter() :
    m_DataSize(0), m_ChannelCount(0), m_SampleRate(0), m_QuantumBits(0)
{
}

WaveFileWriter::WaveFileWriter(const char* pPathName) :
    m_DataSize(0), m_ChannelCount(0), m_SampleRate(0), m_QuantumBits(0)
{
    Initialize(pPathName);
}

WaveFileWriter::~WaveFileWriter()
{
    Finalize();
}

void WaveFileWriter::Initialize(const char* pPathName)
{
    if (m_DataSize == 0)
    {
        m_Writer.Initialize(pPathName, true);
    }
}

void WaveFileWriter::Finalize()
{
    if (m_DataSize != 0)
    {
        WriteHeader();
    }
    m_Writer.Finalize();
}

void WaveFileWriter::WriteHeader()
{
    m_Writer.SetPosition(0);

    WaveRiff riff;
    riff.tag = GetTagValue("RIFF");
    riff.size =
        + sizeof(WaveRiff)                        // riff header
        + sizeof(WaveChunk) + sizeof(WaveFormat)  // wave fmt header
        + sizeof(WaveChunk) + m_DataSize          // wave data
        - 8;
    riff.type = GetTagValue("WAVE");
    m_Writer.Write(&riff, sizeof(WaveRiff), true);

    WaveChunk chunk;
    chunk.tag = GetTagValue("fmt ");
    chunk.size = sizeof(WaveFormat);
    m_Writer.Write(&chunk, sizeof(WaveChunk), true);

    WaveFormat format;
    format.fmtId = 1;  // PCM
    format.channel = m_ChannelCount;
    format.sampleRate = m_SampleRate;
    format.quantumBits = m_QuantumBits;
    format.blockSize = format.channel * format.quantumBits / 8;
    format.transRate = format.sampleRate * format.blockSize;
    m_Writer.Write(&format, sizeof(WaveFormat), true);

    chunk.tag = GetTagValue("data");
    chunk.size = m_DataSize;
    m_Writer.Write(&chunk, sizeof(WaveChunk), true);
}

void WaveFileWriter::WriteData(const void* pData, s64 size)
{
    m_Writer.SetPosition(HEADER_SIZE + m_DataSize);
    s32 writtenSize = m_Writer.Write(pData, size, true);
    m_DataSize += writtenSize;
}


#ifdef SND_DEMO_HOST_IO
HostIoWaveFileWriter::HostIoWaveFileWriter() :
    m_DataSize(0), m_ChannelCount(0), m_SampleRate(0), m_QuantumBits(0)
{
}

HostIoWaveFileWriter::HostIoWaveFileWriter(const char* pPathName) :
    m_DataSize(0), m_ChannelCount(0), m_SampleRate(0), m_QuantumBits(0)
{
    Initialize(pPathName);
}

HostIoWaveFileWriter::~HostIoWaveFileWriter()
{
    Finalize();
}

void HostIoWaveFileWriter::Initialize(const char* pPathName)
{
    if (m_DataSize == 0)
    {
        nn::Result result =
            m_Writer.Open(
                pPathName,
                nn::hio::HostFile::ACCESS_MODE_READ_WRITE,
                nn::hio::HostFile::OPEN_DISP_CREATE_ALWAYS
            );
        NN_UTIL_PANIC_IF_FAILED(result);
    }
}

void HostIoWaveFileWriter::Finalize()
{
    if (m_DataSize != 0)
    {
        WriteHeader();
    }
    m_Writer.Close();
}

void HostIoWaveFileWriter::WriteHeader()
{
    m_Writer.Seek(0, nn::hio::HostFile::SEEK_TYPE_SET);

    WaveRiff riff;
    riff.tag = GetTagValue("RIFF");
    riff.size =
        + sizeof(WaveRiff)                        // riff header
        + sizeof(WaveChunk) + sizeof(WaveFormat)  // wave fmt header
        + sizeof(WaveChunk) + m_DataSize          // wave data
        - 8;
    riff.type = GetTagValue("WAVE");
    m_Writer.Write(&riff, sizeof(WaveRiff));

    WaveChunk chunk;
    chunk.tag = GetTagValue("fmt ");
    chunk.size = sizeof(WaveFormat);
    m_Writer.Write(&chunk, sizeof(WaveChunk));

    WaveFormat format;
    format.fmtId = 1;  // PCM
    format.channel = m_ChannelCount;
    format.sampleRate = m_SampleRate;
    format.quantumBits = m_QuantumBits;
    format.blockSize = format.channel * format.quantumBits / 8;
    format.transRate = format.sampleRate * format.blockSize;
    m_Writer.Write(&format, sizeof(WaveFormat));

    chunk.tag = GetTagValue("data");
    chunk.size = m_DataSize;
    m_Writer.Write(&chunk, sizeof(WaveChunk));
}

void HostIoWaveFileWriter::WriteData(const void* pData, s64 size)
{
    m_Writer.Seek(HEADER_SIZE + m_DataSize, nn::hio::HostFile::SEEK_TYPE_SET);
    s32 writtenSize = m_Writer.Write(pData, size);
    m_DataSize += writtenSize;
}
#endif  // SND_DEMO_HOST_IO
