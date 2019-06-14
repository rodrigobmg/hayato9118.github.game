/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     Stack.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_STACK_H_
#define NN_COMMON_SCENE_STACK_H_

#include <nn.h>

namespace scene
{

template <class T, int SIZE> class Stack
{
private:
    T m_stack[SIZE];
    int m_index;
public:
    Stack() : m_index(0) {}
    ~Stack() {}
    void push(const T &item)
    {
        NN_ASSERT(m_index<SIZE);
        m_stack[m_index]=item;
        ++m_index;
    }
    void pop()
    {
        NN_ASSERT(m_index>0);
        --m_index;
    }
    const T &top() const
    {
        NN_ASSERT(m_index>0);
        return m_stack[m_index-1];
    }
    int size() const { return m_index; }
    bool empty() const { return m_index==0; }
};

} // namespace scene

#endif // NN_COMMON_SCENE_STACK_H_
