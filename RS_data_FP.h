#pragma once
#include"ROB_data.h"

// 定义存在Reservation Status add和mult单元里面的数据

class RS_data_FP {
public:
	string name;
	bool busy;
	string op;
	string vj;
	string vk;
	int Qj;
	int Qk;
	int Dest;
	string A;
	RS_data_FP();
	void print(ofstream& outputfile);
};

