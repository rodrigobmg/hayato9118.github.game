/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_CommandCache.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_COMMAND_CACHE_H_
#define DEMO_COMMAND_CACHE_H_

#include "demo/Utility/demo_Utility.h"

namespace demo
{

    namespace detail 
    {        
        /*!
        :private
   
        @brief 3Dコマンド(64bit)の構造体です。
        */
        struct CommandBuffer3d
        {        
            unsigned data : 32; // レジスタに書き込むデータ
            unsigned addr : 16; // 書き込みを行うレジスタのアドレス
            unsigned be   : 4;  // バイトイネーブル        
            unsigned size : 7;  // データの個数 - 1。0のときシングルアクセス、1以上のときバーストアクセス        
            unsigned rsv  : 4;  // 予約領域
            unsigned seq  : 1;  // バーストアクセス時のアクセスモード。
                                // 0のとき単一レジスタ書き込み、1のとき連続レジスタ書き込み */
        };

        // 頂点シェーダの浮動小数点レジスタのインデックスを設定するレジスタのアドレス
        const u32 VS_UNIFORM_FLOAT_REG_INDEX = 0x2c0;
        // 頂点シェーダの浮動小数点のuniformの値を書き込む先のレジスタの開始アドレス
        const u32 VS_UNIFORM_FLOAT_REG_VALUE_BEGIN = 0x2c1;
        // 頂点シェーダの浮動小数点のuniformの値を書き込む先のレジスタの終了アドレス
        const u32 VS_UNIFORM_FLOAT_REG_VALUE_END = 0x2c8;
    
        // 頂点シェーダの浮動小数点レジスタの個数
        const u32 VS_UNIFORM_FLOAT_REG_NUM = 96;

        // 頂点シェーダの浮動小数点レジスタを4x4行列として使うときの最大個数
        const u32 VS_UNIFORM_FLOAT_REG_MATRIX_NUM = VS_UNIFORM_FLOAT_REG_NUM / 16;

        /*!
        :private
   
        @brief 頂点シェーダのuniformの行列用のクラスです。
        */
        class VertexShaderUniformMatrix : 
            private nn::util::NonCopyable<VertexShaderUniformMatrix>
        {
        public:
            VertexShaderUniformMatrix(void);
            virtual ~VertexShaderUniformMatrix();
        public:
            void Initialize(void);
            void Finalize(void);

        public:
            bool m_Valid;
            bool m_Padding[3];

            // 頂点シェーダの浮動小数点レジスタのインデックス
            u32 m_RegisterIndex;
            // 3Dコマンドバッファ中のオフセット
            u32 m_Offset[4][4];
        };
        
        void GetVSUniformOffset(u8* bufferAddress, const u32 startOffset,
            const GLsizei bufferSize, const u32 uniformFloatRegIndex, u32* uniformOffsetArray);
    }

    enum
    {
        COMMAND_CACHE_UNINITIALIZED, // コマンドリストが未初期化
        COMMAND_CACHE_INITIALIZED,   // コマンドリストが初期化済み
        COMMAND_CACHE_FINALIZED,     // コマンドリストが終了化済み
        COMMAND_CACHE_BEGIN_SAVE,    // コマンドリストの保存開始済み
        COMMAND_CACHE_END_SAVE       // コマンドリストの保存終了済み
    };

    const u32 DEMO_MAX_COMMAND_CACHE_MODELS_NUM = 128;
    const u32 DEMO_MAX_COMMAND_CACHE_MATRIX_NUM = demo::detail::VS_UNIFORM_FLOAT_REG_MATRIX_NUM;
    
    /*!
    :private
   
    @brief コマンドを保存して再利用するためのコマンドキャッシュのクラスです。
    */
    class CommandCache : private nn::util::NonCopyable<CommandCache>
    {
    public:    
        CommandCache(void);
        virtual ~CommandCache(void);

    public:
        void Initialize(const u32 CommandListSize, const u32 RequestNum,            
            const GLboolean copyCommandBuffer, const GLbitfield stateMask,
            const u32 totalModelNum = 1);
        void Finalize(void);

    public:
        // 頂点シェーダの浮動小数点レジスタのインデックスを、指定した配列のインデックスに保存します。
        void SetVSUniformMatrixRegisterIndex(const u32 modelIndex, const u32 uniformMatrixIndex, const u32 regsiterIndex);

    public:
        // コマンドリストの保存を開始します。
        void BeginSave(void);
        // 1つのモデルを描画するときのコマンドリストの先頭のオフセットを保存します。
        void SaveCommandBufferStartOffset(const u32 modelIndex);
        // コマンドリストの保存を終了します。
        void EndSave(void);

    public:
        // 指定した行列の配列のインデックスで指定した、頂点シェーダの浮動小数点レジスタの行列の値を更新します。
        void UpdateVSUniformMatrix(const u32 modelIndex, const u32 uniformMatrixIndex, const nn::math::MTX44& matrix);
        // カレントのコマンドリストに、この保存したコマンドリストを追加します。
        void Append(void);    

    public:
        void Print(void);
    
    protected:
        u32 m_State;

        GLsizei m_CommandListSize;
        GLsizei m_CommandListRequestNum;
        GLboolean m_CopyCommandBuffer;
        GLboolean m_Padding[3];
        GLbitfield m_StateMask;

        u32 m_TotalModelNum;
        u32 m_ModelIndex;

        GLuint m_CurrentCommandListId;
        GLuint m_CommandListId;        
        GLint m_CommandBufferStartOffsetArray[DEMO_MAX_COMMAND_CACHE_MODELS_NUM];
        GLuint m_CommandBufferOffset;
        GLsizei m_CommandBufferSize;
        GLuint m_CommandRequestBeginId;
        GLsizei m_CommandRequestSize;
        u8* m_CommandBufferAddr;

        // 頂点シェーダのuniform行列の3Dコマンドバッファオフセットを保存する配列        
        detail::VertexShaderUniformMatrix m_VertexShaderUniformMatrixArray2d[DEMO_MAX_COMMAND_CACHE_MODELS_NUM][DEMO_MAX_COMMAND_CACHE_MATRIX_NUM];
    };

    void PrintCurrentCmdlist(void);

}

#endif
