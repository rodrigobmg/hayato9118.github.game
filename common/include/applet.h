/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     applet.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_SAMPLE_DEMOS_DEMO1_APPLET_H_
#define NN_SAMPLE_DEMOS_DEMO1_APPLET_H_

#include <nn/os.h>
#include <nn/applet.h>

class TransitionHandler
{
public:
    typedef void (*TRANSITION_CALLBACK)();
public:
    TransitionHandler() {}
    ~TransitionHandler() {}

    /*!
        @brief  アプレットの初期化を行います。
                スリープの許可は @ref EnableSleep で行います。
     */
    static void Initialize();
    /*!
        @brief  アプレットの終了処理を行います。
     */
    static void Finalize();
    /*!
        @brief  スリープの許可を行います。
     */
    static void EnableSleep();
    /*!
        @brief  スリープを拒否にします。
     */
    static void DisableSleep();
    /*!
        @brief  スリープ、ホームボタンおよび電源ボタンに
                関する処理を行います。
     */
    static void Process();
    /*!
        @brief  アプリ終了されているかを確認します。
     */
    static bool IsExitRequired();
    /*!
        @brief  遷移に関する排他ロックを行います。
     */
    static void Lock();
    /*!
        @brief  遷移に関する排他ロックを試みます。
     */
    static bool TryLock();
    /*!
        @brief  遷移に関する排他ロックを解除します。
     */
    static void Unlock();
    /*!
        @brief  スリープに関する排他ロックを行います。
     */
    static void LockForSleep();
    /*!
        @brief  スリープに関する排他ロックを試みます。
     */
    static bool TryLockForSleep();
    /*!
        @brief  スリープに関する排他ロックを解除します。
     */
    static void UnlockForSleep();


    /*!
        @brief  スリープ移行前の処理を設定します。
     */
    static void SetPrepareSleepCallback(TRANSITION_CALLBACK f) { s_PrepareSleepCallback = f; }

    /*!
        @brief  スリープからの復帰後の処理を設定します。
     */
    static void SetAfterSleepCallback(TRANSITION_CALLBACK f) { s_AfterSleepCallback = f; }

    /*!
        @brief  HOMEメニュー移行前の処理を設定します。
     */
    static void SetPrepareHomeButtonCallback(TRANSITION_CALLBACK f) { s_PrepareHomeButtonCallback = f; }

    /*!
        @brief  HOMEメニューからの復帰後の処理を設定します。
     */
    static void SetAfterHomeButtonCallback(TRANSITION_CALLBACK f) { s_AfterHomeButtonCallback = f; }

    /*!
        @brief  電源メニュー表示前の処理を設定します。
     */
    static void SetPreparePowerButtonCallback(TRANSITION_CALLBACK f) { s_PreparePowerButtonCallback = f; }

private:
    /*!
        @brief  SleepQuery に対するコールバックです。
     */
    static AppletQueryReply SleepQueryCallback( uptr arg );
    /*!
        @brief  Awake に対するコールバックです。
     */
    static void             AwakeCallback( uptr arg );

private:
    static bool                    s_IsExitRequired;

    // スリープからの復帰後、1フレーム分描画を待ってから LCD を有効にするためのフラグ
    static volatile bool           s_IsAfterWakeUp;

    // スリープ用 Event
    static nn::os::LightEvent      s_AwakeEvent;

    // 遷移およびスリープに関する排他制御用 CriticalSection
    static nn::os::CriticalSection s_CriticalSection;
    static nn::os::CriticalSection s_CriticalSectionForSleep;

    // スリープ移行前の処理
    static TRANSITION_CALLBACK s_PrepareSleepCallback;
    // スリープからの復帰後の処理
    static TRANSITION_CALLBACK s_AfterSleepCallback;

    // HOMEメニュー移行前の処理
    static TRANSITION_CALLBACK s_PrepareHomeButtonCallback;
    // HOMEメニューからの復帰後の処理
    static TRANSITION_CALLBACK s_AfterHomeButtonCallback;

    // 電源メニュー表示前の処理
    static TRANSITION_CALLBACK s_PreparePowerButtonCallback;
};

#endif // NN_SAMPLE_DEMOS_DEMO1_APPLET_H_
