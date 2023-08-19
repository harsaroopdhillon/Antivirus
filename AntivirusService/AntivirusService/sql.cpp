#include<iostream>
#include <sqlite_modern_cpp.h>
#include "config.h"
using namespace  sqlite;
void createdatabase() {

	database db(DATABASE);
     db <<
         "create table if not exists user ("
         "   _id integer primary key autoincrement not null,"
         "   age int,"
         "   name text,"
         "   weight real"
         ");";

}
   
const std::string selecthash(const std::string& hash) {

	database db(DATABASE);
	std::string type;
	db << "select type from hashs where hash = ?" << hash >> type;
	return type;
}


const std::string selectapi(const std::string& api) {

	database db(DATABASE);
	std::string type;
	db << "select type from malapi where api = ?" << api >> type;
	return type;
}