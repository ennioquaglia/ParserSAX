#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <stdio.h>

using namespace std;

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


class SAXParser
{
protected:
	string filename;
	SAXParser(){}   	
public:
	SAXParser(string Filename) {
		filename = Filename;			
	}
	
	void execute(){
		ifstream in(filename.data());
		
		if(!in.is_open()) {
			cout << "errore apertura file\n";
			return;
		}
		
		bool startTag=0,endTag=0;
		string element;
		string s;
		char c=in.get(),pc=c;
		int pointer_pos=0;
		
		while(!in.eof()){
			if(pc=='<'&&c!=pc){
				if(c=='/'){
					endTag=1;
				}else{
					startTag=1;
				   	s+=c;
				}
			}else{
				if(c=='>'){
					size_t pos = s.find(" ");
					if(pos!=-1) element = s.substr (0,pos);  
   	   	   	   	    else 		element=s;
   	   	   	   	    
					if(pc=='/'||pc=='?'){
						if(element.size())
						singleElement(s,element);
					}else{
						if(startTag){
							startElement(s,element);
						}
						if(endTag){
							endElement(s,element);
						}
					}
					endTag=0;
					startTag=0;
					s.clear();
					   	
				}else{
					if(c!='<'&&c!='>'){
						s+=c;
					}else{
						s=trim(s);
						if(s.size()){	
						   characters(s);
					   	   s.clear();
						}	
					}
				}
			}
			pc=c;
			c=in.get();	
			pointer_pos++;
		}
		endOfFile();
	}
	string getAttributeValue(string s,string AttributeName){
		
		size_t pos = s.find(" "+AttributeName);
		if(pos==-1)return "";
		pos=pos+AttributeName.length()+3;
		return s.substr(pos,s.find("\"",pos)-pos);
	}
	virtual void endOfFile(){}
	virtual void startElement(string s, string element) {}
	virtual void singleElement(string s, string element) {}
	virtual void endElement(string s, string element) {}
	virtual void characters(string s) {}

};