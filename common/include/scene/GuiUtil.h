/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     GuiUtil.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_GUIUTIL_H_
#define NN_COMMON_SCENE_GUIUTIL_H_

#include <nn.h>
#include <demo.h>
#include <string.h>
#include "TextWriter.h"

namespace scene
{
    /*!
        @brief 描画オブジェクトを登録します。

        @param[in]  pRenderSystem  GUIライブラリが使用するレンダラ。
        @param[in]  pTextWriter    GUIライブラリが使用するテキストライター。
    */
    void RegisterDrawers(demo::RenderSystemDrawing* pRenderSystem, TextWriter* pTextWriter);

    /*!
        @brief GUIライブラリが使用するレンダラを取得します。

        @return レンダラ。
    */
    demo::RenderSystemDrawing* GetRenderSystem();

    /*!
        @brief GUIライブラリが使用するテキストライターを取得します。

        @return テキストライター。
    */
    TextWriter* GetTextWriter();

    namespace util
    {
        /*!
            @brief 線分で箱を描画します。

            @param[in]  x          箱のX座標。
            @param[in]  y          箱のY座標。
            @param[in]  width      箱の幅。
            @param[in]  height     箱の高さ。
            @param[in]  lineWidth  ライン幅。
        */
        void DrawBox(f32 x, f32 y, f32 width, f32 height, f32 lineWidth = 1.0f);
    }
}

#endif // NN_COMMON_SCENE_GUIUTIL_H_
