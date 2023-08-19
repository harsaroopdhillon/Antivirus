%{
  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <map>
  #include <vector>
  #include <iterator>
  #include <iostream>
  #include <sstream>
  #include <regex>
  #include "SyntaxTree.h"

  int yylex(void);
  void yyerror(char const *);

  using namespace std;

  enum VAR_TYPE
  {
    notype,
    dec,
    hex,
    text
  };

  class dataDef
  {
  private:
    map< string , map<VAR_TYPE , vector<string>>> comp_holder;

  public:

    dataDef() = default;
    ~dataDef() = default;

    void addData(string var , VAR_TYPE type, vector<string> lst)
    {
        comp_holder[var] = {{type , lst}};
    }

    VAR_TYPE getType(string var)
    {
        for(auto outer : comp_holder)
        {
            if(outer.first.c_str() == var)
            {
                for(auto inner : outer.second)
                {
                    return inner.first;
                }
            }
        }
       return notype;
    }

    vector<string> getData(string var)
    {
        for(auto outer : comp_holder)
        {
            if(outer.first.c_str() == var)
            {
                for(auto inner : outer.second)
                {
                    return inner.second;
                }
            }
        }
        return {};
    }

    void clear()
    {
        for(auto outer : comp_holder)
        {
            outer.second.clear();
        }
        comp_holder.clear();
        comp_holder = map< string , map<VAR_TYPE , vector<string>>>();
    }

    int varCount() { return comp_holder.size(); }
}DataHolder;

%}

%token STR NUM VAR ADD SUB EQL GTR LSS LPRN RPRN LSQR TO COMMA IMPORT EXPORT INSENSITIVE
%token RSQR EOL EOF_F LEFT_BRACE RIGHT_BRACE UNK INFO TYPES CHECK OR AND NNOT EXPR AT
%token SIG TXT COLON WIDE ASCII NONALPHA HEX STAR NOT FNAME FSIZE CALL REG

%start program

%%
  program:
    program program
    | SIG TXT COLON NUM LEFT_BRACE body RIGHT_BRACE { return 0; }
    | EOF_F { return -1; }

  body:
    info_grp type_grp check_group
    | type_grp info_grp check_group
    | type_grp check_group info_grp

  info_grp:
    INFO TXT EQL STR EOL TXT EQL NUM EOL

  type_grp:
    TYPES declr

  declr:
    declr var_dec

  var_dec:

    VAR EQL REG           {
                            string tp((reinterpret_cast<compiler::SyntaxTree *>($3))->toCode().c_str());
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                            { tp });
                            free($1); free($3);
                          }
    | VAR EQL STR EOL      {
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                            {(reinterpret_cast<compiler::SyntaxTree *>($3))->toCode()});
                            free($1); free($3);
                         }
    | INSENSITIVE VAR EQL STR EOL
                        {
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($2))->toCode() , VAR_TYPE::text ,
                            {"insensitive" , (reinterpret_cast<compiler::SyntaxTree *>($4))->toCode()});
                            free($2); free($4);
                        }
    | WIDE VAR EQL STR EOL
                        {
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($2))->toCode() , VAR_TYPE::text ,
                            {"wide" , (reinterpret_cast<compiler::SyntaxTree *>($4))->toCode()});
                            free($2); free($4);
                        }
    | ASCII VAR EQL STR EOL
                        {
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($2))->toCode() , VAR_TYPE::text ,
                            {"ascii" , (reinterpret_cast<compiler::SyntaxTree *>($4))->toCode()});
                            free($2); free($4);
                        }
    | NONALPHA VAR EQL STR EOL
                        {
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($2))->toCode() , VAR_TYPE::text ,
                            {"nonalpha" , (reinterpret_cast<compiler::SyntaxTree *>($4))->toCode()});
                            free($2); free($4);
                        }
    | VAR EQL FNAME EQL EQL STR EOL
                          {
                              stringstream ss;
                              ss << "True if filename " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode().c_str() << " exist else false";
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($6);
                          }

    | VAR EQL FNAME NOT EQL STR EOL
                          {
                              stringstream ss;
                              ss << "False if filename " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode().c_str() << " exist else true";
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($6);
                          }

    | VAR EQL FSIZE GTR EQL NUM EOL
                          {
                              stringstream ss;
                              ss << "True if filesize is greater or equal to " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode().c_str();
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($6);
                          }

    | VAR EQL FSIZE LSS EQL NUM EOL
                          {
                              stringstream ss;
                              ss << "True if filesize is less or equal to " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode().c_str();
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($6);
                          }

    | VAR EQL FSIZE EQL EQL NUM EOL
                          {
                              stringstream ss;
                              ss << "True if filesize is equal to " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode().c_str();
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($6);
                          }
    | VAR EQL FSIZE NOT EQL NUM EOL
                          {
                              stringstream ss;
                              ss << "True if filesize is not equal to " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode().c_str();
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($6);
                          }

    | VAR EQL FSIZE GTR NUM EOL
                          {
                              stringstream ss;
                              ss << "True if filesize is greater than " << (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str();
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($5);
                          }

    | VAR EQL FSIZE LSS NUM EOL
                          {
                              stringstream ss;
                              ss << "True if filesize is less than " << (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str();
                              DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                              {ss.str()});
                              free($1); free($5);
                          }


    | VAR EQL NUM EOL    {
                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::dec ,
                            {(reinterpret_cast<compiler::SyntaxTree *>($3))->toCode()});
                            free($1); free($3);
                         }

    | VAR EQL LEFT_BRACE NUM NUM NUM NUM RIGHT_BRACE EOL
                        {
                          stringstream strm;
                          strm << (reinterpret_cast<compiler::SyntaxTree *>($4))->toCode() << " " << (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode()
                               << " " << (reinterpret_cast<compiler::SyntaxTree *>($6))->toCode() << " " << (reinterpret_cast<compiler::SyntaxTree *>($7))->toCode();
                          DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text ,
                          { strm.str() });
                          free($1); free($4);  free($5); free($6);  free($7);
                        }

    | VAR EQL LEFT_BRACE NUM NUM COMMA LSQR NUM TO NUM RSQR COMMA NUM COMMA NUM RIGHT_BRACE EOL
                        {
                            int start1 = strtol((reinterpret_cast<compiler::SyntaxTree *>($4))->toCode().c_str() , NULL , 16);
                            int start2 = strtol((reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str() , NULL , 16);

                            int range_start = strtol((reinterpret_cast<compiler::SyntaxTree *>($8))->toCode().c_str() , NULL , 16);
                            int range_stop = strtol((reinterpret_cast<compiler::SyntaxTree *>($10))->toCode().c_str() , NULL , 16);

                            int stop1 = strtol((reinterpret_cast<compiler::SyntaxTree *>($13))->toCode().c_str() , NULL , 16);
                            int stop2 = strtol((reinterpret_cast<compiler::SyntaxTree *>($15))->toCode().c_str() , NULL , 16);

                            int r_num = range_start + int( ((range_stop - range_start) + 1) * rand() / (RAND_MAX + 1.0) );

                            stringstream strm;
                            strm << std::hex << start1 << " " << std::hex << start2 << " ";

                            for(int i = 0; i < r_num; i++)
                            {
                              int new_num =  rand() % 255;
                              strm << std::hex << new_num << " ";
                            }

                            strm << std::hex << stop1 << " " << std::hex << stop2 << "\n";
                            string rslt(strm.str());

                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::hex,
                            {rslt});
                            free($1); free($4); free($5); free($8); free($10); free($13); free($15);
                        }
    | VAR EQL LEFT_BRACE NUM NUM LPRN NUM NUM OR NUM RPRN NUM RIGHT_BRACE EOL
                        {
                            int v1 = strtol((reinterpret_cast<compiler::SyntaxTree *>($4))->toCode().c_str() , NULL , 16);
                            int v2 = strtol((reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str() , NULL , 16);

                            int o1 = strtol((reinterpret_cast<compiler::SyntaxTree *>($7))->toCode().c_str() , NULL , 16);
                            int o2 = strtol((reinterpret_cast<compiler::SyntaxTree *>($8))->toCode().c_str() , NULL , 16);
                            int o3 = strtol((reinterpret_cast<compiler::SyntaxTree *>($10))->toCode().c_str() , NULL , 16);

                            int l1 = strtol((reinterpret_cast<compiler::SyntaxTree *>($12))->toCode().c_str() , NULL , 16);

                            stringstream strm;
                            strm << std::hex << v1 << v2 << o1 << o2 << l1;
                            string rslt(strm.str());

                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::hex,
                            {rslt});
                            free($1); free($4); free($5); free($7); free($8); free($10); free($12);
                        }
    | VAR EQL IMPORT LPRN STR COMMA STR COMMA STR COMMA STR RPRN EOL
                        {
                          vector<string> vec = {"Import:" , (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($7))->toCode(),
                                                (reinterpret_cast<compiler::SyntaxTree *>($9))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($11))->toCode()};
                          DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text,
                                                {(reinterpret_cast<compiler::SyntaxTree *>($5))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($7))->toCode() ,
                                                (reinterpret_cast<compiler::SyntaxTree *>($9))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($11))->toCode() });
                          free($1); free($5); free($7); free($9); free($11);
                        }
    | VAR EQL EXPORT LPRN STR COMMA STR COMMA STR COMMA STR RPRN EOL
                        {
                          vector<string> vec = {"Export:" , (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($7))->toCode(),
                                                  (reinterpret_cast<compiler::SyntaxTree *>($9))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($11))->toCode()};
                          DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text,
                              {(reinterpret_cast<compiler::SyntaxTree *>($5))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($7))->toCode() ,
                              (reinterpret_cast<compiler::SyntaxTree *>($9))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>($11))->toCode() });
                          free($1); free($5); free($7); free($9); free($11);
                        }


    | VAR EQL CALL LPRN STR RPRN EOL
                        {
                            stringstream strm;
                            strm << "exports " << (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str();
                            string rslt(strm.str());

                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text,
                            {rslt});
                            free($1); free($5);
                        }

    | VAR EQL CALL LPRN STR COMMA STR RPRN EOL
                        {
                            stringstream strm;
                            strm << "exports " << (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str() << " and "
                                 << (reinterpret_cast<compiler::SyntaxTree *>($7))->toCode().c_str();
                            string rslt(strm.str());

                            DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() , VAR_TYPE::text,
                            {rslt});
                            free($1); free($5); free($7);
                        }

  check_group:
    CHECK ch

  ch:
    ch ch_loop

  ch_loop:

    CALL LPRN STR RPRN EOL
                            {
                              printf("call export %s\n" , (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode().c_str());
                            }

    | CALL LPRN STR COMMA STR RPRN EOL
                            {
                              printf("call export %s and %s\n" , (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode().c_str() , (reinterpret_cast<compiler::SyntaxTree *>($5))->toCode().c_str());
                            }

    | VAR OR VAR EOL  {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() << endl;
                              exit(1);
                            }

                            vector<string> var2 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($3))->toCode());
                              if(var2.size() <= 0){
                                cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode() << endl;
                                exit(1);
                              }

                            for(int j = 0; j < var1.size(); j++)
                              cout << var1[j].c_str() << " ";
                            cout << " or ";
                            for(int j = 0; j < var2.size(); j++)
                              cout << var2[j].c_str() << " ";
                            cout << " matching " << endl;
                            free($1); free($3);
                            DataHolder.clear();
                          }
      | VAR AND VAR EOL  {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() << endl;
                              exit(1);
                            }

                            vector<string> var2 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($3))->toCode());
                            if(var2.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode() << endl;
                              exit(1);
                            }

                            for(int j = 0; j < var1.size(); j++)
                              cout << var1[j].c_str() << " ";
                            cout << " and ";
                            for(int j = 0; j < var2.size(); j++)
                              cout << var2[j].c_str() << " ";
                            cout << " matching " << endl;
                            free($1); free($3);
                            DataHolder.clear();
                          }
      | VAR NNOT VAR EOL  {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() << endl;
                              exit(1);
                            }

                            vector<string> var2 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($3))->toCode());
                            if(var2.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode() << endl;
                              exit(1);
                            }

                            for(int j = 0; j < var1.size(); j++)
                              cout << var1[j].c_str() << " ";
                            cout << " not ";
                            for(int j = 0; j < var2.size(); j++)
                              cout << var2[j].c_str() << " ";
                            cout << " matching " << endl;
                            free($1); free($3);
                            DataHolder.clear();
                          }
    | VAR AT NUM EOL
                          {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() << endl;
                            exit(1);
                            }
                            printf("%s at %s \n" , var1[0].c_str() , (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode().c_str());
                            free($1); free($3);
                            DataHolder.clear();
                          }
    | VAR AT HEX EOL
                          {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($1))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($1))->toCode() << endl;
                            exit(1);
                            }
                            printf("%s at %s \n" , var1[0].c_str() , (reinterpret_cast<compiler::SyntaxTree *>($3))->toCode().c_str());
                            free($1); free($3);
                            DataHolder.clear();
                          }
    | LPRN VAR STAR NUM RPRN EOL
                          {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($2))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($2))->toCode() << endl;
                            exit(1);
                            }
                            printf("if %s occurs %s times\n" , var1[0].c_str() , (reinterpret_cast<compiler::SyntaxTree *>($4))->toCode().c_str());
                            free($2); free($4);
                            DataHolder.clear();
                          }
    | VAR EOL       {
                            vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>($2))->toCode());
                            if(var1.size() <= 0){
                              cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>($2))->toCode() << endl;
                              exit(1);
                            }

                            for(int i{0}; i < var1.size(); i++)
                            {
                              string outpt_uncen = string(var1[i].c_str());
                              stringstream ss;

                              for(int i{0}; i < (outpt_uncen.length() - 1); i++)
                              {
                                if(outpt_uncen[i] == '\\')
                                {
                                  if(outpt_uncen[i+1] == 'n')
                                    { ss << '\n'; i += 1; }
                                  else if(outpt_uncen[i+1] == 't')
                                    { ss << '\t'; i += 1; }
                                  else if(outpt_uncen[i+1] == '\\')
                                    { ss << '\\'; i += 1; }
                                }
                                else
                                ss << outpt_uncen[i];
                              }

                              if((outpt_uncen[outpt_uncen.length() - 1] != 'n' || outpt_uncen[outpt_uncen.length() - 1] != 't' || outpt_uncen[outpt_uncen.length() - 1] != '\\')
                                && outpt_uncen[outpt_uncen.length() - 2] != '\\')
                                ss << outpt_uncen[outpt_uncen.length() - 1];

                              string outpt(ss.str());
                              cout << outpt << " ";
                              }
                              cout << endl;
                            free($2);
                          }


%%

std::unique_ptr<compiler::SyntaxTree> root;
void yyerror(char const *x)
{
  printf("Error %s\n" , x);
}
