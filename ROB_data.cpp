#include"ROB_data.h"

static int get_number(string s) { // ��ȡ�ַ����е�����
	string num = "";
	int i = 0;
	while (i<s.length() && !isdigit(s[i])) i++;
	while (i < s.length() && isdigit(s[i])) {
		num += s[i];
		i++;
	}
	return stoi(num);  // תΪ����
}

ROB_data::ROB_data()
{
	entry = -1;
	busy = 0;
	inst = "";
	state = 0;
	destination = "";
	value = "";

	source_reg1 = 0;
	source_reg2 = 0;
	dest_reg = 0;
}

ROB_data::ROB_data(Instruction instruction,int entry) {
	this->entry = entry;
	busy = 0;
	inst += instruction.op;
	inst += " ";
	// ����inst
	if (instruction.op == "LD" || instruction.op == "SD") {
		inst += instruction.dest;
		inst += " ";
		inst += to_string(get_number(instruction.sources1));
		inst += "(" + instruction.sources2 + ")";
		source_reg1 = source_reg2 = -1;  // ��ʹ��Դ�Ĵ���
	}
	else {
		inst += instruction.dest + "," + instruction.sources1 + "," + instruction.sources2;
		//��¼Դ�Ĵ������
		source_reg1 = get_number(instruction.sources1);
		source_reg2 = get_number(instruction.sources2);
	}
	state = 1;
	destination = instruction.dest;
	value = "";
	//��¼Ŀ�ļĴ������
	dest_reg = get_number(destination);
}

void ROB_data::print(ofstream& outputfile) {
	outputfile << "entry" << entry<<" : ";
	if (busy) {
		outputfile << "yes,";
	}
	else {
		outputfile << "No ,";
	}
	outputfile << inst << ",";
	switch (state) {
	case 0:
		outputfile << ",";
		break;
	case 1:
		outputfile << "Issue,";
		break;
	case 2:
		outputfile << "Execute,";
		break;
	case 3:
		outputfile << "Write result,";
		break;
	case 4:
		outputfile << "Commit,";
		break;
	}
	outputfile << destination<<",";
	outputfile << value << ";";
	outputfile << endl;
}

