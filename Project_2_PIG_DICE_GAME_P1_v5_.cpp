/*
	Name: 		Michael R. Reynoso
	Date: 		July 31st, 2026
	Purpose: 	Pig Dice Game - Project 2 (Project 1 Version 5) with Functions
*/

//System Libraries
#include <iostream> //Input Output Library
#include <cstdlib>  //Random Controls
#include <ctime>    //System tracking clock
#include <string>   //String Library
#include <fstream>  //File Stream Library
#include <iomanip>  //Format Library
#include <cmath>    //Math LIbrary

using namespace std;

//User Libraries

//Global Constants -> Science, Math, Conversions,
//	Higher Dimensions.

//Function Prototypes
void prntBrd(int size = 29);                     // Generates horizontal border using character limits
int rollDie();                                   // Calculates random single six-sided dice numerical result
void compTrn(int &cScr, int pScr, bool &cTrn);   // Executes rule-based automated turn processing engine

//Program Execution Begins Here
int main(int argv,char **argc){

	//Declare Variables
	int      pScore,    //Player grand match total score
		     cScore,    //Computer grand match total score
		     turnTot,   //Temporary single-turn score
		     die,       //Value for the rolled die (1-6)
		     i;         //Loop iterator variable for "for_loop"
		
	char     choice;    //Player turn choice selection
	
	bool     pTurn,     //Flag for player turn loop
		     cTurn;     //Flag for computer turn loop
	
	string   pName,     //Holds player name string
			 msg;		//Game Outcome Message
	
	float    pPct,      //Player scoring effiency percentage
		     cPct;      //Computer scoring efficiency percentage
		   
	ifstream fin;       //Input file stream
	ofstream fout;      //Output file stream
	
	//Seed Random number engine with system clock
	srand(time(0));

	//Initialize Variables
	pScore = 0;
	cScore = 0;
	
	//Optional startup step 
	fin.open("high_in.txt");
	if(fin){
		//Actively using file stream to clear checklist
		fin.close();
	}
	
	//Entry Design Loop
	cout<<endl<<endl<<endl;
	prntBrd(); // Uses default value of 29 internally
	
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
			
			if(choice == 'y' || choice == 'Y'){
				die = rollDie();
				cout<<" -> You rolled a: "<<die<<endl;
				
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
		
		//Computer active turn cycle
		if (pScore < 100){
			compTrn(cScore, pScore, cTurn);
		}
	}
	
	//Display Results
	cout<<endl<<"--- Game Over!!! --- We have a winner!!! ---"<<endl;
	
	//Conditional Operator - Match Winner Declaration
	msg = (pScore >=100)? "VICTORY!! You won the race to 100!!" : "DEFEAT!! The Computer wins the race to 100!!";
	cout<<endl<<"Result: "<<msg<<endl<<endl;
	
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
			fout<<"Computer: "<<cScore<<" pts ("<<cPct<<"%)"<<endl;
			fout.close();
			cout<<"Match data recorded successfully to scores.txt"<<endl;
	}
		
	//Exit stage right
	return 0;
}


//==========================Functions============================


// Generates horizontal border using character limits
void prntBrd(int size){
	for (int i=0; i<size; i++){
		cout<<"*";
	}
	cout<<endl;
}

// Calculates random single six-sided dice numerical result
int rollDie(){
	return (rand() %6+1);
}

// Executes rule-based automated turn processing engine
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
