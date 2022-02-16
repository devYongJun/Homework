#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

static void Replace(string& _Text, const string& _Prev, const string& _Next, int _Count = 1)
{
    size_t strPos = 0;
    int count = 0;
    while ((strPos = _Text.find(_Prev)) != string::npos)
    {
        _Text.replace(strPos, _Prev.size(), _Next);

        count++;
        if (count == _Count)
        {
            break;
        }
    }
}

static void Insert(string& _Text, int _Index, const string& _InsertText)
{
    if (_Text.size() < _Index)
    {
        _Text.resize(_Index);
    }
    _Text.insert(_Index, _InsertText);
}


static void Remove(string& _Text, const string& _RemoveText)
{
    size_t strPos = 0;
    while ((strPos = _Text.find(_RemoveText)) != string::npos)
    {
        _Text.erase(strPos, _RemoveText.size());
    }
}


static void TrimRemove(string& _Text)
{
    Remove(_Text, " ");
}


static void ToUpper(string& _Text)
{
    transform(_Text.begin(), _Text.end(), _Text.begin(), [](unsigned char c) {
        return toupper(c);
        });
}

// 콘솔 출력 관련
#define TITLE(str) left << setw(10) << str << " : " 

int main()
{
    
    string source = "Hello World 게 임 서버-----";
    cout << TITLE("source") << source << endl << endl;

    string replaceOne = source;
    Replace(replaceOne, "게 임", "Game");
    cout << TITLE("replaceOne") << replaceOne << endl;

    string replaceTwo = source;
    Replace(replaceTwo, "l", "::", 2);
    cout << TITLE("replaceTwo") << replaceTwo << endl;

    string insert = source;
    Insert(insert, 4, "------");
    cout << TITLE("insert") << insert << endl;

    string remove = source;
    Remove(remove, "--");
    cout << TITLE("remove") << remove << endl;

    string trim = source;
    TrimRemove(trim);
    cout << TITLE("trim") << trim << endl;
    
    string upper = source;
    ToUpper(upper);
    cout << TITLE("upper") << upper << endl;


    return 0;
}