/*
	Name: 		Michael R. Reynoso
	Date: 		August 2nd, 2026
	Purpose: 	Pig Dice Game - Project 2 Version 4 - Linear Search Added
				(8th version of Pig Dice Game)
*/

//System Libraries
#include <iostream> //Input Output Library
#include <cstdlib>  //Random Controls
#include <ctime>    //System tracking clock
#include <string>   //String Library
#include <fstream>  //File Stream Library
#include <iomanip>  //Format Library
#include <cmath>    //Math LIbrary
#include <vector>   //Vector Library

using namespace std;

//User Libraries

//Global Constants -> Science, Math, Conversions,
//	Higher Dimensions.

//Function Prototypes
void prntBrd(int = 29);                  //Generates horizontal border using character limits
int  rollDie();                          //Calculates random single six-sided dice numerical result
void compTrn(int &, int, bool &);        //Executes rule-based automated turn processing engine
void logTurn(int [][10], int, int, int); //Populates the 2D tracking matrix with round scoring histories 
void srtVec(vector<int> &);				 //Sorts the historical leaderboard vector from lowest to highest
int  srchVal(const int [], int, int);	 //Scans the 1D roll history array to find matching target values

//Program Execution Begins Here
int main(int argv,char **argc){

	//Declare Variables
	int      pScore,     //Player grand match total score
		     cScore,     //Computer grand match total score
		     turnTot,    //Temporary single-turn score
		     die,        //Value for the rolled die (1-6)
		     i,          //Loop iterator variable for "for_loop"
			 oldCscr;    //Old Score
			 
		
	char     choice;     //Player turn choice selection
	
	bool     pTurn,      //Flag for player turn loop
		     cTurn;      //Flag for computer turn loop
	
	string   pName,      //Holds player name string
			 msg;		 //Game Outcome Message
	
	float    pPct,       //Player scoring effiency percentage
		     cPct;       //Computer scoring efficiency percentage
		   
	ifstream fin;        //Input file stream
	ofstream fout;       //Output file stream
	
	//Array/Vector Declarations
	int pRolls[50];      //1D Array tracking human dice values
	int pCount;          //Tracks index counter for human rolls
	int rndStat[2][10];  //2D Array tracking row 0 (player) and row 1 (computer) turn scores
	int turnIdx;         //Tracks turn counter column for 2D array
	vector<int> hScores; //STL Vector tracking total match score logs
	
	//Search Variables
	int target;          //Search value target
	int srchIdx;         //Result index location matching target
	
	//Seed Random number engine with system clock
	srand(time(0));

	//Initialize Variables
	pScore  = 0;
	cScore  = 0;
	pCount  = 0;
	turnIdx = 0;
	oldCscr = 0;
	
	//Explicitly clear/initialize our 2D statistics array table
	for(int r=0; r<2; r++){
		for(int c=0; c<10; c++){
			rndStat[r][c] = 0;
		}
	}
	
	//Optional Startup
	fin.open("high_in.txt");
	if(fin){
		int tempScr;
		//Populate vector container with baseline data if present
		while(fin >> tempScr) {
			hScores.push_back(tempScr); //Actively uses STL vector API
		}
		fin.close();
	}
	
	//Entry Design Loop
	cout<<endl<<endl<<endl;
	prntBrd(); //Uses default value of 29 internally
	
	cout<<" Welcome to my PIG dice Game"<<endl;
	cout<<" Enter your player name"<<endl<<endl;
	cin>>pName;
	
	//Outer Loop
	while (pScore<100 && cScore<100){
		
		//Tracking variales at round startup
		turnTot = 0;
		pTurn = true;
		
		//Player Active Turn Cycle
		do{
			cout<<endl<<pName<<"'s Current Match Score: "<<pScore<<endl;
			cout<<"Current Turn Accumulation: "<<turnTot<<endl;
			cout<<"Roll (y) or Hold (h)? ";
			cin>>choice;
			
			if(choice=='y' || choice=='Y'){
				die = rollDie();
				cout<<" -> You rolled a: "<<die<<endl;
				
				//Log to 1D Array if space permits
				if(pCount<50){
					pRolls[pCount] = die;
					pCount++;
				}
				
				if (die == 1){
					cout<<"Bust! Your rolled a 1. You've lost all points this turn."<<endl<<endl;
					turnTot = 0;
					pTurn = false;
				}else{
					turnTot += die;
				}
			}
			else if(choice == 'h' || choice == 'H'){
				pScore += turnTot;
				cout<<"Points saved! Your current score as it stands: "<<pScore<<endl;
				pTurn = false;
			}
			else{
				cout<<"Invalid selection! you must enter 'y' or 'h'."<<endl;
			}
		}while (pTurn == true && pScore<100);
		
		//Log Player Turn Score to 2D Array
		if(turnIdx<10){
			logTurn(rndStat, 0, turnIdx, turnTot);
		}
		
		//Computer active turn cycle
		if (pScore<100){
			oldCscr = cScore; //Sync tracker base score
			compTrn(cScore, pScore, cTurn);
			
			//Log Computer Turn Score to 2D Array
			if(turnIdx<10){
				logTurn(rndStat, 1, turnIdx, (cScore - oldCscr));
			}
			turnIdx++;
		}
	}
	
	//Display Results
	cout<<endl<<"--- Game Over!!! --- We have a winner!!! ---"<<endl;
	
	//Conditional Operator - Match Winner Declaration
	msg = (pScore >=100)? "VICTORY!! You won the race to 100!!" : "DEFEAT!! The Computer wins the race to 100!!";
	cout<<endl<<"Result: "<<msg<<endl<<endl;
	
	//Track the winning score inside our high score history vector
	if(pScore>=100){
		hScores.push_back(pScore);
	}else{
		hScores.push_back(cScore);
	}
	
	//Calculate Performance Stats
	pPct = (static_cast<float>(pScore) / (pScore+cScore))*100.0f;
	cPct = (static_cast<float>(cScore) / (pScore+cScore))*100.0f;
	
	//Performance Table Display
	cout<<fixed<<setprecision(1);
	cout<<setw(15)<<left<<"PARTICPANT"
	<<setw(12)<<right<<"SCORE"<<setw(12)<<right<<"SHARE %"<<endl;
	cout<<"----------------------------------------"<<endl;
	cout<<setw(15)<<left<<pName<<setw(12)<<right<<pScore
	<<setw(11)<<right<<pPct<<"%"<<endl;
	cout<<setw(15)<<left<<"Computer"<<setw(12)<<right<<cScore<<setw(11)<<right<<cPct<<"%"<<endl;
	cout<<"----------------------------------------"<<endl<<endl;
	
	//Display Historical Round Performance Logs From our 2D Array
	cout<<"Historical Round Point Breakdown (Max 5 Turns Visualized):"<<endl;
	
	// Row Labels set to left-aligned width 16, Data columns set to right-aligned width 6
	cout<<setw(16)<<left<<"Turn Index:"
		<<setw(6)<<right<<"1"<<setw(6)<<right<<"2"<<setw(6)<<right<<"3"
		<<setw(6)<<right<<"4"<<setw(6)<<right<<"5"<<endl;
	cout<<setw(16)<<left<<"Human Score:"
		<<setw(6)<<right<<rndStat[0][0]<<setw(6)<<right<<rndStat[0][1]
		<<setw(6)<<right<<rndStat[0][2]<<setw(6)<<right<<rndStat[0][3]
		<<setw(6)<<right<<rndStat[0][4]<<endl;
	cout<<setw(16)<<left<<"Computer Score:"
		<<setw(6)<<right<<rndStat[1][0]<<setw(6)<<right<<rndStat[1][1]
		<<setw(6)<<right<<rndStat[1][2]<<setw(6)<<right<<rndStat[1][3]
		<<setw(6)<<right<<rndStat[1][4]<<endl<<endl;
		
	//Sort Execution
	cout<<"Sorting Match Leaderboard Records via Selection Sort..."<<endl;
	srtVec(hScores);
	cout<<"Historical Ranked Scores Log: ";
	for(size_t s=0; s<hScores.size(); s++){
		cout<<hScores[s]<<" ";
	}
	cout<<endl<<endl;

	//Linear Search Execution
	cout<<"Verify Dice Integrity Algorithm. Enter roll value to search for (1-6): ";
	cin>>target;
	srchIdx = srchVal(pRolls, pCount, target);
	if(srchIdx != -1) {
		cout<<"Target value "<<target
		<<" found in your match history at roll index #"<<srchIdx<<"!"<<endl;
	}else{
		cout<<"The value "<<target<<" was never rolled during this match sequence."<<endl;
	}
	cout<<endl;
	
	//Switch Statement evaluating the final die value of the game
	cout<<"How did the game Finish? ";
	switch(die){
		case 1:  cout<<"The roll of a 1 concludes the game!!"<<endl; break;
		case 2:  cout<<"All it took was a final roll of a 2 to win it!!"<<endl;break;
		case 3:  cout<<"3 was the roll that ends the game!!"<<endl; break;
		case 4:  cout<<"4 is the magic number to finish the Game!!"<<endl;break;
		case 5:  cout<<"A big 5 points to end it!!"<<endl; break;
		case 6:  cout<<"Way to go out on a high by rolling a 6 to finish the Game!"<<endl; break;
		default: cout<<"The dice rest."<<endl;
	}
	cout<<endl;
	
	//Save game logs to Document
	fout.open("scores.txt");
	if(fout){
			fout<<"--- Match Record ---"<<endl;
			fout<<pName<<": "<<pScore<<" pts ("<<pPct<<"%)"<<endl;
			fout<<"Computer: "<<cScore<<" pts ("<<cPct<<"%)"<<endl<<endl;
			fout<<"--- Sorted Match History Archive ---"<<endl;
			for(size_t s = 0; s < hScores.size(); s++){
				fout<<"Match Log:"<<hScores[s]<<" pts"<<endl;
			}
			fout.close();
			cout<<"Match data recorded successfully to scores.txt"<<endl;
	}
		
	//Exit stage right
	return 0;
}


//==========================Functions============================


//Generates horizontal border using character limits
void prntBrd(int size){
	for (int i=0; i<size; i++){
		cout<<"*";
	}
	cout<<endl;
}

//Calculates random single six-sided dice numerical result
int rollDie(){
	return (rand() %6+1);
}

//Executes rule-based automated turn processing engine
void compTrn(int &cScr, int pScr, bool &cTrn) {
	cout<<endl<<"--- Computer's Turn ---"<<endl<<endl;
	int turnTot = 0;
	int die = 0;
	cTrn = true;
	
	// Computer processing matching pure algorithm rules
	while(cTrn == true && turnTot<20 && (cScr+turnTot)<100){
		die = rollDie();
		cout<<" -> Computer rolled a "<<die<<endl<<endl;
		
		if(die == 1){
			cout<<"Computer Busts! Rolled a 1 and has lost all its points this round!"<<endl<<endl;
			turnTot = 0;
			cTrn = false; 
		}else{
			turnTot += die;
			cout<<"     Computer points accumulated this turn: "<<turnTot<<endl;
		}
	}
	
	if(turnTot>0){
		cScr += turnTot;
		cout<<"Computer Holds! Score accumulated this turn: "<<turnTot<<endl;
	}
	cout<<"Computer Total Match Score: "<<cScr<<endl<<endl;
}

//Passing 2D Multi-Dimensional Arrays to Functions
void logTurn(int stats[][10], int row, int col, int val) {
	stats[row][col] = val;
}

//Selection Sort Array/Vector Algorithm
void srtVec(vector<int> &vec){
	int n = vec.size();
	for (int i=0; i<n-1; i++){
		int minIdx = i;
		for (int j= i+1; j<n; j++){
			if (vec[j] < vec[minIdx]){
				minIdx = j;
			}
		}
		int temp = vec[minIdx];
		vec[minIdx] = vec[i];
		vec[i] = temp;
	}
}

//Linear Search Array Algorithm
int srchVal(const int arr[], int size, int target){
	//Loop through every valid populated index element in the 1D array
	for (int i=0; i<size; i++){
		// Check if the current array element matches our search target
		if (arr[i] == target){
			return i; //Target located! Instantly return the index position
		}
	}
	return -1; //Scanned the entire array and found no matching values
}