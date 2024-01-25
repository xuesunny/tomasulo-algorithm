#include"RS_data_load.h"

RS_data_load::RS_data_load() {
	name = "";
	busy = 0;
	op = vj = vk = "";
	Qj = Qk = 0;
	Dest = -1;
	A = "";
}

void RS_data_load::print(ofstream& outputfile) {
	outputfile << name << " : ";
	if (busy) {
		outputfile << "yes,";
	}
	else {
		outputfile << "No,";
	}
	outputfile <<op<<","<< vj << "," << vk << ",";
	if (Qj == 0) {
		outputfile << ",";
	}
	else {
		outputfile << "#" << Qj << ",";
	}
	if (Qk == 0) {
		outputfile << ",";
	}
	else {
		outputfile << "#" << Qk << ",";
	}
	if (Dest == -1) {
		outputfile << ",";
	}
	else {
		outputfile << "#" << Dest << ",";
	}
	outputfile << A << ";";
	outputfile << endl;
}