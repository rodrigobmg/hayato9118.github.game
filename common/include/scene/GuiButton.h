/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     GuiButton.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_GUIBUTTON_H_
#define NN_COMMON_SCENE_GUIBUTTON_H_

#include "GuiControlBase.h"

namespace scene
{
    /*!
        @brief ボタンコントロールクラスです。
    */
    class Button : public ControlBase
    {
    public:
        /*!
            @brief コンストラクタです。

            @param[in]  id          コントロールID。
            @param[in]  x           X座標。
            @param[in]  y           Y座標。
            @param[in]  width       幅。
            @param[in]  height      高さ。
            @param[in]  pText       キャプション文字列。
            @param[in]  pExtraData  拡張データ。
        */
        Button(u32 id, s32 x, s32 y, u32 width, u32 height, const wchar_t* pText = NULL, void* pExtraData = NULL, f32 fontScale = DEFAULT_FONT_SCALE);

        /*!
            @brief デストラクタです。
        */
        virtual ~Button();

    protected:
        // ペンで押された時の処理
        virtual void OnPenDown();
        // ペンが離された時の処理
        virtual void OnPenUp(bool isIn);
        // ペンでスライドされた時の処理
        virtual void OnPenSlide(bool isIn);
        // ペンでタッチされた時の処理
        virtual void OnPenTouch();
        // 描画時の処理
        virtual void OnDraw();
    };
}

#endif // NN_COMMON_SCENE_GUIBUTTON_H_
