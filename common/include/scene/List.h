/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     List.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_LIST_H_
#define NN_COMMON_SCENE_LIST_H_

namespace scene
{

template <class T> class Node
{
public:
    // データ
    T data;
    // 次のノード
    Node<T>* pNextNode;
};

template <class T> class List
{
public:
    // イテレータ
    class iterator
    {
    public:
        // コンストラクタ
        iterator(Node<T>* pNode)
        {
            m_pNode = pNode;
        }

        // データの参照を返す
        T operator*()
        {
            return m_pNode->data;
        }

        // 比較
        bool operator==(List<T>::iterator it)
        {
            return (m_pNode == it.m_pNode);
        }

        // 比較
        bool operator!=(List<T>::iterator it)
        {
            return (m_pNode != it.m_pNode);
        }

        // 後置きインクリメント
        void operator++(int)
        {
            m_pNode = m_pNode->pNextNode;
        }

    protected:
        Node<T>* m_pNode;
    };

public:
    // コンストラクタ
    List()
    {
        m_pHeadNode = NULL;
        m_pTailNode = NULL;
    }

    // 先頭に追加する
    void push_front(T data)
    {
        Node<T>* pNode = new Node<T>;

        pNode->data      = data;
        pNode->pNextNode = NULL;

        if (m_pHeadNode == NULL)
        {
            m_pHeadNode = pNode;
        }
        if (m_pTailNode != NULL)
        {
            m_pTailNode->pNextNode = pNode;
        }
        m_pTailNode = pNode;
    }

    // クリアする
    void clear()
    {
        Node<T>* pCurNode = m_pHeadNode;

        while (pCurNode)
        {
            Node<T>* pNextNode = pCurNode->pNextNode;

            delete pCurNode;
            pCurNode = pNextNode;
        }

        m_pHeadNode = NULL;
        m_pTailNode = NULL;
    }

    // 先頭のイテレータを返す
    List<T>::iterator begin()
    {
        return iterator(m_pHeadNode);
    }

    // 終端のイテレータを返す
    List<T>::iterator end()
    {
        return iterator(NULL);
    }

protected:
    // 先頭ノード
    Node<T>* m_pHeadNode;
    // 終端ノード
    Node<T>* m_pTailNode;
};

} // namespace scene

#endif // NN_COMMON_SCENE_LIST_H_
