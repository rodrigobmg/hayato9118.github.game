/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     GuiControlBase.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_GUICONTROLBASE_H_
#define NN_COMMON_SCENE_GUICONTROLBASE_H_

#include "GuiUtil.h"
#include "GuiTypes.h"
#include <nn/util/util_SizedEnum.h>

namespace scene
{
    class ControlManager;

    /*!
        @brief 全てのコントロールの基本クラスです。
    */
    class ControlBase
    {
    public:
        friend class ControlManager;

    public:
        /*!
            @brief コンストラクタです。

            @param[in]  type        コントロールの種類。
            @param[in]  id          コントロールID。
            @param[in]  x           X座標。
            @param[in]  y           Y座標。
            @param[in]  width       幅。
            @param[in]  height      高さ。
            @param[in]  pText       キャプション文字列。
            @param[in]  pExtraData  拡張データ。
        */
        ControlBase(ControlType type, u32 id, s32 x, s32 y, u32 width, u32 height, const wchar_t* pText, void* pExtraData, f32 fontScale);

        /*!
            @brief デストラクタです。
        */
        virtual ~ControlBase();

        /*!
            @brief コントロールの種類を取得します。
        */
        ControlType GetType() const
        {
            return m_type;
        }

        /*!
            @brief コントロールIDを取得します。
        */
        u32 GetId() const
        {
            return m_id;
        }

        /*!
            @brief コントロールのX座標を取得します。
        */
        s32 GetX() const
        {
            return m_x;
        }

        /*!
            @brief コントロールのY座標を取得します。
        */
        s32 GetY() const
        {
            return m_y;
        }

        /*!
            @brief コントロールの幅を取得します。
        */
        u32 GetWidth() const
        {
            return m_width;
        }

        /*!
            @brief コントロールの高さを取得します。
        */
        u32 GetHeight() const
        {
            return m_height;
        }

        /*!
            @brief テキストを取得します。
        */
        const wchar_t* GetText() const
        {
            return m_pText;
        }

        /*!
            @brief テキスト長を取得します。
        */
        s32 GetTextLength() const
        {
            return m_textLength;
        }

        /*!
            @brief 枠線の色を取得します。
        */
        nn::util::Color8 GetBorderColor(ControlState state) const
        {
            if (state < CONTROL_STATE_MAX)
            {
                return m_borderColors[state];
            }

            return nn::util::Color8();
        }

        /*!
            @brief 文字の色を取得します。
        */
        nn::util::Color8 GetTextColor(ControlState state) const
        {
            if (state < CONTROL_STATE_MAX)
            {
                return m_textColors[state];
            }

            return nn::util::Color8();
        }

        /*!
            @brief テキストの配置位置を取得します。
        */
        TextAlign GetTextAlign() const
        {
            return m_textAlign;
        }

        /*!
            @brief 枠線の幅を取得します。
        */
        f32 GetBorderWidth() const
        {
            return m_borderWidth;
        }

        /*!
            @brief コントロールの状態を取得します。
        */
        ControlState GetState() const
        {
            return m_state;
        }

        /*!
            @brief 拡張データを取得します。
        */
        void* GetExtraData() const
        {
            return m_pExtraData;
        }

        /*!
            @brief 自分を管理するコントロールマネージャを取得します。
        */
        ControlManager* GetControlManager() const
        {
            return m_pManager;
        }

        /*!
            @brief コントロールのX座標を設定します。
        */
        void SetX(s32 x)
        {
            m_x = x;
        }

        /*!
            @brief コントロールのY座標を設定します。
        */
        void SetY(s32 y)
        {
            m_y = y;
        }

        /*!
            @brief コントロールの幅を設定します。
        */
        void SetWidth(u32 width)
        {
            if (width > 0)
            {
                m_width = width;
            }
        }

        /*!
            @brief コントロールの高さを設定します。
        */
        void SetHeight(u32 height)
        {
            if (height > 0)
            {
                m_height = height;
            }
        }

        /*!
            @brief テキストを設定します。
        */
        void SetText(const wchar_t* pText);

        /*!
            @brief テキストを設定します。
        */
        void SetFormattedText(const wchar_t* pFormat, ...);

        /*!
            @brief 枠線の色を設定します。
        */
        void SetBorderColor(ControlState state, nn::util::Color8 color)
        {
            if (state < CONTROL_STATE_MAX)
            {
                m_borderColors[state] = color;
            }
        }

        /*!
            @brief 文字の色を設定します。
        */
        void SetTextColor(ControlState state, nn::util::Color8 color)
        {
            if (state < CONTROL_STATE_MAX)
            {
                m_textColors[state] = color;
            }
        }

        /*!
            @brief テキストの配置位置を設定します。
        */
        void SetTextAlign(TextAlign textAlign)
        {
            m_textAlign = textAlign;
        }

        /*!
            @brief 枠線の幅を設定します。
        */
        void SetBorderWidth(f32 borderWidth)
        {
            m_borderWidth = borderWidth;
        }

        /*!
            @brief コントロールの状態を設定します。
        */
        void SetState(ControlState state)
        {
            if (state < CONTROL_STATE_MAX)
            {
                m_state = state;
            }
        }

        /*!
            @brief 拡張データを設定します。
        */
        void SetExtraData(void* pExtraData)
        {
            m_pExtraData = pExtraData;
        }

        /*!
            @brief 自分を管理するコントロールマネージャを設定します。
        */
        void SetControlManager(ControlManager* pManager)
        {
            m_pManager = pManager;
        }

    protected:
        // テキストの描画開始位置を計算します。
        s32 CalculateTextX(s32 offsetL, s32 offsetR);
        // テキストの描画開始位置を計算します。
        s32 CalculateTextY(s32 offset);

        // ペンで押された時の処理
        virtual void OnPenDown() = 0;
        // ペンが離された時の処理
        virtual void OnPenUp(bool isIn) = 0;
        // ペンでスライドされた時の処理
        virtual void OnPenSlide(bool isIn) = 0;
        // ペンでタッチされた時の処理
        virtual void OnPenTouch() = 0;
        // 描画時の処理
        virtual void OnDraw() = 0;

    protected:
        // コントロールの種類
        nn::util::SizedEnum4<ControlType> m_type;
        // コントロールID
        u32 m_id;
        // 座標
        s32 m_x;
        s32 m_y;
        // サイズ
        u32 m_width;
        u32 m_height;
        // テキスト
        wchar_t* m_pText;
        // テキスト長
        s32 m_textLength;
        s32 m_textBufferLength;
        // テキストスケール
        f32 m_TextScale;
        // テキストサイズ
        f32 m_textWidth;
        f32 m_textHeight;
        // 枠線の色
        nn::util::Color8 m_borderColors[CONTROL_STATE_MAX];
        // 文字の色
        nn::util::Color8 m_textColors[CONTROL_STATE_MAX];
        // テキストの配置位置
        nn::util::SizedEnum4<TextAlign> m_textAlign;
        // 枠線サイズ
        f32 m_borderWidth;
        // コントロールの状態
        nn::util::SizedEnum4<ControlState> m_state;
        // 拡張データ
        void* m_pExtraData;
        // コントロールマネージャ
        ControlManager* m_pManager;
    };
}

#endif // NN_COMMON_SCENE_GUICONTROLBASE_H_
