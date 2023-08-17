#include <iostream>
#include <thread>
#include <ctime>

#define _UP true
#define _DOWN false

class floor{
	int floornum;
	bool up;
	bool down;
	bool halt;
	public:
	floor(int floor){
		floornum = floor;
		up = false;
		down = false;
		halt = false;
	}
	void setrequp(bool k){ up = k; }
	void setreqdown(bool k){ down = k; }
	void sethalt(bool k){ halt = k; }
	bool pullreqhalt(){ return halt; }
	bool pullrequp(){ return up; }
	bool pullreqdown(){ return down; }
	int checkfloor(){ return floornum; }
};

class lift{
	int currpos;
	int dest;
	bool dir; //true == up; false == down;
	bool ismoving;
	clock_t startTime;
	
	public:
	lift(){
		currpos = 0;
		ismoving = false;
		startTime = clock();
	}
	void setdest(int k){
		dest = k;
	}
	bool getdir(){ return dir; }
	void setdir(bool d){ dir = d; }
	bool getstatus(){ return ismoving; }
	int getpos(){ return currpos; }
	void start(floor *f){
		clock_t stime = clock();
		for(;;){
			if(!ismoving){
				for(int i = 0; i < 6; i++){
					if(f[i].pullrequp() || f[i].pullreqdown()){
						dest = i;
						f[i].sethalt(true);
						setdir((dest - currpos) > 0);
						ismoving = true;
					}
				}
			}
			if(ismoving){
				if(f[currpos].pullrequp() || dir){
					ismoving = false;
					int d;
					std::cout << "Reached floor " << currpos << std::endl;
					std::cout << "enter destination" << std::endl;
					std::cin >> d;
					if(d > dest){ dest = d; }
					f[d].sethalt(true);
					f[currpos].setrequp(false);
					ismoving = true;
				}
				else if(f[currpos].pullreqdown() || !dir){
					ismoving = false;
					int d;
					std::cout << "Reached floor " << currpos << std::endl;
					std::cout << "enter destination" << std::endl;
					std::cin >> d;
					if(d < dest){ dest = d; }
					f[d].sethalt(true);
					f[currpos].setreqdown(false);
					ismoving = true;
				}
				else if(f[currpos].pullreqhalt()){
					if(currpos == dest){ ismoving = false; continue;}
					else{
						std::cout << "reached floor " << currpos << std::endl;
					}
				}
			}
			if((stime - clock()) > 20){
				if(ismoving && dir){
					currpos += 1;
				}
				else if(ismoving && !dir){
					currpos -= 1;
				}
				stime = clock();
			}
		}
	}
};

int main(){
	lift mlift;
	floor f[6]{ 0,1,2,3,4,5 };
	std::cout << "INITIALIZING LIFT SYSTEMS....." << std::endl;
	std::cout << "Lift system is active" << std::endl;
	std::thread runlift(&lift::start, &mlift, f);
	for(;;){
		int inp;
		int command;
		std::cout << "Select the floor number" << std::endl;
		std::cout << "0 1 2 3 4 5" << std::endl;
		std::cin >> inp;
		std::cout << "processing..." << std::endl;
		std::cout << "where do u want to go up = 1 or down = 0" << std::endl;
		std::cin >> command;
		if(command == 1 && inp != 6){ f[inp].setrequp(true); }
		else if (command == 0 && inp != 0){ f[inp].setreqdown(true); }
		else{ std::cout << "Invalid command" << std::endl; continue; }
	}
	return 0;
}
