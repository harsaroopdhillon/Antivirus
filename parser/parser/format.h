#ifndef FORMAT_H
#define FORMAT_H
#include <fstream>
#include <string>
#include <sstream>
#include <list>
using namespace std;

class typeVar
{
  public:
    typeVar(string n, string v, int t):name(n), value(v), type(t){}
    string name;
    string value;
    int type;
};

class signature
{
  public:
    string title;
    string Name;
    string infoDes;
    int infoLev;
    string check;
    list<typeVar*> types;
};

class formater
{
public:
    formater(string in);
    bool checkFormat();
    bool performCheck(list<typeVar*> typeList, string s, string check);
    enum formatState {startBlock, endBlock, blockHead, info, types, check };
    fstream file;
    list<signature*> signatures;
private:
    string rmvStSp(string s);
    string rmvAllSp(string s);
    string formatHex(string s);
    bool validateStart(string line);
    bool validateinfo(string line);
    bool validateTypes(string line);
    bool validateCheck(string line);
    bool replaceStr(std::string& str, const std::string& from, const std::string& to);
    bool validateElement(list<typeVar*> typeList, string typeName, string s);
    bool evaluateCheck(string val, string target);
};

#endif // FORMAT_H
