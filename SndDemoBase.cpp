/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     SndDemoBase.cpp

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#include <nn/fs.h>

#include "SndDemoBase.h"
#include "applet.h"

extern nn::fnd::ExpHeap s_AppHeap;

void SndDemoBase::Initialize()
{
    // applet まわりの初期化
    // TransitionHandler::EnableSleep() を呼ぶまでは、自動的にスリープを拒否する
    TransitionHandler::Initialize();
}

void SndDemoBase::Finalize()
{
    // applet まわりの終了処理
    TransitionHandler::Finalize();

    // アプリを終了します。nn::applet::CloseApplication() からは戻らない
//    nn::applet::CloseApplication();
}

void SndDemoBase::InitializeInternal()
{
    // fs の初期化
//    nn::fs::Initialize();//他でやってる

    OnInitialize();
}

void SndDemoBase::FinalizeInternal()
{
    OnFinalize();

    // デバイスメモリの解放
 //   m_DeviceHeap.Finalize();

}

void SndDemoBase::Start()
{
    // この時点で終了通知が来ていないかを確認する必要がある
    if (!TransitionHandler::IsExitRequired())
    {
        // 初期化
        InitializeInternal();

        // 以降、スリープをアプリ自身でハンドリング
        TransitionHandler::EnableSleep();
	}
}

void SndDemoBase::Exec(void)
{
            OnUpdate();
}

void SndDemoBase::End(){
       // 終了処理
       FinalizeInternal();
}

void SndDemoBase::SoundThreadFunc(uptr arg)
{
    SndDemoBase* pApp = reinterpret_cast<SndDemoBase*>(arg);
    NN_ASSERT(pApp);
    pApp->SoundThreadFuncImpl();
}

void* SndDemoBase::AllocateMemory(size_t size, int alignment)
{
//    return m_DeviceHeap.Allocate(size, alignment);
    return s_AppHeap.Allocate(size, alignment);

}

void SndDemoBase::FreeMemory(void* p)
{
    if (p)
    {
        m_DeviceHeap.Free(p);
    }
}
