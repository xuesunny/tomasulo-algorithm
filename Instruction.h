#pragma once
#include<bits/stdc++.h>
using namespace std;

//����洢ָ������ݽṹ

class Instruction {
public:
	string op;  // �������ͣ�LD,SD,DIVID,ADDD,SUB,MULTD�ȣ�
	string dest;  // Ŀ�ļĴ���
	string sources1;  // Դ������1
	string sources2;  // Դ������2

	Instruction(std::string s);
	void print(ofstream &outputfile);
};