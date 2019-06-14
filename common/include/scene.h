/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     scene.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_H_
#define NN_COMMON_SCENE_H_

#if defined(NN_PLATFORM_CTR)
#ifdef  __cplusplus
    #include "scene/Gui.h"
    #include "scene/SceneManager.h"
    #include "scene/TenKey.h"
    #include "scene/Util.h"
#endif // __cplusplus

#else
    #error no platform selected
#endif

#endif // NN_COMMON_SCENE_H_
