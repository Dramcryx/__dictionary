#include <iostream>
#include <vector>

#include "map_dictionary.h"

using namespace std;

int main()
{
    MapDictionary<int, int> x;
    x.Set(1, 2);
    //x.Get(3);
    MapDictionary<int *, int *> z;
    MapDictionary<const int *, int *> y;
    MapDictionary<const int *, int *> g;

    int s = 22;
    int & rs = s;
    int zaz = 1;
    int & rss = zaz;


    MapDictionary<const int *, int &> gg;
    int stats = 0;
    gg.Set(&s, rs);
    const int & ff = gg.Get(&s);
    int & df = gg.Get(&s);
    df = 44;
    MapDictionary<int&, int *> ss;
    ss.Set(rs, &s);
    int * zz = ss.Get(rs);

    MapDictionary<const int &, int *> w;
    std::vector<int> num{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int*> refs;
    for (auto & i: num)
    {
        refs.push_back(&i);
        w.Set(*(&i), &i);
    }
    for (auto i: refs)
    {
        w.Get(*i);
    }
    w.Set(rs, &s);
    auto www = w.Get(rs);

    return 0;
}
