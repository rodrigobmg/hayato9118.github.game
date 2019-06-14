/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_Sphere.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_SPHERE_H_
#define DEMO_SPHERE_H_

#include "demo/Body/demo_Body.h"

namespace demo
{
    /*!
    :private
   
    @brief 球のクラスです。
    */

    class Sphere : public Body
    {        
    public:
        /*!
        @brief    コンストラクタです。
        */
        Sphere(void);
        
        /*!
        @brief    デストラクタです。
        */
        virtual ~Sphere(void);

    public:
        virtual void Initialize(void);
        void InitializeSphere(const u32 vertexAttributes,
            const f32 radius, const u32 division);
        
    protected:
        f32 m_Radius;
        u32 m_Division;
    };

}

#endif
