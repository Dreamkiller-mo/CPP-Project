#include <iostream>
#include <vector>
#include <list>

using namespace std;

namespace mly
{
    template<class T,class Container>
    class Stack
    {
        public:
            void push(const T& x){_con.push_back(x);}
            void pop(){_con.pop_back();}
            T& top(){return _con.back();}
            bool empty(){return _con.empty();}
            size_t size(){return _con.size();}
        private:
            Container _con;
    };
};

void Text_Stack()
{
    mly::Stack<int,vector<int>> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);
    s.push(6);
    while(!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }
}
int main()
{
    Text_Stack();
    return 0;
}