
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

template <class K, class V>
struct Node
{
    K key;
    V value;
    Node * pPrev;
    Node * pNext;
};

template <class K, class V>
class LRUCache
{
public:
    LRUCache(size_t size)
    {
        m_pEntries = new Node<K,V>[size];
        for(int i = 0; i < size; i++)
        {
            m_vFreeNode.push_back(m_pEntries + i);
        }
        
        m_pHead = new Node<K,V>;
        m_pTail = new Node<K,V>;
        m_pHead->pPrev = NULL;
        m_pHead->pNext = m_pTail;
        m_pTail->pPrev = m_pHead;
        m_pTail->pNext = NULL;
    }
    ~LRUCache()
    {
        delete m_pHead;
        delete m_pTail;
        delete [] m_pEntries;
    }

    void setCache(K key, V value)
    {
        Node<K,V> * node = m_mHashMap[key];

        //假如存在
        if(node)
        {
            detach(node);
            node->value = value;
            attach(node);
        }
        else
        {
            //假如当前可用节点为空, 则删除最久未使用的节点
            if(m_vFreeNode.empty())
            {
                node = m_pTail->pPrev;
                detach(node);
                m_mHashMap.erase(node->key);
            }
            else
            {
                //使用一个空闲地址
                node = m_vFreeNode.back();
                m_vFreeNode.pop_back();
            }

            node->key = key;
            node->value = value;
            m_mHashMap[key] = node;
            attach(node);
        }
    }

    V getCache(K key)
    {
        Node<K,V> * node = m_mHashMap[key];
        if(node)
        {
            detach(node);
            attach(node);
            return node->value;
        }
        else
        {
            return V();
        }
    }

    void printTail()
    {
        Node<K,V> * pNow = m_pTail;
        cout << "printTail" << endl;
        while(pNow)
        {
            if(pNow == m_pTail)
            {
                pNow = pNow->pPrev;
                continue;
            }

            if(pNow == m_pHead)
            {
                break;
            }

            cout << pNow->key << ":" << pNow->value << endl;
            pNow = pNow->pPrev;
        }

    }

    void printHead()
    {
        Node<K,V> * pNow = m_pHead;
        cout << "printHead" << endl;
        while(pNow)
        {
            if(pNow == m_pHead)
            {
                pNow = pNow->pNext;
                continue;
            }

            if(pNow == m_pTail)
            {
                break;
            }

            cout << pNow->key << ":" << pNow->value << endl;
            pNow = pNow->pNext;
        }
    }

private:
    //取出该节点
    void detach(Node<K,V> * node)
    {
        node->pPrev->pNext = node->pNext; //该节点的上一个节点的下一个节点指定该节点的下一个节点
        node->pNext->pPrev = node->pPrev; //该节点的下一个节点的上一个节点指定该节点的上一个节点
    }

    //将该节点插入头部
    void attach(Node<K,V> * node)
    {
        node->pPrev = m_pHead;
        node->pNext = m_pHead->pNext;
        m_pHead->pNext->pPrev = node;
        m_pHead->pNext = node;
    }

private:
    unordered_map<K, Node<K,V>*> m_mHashMap; //cache具体内容的存储
    vector<Node<K,V>*> m_vFreeNode; //存储当前空闲的节点
    Node<K,V> * m_pHead; //链表头结点
    Node<K,V> * m_pTail; //链表尾结点
    Node<K,V> * m_pEntries; // 双向链表中的结点
};

int main(void)
{
    LRUCache<int, string> lru_cache(5);
    lru_cache.setCache(1, "one");
    lru_cache.setCache(2, "two");
    lru_cache.setCache(3, "three");
    lru_cache.setCache(4, "four");
    lru_cache.getCache(2);
    lru_cache.getCache(3);

    lru_cache.printHead();
    lru_cache.printTail();
    return 0;
}