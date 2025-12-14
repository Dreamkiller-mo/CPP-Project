#include <iostream>
#include <cassert>
#include <vector>
#include <string.h>
#include <cassert>
#include <string>
using namespace std;

//vector的使用
void Text_2()
{
    //增
    vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);
    v1.push_back(6);
    //三种打印方法
    //1. for循环遍历
    for (size_t i = 0; i < v1.size(); i++)
    {
        cout << v1[i] << " ";
    }
    cout << endl;
    //2. 迭代器
    vector<int>::iterator it = v1.begin();
    while (it != v1.end())
    {
        cout << *it << " ";
        it++;
    }
    cout << endl;
    //3. 范围for    --->本质还是迭代器
    for(auto e : v1)
    {
        cout << e << " ";
    }
    cout << endl;

    //反向遍历
    //1. for循环
    for (int i = v1.size()-1; i >= 0; i--)
    {
        cout << v1[i] << " ";
    }
    cout << endl;

    //2. 修改+反向迭代器
    vector<int>::reverse_iterator rit = v1.rbegin();
    while (rit != v1.rend())
    {
        *rit *= 2;
        cout << *rit << " "; 
        ++rit;
    }
    cout << endl;
}

//迭代器的失效
void Text_3()
{
    vector<int> v;
    v.push_back(6);
    v.push_back(2);
    v.push_back(8);
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    v.push_back(7);
    vector<int>::iterator it = v.begin();
    while (it != v.end())
    {
        if (*it % 2 == 0)
        {
            it = v.erase(it);                           //vector调用erase后返回的迭代器是指向删除元素的下一个元素的位置，string调用erase却是返回删除元素的当前位置
        }
        else
        {
            cout << *it << " ";
            it++;
        }
    }
    cout << endl;
}
//vector源码实现
namespace mly
{
    template<class T>
    class vector
    {
        public:
            typedef T* iterator;
            typedef const T* const_iterator;
            //返回可读可写的迭代器 ， 但返回的只是一个_start的副本，在外面是改变不了_start的值的
            iterator begin()
            {
                return _start;
            }
            iterator end()
            {
                return _finish;
            }
            //返回只读的迭代器，在外面只能对对象进行查看，并不能修改指针指向的内容
            const_iterator begin() const
            {
                return _start;
            }
            const_iterator end() const
            {
                return _finish;
            }
            //构造函数
            vector()
            :_start(nullptr)
            ,_finish(nullptr)
            ,_endofstorage(nullptr)
            {}

            //拷贝构造      v2(v1)
            vector(const mly::vector<T>& v)
            {
                //避免浅拷贝
                int n = v.size();
                //开空间
                _start = new T[v.capacity()];
                _finish = _start;
                for (int i = 0; i < n; i++)
                {
                    //拷贝v2
                    *_finish = v[i];
                    _finish++;
                }
                _endofstorage = _start + v.capacity();
            }
            //v1 = v3
            /*mly::vector<T> operator=(const mly::vector<T>& v)
            {
                if (this != &v)
                {
                    delete[] _start;
                    _start = new T[v.capacity()];
                    copy(v._start,v._start+v.size(),_start);
                    _finish = _start + v.size();
                    _endofstorage = _start + v.capacity();
                }
                return *this;
            }*/
           mly::vector<T> operator=(mly::vector<T> v)
           {
                //传入进来的v会被拷贝一份，就是v1想要的
                this->swap(v);
                return *this;
           }
           //v1.swap(v2)                                                    //如果使用库里面的swap就会发生深拷贝，代价很大
           void swap(mly::vector<T>& v)
           {
                std::swap(_start,v._start);
                std::swap(_finish,v._finish);
                std::swap(_endofstorage,v._endofstorage);
           }
            //析构函数
            ~vector()
            {
                delete[] _start;                                            //delete本质还是free，它有判断传入进来的参数是非为空    new会抛异常
                _start = _finish = _endofstorage = nullptr;
            }
            //reserve
            void reserve(size_t n)
            {
                if (n > capacity())
                {
                    //创建新的空间
                    size_t sz = size();
                    T* temp = new T[n];                                     //如果new创建的是一个对象，那new会先调用operator new ---> malloc --->构造函数 会抛异常
                    //旧空间的内容拷贝到新空间
                    //copy(_start,_start+size(),temp);
                    for (int i = 0; i < sz; i++)
                    {
                        temp[i] = _start[i];                                //调用的是operator=是一个深拷贝
                    }
                    
                    //释放旧空间
                    if (_start != nullptr)
                    {
                        delete[] _start;
                    }
                    //连接新空间                                    
                    _start = temp;
                    _finish = temp + sz;                                    //此时的_start已经发生了改变，指向了一块新的空间，而size()前提就是要保证三个指针在同一块空间
                    _endofstorage = temp + n;
                }
            }
            //resize()
            void resize(size_t n,const T& val = T())
            {
                //如果小于size()
                if (n < size())
                {
                    /* 截断*/
                    _finish = _start + n;
                }
                else
                {
                    //容量超了
                    if (n > capacity())
                    {
                        //增容
                        reserve(n);
                    }
                    //超过size()
                    while (_finish < _start + n)
                    {
                        *_finish = val;
                        _finish++;
                    }
                }
                
            }
            //push_back
            void push_back(const T& v)
            {
                /*if (size() == capacity())
                {
                    size_t newcapacity = capacity()==0 ? 2 : capacity()*2;
                    this->reserve(newcapacity);
                }
                *_finish = v;
                _finish++;*/
                insert(_finish,v);
            }
            //size()
            size_t size() const
            {
                return _finish - _start;
            }
            //capacity()
            size_t capacity() const
            {
                return _endofstorage - _start;
            }
            //重载 []
            T& operator[](size_t n)
            {
                assert(n < this->size() && n >= 0);
                return *(_start + n);
            }
            const T& operator[](size_t n) const
            {
                assert(n < this->size() && n >= 0);
                return *(_start + n);
            }
            //pop_back
            void pop_back()
            {
                /*assert(_start < _finish);
                _finish--;*/
                this->erase(_finish - 1);
            }
            //insert()
            iterator insert(iterator pos , const T& x)
            {
                //pos合法性
                assert(pos <= _finish);
                //可能需要扩容
                size_t sz = pos - _start;
                if (_finish == _endofstorage)
                {
                    size_t newcapacity = (0==capacity()) ? 2 : capacity()*2;
                    this->reserve(newcapacity);
                    //发生增容要重置pos
                    pos = _start + sz;
                }
                //往后移动元素，让x插入进来
                iterator end = _finish-1;                                     //创建一个临时变量end，就是防止让_finish发生改变
                while (end >= pos)
                {
                    *(end + 1) = *end;
                    end--;
                }
                *pos = x;
                _finish++;
                return pos;
            }
            iterator erase(iterator pos)                                  //不是iterator&的原因是传入进来的参数可能是一个临时变量
            {
                assert(pos < _finish);
                iterator it = pos;
                while (it <= _finish)
                {
                    *it = *(it+1);
                    it++;
                }
                --_finish;
                return pos;                                               //返回pos位置的下一个元素，因为向前移动了一个，所以就是它本身的位置
            }
        private:
            //指向开始的指针
            iterator _start;
            //指向最后一个元素的下一个位置
            iterator _finish;
            //指向该数组的容量
            iterator _endofstorage;
            //size = _finish - _start
            //capacity = _endogstorage - _start
    };
};
void print_vector(const mly::vector<int>& v)
{
    //迭代器实现
    mly::vector<int>::const_iterator it = v.begin();                    //v是const_iterator类型，自动调用const_iterator的begin函数。因为begin返回来的是一个可读可写的T&类型，而传入进来的只是可读的，所以要添加一个const_begin and const_end
    while (it != v.end())
    {
        cout << *it << " ";
        it++;
    }
    cout << endl;    
}
void Text_4()
{
    mly::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    mly::vector<int>::iterator it = v.begin();
    it = v.insert(it+1,99);                                 //iterator temp = it + 1; iterator pos = temp 所以这里的it + 1 是一个临时变量，参数pos只能是const_iterator 或者 iterator 而不是iteraotor&
    v.erase(it);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    it = v.begin();
    while (it != v.end())
    {
        cout << *it << " ";
        it++;
    }
    cout << endl;
}
void Text_5()
{
    mly::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    mly::vector<int> v2;
    v2.push_back(10);
    v2.push_back(20);
    v2.push_back(30);
    v2.push_back(40);
    v2.push_back(50);
    v2.push_back(60);
    v2.push_back(70);
    v1 = v2;
    print_vector(v1);
}
void Text_6()
{
    mly::vector<string> s;
    s.push_back("111");
    s.push_back("222");
    s.push_back("333");
    s.push_back("444");
    for(auto e : s)
    {
        cout << e << endl;
    }
}
int main()
{
    Text_6();
    return 0;
}