#pragma once
#include<bits/stdc++.h>
using namespace std;

//定义存储指令的数据结构

class Instruction {
public:
	string op;  // 操作类型（LD,SD,DIVID,ADDD,SUB,MULTD等）
	string dest;  // 目的寄存器
	string sources1;  // 源操作数1
	string sources2;  // 源操作数2

	Instruction(std::string s);
	void print(ofstream &outputfile);
};