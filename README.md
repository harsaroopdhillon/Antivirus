# Antivirus
A little project I made while I was in high school. (A silly attempt at making an antivirus for that age)
It utilizes the unicorn engine to emualte files in an environment. it has its own signature format and drivers for the file system etc.
The code is very old so some portions may require updating. The project is not "complete". There is a lot of code that goes into a >functioning antivirus
The GUI was created using QT
![image](https://github.com/harsaroopdhillon/Antivirus/assets/115299272/3e9949a5-8e4b-42da-a817-d6aea7926c29)

![image](https://github.com/harsaroopdhillon/Antivirus/assets/115299272/86a14981-bae3-4e3d-b040-39a07822e3fe)
![image](https://github.com/harsaroopdhillon/Antivirus/assets/115299272/8bd95498-17aa-4ec3-87d4-056dac4b35c6)
![image](https://github.com/harsaroopdhillon/Antivirus/assets/115299272/19db4dd6-60c8-4b3e-a7af-e83b9c087174)
![image](https://github.com/harsaroopdhillon/Antivirus/assets/115299272/06f56efb-ddf9-4f28-8e98-9333a56bb212)

an example of signatures format can be seen below. I believe they are self explanatory 

``` Signature Banker : 12312312312
{
    info:
    	description= "this is blah blah...";
    	level=5;
    types:
        $b = /md5: [0-9a-zA-Z]{32}/;
    check:
        $b;
}


Signature Banker : 1235
{
   info:
    	description= "this is blah blah...";
    	level=5;
    types:
      $a = pe.call("OutputDebugString('O')");
      $b = pe.call("OutputDebugString('O')", "MessageBox(0, 'A', 'M', NULL)");
    check:
      $a;
      $b;
}

Signature Banker : 12312312312
{
    info:
    	description= "this is blah blah...";
    	level=5;
    types:
        $b = "something";
    check:
        $b at 0x2;
}

Signature Banker : 12312312312
{
    info:
    	description= "this is blah blah...";
    	level=5;
    /*
      This is a comment
    */
    // this is another comment
    types:
        $b = "something";
    check:
        ($b * 5);
}

Signature Banker : 12312312312
{
    info:
    	description= "this is blah blah...";
    	level=5;
    types:
        $a = filename = = "something.exe";
    check:
        $a;
}
