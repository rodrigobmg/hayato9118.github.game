/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     Util.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_UTIL_H_
#define NN_COMMON_SCENE_UTIL_H_

#include <nn.h>
#include "TextWriter.h"
#include "Gui.h"

#define  AOC_PANIC_IF_FAILED(result) NN_UTIL_PANIC_IF_FAILED(result)

namespace scene
{
    /*!
        @brief ワイド文字列の長さを取得します。
    */
    u32 wstrlen(const wchar_t* pText);

    /*!
        @brief ワイド文字列を数値に変換します。
    */
    u32 wtoi(const wchar_t* pText);

    /*!
        @brief メモリを初期化します。
    */
    void SetupMemory(u32 deviceMemorySize);

    /*!
        @brief メモリを開放します。
    */
    void ReleaseMemory();

    /*!
        @brief ヒープを取得します。
    */
    nn::fnd::ExpHeap* GetHeap();

    /*!
        @brief メモリを確保します。
    */
    void* Allocate(size_t size, s32 alignment = 4);

    /*!
        @brief メモリを開放します。
    */
    void Free(void* pBuffer);

    /*!
        @brief タイトルを描画します。
    */
    void SetTitle(const wchar_t* pTitle, size_t length);
    void DrawTitle(TextWriter* pTextWriter);
    void DrawTitleForApplet(TextWriter* pTextWriter);
    void DrawTitleForApplication(TextWriter* pTextWriter);

    /*!
        @brief シーンIDを描画します。
    */
    void DrawSceneId(TextWriter* pTextWriter, const wchar_t* pSceneId);

    /*!
        @brief Resultを描画します。
    */
    void DrawResult(TextWriter* pTextWriter, const nn::Result& result);

    /*!
        @brief ビジーアイコンを描画します。
    */
    void DrawBusyIcon(TextWriter* pTextWriter, u16 x, u16 y, u32 counter);

    /*!
        @brief 確認メッセージを描画します。
    */
    void DrawConfirmMessage(TextWriter* pTextWriter, const wchar_t* pMessage1, const wchar_t* pMessage2);

    /*!
        @brief UNIX時間からDateTimeを取得します。
    */
    nn::fnd::DateTime GetDateTimeFromUnixTime(s64 unixTime);

    /*!
        @brief 左ボタンを追加します。
    */
    void AddLButton(scene::ControlManager* pManager, u32 id, const wchar_t* pCaption);

    /*!
        @brief 右ボタンを追加します。
    */
    void AddRButton(scene::ControlManager* pManager, u32 id, const wchar_t* pCaption);

    /*!
        @brief YES/NOボタンを追加します。
    */
    void AddYesNoButtons(scene::ControlManager* pManager, u32 idYes, u32 idNo, const wchar_t* pCaptionYes = L"はい", const wchar_t* pCaptionNo = L"いいえ");

}

#endif // NN_COMMON_SCENE_UTIL_H_
