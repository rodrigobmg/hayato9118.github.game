/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_Particle.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_PARTICLE_H_
#define DEMO_PARTICLE_H_

#include "demo/Utility/demo_Utility.h"

#include "demo/Body/demo_Body.h"
#include "demo/Body/demo_Sphere.h"

namespace demo
{
    /*!
    :private
   
    @brief パーティクルのクラスです。
    */

    class Particle : public Body
    {
    public:
        Particle(void);
        virtual ~Particle(void);

    public:
        virtual void Initialize(void);
        void InitializeParticle(const u32 vertexAttributes, const f32 radius, const u32 division);
        virtual void Finalize(void);
    
    public:
        void Update(const f32 time);
    protected:
        void Spawn(void);

    public:
        virtual nn::math::MTX44 GetWorldMatrix(void) const;

    public:
        virtual void Draw(void);

    public:    
        f32 m_LifeTime;

        f32 m_Mass;
        nn::math::Vector3 m_Position;
        nn::math::Vector3 m_Velocity;
        nn::math::Vector3 m_Acceleration;

        nn::math::Vector3 m_Color;

        demo::Sphere m_Sphere;

    public:
        static bool s_Initialize;
        static u32 s_InitialLifeTime;
        static f32 s_InitialMass;
        static nn::math::Vector3 s_InitialPosition;
        static nn::math::Vector3 s_InitialVelocity;
        static nn::math::Vector3 s_RandomVelocityScale;
        static nn::math::Vector3 s_Gravity;

        static f32 s_InitialRadius;
    };

}

#endif
