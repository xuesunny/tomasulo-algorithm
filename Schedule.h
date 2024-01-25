#pragma once
#include"Instruction.h"

/*用于记录 issue、 excute、 write result和commit的时间*/
class Schedule {
public:
	int clock;  // 记录当前时钟周期
	int num;  // 指令的数目
	int time_table[15][4];  // 存储指令状态
	string inst[15];

	Schedule();
	void issue(int entry);  // 有指令需要发送
	void write_result(int entry); // 有指令写回（execute complete在写回的前一个周期）
	void commit(int entry);  // 有指令提交
	void print(ofstream& outputfile);
};