/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_H_
#define DEMO_H_

#if defined(NN_PLATFORM_CTR)
#ifdef  __cplusplus
    #include "demo/Utility/demo_Utility.h"    
    #include "demo/Utility/demo_MemoryManager.h"
    #include "demo/Utility/demo_TextureConverter.h"    

    #include "demo/Render/demo_RenderSystem.h"
    #include "demo/Render/demo_RenderSystemDrawing.h"
    #include "demo/Render/demo_RenderSystemExt.h"
    #include "demo/Render/demo_Light.h"

    #include "demo/Body/demo_Body.h"
    #include "demo/Body/demo_Cube.h"
    #include "demo/Body/demo_Cylinder.h"
    #include "demo/Body/demo_Sphere.h"
    #include "demo/Body/demo_Particle.h"

    #include "demo/CommandCache/demo_CommandCache.h"
#endif // __cplusplus

#else
    #error no platform selected
#endif

#endif
