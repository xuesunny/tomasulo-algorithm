#pragma once
#include"ROB_data.h"

// �������Reservation Status add��mult��Ԫ���������

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

