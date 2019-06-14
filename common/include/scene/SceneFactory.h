/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     SceneFactory.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_SCENEFACTORY_H_
#define NN_COMMON_SCENE_SCENEFACTORY_H_

#include "Scene.h"

namespace scene
{

/*!
    @brief シーンファクトリのインターフェイスクラス。

           本クラスを継承し、 @ref Generate() を実装して @ref SceneManager に登録して下さい。
*/
class SceneFactory
{
public:
    /*!
        @brief シーンを生成します。

        @return 生成したいシーンのシーンID。
    */
    virtual Scene* Generate(s32 sceneId) = 0;
};

} // namespace scene

#endif // NN_COMMON_SCENE_SCENEFACTORY_H_
