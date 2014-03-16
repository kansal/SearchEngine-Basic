#include<iostream>
#include<math.h>
using namespace std;

int getNum(string s)
{
    int num=0;
    int j=s.size()-1;
    for(int i=0;i<s.size();i++)
    {
        num+=(s[i]-'0')*pow(10,j);
        j--;
    }
    return num;
}

int main()
{
    string s;
    cin>>s;
    cout<<getNum(s)<<endl;
}

