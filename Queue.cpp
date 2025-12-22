#include <iostream>
#include <vector>
#include <list>

using namespace std;

namespace mly
{
    template<class T,class Container>
    class Queue
    {
        public:
            void push(const T& x){_con.push_back(x);}
            void pop(){_con.pop_front();}
            T& front(){return _con.front();}
            T& back(){return _con.back();}
            bool empty(){return _con.empty();}
            size_t size(){return _con.size();}
        private:
            Container _con;
    };
};

void Text_Queue()
{
    mly::Queue<int,list<int>> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);
    s.push(6);
    while(!s.empty())
    {
        cout << s.front() << " ";
        s.pop();
    }
}
int main()
{
    Text_Queue();
    return 0;
}