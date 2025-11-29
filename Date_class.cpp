#include <iostream>
#include <cstdlib>
using namespace std;
typedef int ElemType;
class Stack
{
private:
    ElemType* _p;
    size_t _size;
    size_t _capacity;
public:
    //栈的构造函数----初始化
    Stack(int n=5)
    {
        ElemType* temp = (ElemType*)malloc(sizeof(ElemType)*n);
        if (temp == NULL)
        {
            cout << "Inite malloc fail!!!" << endl;
            exit(-1);
        }
        _p = temp;
        _size = 0;
        _capacity = n;
        
    }
    //栈的清理
    ~Stack()
    {
        if (_p != nullptr)
        {
            free(_p);
            _p = nullptr;
            _size = _capacity = 0;
            cout << "clear successful !!!"<<endl;
        }
    }
};
class Date
{
public:
    friend ostream& operator<<(ostream& out,const Date& d);
    friend istream& operator>>(istream& in,Date& d);
    //获取某年某月的天数
    int GetMonthDays(int year,int month)
    {
        static int list[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
        //闰年
        if (month==2 && ((year % 4 == 0&&year % 100 != 0) || (year % 400 == 0)))
        {
            return 29;
        }
        return list[month];
    }
    //日期的构造函数----初始化
    Date(int year = 0,int month = 1,int day = 1)
    {
        //排除异常数据
        if (year>=0 && month>=1 && month <=12 && day>=1&&day<=GetMonthDays(year,month))
        {
            _year = year;
            _month = month;
            _day = day;
        }
        else
        {
            cout  << "输入数据异常" << endl;
        }
    }
    //日期的拷贝函数
    Date(const Date& d)
    {
        if (this != &d)
        {
            _year = d._year;
            _month = d._month;
            _day = d._day;   
        }
    }
    //日期的打印
    void Print()
    {
        cout << _year << "-" << _month << "-" << _day << endl;
    }
    //符号重载函数--->大于
    bool operator>(const Date& d)
    {
        if (_year > d._year)
        {
            return true;
        }
        if (_year == d._year && _month > d._month)
        {
            return true;
        }
        if (_year == d._year && _month == d._month && _day > d._day)
        {
            return true;
        }
        return false;
    }
    //等于符号
    bool operator==(const Date& d)
    {
        if (_year = d._year && _month == d._month && _day == d._day)
        {
            return true;
        }
        return false;
    }
    //小于,d1<d2
    bool operator<(const Date& d)
    {
        return !(*this > d);
    }
    //大于等于,d1>=d2
    bool operator>=(const Date& d)
    {
        return (*this > d && *this == d);
    }
    //小于等于
    bool operator<=(const Date& d)
    {
        return (*this < d && *this == d);
    }
    //d1 + day;日期加天数加法----->返回日期，原本日期不发生改变，返回一个新的日期
    Date operator+(int day)
    {
        Date temp(*this);
        temp._day = temp._day + day;
        while (temp._day > GetMonthDays(this->_year,this->_month))
        {
            temp._day -= GetMonthDays(this->_year,this->_month);
            temp._month++;
            while (temp._month >= 13)
            {
                temp._month -= 12;
                temp._year++;
            }
        }
        return temp;
    }
    //d1 - day
    Date operator-(int day)
    {
        Date temp = *this;
        temp._day = temp._day - day;
        while (temp._day < 0)
        {
            temp._month--;
            temp._day = temp._day + GetMonthDays(temp._year,temp._month);
            if (temp._month == 0)
            {
                temp._year--;
                temp._month = 12;
            }
        }
        return temp;
    }
    //前自加---->++d1 = d1 = d1 + 1;
    Date& operator++()
    {
        _day = _day + 1;
        while (_day > GetMonthDays(this->_year,this->_month))
        {
           _day -= GetMonthDays(this->_year,this->_month);
            _month++;
            while (_month >= 13)
            {
                _month -= 12;
                _year++;
            }
        }
        return *this;
    }
    //后自加---->d1++
    Date operator++(int) //构成重载
    {
        Date temp = *this;
        ++*this;
        return temp;
    }
    //自减
    Date& operator--()
    {
        _day = _day - 1;
        while (_day > GetMonthDays(this->_year,this->_month))
        {
           _day -= GetMonthDays(this->_year,this->_month);
            _month++;
            while (_month >= 13)
            {
                _month -= 12;
                _year++;
            }
        }
        return *this;
    }
    Date operator--(int)
    {
        Date temp = *this;
        --*this;
        return temp;
    }
    //赋值符号d5 = d4
    Date& operator=(const Date& d)
    {
        if (this != &d)
        {
            _year = d._year;
            _month = d._month;
            _day = d._day;
        }
        return *this;//防止拷贝构造。浪费空间，直接传引用返回
    }
    // d1 += day;
    Date& operator+=(int day)
    {
        if (day < 0)
        {
            *this = *this - (-day);
            return *this;
        }
        
        *this = *this + day;
        return *this;
    }
    // d1 -+ day
    Date& operator-=(int day)
    {
        if (day < 0)
        {
            *this = *this + (-day);
            return *this;
        }
        *this = *this - day;
        return *this;
    }
    // 日期-日期---->d1 - d2
    int operator-(const Date& d)
    {
        Date max = *this;
        Date min = d;
        if (*this < d)
        {
            max = d;
            min = *this;
        }
        int count = 0;
        while(min < max)
        {
            min++;
            count++;
        }
        return count;
    }
private:
    int _year;
    int _month;
    int _day;
};
//cout实现
ostream& operator<<(ostream& out,const Date& d)
{
    out << d._year << "-" << d._month << "-" <<d._day << endl;
    return out;
}
//cin实现
istream& operator>>(istream& in,Date& d)
{
    in >> d._year >>d._month >> d._day;
    return in;
}  
//日期的实现
void Text_1()
{
    Date d1(2025,10,10),d2;
    cin >> d1 >> d2;
    cout << d1 << d2 <<endl;
}
//栈的实现
void Text_2()
{
    Stack s;

}
int main()
{
    Text_1();
    return 0;
}
