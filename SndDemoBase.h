/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     SndDemoBase.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_SAMPLE_DEMOS_SND_COMMON_SND_DEMO_BASE_H_
#define NN_SAMPLE_DEMOS_SND_COMMON_SND_DEMO_BASE_H_

#include <nn/fnd.h>
#include <nn/hid.h>
#include "demo.h"

class SndDemoBase
{
public:
	void Initialize();
	void Start();
	void Exec();
	void End();
	void Finalize();

protected:
	SndDemoBase() {};
	virtual ~SndDemoBase() {};

	virtual void OnInitialize() {};
	virtual void OnFinalize() {};
	virtual void OnUpdate() {};


	static void SoundThreadFunc(uptr arg);
	virtual void SoundThreadFuncImpl() {};

	void* AllocateMemory(size_t size, int alignment = 4);
	void FreeMemory(void* memory);

private:
	void InitializeInternal();
	void FinalizeInternal();

private:
	static const size_t DEVICE_MEMORY_SIZE = 32 * 1024 * 1024;

	nn::fnd::ExpHeap m_DeviceHeap;

	uptr m_HeapAddressForGx;

};

#endif  // NN_SAMPLE_DEMOS_SND_COMMON_SND_DEMO_BASE_H_
