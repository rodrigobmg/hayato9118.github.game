﻿/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_Utility.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_UTILITY_H_
#define DEMO_UTILITY_H_

#include <cstdio>
#include <cstdlib>

#include <nn/types.h>
#include <nn/assert.h>
#include <nn/util/util_NonCopyable.h>

#include <nn/fnd/fnd_ExpHeap.h>

#include <nn/os/os_MemoryTypes.h>

#include <nn/gx.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <nn/gx/CTR/gx_CTR.h>
#include <nn/gx/CTR/gx_Lcd.h>

#include <nn/math.h>

#include <nn/applet.h>

namespace demo
{
    /*!            
    :private

    @brief CTRでの上下画面のアスペクト比
    */
    
    const f32 DISPLAY0_ASPECT = static_cast<f32>(nn::gx::CTR::DISPLAY0_HEIGHT) / static_cast<f32>(nn::gx::CTR::DISPLAY0_WIDTH);
    const f32 DISPLAY1_ASPECT = static_cast<f32>(nn::gx::CTR::DISPLAY1_HEIGHT) / static_cast<f32>(nn::gx::CTR::DISPLAY1_WIDTH);

    /*!
    :private

    @brief GLエラーが発生したら、エラーの種類をデバッグ出力する関数です。
    */
    
    void PrintGLError(const GLenum errorCode);

    /*!
    :private

    @brief GLエラーが発生したらエラーの種類をデバッグ出力し、NN_TPANIC_()を呼ぶマクロです。
    */
    
#define DEMO_ASSERT_GL_ERROR()            \
    {                                     \
       GLenum errorCode = glGetError();   \
       if ( errorCode != GL_NO_ERROR )    \
       {                                  \
           demo::PrintGLError(errorCode); \
           NN_TPANIC_("\n");             \
       }                                  \
    }

#define DEMO_FORCE_LINK_APPLET_LIB()               \
    {                                              \
        volatile bool initialized;                 \
        initialized = nn::applet::IsInitialized(); \
    }

    /*!
    :private

    @brief シェーダのUniformのロケーションを保存する配列のインデックス用の列挙体
    */
    
    enum
    {
        // Vertex uniform
        VERTEX_UNIFORM_PROJECTION = 0,
        VERTEX_UNIFORM_MODELVIEW,
        // Fragment uniform
        FRAGMENT_UNIFORM_ALPHA_TEST,
        // Fragment uniform : Texture samplerType
        FRAGMENT_UNIFORM_TEXTURE0_SAMPLER_TYPE,
        FRAGMENT_UNIFORM_TEXTURE1_SAMPLER_TYPE,
        FRAGMENT_UNIFORM_TEXTURE2_SAMPLER_TYPE,
        FRAGMENT_UNIFORM_TEXTURE3_SAMPLER_TYPE,
        // LightEnv
        FRAGMENT_UNIFORM_LIGHT_ENV_CONFIG,        
        FRAGMENT_UNIFORM_LIGHT_ENV_CLAMP_HIGHLIGHTS,
        FRAGMENT_UNIFORM_LIGHT_ENV_LUT_ENABLED_REFL,
        // D0
        FRAGMENT_UNIFORM_LIGHT_ENV_LUT_ENABLED_D0,
        FRAGMENT_UNIFORM_LIGHT_ENV_LUT_INPUT_D0,
        FRAGMENT_UNIFORM_LIGHT_ENV_ABS_LUT_INPUT_D0,
        // D1
        FRAGMENT_UNIFORM_LIGHT_ENV_LUT_ENABLED_D1,
        FRAGMENT_UNIFORM_LIGHT_ENV_LUT_INPUT_D1,
        FRAGMENT_UNIFORM_LIGHT_ENV_ABS_LUT_INPUT_D1,
        // Fragment uniform : Fragment lighting
        FRAGMENT_UNIFORM_FRAGMENT_LIGHTING_ENABLED,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHTING_AMBIENT,
        // Fragment uniform : Fragment light0
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_ENABLED,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_AMBIENT,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_DIFFUSE,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_SPECULAR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_SPECULAR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_POSITION,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_GEOM_FACTOR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_GEOM_FACTOR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE0_TWO_SIDE_DIFFUSE,
        // Fragment uniform : Fragment light1
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_ENABLED,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_AMBIENT,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_DIFFUSE,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_SPECULAR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_SPECULAR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_POSITION,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_GEOM_FACTOR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_GEOM_FACTOR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE1_TWO_SIDE_DIFFUSE,
        // Fragment uniform : Fragment light2
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_ENABLED,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_AMBIENT,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_DIFFUSE,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_SPECULAR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_SPECULAR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_POSITION,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_GEOM_FACTOR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_GEOM_FACTOR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE2_TWO_SIDE_DIFFUSE,
        // Fragment uniform : Fragment light3
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_ENABLED,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_AMBIENT,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_DIFFUSE,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_SPECULAR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_SPECULAR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_POSITION,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_GEOM_FACTOR0,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_GEOM_FACTOR1,
        FRAGMENT_UNIFORM_FRAGMENT_LIGHT_SOURCE3_TWO_SIDE_DIFFUSE,
        // Fragment uniform : Material
        FRAGMENT_UNIFORM_FRAGMENT_MATERIAL_AMBIENT,
        FRAGMENT_UNIFORM_FRAGMENT_MATERIAL_DIFFUSE,
        FRAGMENT_UNIFORM_FRAGMENT_MATERIAL_SPECULAR0,
        FRAGMENT_UNIFORM_FRAGMENT_MATERIAL_SPECULAR1,
        FRAGMENT_UNIFORM_FRAGMENT_MATERIAL_SAMPLER_D0,
        FRAGMENT_UNIFORM_FRAGMENT_MATERIAL_SAMPLER_D1,
        // Fragment uniform : Texture combiner0
        FRAGMENT_UNIFORM_TEXENV0_SRC_RGB,
        FRAGMENT_UNIFORM_TEXENV0_SRC_ALPHA,
        FRAGMENT_UNIFORM_TEXENV0_OPERAND_RGB,
        FRAGMENT_UNIFORM_TEXENV0_OPERAND_ALPHA,
        FRAGMENT_UNIFORM_TEXENV0_COMBINE_RGB,
        FRAGMENT_UNIFORM_TEXENV0_COMBINE_ALPHA,
        FRAGMENT_UNIFORM_TEXENV0_CONST_RGBA,
        // Fragment uniform : Texture combiner1
        FRAGMENT_UNIFORM_TEXENV1_SRC_RGB,
        FRAGMENT_UNIFORM_TEXENV1_SRC_ALPHA,
        FRAGMENT_UNIFORM_TEXENV1_OPERAND_RGB,
        FRAGMENT_UNIFORM_TEXENV1_OPERAND_ALPHA,
        FRAGMENT_UNIFORM_TEXENV1_COMBINE_RGB,
        FRAGMENT_UNIFORM_TEXENV1_COMBINE_ALPHA,
        FRAGMENT_UNIFORM_TEXENV1_CONST_RGBA,
        // Fragment uniform : Texture combiner2
        FRAGMENT_UNIFORM_TEXENV2_SRC_RGB,
        FRAGMENT_UNIFORM_TEXENV2_SRC_ALPHA,
        FRAGMENT_UNIFORM_TEXENV2_OPERAND_RGB,
        FRAGMENT_UNIFORM_TEXENV2_OPERAND_ALPHA,
        FRAGMENT_UNIFORM_TEXENV2_COMBINE_RGB,
        FRAGMENT_UNIFORM_TEXENV2_COMBINE_ALPHA,
        FRAGMENT_UNIFORM_TEXENV2_CONST_RGBA,
        // Max uniform locations num
        MAX_UNIFORM_LOCATIONS_NUM
    };
    
    /*!
    :private

    @brief シェーダのUniformのロケーションを保存する配列
    */    
    extern s32 s_UniformLocations[MAX_UNIFORM_LOCATIONS_NUM];

    /*!            
    :private

    @brief シェーダのUniformのロケーションを保存する配列を初期化します。
    */    
    void InitializeUniforms(const GLuint programId);
    
    /*!
    :private

    @brief ランダムな色を返します。
    */  
    void GetRandomColor(f32& red, f32& green, f32& blue);
}

#endif
