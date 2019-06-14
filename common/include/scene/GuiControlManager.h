/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     GuiControlManager.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_GUICONTROLMANAGER_H_
#define NN_COMMON_SCENE_GUICONTROLMANAGER_H_

#include "GuiControlBase.h"
#include "List.h"

namespace scene
{
    /*!
        @brief コントロールイベントコールバック
    */
    typedef void (*ControlEventCallback)(ControlEvent event, ControlBase* pControl, s16 x, s16 y, void* pParam);

    /*!
        @brief コントロールリスト
    */
    typedef List<ControlBase*> ControlList;

    /*!
        @brief コントロールを管理クラスです。
    */
    class ControlManager
    {
    public:
        /*!
            @brief コンストラクタです。
        */
        ControlManager();

        /*!
            @brief デストラクタです。
        */
        virtual ~ControlManager();

        /*!
            @brief コントロールを追加します。

            @param[in]  pControl  コントロール。
        */
        void Add(ControlBase* pControl);

        /*!
            @brief コントロールをすべて削除します。
        */
        void Clear();

        /*!
            @brief コントロールを検索します。

            @param[in]  id  検索するID。

            @return コントロール。
        */
        ControlBase* FindBy(u32 id);

        /*!
            @brief コントロールリストを取得します。

            @return コントロールリスト。
        */
        List<ControlBase*>* GetList();

        /*!
            @brief コントロールイベントコールバックを登録します。

            @param[in]  pCallback  コールバック関数。
            @param[in]  pParam     パラメータ。
        */
        void RegisterControlEventCallback(ControlEventCallback pCallback, void* pParam)
        {
            m_pCallback = pCallback;
            m_pParam    = pParam;
        }

        /*!
            @brief 更新処理を行います。
        */
        void Update();

        /*!
            @brief 描画処理を行います。
        */
        void Draw();

    protected:
        // この位置に、vtable 4byte が存在する (パディング調整の際に留意)
        // アクティブなコントロールのID
        u32 m_activeId;
        // コールバック
        ControlEventCallback m_pCallback;
        // コールバックパラメータ
        void* m_pParam;

        // コントロールリスト
        ControlList m_controlList;
        // タッチパネルリーダー
        nn::hid::TouchPanelReader m_touchReader;
        // タッチパネルの入力情報
        nn::hid::TouchPanelStatus m_touchStatus;
        nn::hid::TouchPanelStatus m_touchStatusOld;
    };
}

#endif // NN_COMMON_SCENE_GUICONTROLMANAGER_H_
