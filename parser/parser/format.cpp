#include "format.h"

int char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    if(input == '?')
        return '?';

    throw std::invalid_argument("Invalid Hex Value");
}

string hex2bin(string src)
{
    string retval;
    for(unsigned int i = 0; i < src.length(); i += 2){
        if(src[i] != '?')
            retval = retval + (char)(char2int(src[i])*16 + char2int(src[i + 1]));
        else
            retval = retval + src[i];
    }
    return retval;
}


formater::formater(string in)
{
    file.open(in, ios_base::in | ios::binary);
}

string formater::rmvStSp(string s){
    while(s.length() > 0){ // Remove starting spaces
        if(s[0] == ' ')
            s = s.substr(1, s.length() - 1);
        else
            break;
    }
    return s;
}

bool formater::replaceStr(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

string formater::rmvAllSp(string s){
    string retval = "";
    retval.reserve(s.size());
    for(unsigned int i = 0; i < s.length(); ++i){
        if(s[i] != ' '){
            retval = retval + s[i];
        }
    }
    return retval;
}

string formater::formatHex(string s){
    string retval = "";
    retval.reserve(s.size());
    for(unsigned int i = 0; i < s.length(); ++i){
        if((s[i] >= '0' && s[i] <= '9') ||  (s[i] >= 'A' && s[i] <= 'F') || (s[i] >= 'a' && s[i] <= 'f') || s[i] == '?'){
            retval = retval + s[i];
        }
    }
    return retval;
}

bool formater::validateStart(string line){
    stringstream ss(line);
    string sig, title, value, colon;
    ss >> sig >> title >> colon >> value;
    if(sig == "Signature" && colon == ":" && ss.rdbuf()->in_avail() < 2){
        signature* ptr = new signature;
        ptr->title = title;
        ptr->Name = value;
        signatures.push_front(ptr);
        return true;

    } else
        return false;
}

bool formater::validateinfo(string line){
    line = rmvAllSp(line);
    if(line == "info:\r"){
        string des, lev;
        std::getline(file, des);
        des = rmvAllSp(des);
        std::getline(file, lev);
        lev = rmvAllSp(lev);
        if(des.substr(0, 13) == "description=\"" && des.substr(des.length() - 3, 3) == "\";\r"
                && lev.substr(0, 6) == "level=" && lev.substr(lev.length() - 2, 2) == ";\r"){
            des = des.substr(13, des.length() - 16);
            lev = lev.substr(6, lev.length() - 8);
            signature* ptr = signatures.front();
            ptr->infoDes = des;
            ptr->infoLev = stoi(lev);
            return true;
        } else
            return false;
    } else
        return false;
}

bool formater::validateTypes(string line){
    line = rmvAllSp(line);
    if(line == "types:\r"){
        signature* ptr = signatures.front();

        unsigned int pos = file.tellg();
        while(std::getline(file, line)){
            string xline = rmvAllSp(line);
            line = rmvStSp(line);
            if(xline == "check:\r"){
                file.seekg(pos, file.beg);
                break;
            } else if(xline == "\r"){
                continue;
            } else if(xline[0] == '$' && xline[xline.length()-2] == ';' && xline.find('=') > 1){

                string name, val;
                int type;
                int eqPos = xline.find('=');
                name = xline.substr(0, eqPos);
                val = xline.substr(eqPos + 1, xline.length() - eqPos - 3);
                if(val[0] == val[val.length()-1]){
                    val = val.substr(1, val.length()-2);
                    type = 0;
                }else if(val[0] == '{' && val[val.length()-1] == '}'){
                    val = formatHex(val);
                    type = 1;
                } else{
                    eqPos = line.find('=');
                    val = rmvStSp(line.substr(eqPos + 1, line.length() - eqPos - 3));
                    type = 2;
                    if(!validateCheck(val))
                        return false;
                }
                ptr->types.push_front(new typeVar(name, val, type));

            } else
                return false;

            pos = file.tellg();
        }
    } else
        return false;
    return true;
}

bool formater::validateCheck(string line){
    string temp;
    stringstream ss(line);
    while(temp != ""){
        ss >> temp;
        if(temp == "or" || temp == "and" || temp == "not")
            continue;
        else if(temp[1] == '$'){
            bool flag = false;
            signature* ptr = signatures.front();
            list<typeVar*>::iterator typeIter = ptr->types.begin();
            while(typeIter != ptr->types.end()){
                typeVar* currentType = *typeIter;
                if(currentType->name == temp)
                    flag = true;
                ++typeIter;
            }
            if(!flag)
                return false;
        } else
            return false;
    }
    return true;
}

bool formater::checkFormat()
{
    formatState state = blockHead;
    string line = "";
    int lineNum = 0;
    while(std::getline(file, line)){
        ++lineNum;
        if(line.size() > 1 && line.substr(0,2) != "//" && line[0] != '\r'){
            line = rmvStSp(line);
            if(state == blockHead){
                if(validateStart(line))
                    state = startBlock;
                else
                    return false;
            }
            else if(state == startBlock){
                if(line == "{\r"){
                    state = info;
                } else
                    return false;
            }
            else if(state == info){
                if(validateinfo(line))
                    state = types;
                else
                    return false;
            } else if(state == types){
                if(validateTypes(line))
                    state = check;
                else
                    return false;
            } else if(state == check){
                if(line == "check:\r"){
                    std::getline(file, line);
                    line = rmvStSp(line);
                    string templine = rmvAllSp(line);
                    if(templine[templine.length()-2] == ';'){
                        signatures.front()->check = line.substr(0, line.length() - 2);
                        if(validateCheck(signatures.front()->check))
                            state = endBlock;
                        else
                            return false;
                    } else
                        return false;
                }
            } else if(state == endBlock){
                if(line == "}\r"){
                    state = blockHead;
                } else
                    return false;
            }
        }
    }
    return true;
}

bool formater::performCheck(list<typeVar*> typeList, string s, string check){
    int start_pos = check.find("not");
    string s1;
    s1 = check.substr(0, start_pos);

    stringstream ss(s1);
    string element, op;

    ss >> element;
    bool result1 = validateElement(typeList, element, s);
    while(ss >> op >> element){
        if(op == "and"){
            result1 = result1 && validateElement(typeList, element, s);
        } else if(op == "or"){
            result1 = result1 || validateElement(typeList, element, s);
        } else
            break;
    }

    if(result1 == false)
        return false;

    if(start_pos == -1)
        return result1;

    string s2;
    s2 = check.substr(start_pos, check.length() - s1.length());
    ss.clear();
    ss.str(s2);
    ss >> element >> element;
    bool result2 = !validateElement(typeList, element, s);
    while(ss >> op >> element){
        if(op == "and"){
            result2 = result2 && validateElement(typeList, element, s);
        } else if(op == "or"){
            result2 = result2 || validateElement(typeList, element, s);
        } else
            break;
    }

    return result1 && result2;
}

bool formater::validateElement(list<typeVar*> typeList, string typeName, string s){
    list<typeVar*>::iterator iter = typeList.begin();
    typeVar* element = nullptr;
    while(iter != typeList.end()){
        typeVar* t = *iter;
        if(t->name == typeName){
            element = t;
            break;
        }
        ++iter;
    }
    if(element != nullptr){
        if(element->type == 2)
            return performCheck(typeList, s, element->value);
        else if(element->type == 1)
            return evaluateCheck(hex2bin(element->value), s);
        else
            return evaluateCheck(element->value, s);
    } else
        return false;
}

bool formater::evaluateCheck(string val, string target){
    size_t pos = 0;
    for(unsigned int i = 0; i < val.length(); ++i)
        if(val[i] != '?')
            break;
        else{
            val = val.substr(1, val.length()-2);
            --i;
        }
    bool flag = true, found = false;
    while(flag && !found){
        pos = target.find(val[0], pos + 1);
        if(pos != std::string::npos){
            found = true;
            for(unsigned int j = 1; j < val.length(); ++j){
                if(pos + j < target.length()){
                    if(val[j] == target[pos + j] || val[j] == '?'){
                        continue;
                    } else {
                        found = false;
                        break;
                    }
                } else
                    found = false;
            }
        } else
            flag = false;
    }
    return found;
}
