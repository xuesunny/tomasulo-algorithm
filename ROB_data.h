#pragma once
#include<bits/stdc++.h>
#include"Instruction.h"
using namespace std;

// ����Reorder buffer�洢���ݵĽṹ

class ROB_data {
public:
	int entry;
	bool busy;
	string inst;  // �洢ָ�� eg LD F6��32(R2)
	int state;  // ״̬��1��ʾissue,2��ʾexecution,3��ʾwrite back,4��ʾcommit
	string destination;  // Ŀ�ļĴ���
	string value;  // ���

	/*�������¼�Ĵ����±�*/
	int source_reg1;
	int source_reg2;
	int dest_reg;

	ROB_data();
	ROB_data(Instruction instruction,int entry);
	void print(ofstream &outputfile);  // �����������
};