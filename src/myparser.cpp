// -*- C++ -*-

/* myparser.cc
 *
 * Copyright (C) 2002 The libxml++ development team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "myparser.h"
#include <glibmm/convert.h> //For Glib::ConvertError
#include<string.h>
#include <iostream>
#include<vector>
#include<fstream>
#include <boost/algorithm/string.hpp>
#include<stdio.h>
#include<map>
#include "porter2_stemmer.h"
#include<boost/algorithm/string/predicate.hpp>
#include<boost/algorithm/string/classification.hpp>
#include<set>
using namespace std;
vector<string> strs;
vector<string> newStr;
string prevStr;

//map< string,set < string> > Words;
map< string, map <string, map <string,int> > > Words; //string points to the map of docID pointing to its frequency. for phase 2

map< string, map <string,map <string,int> > > ::iterator it1; // for phase 2
map<string,map<string,int > >:: iterator it2;
map<string,int>::iterator it3;

map <string,int> myArray;

string docID,tempdoc;
int countFlush=0;
bool LINK,TEXT,INFOBOX,ID,TIMESTAMP,CONTRIBUTOR,REVISION,MINOR,COMMENT,PAGE,CATEGORY,REFERENCE,TITLE;
set <string> :: iterator setIt;
extern string outpath;
int docWord=0;


void readStop()
{
    string line;
    ifstream file("stopwords.txt");
    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file,line);
            myArray[line]=1;
        }
    }
}

MySaxParser::MySaxParser()
: xmlpp::SaxParser()
{
}

MySaxParser::~MySaxParser()
{
}

void MySaxParser::on_start_document()
{
    readStop();
    //  std::cout << "on_start_document()" << std::endl;
}
void flushdoc()
{
     ofstream docfile;
     docfile.open(outpath+"docWord.txt",ios::out|ios::app);
     string docTitle;
     for(int i=0;i<newStr.size();i++)
         docTitle+=newStr[i];
     docfile<<tempdoc<<":"<<docTitle <<":"<<docWord<<endl;
     docfile.close();
}

void flushTofile()
{
    ofstream myfile;
    myfile.open (outpath+"parse.txt", ios::out|ios::app);
    if(myfile.is_open())
    {
        for(it1=Words.begin();it1!=Words.end();it1++)
        {
            for(it2=(it1->second).begin();it2!=(it1->second).end();it2++)
            {
                for(it3=(it2->second).begin();it3!=(it2->second).end();it3++)
                {
                    myfile<<it1->first<<"-";
                    myfile<<it3->first<<":";
                        myfile<<it2->first<<"-"<<it3->second;
                    myfile<<endl;
                }
            }
        }
    }
    myfile.close();
    Words.clear();
}
void MySaxParser::on_end_document()
{
    //flushTofile();
    //std::cout << "on_end_document()" << std::endl;
    //for phase 2
        /*  if(myfile.is_open())
    {
        for(it=Words.begin();it!=Words.end();it++)
        {
            myfile<<it->first<<":";
            for(setIt=(it->second).begin();setIt!=(it->second).end();setIt++)
                myfile<<*setIt<<",";
            myfile<<endl;
        }
    }*/

/*    for(it=Words.begin();it!=Words.end();it++)
    {
        cout<<it->first<<":";
        for(setIt=(it->second).begin();setIt!=(it->second).end();setIt++)
            cout<<*setIt<<",";
        cout<<endl;
    }*/
}

void MySaxParser::on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
{
//  std::cout << "node name=" << name << std::endl;
  /*if(name=="page")
      flag=0;
  else if(name=="id" && flag==0)
      flag=1;*/
  if(name=="title")
      TITLE=true;
  if(name=="id")
      ID=true;
  if(name=="revision")
      REVISION=true;
  if(name=="timestamp")
      TIMESTAMP=true;
  if(name=="contributor")
      CONTRIBUTOR=true;
  if(name=="minor")
      MINOR=true;
  if(name=="comment")
      COMMENT=true;
  if(name=="text")
      TEXT=true;
  if(name=="page")
  {
      docWord=0;
      docID.clear();
      PAGE=true;

      INFOBOX=false;
      LINK=false;
      REFERENCE=false;
      CATEGORY=false;
  }
}

void MySaxParser::on_end_element(const Glib::ustring&  name)
{
    if(name=="page")
    {
        flushTofile();
        flushdoc();
    }
    if(name=="title")
            TITLE=false;
    if(name=="id")
            ID=false;
    if(name=="revision")
            REVISION=false;
    if(name=="timestamp")
            TIMESTAMP=false;
    if(name=="contributor")
            CONTRIBUTOR=false;
    if(name=="minor")
            MINOR=false;
    if(name=="comment")
            COMMENT=false;
    if(name=="text")
            TEXT=false;

  //std::cout << "on_end_element()" << std::endl;
}

void MySaxParser::on_characters(const Glib::ustring& text)
{
    /*if(flag==1)
      {
      docID.clear();
      docID=text;
      flag=2;
      }*/
    //else
    //{
   /* if(countFlush==1)
    {
        cout<<"count is 1"<<endl;
        flushTofile();
        countFlush=0;
    }
    countFlush++;*/
    string charFlag;
    string s;
    try
    {
        if(TITLE)
        {
            docID.clear();
            s=text.lowercase();
            strs.clear();
            newStr.clear();
            if(prevStr.size()!=0)
                s=prevStr+s;
            boost::split(strs,s, boost::is_any_of("\"=,|][_:{}<>!-' .//*()\n#$%&*@1234567890^-+~?;\\\t\r"),boost::token_compress_on); //tokenize
            prevStr.clear();
            prevStr=strs[strs.size()-1];
            charFlag="t";
            for(size_t i=0;i<strs.size()-1;i++)
            {
                if(myArray.find(strs[i]) == myArray.end() && boost::all(strs[i],boost::is_alpha()))
                {
                    Porter2Stemmer::stem(strs[i]);
                    newStr.push_back(strs[i]);
                    docWord++;
                    //Words[strs[i]][docID][charFlag]+=1;

                }
            }
        }
        if(ID && !REVISION)
        {
            docID.clear();
            docID=text;
            tempdoc=docID;
            for(size_t i=0;i<newStr.size();i++)
                    Words[newStr[i]][docID]["t"]+=1;
        }
        if(TEXT)
        {
            s=text.lowercase();
            strs.clear();
            //newStr.clear();
            if(prevStr.size()!=0)
                s=prevStr+s;
            boost::split(strs,s, boost::is_any_of("\",|][:}<>!-_' .//*()\n#$%&*@1234567890^-+~?;\\\t\r"),boost::token_compress_on); //tokenize
            prevStr.clear();
            prevStr=strs[strs.size()-1];
            for(size_t i=0;i<strs.size()-1;i++)
            {
                if(strs[i].find("{{infobox")!=string::npos)
                    INFOBOX=true;
                else if(strs[i]=="footnotes")
                    INFOBOX=false;
                if(strs[i]=="==external")
                    LINK=true;
                else if(strs[i]=="category")
                {
                    CATEGORY=true;
                    LINK=false;
                }
                if(strs[i]=="ca" && CATEGORY==true)
                    CATEGORY=false;
                if(myArray.find(strs[i]) == myArray.end() && boost::all(strs[i],boost::is_alpha()))
                {
                    strs[i].erase (std::remove(strs[i].begin(), strs[i].end(), '{'), strs[i].end());
                    strs[i].erase (std::remove(strs[i].begin(), strs[i].end(), '='), strs[i].end());
                    Porter2Stemmer::stem(strs[i]);
                    docWord++;
                    //newStr.push_back(strs[i]);
                    if(INFOBOX)
                        charFlag="i";
                    else
                    {
                        if(LINK)
                            charFlag="l";
                        else
                        {
                            if(CATEGORY)
                                charFlag="c";
                            else
                                charFlag="b";
                        }
                    }
                    Words[strs[i]][docID][charFlag]+=1;// for phase 2
                    //Words[strs[i]].insert(docID);
                }
            }
            //for(size_t i=0;i<newStr.size();i++)
            //cout<<newStr[i]<<" ";
            //std::cout << "on_characters(): " << text << std::endl;
        }
    }
    catch(const Glib::ConvertError& ex)
    {
        std::cerr << "MySaxParser::on_characters(): Exception caught while converting text for std::cout: " << ex.what() << std::endl;
    }
    // }
}

void MySaxParser::on_comment(const Glib::ustring& text)
{
  try
  {
    //std::cout << "on_comment(): " << text << std::endl;
  }
  catch(const Glib::ConvertError& ex)
  {
    //std::cerr << "MySaxParser::on_comment(): Exception caught while converting text for std::cout: " << ex.what() << std::endl;
  }
}

void MySaxParser::on_warning(const Glib::ustring& text)
{
  try
  {
    //std::cout << "on_warning(): " << text << std::endl;
  }
  catch(const Glib::ConvertError& ex)
  {
    std::cerr << "MySaxParser::on_warning(): Exception caught while converting text for std::cout: " << ex.what() << std::endl;
  }
}

void MySaxParser::on_error(const Glib::ustring& text)
{
  try
  {
    //std::cout << "on_error(): " << text << std::endl;
  }
  catch(const Glib::ConvertError& ex)
  {
    std::cerr << "MySaxParser::on_error(): Exception caught while converting text for std::cout: " << ex.what() << std::endl;
  }
}

void MySaxParser::on_fatal_error(const Glib::ustring& text)
{
  try
  {
    std::cout << "on_fatal_error(): " << text << std::endl;
  }
  catch(const Glib::ConvertError& ex)
  {
    std::cerr << "MySaxParser::on_characters(): Exception caught while converting value for std::cout: " << ex.what() << std::endl;
  }
}

