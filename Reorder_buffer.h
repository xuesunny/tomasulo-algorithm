#pragma once
#include"ROB_data.h"
#include"Schedule.h"

class Reorder_buffer {
public:
	int* reorder;  // ָ���㷨��reorder���� 
	bool* busy;  // ָ���㷨��busy����
	Schedule* schedule;  // ���ڹ���schedule��

	ROB_data table[15];
	int commited;  // ָ�����ύ�������һ��
	int back;  // ָ��reorder buffer�����һ��Ԫ��

	Reorder_buffer();
	bool check_update();  // ����Ƿ���Ҫ���±�
	bool cycle();  // ÿ�����ڼ��һ���Ƿ��н����Ҫ�ύ,���صĲ���ֵ���ڹ۲��Ƿ���״̬����
	void issue(Instruction instruction);  // ����ָ��

	void print(ofstream& outputfile,int num);  // ��������Ϣ
};