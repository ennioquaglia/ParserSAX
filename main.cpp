#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <stdio.h>

#include "SAXparser.h"

using namespace std;

void copyFile(string input,string output){
	ifstream fin(input.data());
	ofstream fout(output.data());
	fout << fin.rdbuf();
	fout.close();
	fin.close();
}

class XmlDebugger : public SAXParser
{
public:
	XmlDebugger(string Filename) {
		filename = Filename;
		execute();			
	}
	
	
	virtual void startElement(string s, string elemento) {
		cout<<"Inizio elemento :"<<elemento<<"  ("<<s<<")"<<"\n";
	}
	virtual void singleElement(string s, string elemento) {
		cout<<"Singolo elemento :"<<elemento<<"  ("<<s<<")"<<"\n";
	}
	virtual void endElement(string s, string elemento) {
		cout<<"Fine elemento :"<<elemento<<"  ("<<s<<")"<<"\n";
	}
	virtual void characters(string s) {
		cout<<"Testo :"<<s<<"\n";
	}

};


class XmlFormatter : public SAXParser
{
	ofstream *out;
	bool caratteri,end,newline;
	int level;
	string outFilename;
public:
	XmlFormatter(string Filename,string OutFilename) {
		level=0;
		newline=1;
		end=caratteri=0;
		filename = Filename;
		outFilename=OutFilename;
		out=new ofstream("temp.xml");
		if(!out->is_open()){
			delete this; //suicide
			return;
		}
		execute();			
	}

	virtual void endOfFile(){
		out->close();
		copyFile("temp.xml",outFilename);
		remove("temp.xml");
	}
	virtual void startElement(string s, string elemento) {
		if(!newline)
			(*out)<<"\n";
		
		for(int i=0;i<level;i++)(*out)<<"  ";
		
		(*out)<<"<"<<s<<">";
		newline=0;
		caratteri=0;
		level++;
	}
	virtual void singleElement(string s, string elemento) {
		if(!newline)
			(*out)<<"\n";
			
		for(int i=0;i<level;i++)(*out)<<"  ";
	
		(*out)<<"<"<<s<<">\n";
		newline=1;
	}
	virtual void endElement(string s, string elemento) {
		level--;
		if(!caratteri)
		for(int i=0;i<level;i++)(*out)<<"  ";
		(*out)<<"</"<<s<<">\n";
		newline=1;
		caratteri=0;
	}
	virtual void characters(string s) {
		if(!s.size())return;
		(*out)<<s;
		caratteri=1;
		newline=0;
	}

};



int main()
{
	string filename = "content.xml";

	XmlFormatter h(filename,"formatted.xml");
	
	return 0;
}
