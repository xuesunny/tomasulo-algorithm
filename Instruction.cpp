#define _CRT_SECURE_NO_WARNINGS 1

#include "Instruction.h"

/*����split����*/
vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if ("" == str) return res;
    //�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����
    char* strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char* p = strtok(strs, d);
    while (p) {
        string s = p; //�ָ�õ����ַ���ת��Ϊstring����
        res.push_back(s); //����������
        p = strtok(NULL, d);
    }

    return res;
}

Instruction::Instruction(string s) {
    vector<string> vec;
    vec = split(s, " ");
    if (vec.size() != 4 && vec[0] != "BNEZ") {
        cout << "����ָ������" << endl;
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