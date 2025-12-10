#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <string>
using namespace std;

/*class string
{
public:
    //传统写法构造函数，没有传入值的时候为空
    string(const char* str="")
    {
        if(str==nullptr)
        {
            assert(false);
            return;
        }
        //开内存，将字符放入
        _str=new char[strlen(str)+1];
        //将str中的内容放入_str中
        strcpy(_str,str);
    }
    //拷贝函数,此时传入的是string类型的s，所以计算和复制的时候是传入它的成员变量_str指针
    string(const string& s)
    :_str(new char[strlen(s._str)+1])           //开辟独立的空间，避免浅拷贝时的二次析构
    {
        strcpy(_str,s._str);
    }
    //复制函数  s1=s2
    string& operator=(const string& s)
    {
        //拷贝s一份
        if(this != &s)
        {
            char* temp = new char[strlen(s._str)+1];
            strcpy(temp,s._str);
            delete[] _str;
            _str = temp;
        }
        return *this;
    }
    ~string()
    {
        if (_str != nullptr)
        {
            delete[] _str;
            _str = nullptr;
        }
        
    }
private:
    //指向字符数组的指针
    char* _str;
};
*/
//现代写法
class String
{
public:
    String(const char* str="")              //给一个全缺省,""表示只有一个'\0',strlen遇到'\0'就停止,strcpy遇到'\0'先将'\0'拷贝过去在中止
    {
        if (str == nullptr)
        {
            //抛开传入空指针
            assert(false);
            return;
        }
        _str = new char[strlen(str)+1];     //在堆上开辟空间而不是栈上开辟的原因是:1.栈上不可以扩容 2.栈上是使用指针直接指向代码段（静态区）的字符串不可以修改
        strcpy(_str,str);
    }
    //拷贝函数  s1(s2)
    String(const String& s)
    :_str(nullptr)
    {
        String temp(s._str);                //----->传入s._str去调用构造函数初始化,而不是s不然会一直在拷贝这里循环
        swap(_str,temp._str);
    }
    ~String()
    {
        if (_str != nullptr)
        {
            delete[] _str;
            _str = nullptr;
        }
        
    }
    //重载复值函数 s1=s2
    String& operator=(String s)
    {
        if (this != &s)                 //防止自己给自己赋值
        {
            swap(_str,s._str);
        }
        
        return *this;
    }
    size_t size()
    {
        return strlen(_str);
    }
    //重载[],让其可以获取到字符串第i的一个元素----->返回值是char&,在外面不仅可以读也可以修改
    char& operator[](size_t i)
    {
        //判断合法性
        assert(i>=0 && i<=((*this).size()));
        return *(_str+i);   //_str[i]
    }
    //打印函数
    void Print()
    {
        cout << _str <<endl;
    }
private:
    char* _str;
};
void Text_1()
{
    String s1("hello world!!!");
    String s2(s1);
    s1.Print();
    s2.Print();
    cout << s1[0] <<" "<<s2[0]<<endl;
}

//string类的基本用法
//提取url
void Spilt_Url(const string& url)
{
    //提取协议
    size_t pos_1 = url.find(':');
    if (pos_1 != string::npos)
    {
        cout<< url.substr(0,pos_1) << endl;
    }
    
    //提取域名
    size_t pos_2 = url.find('/',pos_1+3);
    if (pos_2 != string::npos)
    {
        cout<< url.substr(pos_1+3,pos_2-pos_1-3) << endl;
    }
    //提取资源位置
    cout << url.substr(pos_2);
}
void Text_2()
{
    string url("https://legacy.cplusplus.com/reference/string/string/find/");
    Spilt_Url(url);
}
//实现string的增删查改
namespace mly
{
    class string
    {
    public:
        static const size_t npos = -1;
        typedef char* iterator;
        iterator begin()
        {
            return _str;
        }
        iterator end()
        {
            return _str+_size;
        }
        //构造函数的实现
        string(const char* str="")            
        {
            _size = strlen(str);
            _capacity = _size;
            _str = new char[_capacity+1];               //多给一个空间存储'\0'
            strcpy(_str,str);
        }
        //拷贝函数
        string(const mly::string& s)
        :_str(nullptr)
        ,_size(0)
        ,_capacity(0)
        {
            _str = new char[s.size()+1];
            strcpy(_str,s._str);
            _size = s._size;
            _capacity = s._capacity;
        }
        //赋值函数
        string& operator=(mly::string s)
        {
            /*------->传入参数为const string& s
            if ((this != &s)&&(this!=nullptr))
            {
                char* temp = new char[s.size()+1];
                strcpy(temp,s._str);
                delete[] _str;
                _str = temp;
                _size = s._size;
                _capacity = s._capacity;
            }*/
            std::swap(_str,s._str);
            return *this;
        }
        //reserve
        void reserve(size_t n)
        {
            if(n>_capacity)
            {
                char* temp = new char[n+1];
                strcpy(temp,_str);
                delete[] _str;
                _str = temp;
                _capacity = n;
            }
        }
        //push_back函数
        void push_back(const char ch)
        {
            //空间满了要增容
            if (_size == _capacity)
            {
                size_t newcapacity = _capacity == 0 ? 2 : _capacity*1.5;
                /*char* temp = new char[newcapacity+1];
                strcpy(temp,_str);
                delete[] _str;
                _str = temp;
                _capacity = newcapacity;*/
                (*this).reserve(newcapacity);
            }
            _str[_size] = ch;
            _size++;
            _str[_size] = '\0';
        }
        //swap函数
        void swap(mly::string s)
        {
            std::swap(_str,s._str);
            std::swap(_size,s._size);
            std::swap(_capacity,s._capacity);
        }
        //append函数
        void append(const char* s)
        {
            size_t len = _size + strlen(s);
            if (len > _capacity)
            {
                size_t newcapacity = len;
                /*char* temp = new char[len+1];
                strcpy(temp , _str);
                delete[] _str;
                _str = temp;
                _capacity = newcapacity;*/
                (*this).reserve(len);                       //使用reserve来实现
            }
            strcpy(_str+_size,s);                           //strcpy会把'\0'复制过去，所以append不需要再在最后添加'\0'
            _size += strlen(s);
        }
        //重载+=符号        s1+='o'
        string& operator+=(const char ch)
        {
            this->push_back(ch);
            return *this;
        }
        string& operator+=(const char* c)
        {
            this->append(c);
            return *this;
        }
        //析构函数
        ~string()
        {
            assert(this!=nullptr);
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }
        //size()函数
        const size_t size() const
        {
            return _size;
        }
        //capacity()函数
        const size_t capacity() const
        {
            return _capacity;
        }

        //c_str的实现
        char* c_str()
        {
            return _str;
        }
        //重载[]
        char& operator[](size_t i)
        {
            assert(i<_size && i>=0);
            return _str[i];
        }
        const char& operator[](size_t i) const
        {
            assert(i<_size && i>=0);
            return _str[i];
        }
        //clear
        void clear()
        {
            //不销毁内存空间
            _size = 0;
            _str[_size] = '\0';
        }
        //empty
        bool empty()
        {
            return _size == 0 ? true : false;
        }
        //find()
        size_t find(const char& ch,size_t pos=0) const
        {
            if (pos > _size)
            {
                return npos;
            }
            for (size_t i = 0; i < _size; i++)
            {
                if (_str[i] == ch)
                {
                    return i;
                }
            }
            return npos;
        }
        //find()函数查找字符串需要KMP字符串匹配
        //朴素时间效率不高
        size_t find(const char* ch,size_t pos = 0)
        {
            if (pos > _size)
            {
                return npos;
            }
        }
        //insert()函数:在字符串中的pos位置插入一个字符
        string& insert(char ch,int pos)
        {
            //保证pos位置在_size范围之内
            assert(pos <= _size && pos >= 0);
            //检查空间是否够
            if (_size == _capacity)                         //默认有一个this指针,相当与this->_size,this->_capacity
            {
                //如果空间满了，就扩容
                int newcapacity = _capacity == 0 ? 2 : _capacity*1.5;
                //调用reserve获取新的足够大的内存空间
                reserve(newcapacity);
            }
            //获得足够空间之后插入字符
            int end = _size;                                //end表示'\0'的位置
            while (end >= pos)
            {
                //元素后移
                _str[end+1] = _str[end];
                end--;
            }
            _str[end+1] = ch;
            _str[_size+1]='\0';
            _size++;
            return *this;
        }
        //insert()函数的重载:在字符串的pos位置插入一个字符串
        string& insert(char* str,int pos)
        {
            //判断pos的合法性
            assert(pos <= _size && pos >= 0);
            //空间满的情况
            int str_len = strlen(str);                              //避免多次调用strlen函数
            if (_size + str_len > _capacity)
            {
                this->reserve(_size + str_len);
            }
            //元素后移
            int end = _size;
            int j=0;                                        //'\0'的位置
            while (end >= pos)
            {
                _str[end + str_len] = _str[end];
                end--;
            }
            /*for (int i = pos; i < pos + str_len; i++)
            {
                _str[i] = str[j++];
            }*/
           strncpy(_str+pos,str,str_len);
            //_size增加
            _size += str_len;
            //后面要加'\0'
            _str[_size+1] = '\0';
            return *this;
        }
        void resize(int newSize , char ch = '\0')
        {
            assert(newSize>0);
            if (newSize < _size)
            {
                _str[newSize] = '\0';
                _size = newSize;
            }
            if (newSize > _size)
            {
                //扩容
                this->reserve(newSize);
                //填入字符占位
                for (size_t i = _size; i < newSize; i++)
                {
                    _str[i] = ch;
                }
                //末尾补上'\0'
                _str[newSize] = '\0';
                _size = newSize;
            }
            
        }
        string& erase(size_t pos , size_t len = npos)
        {
            assert(pos < _size && pos >= 0);
            if (len >= (_size - pos))
            {
                _str[pos] = '\0';
                _size = pos;   
            }
            else
            {
                size_t i = pos + len;
                while (i <= _size)
                {
                    _str[i-len] = _str[i];
                    i++;
                }
                _size -= len;
            }
        }
        //Print
        void Print()
        {
            assert(this != nullptr);
            cout << this->_str << endl;
            cout << this->_size <<endl;
            cout << this->_capacity <<endl;
        }
    private:
        char* _str;
        //有多少个有效字符
        size_t _size;
        //可以存储多少个有效字符
        size_t _capacity;
    };
    //输出
    ostream& operator<<(ostream& _cout,const mly::string& s)
    {
        for(size_t i = 0;i<s.size();i++)
        {
            _cout << s[i];                                      //s是只读的---->还要写一个const重载的[]
        }
        return _cout;
    }
    //输入
    istream& operator>>(istream& _cin,mly::string& s)
    {
        //跳过空白符
        mly::string temp;
        _cin >> std::ws;
        //输入
        char ch;
        while(_cin.get(ch) && !isspace(ch))
        {
            temp.push_back(ch);
        }
        if (temp.size()>0)
        {
            s.swap(temp);
        }
        return _cin;
    }
}

void Text_3()
{
    /*mly::string s1("hello world");
    cout << s1 <<endl;
    mly::string s2(s1);
    cout << s2.c_str() <<endl;
    mly::string::iterator it = s1.begin();
    while(it != s1.end())
    {
        cout << *it;
        it++;
    }
    cout << endl;
    //范围for是由迭代器支持的------->会被编译器替代成迭代器
    //需要支持iteraotr begin() end()  使用默认名字命名
    for(auto e : s2)
    {
        cout << e;
    }
    cout << endl;
    s2.push_back('o');
    cout << s2 <<endl;
    s2.append("dark morning!!!");
    s2.Print();*/
    mly::string s1;
    s1 += "hello";
    s1 += ' ';
    s1 += "world";
    cout << s1 <<endl;
    cout << s1[s1.size()-1] <<endl;
}
void Text_4()
{
    mly::string s1("hello world!");
    s1.Print();
    s1.erase(5,9);
    s1.Print();
}
int main()
{
    Text_4();
    return 0;
}