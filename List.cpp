#include <iostream>
#include <cassert>
using namespace std;

namespace mly
{
    /*实现一个简单的链表*/
    template<class T>
    //先定义链表结点
    struct __list_node
    {
        //构造函数
        __list_node(const T& x = T())
        :data(x)
        ,_pre(nullptr)
        ,_next(nullptr)
        {}
        /*双向循环链表带头的*/
        __list_node* _pre;
        __list_node* _next;
        T data;
    };
    //在定义链表的迭代器
    template<class T,class Ref,class Ptr>
    struct __list_iterator
    {
        typedef __list_node<T> Node;
        typedef __list_iterator<T,Ref,Ptr> Self;
        Node* _node;
        //构造函数
        __list_iterator(Node* n)
        :_node(n)
        {}
        //重载 *it
        Ref operator*()
        {
            return _node->data;
        }
        //重载 ++it
        Self& operator++()
        {
            _node = _node->_next;
            return *this;
        }
        Self operator++(int)                                    //注意后置++返回的是旧的值，不是新的
        {
            //创建副本，移动原先迭代器，返回副本迭代器
            Self temp = *this;
            ++(*this);
            return temp;
        }
        //重载--
        Self& operator--()                                      
        {
            _node = _node->_pre;
            return *this;
        }
        Self operator--(int)                                    //编译器会自动调用operator--(0)来区分前置--和后置--,int 只是一个占位符
        {
            Self temp = _node;
            _node = _node->_pre;
            return temp;
        }
        //重载 !=
        bool operator!=(const Self& it)
        {
            return _node != it._node;
        }
        // == 
        bool operator==(const Self& it)
        {
            return _node == it._node;
        }
        // -> 指针可以通过->访问其所指空间成员，迭代器必须重载operator->
        Ptr operator->()
        {
            return &(_node->data);
        }
    };
    //最后实现链表的类
    template<class T>
    class list
    {
        typedef __list_node<T> Node;
        public:
            typedef __list_iterator<T,T&,T*> iterator;
            typedef __list_iterator<T,const T&,const T*> const_iterator;
            iterator begin()
            {
                return iterator(_head->_next);
            }
            iterator end()
            {
                return iterator(_head);
            }
            const_iterator begin() const
            {
                return const_iterator(_head->_next);
            }
            const_iterator end() const
            {
                return const_iterator(_head);
            }
            //构造函数
            list(const T& x = T())
            {
                _head = new Node;
                _head->_pre = _head;
                _head->_next = _head;

            }
            //拷贝构造
            list(const list<T>& lt)
            {
                //创建头结点并初始化
                _head = new Node;
                _head->_next = _head;
                _head->_pre = _head;
                //迭代器+push_back
                for(auto e : lt)
                {
                    push_back(e);
                }
            }
            //赋值 l1 = l3
            // l1 = l1
            /*list<T>& operator=(const list<T>& lt)
            {
                if (this != &lt)
                {
                    //先清楚原先的空间结点
                    clear();
                    for(auto e : lt)
                    {
                        push_back(e);
                    }
                }
                return *this;
            }*/
           list<T>& operator=(list<T> lt)
           {
                std::swap(_head,lt._head);
                return *this;
           }
            //析构函数
            ~list()
            {
                clear();
                delete _head;
                _head = nullptr;
            }
            //clear
            void clear()
            {
                //不清掉头结点，删除所以数据，结点还留下来
                iterator it = begin();
                while(it != end())
                {
                    erase(it++);                                                //后置++返回的是一个迭代器副本，任意++对原来的迭代器无影响
                }
            }
            //push_back
            void push_back(const T& x)
            {
                /*Node* tail = _head->_pre;
                Node* newnode = new Node(x);
                if (newnode != nullptr) newnode->data = x;
                else return;
                tail->_next = newnode;
                newnode->_pre = tail;
                newnode->_next = _head;
                _head->_pre = newnode;*/
                insert(end(),x);
            }
            //pop_back
            void pop_back()
            {
                //找到尾部指针
                Node* tail = _head->_pre;
                //存储前一个结点
                Node* temp = tail->_pre;
                //删除尾结点
                //前一个_next指向头结点,头结点_pre指向前一个结点
                erase(iterator(tail));
            }
            //push_front
            void push_front(const T& x)
            {
                //创建一个迭代器指向第一个有效元素
                mly::list<T>::iterator it = iterator(_head);
                ++it;
                //复用insert
                insert(it,x);
            }
            //pop_front
            void pop_front()
            {
                //第一个结点位置
                Node* pfront = _head->_next;
                //存储该节点的下一个结点位置
                Node* pfront_next = pfront->_next;
                //删除第一个结点
                erase(iterator(pfront));
            }
            //insert
            void insert(iterator pos,const T& x)
            {
                /*在pos位置前插入一个新结点*/
                //先创建一个新结点newnode
                Node* newnode = new Node(x);
                //用一个变量暂时存储pos位置之前的结点
                Node* temp = pos._node->_pre;
                //前结点和新结点链接
                temp->_next = newnode;
                newnode->_pre = temp;
                //当前结点和新结点链接
                newnode->_next = pos._node;
                pos._node->_pre = newnode;
            }
            //erase返回该节点下一个位置
            iterator erase(iterator pos)
            {
                assert(pos != end());
                //删除指定位置链表里面的结点数据
                Node* pcurrent = pos._node;
                Node* pnext = pcurrent->_next;
                pcurrent->_pre->_next = pnext;
                pcurrent->_next->_pre = pcurrent->_pre;
                delete pcurrent;
                return iterator(pnext);
            }
        private:
            Node* _head;
    };
};
void Print_list(const mly::list<int>& lt)
{
    mly::list<int>::const_iterator it = lt.begin();
    while (it != lt.end())
    {
        //*it = 1;
        cout << *it << " ";
        ++it;
    }
}
void Text_1()
{
    mly::list<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(4);
    l1.push_back(5);
    mly::list<int>::iterator it = l1.begin();
    it++;
    it = l1.erase(it);
    while(it != l1.end())
    {
        cout << *it << " ";
        ++it;
    }
}
void Text_2()
{
    mly::list<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(4);
    l1.push_back(5);
    l1.push_front(100);
    l1.pop_front();
    mly::list<int> l2(l1);
    mly::list<int> l3;
    l3.push_back(99);
    l3.push_back(100);
    Print_list(l3);
    l3 = l2;
    Print_list(l3);
}
int main()
{
    Text_2();
    return 0;
}