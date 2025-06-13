#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


void ColorCout(string output,int color, bool reset = true){
	// colors are: reset, red, blue, green, yellow
	const string COLORTABLE[5] = {"\033[0m","\033[91m","\033[36m","\033[32m","\033[93m"};
	 std::cout << COLORTABLE[color]; 
	 cout << output;
	if(reset){
		 std::cout << COLORTABLE[0];
	} 
}

void dispTitle(){
	cout<<" _______ |  _____  |    __   \n";
	cout<<"|__   __|| |_   _| |  / ___\\ \n";
	cout<<"   | |   |  _| |_  | | |___  \n";
	cout<<"   |_|   | |_____| |  \\____/ \n";
	cout<<"---------+---------+---------\n";
	cout<<" _______ |    _    |    __   \n";
	cout<<"|__   __||   / \\   |  / ___\\ \n";
	cout<<"   | |   |  / _ \\  | | |___  \n";
	cout<<"   |_|   | |_| |_| |  \\____/ \n";
	cout<<"---------+---------+---------\n";
	cout<<" _______ |    _    |  _____  \n";
	cout<<"|__   __||  / _ \\  | |  ___| \n";
	cout<<"   | |   | | (_) | | |  _]_  \n";
	cout<<"   |_|   |  \\___/  | |_____| \n";
}

bool legalMove(int move){
	int moves[18] = {1,2,3,4,5,6,7,8,9,11,12,13,21,22,23,31,32,33};
	for (int i=0;i<18;i++){
		if (move==moves[i]){
			return true;
		}
	}
	return false;
}

class MoveEval{
	public:
		int COW;
		int aMove;
		
		MoveEval(int a=0,int c=0){
			aMove=a;
			COW=c;
		}
		MoveEval fCOW(){
			return MoveEval(this->aMove,-(this->COW));
		}
};

class Board {
	public:
		int gr[3][3];
		int ar[9];
		int open_spaces; 
		int COW;
		
		Board(){
			for(int i=0;i<9;i++){
				ar[i]=0;
			}
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					gr[i][j]=0;
				}
			}
		
			open_spaces = 9;
			COW = 0;
		}
		
		int isWin(){
			for(int i=0;i<3;i++){
				if(gr[i][0]==gr[i][1] && gr[i][1]==gr[i][2]){
					return gr[i][0];
				}
				if(gr[0][i]==gr[1][i] && gr[1][i]==gr[2][i]){
					return gr[0][i]; 
				}
			}
			if(gr[0][0]==gr[1][1] && gr[1][1]==gr[2][2]){
				return gr[1][1];
			}
			if(gr[2][0]==gr[1][1] && gr[1][1]==gr[0][2]){
				return gr[1][1];
			}	
			for(int i=0;i<9;i++){
				if(!ar[i]){
					return 0;
				}
			}
			return 3;
		}
		
		bool playG(int set, int row, int column){
			if(gr[column][row]){
				cout << "there is already a ";
				string pieces[]={" ","X","O"};
				ColorCout(pieces[gr[column][row]],gr[column][row]);
				cout << " at (";
				cout << row+1;
				cout << ",";
				cout << column+1;
				cout << "). Please select another space.";
				return false;
			}
			
			gr[column][row] = set;
			ar[row+(3*column)] = set;
			this->open_spaces-=1;
			return true;
		}
		
		bool playA(int set, int ind){
			if(ind>9){
				cout << "tried to play in ";
				cout << ind+1;
				cout << "but that space is off the board! Please select another space.\n";
			}
			if(ar[ind]){
				cout << "tried to play in ";
				cout << ind+1;
				cout << ", but there is already an ";
				string pieces[]={" ","X","O"};
				ColorCout(pieces[ar[ind]],ar[ind]);
				cout << " there. Please select another space.\n";
				cout << "\n";
				return false;
			}
			this->ar[ind] = set;
			int row = 0;
			while (ind>2){
				ind-=3;
				row++;			
			}
			this->gr[row][ind]=set;
			this->open_spaces-=1;
			return true;
		;
		}
		
		void dispBoard(bool do_numbers=true){
			cout << "\n";
			const string SYMBOLS[3][4] = {{"         ","         ","    ","         "},{" __   __ "," \\ \\_/ / ","  > _ <  "," /_/ \\_\\ "},{"   ___   "," .' _ '. "," | (_) | "," '-___-' "}};
			for(int c=0;c<3;c++){
				if(c){
					ColorCout("---------+---------+---------\n",3);
				}
				for(int l=0;l<4;l++){
					for (int r=0;r<3;r++){
						if(r){
							ColorCout("|",3);
						}
						if(l==2 && this->gr[c][r]==0){
							cout << "    ";
							if (do_numbers){
								cout << (1+(r)+(3*c));
							}
							else{
								cout<<" ";
							}
						}
						ColorCout(SYMBOLS[(this->gr[c][r])][l],gr[c][r]);
					}
					cout << "\n";
				}
			}
		}
		
		~Board(){}
};

MoveEval minMax(Board the_board,int player,int move,int depth){
	int OPP = (player%2)+1;
	the_board.playA(player,move);
	string colors[3] = {"\033[0m","\033[91m","\033[36m"};
	int win_state=the_board.isWin();
	if(depth<=0){
		cout<<"<?>";
		return MoveEval(move,0);
	}
	if (win_state){
		int table[2][3] = {{-1,1,0},{1,-1,0}};
		//cout<<colors[win_state%3]<<"<"<<win_state%3<<">"<<colors[0];
		//cout<<"<"<<win_state%3<<">"<<colors[player]<<")"<<colors[0];
		string prediction = "WTL";
		win_state= table[player-1][win_state-1];
		cout<<"<"<<prediction[win_state+1]<<">"<<colors[player]<<")"<<colors[0];
		//cout<<"<"<<win_state<<">"<<colors[0]<<")";
		return MoveEval(move,win_state);
	}
	int sFlag=true;
	MoveEval best_move;

	for(int m=0;m<9;m++){
		//cout<<m;
		if(!the_board.ar[m]){
			if(!sFlag){
				cout<<",";
			}
			cout<< colors[OPP] << m << "("<<colors[0];
			MoveEval test_move = minMax(the_board,OPP,m,depth-1).fCOW();
			//test_move.aMove=m;
			if(sFlag){
				best_move = test_move;
				sFlag = false;
			}
			else if (test_move.COW>best_move.COW){
				if(test_move.COW==1){
					cout<< colors[player] << ")" << colors[0];
					return test_move;
				}
				best_move = test_move;
				
			}
		}
	}
	cout<< colors[player]<<")"<<colors[0];
	return best_move;
}

int initMinMax(Board the_board, int player, int max_depth){
	
	
	
	
	bool sFlag=true;
	MoveEval best_move;
	string colors[3] = {"\033[0m","\033[91m","\033[36m"};
	for(int m=0;m<9;m++){
		
		
		if(!the_board.ar[m]){
			
			cout<< colors[player]<< m <<"["<<colors[0];
			MoveEval test_move = minMax(the_board,player,m,max_depth-1).fCOW();
			test_move.aMove=m;
			string prediction = "LTW";
			cout<<"]{"<<prediction[test_move.COW+1]<<"}\n";
			if(test_move.COW==1){
				return m;
			}
			if(sFlag){
				best_move = test_move;
				sFlag = false;
			}
			else{
				
				// the second condition for this if makes it so it can randomly pick a move if they have equal COW values. This makes minmax indeterministic, adding a bit of flavor to the bot.
				// this is the most interesting in the case of the hard bot; due to it's depth limiter, it can actually make mistakes during the late game that can allow the player to win given the right circumstances 
				if (test_move.COW>best_move.COW || ( test_move.COW>=best_move.COW && std::rand()%2 )){
					best_move = test_move;
				}
				
			}
		}
	}
	if(sFlag){
		cout<<"SOMETHING WENT WRONG; SELECTING MINIMUM MOVE";
		for(int m=0;m<9;m++){
			if (!the_board.ar[m]){
				return m;
			}
		}
	}
	// because of the nature of this minmax algorithm, it assumes that the computer will lose if it is possible for the player to win for certain given the player plays perfectly, in which case it plays in the first availible spot
	// the issue with this is that the player may be in such a situation without even realizing it, in which case the computer could still win if the player then makes a mistake, but it often just gives up, leaving an easy win for the player
	// this last bit is to prevent that by checking to block any immediately availible 3-in-a-rows the player might have.
	if(best_move.COW==-1){
		for(int i=0;i<9;i++){
			if(!the_board.ar[i]){
				Board i_board=the_board;
				i_board.playA((2%player)+1,i);
				if(i_board.isWin()){
					cout<<"*gulp*";
					return i;
				}
				
			}
		}
	}
	cout<<"\n";
	return best_move.aMove;
}

class Player {
	public:
		bool iscomp;
		int id;
		int difficulty;
		void initPlayer(bool c,int x,int d){
			iscomp = c;
			id = x;
			difficulty = d;
			
		}
		Board yourTurn(Board the_board){
			cout<<"\n\n";
			if(this->iscomp){
				cout<<"Computer turn!\nThe computer is making a selection...\n"<<id<<"\n";
				if(((the_board.open_spaces-1)/2)>difficulty){
					while(!the_board.playA(this->id,(std::rand()%9))){}	
				}
				else{
					int tot=0;
					for(int i=0;i<9;i++){ tot+=the_board.ar[i]; }
					if(difficulty==3 && tot==the_board.ar[4]){
						// in this condition, minmax takes a long time and always plays in the corner, so it makes more sense to just have it play there
						while(!the_board.playA(this->id,2*(std::rand()%5))){}
					}
					else{
						the_board.playA(this->id,initMinMax(the_board,this->id,difficulty*3));
					}
				}	
			}
			else{
				the_board.dispBoard();
				cout<<"Your turn! select where to place ";
				string pieces[] = {" ","X","O"};
				ColorCout(pieces[id],id);
				cout<< "\n";
				int choice;
				while (true){
					cin >> choice;
					if(!legalMove(choice)){
						cout<<"Not a legal move! Please enter a number 1-9.\n";
						continue;
					}
					
					if (choice>9){
						int c=0;
						int r=0;
						while(choice>9){
							choice-=10;
							r++;
						}
						while(choice>0){
							choice--;
							c++;
						}
						if (the_board.playG(this->id,r-1,c-1)){
							break;
						}
					}
					else if(the_board.playA(this->id,choice-1)){
						break;
					}
				}
				the_board.dispBoard();	
			}

		return the_board;
	}
};

class TicTacToe{
	public:
		Player players[2];
		Board the_board;
		int turn=0;
		
		TicTacToe(){
			for(int i=0;i<2;i++){
				players[i]=Player();
			}
			the_board=Board();
		}
		
		void initializeGame(int diff){
			int prand = std::rand() % 2;
			int srand = std::rand() % 2;
			for(int i=0;i<2;i++){
				players[i].initPlayer(prand==i,(srand+i)%2+1,diff);
			}
		}
		
		void winner(){
			cout<<"\n\n";
			int the_winner=the_board.isWin();
			bool didwin;
			if(the_winner==3){
				cout<<"It's a draw.";
			}
			else{
				if(players[0].id==the_winner){
					didwin = !players[0].iscomp;
				}
				else{
					didwin = players[0].iscomp;
				}
				if(didwin){
					cout<<"You won!";
				}
				else{
					cout<<"You lost...";
				}
			}
			the_board.dispBoard(false);
			the_board.~Board();
			
		}
		
		void goGame(){
			while(!the_board.isWin()){
				the_board = players[turn].yourTurn(the_board);
				turn=(turn+1)%2;
			}
			
		}
		~TicTacToe(){}
		
};

bool stringComp(string string1, string string2, bool do_case){
	
	if (string1.length()!=string2.length()){
		return false;
	}
	if (do_case){
		for(int i=0;i<string1.length();i++){
			if(string1[i]!=string2[i]){
				return false;
			}
		}
	}
	else{
		for(int i=0;i<string1.length();i++){
			if(tolower(string1[i])!=tolower(string2[i])){
				return false;
			}
		}
	}
	return true;
}

bool ofResp(string inp, string isof[], int num_check, bool do_case){
	//ofResp() is used to check if a given user input "inp" matches one of set of expected inputs in "isof", i.e. you might check if response is one of {"yes", "y", "no", "n"}
	//the comparisons are done in order of "isof" so make sure to order it by highest expected input for maximum performance
		
	
	for(int i=0;i<num_check;i++){
		if(stringComp(inp,isof[i],do_case)){
			return true;
		}
	}
	return false;
}

int main(){
	std::srand(std::time(0));
	dispTitle();
	int difficulty=0;
	while(true){
	
		cout << "\nPlease select a difficulty:\n";
		cout << "1 - ";
		ColorCout("Easy\n",3);
		cout << "2 - ";
		ColorCout("Hard\n",4);
		cout << "3 - ";
		ColorCout("Impossible\n",1);
		cout << "0 - Exit\n";
	
		while(true){
			cin>>difficulty;
			if(difficulty>=0 && difficulty<=3){
				break;
			}
			cout<<"\nplease enter a value 1-3:\n";
		}
		if(!difficulty){
			break;
		}
	
		while (true){
			TicTacToe the_game;
			//Board the_board;
			the_game.initializeGame(difficulty);
			the_game.goGame();
			the_game.winner();
			int inp;
			cout << "Would you like to play again?\n";
			cout << "1 - play again\n";
			cout << "0 - change difficulty / exit\n";
			cin >> inp;
			string vals[] = {"n","no","quit","exit"};
			if(!inp){
				break;
			} 
			the_game.~TicTacToe();
		}
	}
	return 0;
}