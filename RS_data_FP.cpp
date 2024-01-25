#include"RS_data_FP.h"

RS_data_FP::RS_data_FP() {
	name = "";
	busy = 0;
	op = vj = vk = "";
	Qj = Qk = 0;
	Dest = -1;
}

void RS_data_FP::print(ofstream& outputfile) {
	outputfile << name << " : ";
	if (busy) {
		outputfile << "yes,";
	}
	else {
		outputfile << "No,";
	}
	outputfile << op << "," << vj << "," << vk << ",";
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