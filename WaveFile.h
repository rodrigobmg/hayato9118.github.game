/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     WaveFile.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_H_
#define NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_H_

#include <nn.h>

struct WaveRiff
{
    u32 tag;
    u32 size;
    u32 type;
};

struct WaveChunk
{
    u32 tag;
    u32 size;
};

struct WaveFormat
{
    u16 fmtId;
    u16 channel;
    u32 sampleRate;
    u32 transRate;
    u16 blockSize;
    u16 quantumBits;
};

inline u32 GetTagValue(const char* pTag)
{
    return (pTag[0] <<  0) | (pTag[1] <<  8) |
           (pTag[2] << 16) | (pTag[3] << 24);
}

#endif  // NN_SAMPLE_DEMOS_SND_COMMON_WAVE_FILE_H_
