/*
**Round Robin & Dynamic Average Burst Round Robin 
**Author: Juanelv Salgado
**github: Slayz08
**10/11/18 
*/

#include <iostream>
#include <vector>
#include <queue>
#include <list>
using namespace std;

struct Proceso {
	int id, AT, BT, P, endTime, WT, RT, TAT;
	bool finished;
	bool queued;

	Proceso(int id, int AT, int BT) {
		this->id = id;
		this->AT = AT;
		this->BT = BT;
		WT = RT = TAT = 0;
		finished = false;
		queued = false;
	}

};

struct CPU {
	int time, AWT, ART, Q;
	list<Proceso*> ready;
	queue<Proceso*> readyq;
};

bool processArrived(CPU &cpu, list<Proceso*> procesos, int currentTime) {
	bool arrival = false;

	list<Proceso*>::iterator it;
	for (it = procesos.begin(); it != procesos.end(); it++) {
		if (!it._Ptr->_Myval->finished && !it._Ptr->_Myval->queued && it._Ptr->_Myval->AT <= currentTime) {
			it._Ptr->_Myval->queued = true;
			cpu.ready.push_back(it._Ptr->_Myval);
			arrival = true;
		}
		
	}

	return arrival;
}

bool done(list<Proceso*> procesos) {

	list<Proceso*>::iterator it;
	for (it = procesos.begin(); it != procesos.end(); it++) {
		if (!it._Ptr->_Myval->finished)
			return false;
	}

	return true;
}

void calculate(CPU cpu, list<Proceso*> procesos, int currentTime) {
	int AVGWT = 0, AVGRT = 0;

	list<Proceso*>::iterator it = procesos.begin();

	for (it; it != procesos.end(); it++) {
		it._Ptr->_Myval->RT = it._Ptr->_Myval->endTime - it._Ptr->_Myval->AT;
		it._Ptr->_Myval->WT = it._Ptr->_Myval->RT - it._Ptr->_Myval->BT;
		AVGWT += it._Ptr->_Myval->WT;
		AVGRT += it._Ptr->_Myval->RT;

		cout << "Proceso " << it._Ptr->_Myval->id << ":\n";
		cout << "Inicio: " << it._Ptr->_Myval->AT << "\n";
		cout << "Termino: " << it._Ptr->_Myval->endTime << endl << endl;
	}

	cout << "AVGWT: " << AVGWT / procesos.size();
	cout << "\nAVGRT: " << AVGRT / procesos.size() << endl;
	system("pause");
}

void RoundRobin(CPU &cpu, list<Proceso*> procesos, int quantum) {
	int currentTime = 0;
	list<Proceso*>::iterator it;
	list<Proceso*>::iterator itW;

	processArrived(cpu, procesos, currentTime);
	it = cpu.ready.begin();

	while (!done(procesos)) {

		it = cpu.ready.begin();

		for (int i = 1; i <= quantum; i++) {
			currentTime++;
			it._Ptr->_Myval->BT--;

			cout << "T(" << currentTime - 1 << "-" << currentTime << "): P" << it._Ptr->_Myval->id << endl;

			for (itW = cpu.ready.begin(); itW != cpu.ready.end(); itW++) {
				if (!itW._Ptr->_Myval->finished &&
					itW._Ptr->_Myval->queued &&
					itW._Ptr->_Myval != it._Ptr->_Myval) {
					itW._Ptr->_Myval->WT++;
				}
			}
			processArrived(cpu, procesos, currentTime);

			if (it._Ptr->_Myval->BT == 0)
				break;
		}

	
		if (it._Ptr->_Myval->BT == 0) {
			it._Ptr->_Myval->finished = true;
			it._Ptr->_Myval->endTime = currentTime;
			cpu.ready.pop_front();
		}
		else {
			cpu.ready.push_back(it._Ptr->_Myval);
			cpu.ready.pop_front();
		}
	}

	calculate(cpu, procesos, currentTime);

}

void sortRQ(CPU &cpu, list<Proceso*> procesos)
{

 vector<Proceso*>aux;

 copy(cpu.ready.begin(), cpu.ready.end(), back_inserter(aux));

 bool swapped = false;
	 for (int j = 0; j < aux.size() - 1; j++) {
		 swapped = false;
		 for (int i = 0; i < aux.size() - 1 - j; i++) {
			 if (aux[i + 1]->BT < aux[i]->BT)
			 {
				 Proceso* _aux = aux[i];
				 aux[i] = aux[i + 1];
				 aux[i + 1] = _aux;
				 swapped = true;
			 }
		 }

			 if (!swapped)
				 break;
	 }

	 cpu.ready.clear();
	 copy(aux.begin(), aux.end(), back_inserter(cpu.ready));
}

int calculateQuantum(CPU &cpu, list<Proceso*> procesos, int &n) {
	int TBT = 0;
	list<Proceso*>::iterator it;

	for (it = cpu.ready.begin(); it != cpu.ready.end(); it++) {
		TBT += it._Ptr->_Myval->BT;
	}

	n = cpu.ready.size();

	return TBT / cpu.ready.size();
}

void DABRR(CPU &cpu, list<Proceso*>procesos) {
	int currentTime = 0;
	list<Proceso*>::iterator it;
	list<Proceso*>::iterator itW;
	int TBT = 0;
	int quantum = 0;
	bool newProcess = false;
	int n = 0;

	processArrived(cpu, procesos, currentTime);
	it = cpu.ready.begin();

	quantum = calculateQuantum(cpu, procesos, n);
	

	//ejecutar
	while (!done(procesos)) {


		for (int i = 0; i < n; i++) {
			it = cpu.ready.begin();

			if (it._Ptr->_Myval->BT <= quantum) {
				int btaux = it._Ptr->_Myval->BT;
				for (int i = 0; i < btaux; i++) {
					currentTime++;
					it._Ptr->_Myval->BT--;
					cout << "T(" << currentTime - 1 << "-" << currentTime << "): P" << it._Ptr->_Myval->id << endl;
					}
				it._Ptr->_Myval->finished = true;
				it._Ptr->_Myval->endTime = currentTime;
				cpu.ready.pop_front();
				}
			else {
				for (int i = 0; i < quantum; i++) {
					currentTime++;
					it._Ptr->_Myval->BT--;
					cout << "T(" << currentTime - 1 << "-" << currentTime << "): P" << it._Ptr->_Myval->id << endl;

				}
					cpu.ready.push_back(it._Ptr->_Myval);
					cpu.ready.pop_front();
			}
		}
			
		if (processArrived(cpu, procesos, currentTime)) {
			sortRQ(cpu, procesos);
			quantum = calculateQuantum(cpu, procesos, n);
		}
		else if(!cpu.ready.empty())
			quantum = calculateQuantum(cpu, procesos, n);
		}
	//Author: Juanelv
	calculate(cpu, procesos, currentTime);
	}

int main() {
	CPU cpu = CPU();
	list<Proceso*>procesos;

	procesos.push_back(new Proceso(1, 11, 9));
	procesos.push_back(new Proceso(2, 17, 5));
	procesos.push_back(new Proceso(5, 9, 5));
	procesos.push_back(new Proceso(6, 15, 7));
	procesos.push_back(new Proceso(7, 2, 3));
	procesos.push_back(new Proceso(8, 0, 8));
	procesos.push_back(new Proceso(9, 0, 8));
	procesos.push_back(new Proceso(0, 3, 6));

	//RoundRobin(cpu, procesos, 2);

	DABRR(cpu, procesos);
}