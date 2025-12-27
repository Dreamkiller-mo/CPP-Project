#include <vector>
#include <iostream>
namespace mly
{
    //仿函数
    template <class T>      //泛型函数
    struct less
    {
        bool operator()(const T& a , const T& b)
        {
            return a < b;
        }
    };
    template <class T>      //泛型函数
    struct greater
    {
        bool operator()(const T& a , const T& b)
        {
            return a > b;
        }
    };
    //默认大堆
    template<class T , class Container = std::vector<T> , class Compare = less<T>>
    class priority_queue
    {
        public:
            Compare com; 
            void push(const T& data)
            {
                //向上调整法
                _con.push_back(data);
                AdjustUp(_con.size()-1);
            }
            void pop()
            {
                std::swap(_con[0],_con[_con.size()-1]);
                _con.pop_back();
                AdjustDown(0);
            }
            const T& top() const
            {
                return _con[0];
            }
            size_t size()
            {
                return _con.size();
            }
            bool empty()
            {
                return _con.empty();
            }
        private:
            //向上调整
            void AdjustUp(int pos)
            {
                int child = pos;
                int parent = (child-1) / 2;
                while(child > 0)
                {
                    if(com(_con[parent] , _con[child]))
                    {
                        std::swap(_con[child],_con[parent]);
                        child = parent;
                        parent = (child - 1 ) / 2;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            //向下调整
            void AdjustDown(int root)
            {
                int parent = root;
                int child = parent*2+1;
                while(child < _con.size())
                {
                    if(child + 1 < _con.size() && com(_con[child] , _con[child+1])) ++child;
                    if(com(_con[parent] , _con[child]))
                    {
                        std::swap(_con[child] , _con[parent]);
                        parent = child;
                        child = parent * 2 -1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            Container _con;
    };

    void text_priority_queue()
    {
        mly::priority_queue<int> pq;
        pq.push(1);
        pq.push(2);
        pq.push(9);
        pq.push(0);
        pq.push(55);
        pq.push(654);
        while(!pq.empty())
        {
            std::cout << pq.top() << " ";
            pq.pop();
        }
        std::cout << std::endl;
       
    }
};