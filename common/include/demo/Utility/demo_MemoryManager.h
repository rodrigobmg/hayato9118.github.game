/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_MemoryManager.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_MEMORY_MANAGER_H_
#define DEMO_MEMORY_MANAGER_H_

#include "demo/Utility/demo_Utility.h"

namespace demo
{

    namespace detail
    {
        /*!
            @brief グラフィックス用に割り当てたメモリ領域を管理するクラスです。
                   
                   メインメモリや VRAM 上の領域を要求に応じて確保あるいは解放します。
        */

        class MemoryManager : private nn::util::NonCopyable<MemoryManager> 
        {
        public:
            /*!
                @brief    コンストラクタです。
            */
            MemoryManager(void);

            /*!                
            @brief    デストラクタです。
            */
            virtual ~MemoryManager();

            /*!
                @brief    メモリマネージャクラスを初期化します。
            
                          引数で指定したメモリ領域を用いて DMPGL初期化や描画に必要となる各種バッファの確保を行います。
                          本関数は事前に一度呼ぶ必要があります。
                          
                          二回目以降のコールは、 Finalize() で破棄されていない場合無視されます。
            
                @param[in]   fcramAddress      グラフィックスのために使用するFCRAM上のメモリ領域の先頭アドレス
                @param[in]   memorySize        グラフィックスのために使用するメモリ領域のサイズ
            */
            void Initialize(const uptr fcramAddress, const size_t memorySize);

            /*!
                @brief    メモリマネージャクラスの終了処理を行います。
            */
            void Finalize(void);
            
            /*!
                @brief    本クラスが管理するメモリ領域を割り当てます。
                
                @param[in]   area    確保対象のメモリ領域
                @param[in]   aim     確保したバッファの用途
                @param[in]   id      確保対象バッファのID
                @param[in]   size    確保したいバッファサイズ
                
                @return      確保したバッファのアドレスを返します。
            */
            void* Allocate(GLenum area, GLenum aim, GLuint id, GLsizei size);

            /*!
                @brief    本クラスが割り当てたメモリ領域を解放します。
            
                          ただし、引数 area が VRAM を指す場合は解放していません。
                
                @param[in]   area    解放対象のバッファがあるメモリ領域
                @param[in]   aim     解放対象バッファの用途
                @param[in]   id      解放対象バッファのID
                @param[in]   addr    解放したいバッファのアドレス
            */
            void    Deallocate(GLenum area, GLenum aim, GLuint id, void* addr);


            /*!
                @brief    メインメモリや VRAM の現在の空き容量と最大容量をデバッグ出力します。
            */
            void PrintFreeMemorySize(void);

        private:
            bool            m_Initialized;
            u8              m_Pad[3];
            uptr            m_pStartAddrFcram;
            uptr            m_CurrentAddrVramA;
            uptr            m_CurrentAddrVramB;
            nn::fnd::ExpHeap    m_HeapOnFcram;
            size_t              m_AllocatedBlockSize;
        };
    } // namespace detail
    
    /*!
        @brief    メモリマネージャクラスを初期化します。
        
        @param[in]   fcramAddress      グラフィックスのために使用するFCRAM上のメモリ領域の先頭アドレス
        @param[in]   memorySize        グラフィックスのために使用するメモリ領域のサイズ
    */
    void InitializeMemoryManager(const uptr fcramAddress, const size_t memorySize);

    /*!
        @brief    メモリマネージャの終了処理を行います。
    */
    void FinalizeMemoryManager(void);
    
    /*!
        @brief    メインメモリや VRAM の現在の空き容量と最大容量をデバッグ出力します。
    */
    void PrintMemoryManagerInfo(void);

    /*!
        @brief    nngxInitialize 関数の引数として指定するメモリアロケータ
        
        @param[in]   area    確保対象のメモリ領域
        @param[in]   aim     確保したバッファの用途
        @param[in]   id      確保対象バッファのID
        @param[in]   size    確保したいバッファサイズ
        
        @return      確保したバッファのアドレスを返します。
    */
    void* GetAllocator(GLenum area, GLenum aim, GLuint id, GLsizei size);

    /*!
        @brief    nngxInitialize 関数の引数として指定するメモリデアロケータ
        
        @param[in]   area    解放対象のバッファがあるメモリ領域
        @param[in]   aim     解放対象バッファの用途
        @param[in]   id      解放対象バッファのID
        @param[in]   addr    解放したいバッファのアドレス
    */
    void GetDeallocator(GLenum area, GLenum aim, GLuint id, void* addr);

    /*!
        @brief    メモリマネージャが管理するメインメモリ上のヒープからバッファ確保する関数
    
        :private
    
        demo ライブラリの内部処理向けです。
        システム用途としてアロケータを呼びます。
        
        @param[in]   size    確保したいバッファサイズ
        
        @return      確保したバッファのアドレスを返します。
    */
    void* Alloc(size_t size);
    
    /*!
        @brief    メインメモリ上のヒープから確保していたバッファを解放します。
    
        :private
    
        demo ライブラリの内部処理向けです。
        
        @param[in]    解放したいバッファのアドレス
    */
    void Free(void* ptr);
}

#endif
