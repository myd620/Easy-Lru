
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

        //�������
        if(node)
        {
            detach(node);
            node->value = value;
            attach(node);
        }
        else
        {
            //���統ǰ���ýڵ�Ϊ��, ��ɾ�����δʹ�õĽڵ�
            if(m_vFreeNode.empty())
            {
                node = m_pTail->pPrev;
                detach(node);
                m_mHashMap.erase(node->key);
            }
            else
            {
                //ʹ��һ�����е�ַ
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
    //ȡ���ýڵ�
    void detach(Node<K,V> * node)
    {
        node->pPrev->pNext = node->pNext; //�ýڵ����һ���ڵ����һ���ڵ�ָ���ýڵ����һ���ڵ�
        node->pNext->pPrev = node->pPrev; //�ýڵ����һ���ڵ����һ���ڵ�ָ���ýڵ����һ���ڵ�
    }

    //���ýڵ����ͷ��
    void attach(Node<K,V> * node)
    {
        node->pPrev = m_pHead;
        node->pNext = m_pHead->pNext;
        m_pHead->pNext->pPrev = node;
        m_pHead->pNext = node;
    }

private:
    unordered_map<K, Node<K,V>*> m_mHashMap; //cache�������ݵĴ洢
    vector<Node<K,V>*> m_vFreeNode; //�洢��ǰ���еĽڵ�
    Node<K,V> * m_pHead; //����ͷ���
    Node<K,V> * m_pTail; //����β���
    Node<K,V> * m_pEntries; // ˫�������еĽ��
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