/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     SceneManager.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_SCENEMANAGER_H_
#define NN_COMMON_SCENE_SCENEMANAGER_H_

#include "SceneFactory.h"
#include "Scene.h"
#include "Stack.h"

namespace scene
{

/*!
    @brief シーン管理クラス。

           シーンの初期化、終了、更新、描画のステート管理を行います。
*/
class SceneManager
{
public:
    /*!
        @brief コンストラクタです。
    */
    SceneManager();

    /*!
        @brief デストラクタです。
    */
    virtual ~SceneManager();

    /*!
        @brief 初期化処理を行います。

        @param[in]  pSceneFactory   シーンファクトリ。
        @param[in]  initialSceneId  最初のシーンID。

        @return 初期化に成功したら true。
    */
    bool Initialize(SceneFactory* pSceneFactory, s32 initialSceneId);

    /*!
        @brief 終了処理を行います。
    */
    void Finalize();

    /*!
        @brief 更新処理を行います。
    */
    void Update();

    /*!
        @brief 描画処理を行います。
    */
    void Draw();

    /*!
        @brief 終了すべきかどうかを取得します。

        @return 終了すべきなら true。
    */
    bool IsExpectedToFinish();

    /*!
        @brief HOME を拒否するかどうかを取得します。

        @return HOME 拒否なら true。
    */
    bool IsRejectHome();

protected:
    // シーンファクトリ
    SceneFactory* m_pSceneFactory;
    // シーン
    Scene* m_pScene;
    // シーンID
    s32 m_sceneId;
    // シーンの状態
    s32 m_sceneState;

    // シーンスタック
    Stack<s32, 16> m_sceneStack;
};

} // namespace scene

#endif // NN_COMMON_SCENE_SCENEMANAGER_H_
