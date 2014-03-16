#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<fstream>
#include <boost/algorithm/string.hpp>
#include<boost/algorithm/string/trim.hpp>
#include "porter2_stemmer.h"
using namespace std;
map < string,string >dict;
vector<string>strs;
string outpath;
void makeDict()
{
    string line;
    ifstream file;
    file.open(outpath+"parse.txt");
    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file,line);
            boost::split(strs,line, boost::is_any_of(":"),boost::token_compress_on);
            dict[strs[0]]=strs[1];
        }
    }
}
int main(int argc, char* argv[])
{
    outpath=argv[1];
    if(outpath[outpath.length()-1] != '/') outpath += '/';
    makeDict();
    int n;
    string str;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        str.clear();
        cin>>str;
            transform(str.begin(), str.end(), str.begin(), ::tolower);
            boost::trim_if(str,boost::is_any_of(" \"\n\t\r!@#$%^&*()_+-=~`<>?,./:\";\'[]{}|\\"));
            Porter2Stemmer::stem(str);
            cout<<dict[str]<<endl;
        }
}
