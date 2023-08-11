//
//  main.cpp
//
//  Name: Harsh Dineshkumar Patel
//  Student Number: 200490101
//

#include <cassert>
#include <string>
#include <iostream>

#include "PlaceString.h"
#include "Game.h"

using namespace std;



int main ()
{
	cout << "Welcome to my Go game!" << endl;
	string name;
	while(name == "")
	{
		cout << "Enter your name:  ";
		getline(cin, name);
	}
	cout << "Hello " << name << ".  You will play black." << endl;
	cout << endl;

  while(true){
  
    int Board_Size;
        cout << " Enter your Board Size"<<endl;
        cin >> Board_Size;
        cin.ignore(256,'\n');
       if(Board_Size <= 0){
         cout <<" your Board Size is smaller than 0"<<endl;
       } 
       else if(Board_Size >= 24){
         cout<<" your Board Size is greater than 25"<<endl;
       }
        
       else{
    
       }
    
	Game game(Board_Size);

	bool playing = true;
	while(playing)
	{

    game.printBoard();
		cout << "Enter your move:  ";
		string move_string;
		getline(cin, move_string);

		if(move_string == "quit")
			playing = false;
		else if(move_string == "new")
		{
			game.printWinner();
			game = Game(Board_Size);
		}
		else if(move_string == "load")
		{
			game.printWinner();

			string filename;
			cout << "Enter the file name:  ";
			getline(cin, filename);
			game = Game(filename);
		}
		else if(move_string == "pass")
		{
			game.blackPass();

			bool is_white_played = game.whiteAi();
			if(is_white_played == false)
				playing = false;
		}
    else if (move_string == "undo"){
      game.undo2Moves();
      
    }
		else if(isPlaceStringWellFormed(move_string))
		{
			int row    = placeStringToRow   (move_string);
			int column = placeStringToColumn(move_string);

			bool result = game.blackPlay(row, column);
			if(result == true)
				game.whiteAi();
			// else failure message is printed by blackPlay
		}
		else
			cout << "\"" << move_string << "\" is ill-formed." << endl;
		cout << endl;
	}

	game.printWinner();
	cout << "Goodbye, " << name << "!" << endl;
	return 0;
}
}

