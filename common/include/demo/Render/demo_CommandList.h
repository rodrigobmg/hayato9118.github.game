/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_CommandList.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_COMMAND_LIST_H_
#define DEMO_COMMAND_LIST_H_

#include "demo/Utility/demo_Utility.h"

namespace demo
{

    const u32 DEMO_COMMAND_LIST_NUM = 1;

    /*!
        @brief コマンドリストのクラス
    */
    
    class CommandList : private nn::util::NonCopyable<CommandList>
    {
    public:
        CommandList(void);
        virtual ~CommandList(void);

    public:
        void Initialize(const GLsizei bufferSize, const GLsizei requestNum,
            const bool serialRunMode = false);
        void Finalize(void);
   
    public:
        void Run(void);
    protected:
        void RunSingle(void);
    public:
        void Swap(void);
        void Stop(void);
        void Wait(void);
        void Clear(void);
    
    public:
        void SetSerialRun(const bool flag);

    protected:
        bool m_InitializeFlag;
        bool m_SerialRunFlag;
        bool m_Padding[2];        
        
        u32 m_StoreCommandListIndex;
        u32 m_RunningCommandListIndex;

        GLuint m_CommandListIdArray[DEMO_COMMAND_LIST_NUM];
        
        GLsizei m_BufferSize;
        GLsizei m_RequestNum;
    };
      
}

#endif
