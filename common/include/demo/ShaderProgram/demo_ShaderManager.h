/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_ShaderManager.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_SHADER_MANAGER_H_
#define DEMO_SHADER_MANAGER_H_

#include <nn/gx.h>
#include <nn/math.h>

#include "demo/ShaderProgram/demo_ProgramObject.h"
#include "demo/ShaderProgram/demo_ColorFillProgram.h"
#include "demo/ShaderProgram/demo_FontProgram.h"
#include "demo/ShaderProgram/demo_DecalTextureProgram.h"

namespace demo
{ 
    
    /*!
    :private

    @brief    ShaderManagerクラスの最大シェーダバイナリ数
    */
    const u32 MAX_SHADER_BINARY_NUM = 1;

    /*!
    :private

    @brief    ShaderManagerクラスの単一シェーダバイナリの最大シェーダ数
    */    
    const u32 MAX_SHADER_PROGRAM_NUM = 3;

    /*!
    :private

    @brief    基本的なシェーダを表す列挙型    
    */
    enum ShaderType
    {
        COLOR_FILL_SHADER = 0,   //!< 頂点カラーシェーダ
        FONT_SHADER = 1,         //!< フォントシェーダ
        DECAL_TEXTURE_SHADER = 2, //!< デカールテクスチャシェーダ
        BASIC_SHADER_BINARY_SHADER_NUM = 3 //!< 基本的なシェーダの総数
    };

    /*!
    :private

    @brief    基本的なシェーダのシェーダバイナリのインデックス
    */
    const u32 BASIC_SHADER_BINARY_INDEX = 0;
    
    /*!
        :private

        @brief シェーダを管理するクラスです。        
    */

    class ShaderManager : private nn::util::NonCopyable<ShaderManager>
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        ShaderManager(void);

        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~ShaderManager(void);

    public:        
        /*!
        :private

        @brief    初期処理を行います。        
        */
        virtual bool Initialize(void);

        /*!
        :private

        @brief    終了処理を行います。        
        */
        virtual bool Finalize(void);
    protected:
        virtual bool InitializeBasicShader(void);
    
    public:
        /*!
        :private

        @brief    指定したシェーダの頂点属性を取得します。
        @param[in]    shaderType    シェーダの種類
        @return       頂点属性
        */
        u32 GetVertexAttributes(const ShaderType shaderType);

        /*!
        :private

        @brief    指定したシェーダのポインタを取得します。
        
        @param[in]    shaderType    シェーダの種類
        @return       シェーダプログラムのポインタ
        */
        ProgramObject* GetShaderProgram(const ShaderType shaderType);

    protected:
        bool m_Initialized;
        u8 m_Padding[3];
        GLuint m_ShaderProgramIdArray[MAX_SHADER_BINARY_NUM][MAX_SHADER_PROGRAM_NUM];
        ProgramObject* m_ShaderProgramPtrArray[MAX_SHADER_BINARY_NUM][MAX_SHADER_PROGRAM_NUM];

    protected:
        ColorFillProgram m_ColorFillProgram;
        FontProgram m_FontProgram;
        DecalTextureProgram m_DecalTextureProgram;
    };

}

#endif
