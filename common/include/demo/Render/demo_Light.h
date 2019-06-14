/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_Light.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_LIGHT_H_
#define DEMO_LIGHT_H_

#include "demo/Utility/demo_Utility.h"

namespace demo
{
    /*!
    :private
   
    @brief 光源のクラスです。
    */    

    class Light : private nn::util::NonCopyable<Light>
    {
    public:
        Light(void);
        virtual ~Light();

    public:
        void Initialize(void);

    public:    
        f32 m_Ambient[4];
        f32 m_Diffuse[4];
        f32 m_Specular0[4];
        f32 m_Specular1[4];
        f32 m_Position[4];    
    };

}

#endif
