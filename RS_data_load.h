#pragma once
#include"ROB_data.h"

// �������Reservation Status load��Ԫ���������

class RS_data_load {
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
	RS_data_load();
	void print(ofstream& outputfile);
};
