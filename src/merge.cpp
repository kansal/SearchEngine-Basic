#include<iostream>
#include<fstream>
#include<string>
#include<fstream>
#include<map>
#include<math.h>
#include<set>
#include<stdlib.h>

using namespace std;
map <string ,map<string,string> > dict;
//map <string ,map<string,string> >:: iterator it1;
//map <string,string>:: iterator it2;
set < pair<int,string> > S;
set <pair <int,string> >::iterator it1;
string sub,prev;



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



void flushDict()
{
    ofstream outfile;
    outfile.open("open.txt",ios::app|ios::out);
    /*for(it1=dict.begin();it1!=dict.end();it1++)
    {
        outfile<<it1->first<<" ";
        for(it2=(it1->second).begin();it2!=(it1->second).end();it2++)
        {
            outfile<<it2->first<<":"<<it2->second<<",";
        }
        outfile<<endl;
    }
    outfile.close();*/
    outfile<<prev<<":";
    for(it1=S.begin();it1!=S.end();it1++)
    {
        outfile<<it1->second<<"-"<<it1->first<<",";
    }
    outfile<<endl;
}



int main(int argc, char* argv[])
{
    string line;
    bool firstFlag=true;
    char outpath[1000];
    sprintf(outpath,"%s/sorted.txt",argv[1]);
    ifstream infile;
    infile.open(outpath,ios::in);
    string newSub;
    while(getline(infile,line))
    {
        size_t found=line.find(":");
        sub=line.substr(0,found);
        size_t dash=line.find("-");
        newSub=line.substr(dash+1,line.size()-dash);
        dash=newSub.find("-");
        found=newSub.find(":");
        if(prev!=sub)
        {
            if(!firstFlag)
                flushDict();
            else
                firstFlag=false;
            S.clear();
            prev=sub;
                S.insert(make_pair(getNum(newSub.substr(dash+1,line.size()-dash)),newSub.substr(found+1,dash-found-1)));
            }
            else
            {
                S.insert(make_pair(getNum(newSub.substr(dash+1,line.size()-dash)),newSub.substr(found+1,dash-found-1)));
            }
    }
    infile.close();
}

