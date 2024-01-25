#define _CRT_SECURE_NO_WARNINGS 1

#include "Instruction.h"

/*定义split函数*/
vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char* strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char* p = strtok(strs, d);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}

Instruction::Instruction(string s) {
    vector<string> vec;
    vec = split(s, " ");
    if (vec.size() != 4 && vec[0] != "BNEZ") {
        cout << "输入指令有误" << endl;
    }
    else {
        op = vec[0];
        dest = vec[1];
        sources1 = vec[2];
        sources2 = vec[3];
    }
}

void Instruction::print(ofstream& outputfile) {
    outputfile << op << " " << dest << " " << sources1 << " " << sources2;
}