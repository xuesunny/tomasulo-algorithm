#pragma once
#include<bits/stdc++.h>
#include"Instruction.h"
using namespace std;

// 定义Reorder buffer存储数据的结构

class ROB_data {
public:
	int entry;
	bool busy;
	string inst;  // 存储指令 eg LD F6，32(R2)
	int state;  // 状态：1表示issue,2表示execution,3表示write back,4表示commit
	string destination;  // 目的寄存器
	string value;  // 结果

	/*额外项，记录寄存器下标*/
	int source_reg1;
	int source_reg2;
	int dest_reg;

	ROB_data();
	ROB_data(Instruction instruction,int entry);
	void print(ofstream &outputfile);  // 输出该数据项
};