/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     GuiTypes.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_GUITYPES_H_
#define NN_COMMON_SCENE_GUITYPES_H_

#include <nn/types.h>

namespace scene
{
    /*!
        @brief 使用しないコントロールのID
    */
    static const u32 CONTROL_ID_UNUSED = 0xFFFFFFFF;

    /*!
        @brief コントロールの種類
    */
    enum ControlType
    {
        CONTROL_TYPE_LABEL,  //!< ラベル
        CONTROL_TYPE_BUTTON, //!< ボタン

        CONTROL_TYPE_MAX
    };

    /*!
        @brief コントロールの状態
    */
    enum ControlState
    {
        CONTROL_STATE_DEFAULT,   //!< 通常
        CONTROL_STATE_DISABLED,  //!< 無効
        CONTROL_STATE_SELECTING, //!< 選択中
        CONTROL_STATE_HIDE,      //!< 非表示

        CONTROL_STATE_MAX
    };

    /*!
        @brief コントロールのイベント
    */
    enum ControlEvent
    {
        CONTROL_EVENT_PENDOWN,      //!< ペンで押された
        CONTROL_EVENT_PENUP,        //!< ペンが離された
        CONTROL_EVENT_SLIDE,        //!< スライドされた
        CONTROL_EVENT_TOUCH,        //!< タッチされた
        CONTROL_EVENT_CHANGE_STATE, //!< 状態が変化した

        CONTROL_EVENT_MAX
    };

    /*!
        @brief テキストの配置位置
    */
    enum TextAlign
    {
        TEXT_ALIGN_LEFT,   //!< 左寄せ
        TEXT_ALIGN_RIGHT,  //!< 右寄せ
        TEXT_ALIGN_CENTER, //!< 中央揃え

        TEXT_ALIGN_MAX
    };

    //! @brief フォントスケールのデフォルト値
    const f32 DEFAULT_FONT_SCALE = 0.6f;
}

#endif // NN_COMMON_SCENE_GUITYPES_H_
