#include<iostream>
#include<ctime>
#include<iomanip>
#include<chrono>
#include<thread>
#include<string>
#include<fstream>
using namespace std::this_thread;
using namespace std::chrono;
using namespace std;
void intitialize(char**arr, int x, int y, char type);
void howtoplay();
void highscore();
void options(char command[]);
void credits();
void show(char**arr, int x, int y);
void minegen(char**arr, int x, int y, int num);
void numberassign(char**arr, int x, int y);
void open(char**arrG, char**arrB, int x, int y, int xa, int ya);
void gamend(bool type);
char enterFO();
time_t starting; int flags, mines, sqropn = 0;
ifstream fin; ofstream fout;
int main()
{
	int choice, x, y, x_axis = 0, y_axis = 0, minesdetect = 0; mines = 0, flags = 0;
	char flagOpen = '\0', y_not = 'x', rang[9] = "COLOR 0B"; bool end, firsturn; system("title Minesweeper by KustomAsh");
Menu:
	system("cls"); //needed on return
	system("COLOR 0A");
	cout << "MINE SWEEPER\n\n1. Play Game \n2. How to play\n3. High scores \n4. Game Options"
		<< "\n5. Credits\n6. Exit\n\nEnter Your choice : "; cin >> choice;
	switch (choice)
	{
	case 1:
	{ goto game; }
	case 2:
	{  system("COLOR B1"); howtoplay(); system("pause>0"); goto Menu; }
	case 3:
	{  system("COLOR B1"); highscore(); cout << "\nPress any key to return to Main Menu"; system("pause>0"); goto Menu; }
	case 4:
	{  system("COLOR 0C"); options(rang); goto Menu; }
	case 5:
	{  system("COLOR B1"); credits(); system("pause>0"); goto Menu; }
	case 6:
		goto exit;
	default:
		cout << "Invalid Choice. Choose again\n\7";  sleep_for(milliseconds(2500));
		goto Menu;
	}
game:
	system("COLOR 0C");
	cout << "\nStarting Game\nEnter your desired row size for grid : "; cin >> x;
	cout << "Enter your desired column size for grid : "; cin >> y;
	cout << "Enter number of MINES (Must be less than " << abs(x*y / 2) << ") :"; cin >> mines;
	x = abs(x); y = abs(y); mines = abs(mines);
	if (mines >= x*y){ mines = ((x*y) / 4) + 1; }
	flags = mines;
	// array work
	char**grid = new char*[x];
	for (int i = 0; i < x; i++)
	{
		grid[i] = new char[y];
	}
	char**backstage = new char*[x];
	for (int i = 0; i < x; i++)
	{
		backstage[i] = new char[y];
	}
	intitialize(grid, x, y, '*');
	intitialize(backstage, x, y, 48);
	minegen(backstage, x, y, mines);
	numberassign(backstage, x, y);
	//                                ARRAYS COMPLETED, GRID AND BACKSTAGE
	for (int i = 3; i >= 0; i--)
	{
		system("cls");
		cout << "Game Starting In " << i;
		sleep_for(seconds(1));
	}
	starting = time(0);
	system(rang);
	end = false; firsturn = true;
	while (end != true)
	{
		system("cls");
		show(grid, x, y); // here we go
		flagOpen = '\0';
		flagOpen = enterFO();
		do
		{
			cout << "Enter valid row number (0 to " << x - 1 << ") :"; cin >> x_axis;
		} while (x_axis < 0 || x_axis >= x);
		do
		{
			cout << "Enter valid column number (0 to " << y - 1 << ") :"; cin >> y_axis;
		} while (y_axis < 0 || y_axis >= y);
		if (flagOpen == 'F')
		{
			if (grid[x_axis][y_axis] == '*')
			{
				if (flags > 0)
				{
					grid[x_axis][y_axis] = 'F';
					if (backstage[x_axis][y_axis] == 'M')
					{
						minesdetect++;
					}
					flags--;
				}
				else { cout << "You have already used all flags\7"; sleep_for(seconds(2)); }
			}
			else if (grid[x_axis][y_axis] == 'F')
			{
				grid[x_axis][y_axis] = '*';
				flags++;
			}
		}
		else if (flagOpen == 'O')
		{
			if (backstage[x_axis][y_axis] == 'M') //lose game check
			{
				if (firsturn == true)
				{
					cout << "Please wait...";
					for (;;)
					{
						intitialize(backstage, x, y, 48);
						minegen(backstage, x, y, mines);
						numberassign(backstage, x, y);
						if (backstage[x_axis][y_axis] != 'M'){ break; }
					}
					open(grid, backstage, x, y, x_axis, y_axis); firsturn = false;
				}
				else
				{
					for (int i = 0; i < x; i++)
					{
						for (int j = 0; j < y; j++)
						{
							if (backstage[i][j] == 'M'){ grid[i][j] = 'M'; }
						}
					}
					system("cls"); system("COLOR CF");
					show(grid, x, y);
					gamend(false);  end = true;
				}
			}
			else open(grid, backstage, x, y, x_axis, y_axis);  // opening block
		}
		sqropn = 0;  //wingame check starts here
		for (int i = 0; i < x; i++)	for (int j = 0; j < y; j++)	{ if (grid[i][j] != '*'&&grid[i][j] != 'F')sqropn++; }
		if (minesdetect == mines || sqropn == (x*y - mines))
		{
			system("cls"); show(grid, x, y);
			end = true; gamend(true);
		}
		firsturn = false;
	}
	cout << "\nEnter Y to return to Main Menu and play again / Enter Any other key to Exit : "; cin >> y_not;
	switch (y_not)
	{
	case 'Y':
	case 'y':
		main();
	default:return 0;
	}
exit:
	system("pause");
}
void show(char**arr, int x, int y)
{
	char sidiline = 179, daba = 219;
	cout << "MineSweeper\n\n" << "Total Blocks: " << x*y << ";   Opened: "
		<< sqropn << ";   Flags left: " << flags << ";   Flags Used: " << mines - flags << ";   Mines: "
		<< mines << ";   Time elapsed (seconds): " << time(0) - starting << "\n\n\nColumn";
	for (int k = 0; k < y; k++){ cout << setw(5.5) << k; } cout << endl << "======";
	for (int k = 0; k < y; k++){ cout << "====="; }cout << endl << "Row   " << sidiline << endl;
	for (int i = 0; i<x; i++)
	{
		cout << setw(4) << i << setw(3) << sidiline;
		for (int j = 0; j<y; j++)
		{
			if (arr[i][j] == '*')
				cout << setw(3.5) << sidiline << daba << sidiline;
			else cout << setw(3.5) << sidiline << arr[i][j] << sidiline;
		}
		cout << endl << setw(7) << sidiline << endl;
	}
	cout << "======";
	for (int k = 0; k < y; k++){ cout << "====="; }
}
void highscore()
{
	string line; int hs, ser_no = 1;
	fin.open("score.txt");
	cout << endl << "\t    High Scores\n\n   Time taken   ||    Player Name\n";
	while (!fin.eof())
	{
		fin >> hs >> line;
		cout << ser_no << ". " << hs << " Seconds" << setw(13) << line << endl;
		ser_no++;
	}
	fin.close();
}
void howtoplay()
{
	cout << "\n\t\t\t\tHOW TO PLAY\n\n\tThe objective of the game is to clear a rectangular board containing hidden"
		<< "\n\t\"mines\" or bombs without detonating any of them, with help from clues about "
		<< "\n\tthe number of neighboring mines in each field. The game area consists of the"
		<< "\n\tplaying field, a mine counter, and a timer. The game starts when you enter "
		<< "\n\trow and column size of the grid.\n"
		<< "\n\t•  You can uncover a square by entering O and then its coordinates."
		<< "\n\t•  If you uncover a mine, you lose the game."
		<< "\n\t•  If a number appears on a square, it indicates how many mines are in the eight squares that surround it."
		<< "\n\t•  To mark a square you suspect contains a mine, enter F and then its coordinates."
		<< "\n\t•  To remove a flag, enter F and then enter its coordinates again."
		<< "\n\t•  If you mark all the mines correctly, You win the game."
		<< "\n\t•  Your name will be added to highscores list in you finish in less time than the previous high scores."
		<< "\n\nPress any key to return to Main Menu";
}
void gamend(bool type)
{
	time_t endclick = time(0);
	string lines[5], strtemp[5], name;
	int b = 0, nums[5], numstemp[5], newnum = endclick - starting, flag = -1;
	if (type == true)
	{
		cout << "\n\n\nCongratulations! You have sweeped the mines and WON THE GAME!"
			<< "\nEnter you name : "; cin >> name;
		fin.open("score.txt");
		while (!fin.eof())
		{
			fin >> nums[b] >> lines[b];
			if (nums[b] > newnum && flag == -1){ flag = b; }
			b++;
		}
		for (int a = flag; a < 5; a++)
		{
			strtemp[a] = lines[a]; numstemp[a] = nums[a];
		}
		for (int a = flag; a < 4; a++)
		{
			lines[a + 1] = strtemp[a]; nums[a + 1] = numstemp[a];
		}
		fin.close();
		if (flag != -1)
		{
			lines[flag] = name;
			nums[flag] = newnum;
			fout.open("score.txt");
			for (b = 0; b < 5; b++)
			{
				fout << nums[b] << " " << lines[b];
				if (b != 4)fout << endl;
			}
			fout.close();
		}
		highscore();
	}
	if (type == false)
		cout << "\7\n\n\nGame Over! You hit a mine";
}
void minegen(char**arr, int x, int y, int num)
{
	srand(time(NULL));
	int rand1, rand2;
	for (int i = 0; i < num; i++)
	{
		rand1 = rand() % x; rand2 = rand() % y;
		if (arr[rand1][rand2] != 'M')
		{
			arr[rand1][rand2] = 'M';
		}
		else num++;
	}
}
void intitialize(char**arr, int x, int y, char type)
{
	for (int i = 0; i<x; i++)
	{
		for (int j = 0; j<y; j++)
		{
			arr[i][j] = type;
		}
	}
}
void numberassign(char**arr, int x, int y)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			if (arr[i][j] == 'M')
			{
				if ((i + 1 < x) && (j + 1 < y))  //1
				{
					if (arr[i + 1][j + 1] != 'M')
						arr[i + 1][j + 1]++;
				}
				if ((i - 1 >= 0) && (j + 1 < y))  //2
				{
					if (arr[i - 1][j + 1] != 'M')
						arr[i - 1][j + 1]++;
				}
				if ((i + 1 < x) && (j - 1 >= 0))  //3
				{
					if (arr[i + 1][j - 1] != 'M')
						arr[i + 1][j - 1]++;
				}
				if ((i - 1 >= 0) && (j - 1 >= 0))  //4
				{
					if (arr[i - 1][j - 1] != 'M')
						arr[i - 1][j - 1]++;
				}
				if (i - 1 >= 0)  //5
				{
					if (arr[i - 1][j] != 'M')
						arr[i - 1][j]++;
				}
				if (j - 1 >= 0)  //6
				{
					if (arr[i][j - 1] != 'M')
						arr[i][j - 1]++;
				}
				if (j + 1 < y)  //7
				{
					if (arr[i][j + 1] != 'M')
						arr[i][j + 1]++;
				}
				if (i + 1 < x)  //8				{
					if (arr[i + 1][j] != 'M')
						arr[i + 1][j]++;
				}
			}
		}
	}
}
void open(char**arrG, char**arrB, int x, int y, int xa, int ya)
{
	{
		if ((xa >= 0) && (xa<x) && (ya >= 0) && (ya<y) && (arrG[xa][ya] == '*') && (arrB[xa][ya] != 'M'))
		{
			arrG[xa][ya] = arrB[xa][ya];
			if (arrB[xa][ya] == 48)
			{
				open(arrG, arrB, x, y, xa + 1, ya + 1);  //1
				open(arrG, arrB, x, y, xa - 1, ya - 1);  //2
				open(arrG, arrB, x, y, xa, ya + 1);  //3
				open(arrG, arrB, x, y, xa + 1, ya);  //4
				open(arrG, arrB, x, y, xa - 1, ya);  //5
				open(arrG, arrB, x, y, xa, ya - 1);  //6
				open(arrG, arrB, x, y, xa + 1, ya - 1);  //7
				open(arrG, arrB, x, y, xa + 1, ya - 1);  //8
			}
		}
	}
}
char enterFO()
{
	cout << "\n\n\nOpen or Flag (O / F) ? ";
	char symbol; cin >> symbol;
	switch (toupper(symbol))
	{
	case 'O': return 'O';
	case 'F': return 'F';
	default: cout << "\a"; enterFO();
	}
}
void options(char command[])
{
	char rangeen, gridrang;
	cout << "\n\tOPTIONS\n\n\t1. Change Color of Grid Squares\n\t2. Change Grid Background Color\n\t3. Set Defaults"
		<< "\n\t4. Return to Main Menu\n\n\tEnter Your Choice : "; cin >> rangeen;
	switch (rangeen)
	{
	case '1':
		cout << "\n\t\tSelect Color\n\n\t\t1. Red\n\t\t2. White\n\t\t3. Green\n\t\t4. "
			<< "Blue\n\n\t\tEnter Color Code :"; cin >> gridrang;
		switch (gridrang)
		{
		case '1': command[7] = 'C'; break;
		case '2': command[7] = '7'; break;
		case '3': command[7] = 'A'; break;
		default: command[7] = 'B'; break;
		}
		cout << "\n\t\tColor Set! Press any key to return to Main Menu"; system("pause>0");
		break;
	case '2':
		cout << "\n\t\tBackground Color\n\n\t\t1. Black\n\t\t2. White\n\n\t\tEnter Color Code :";
		cin >> gridrang;
		switch (gridrang)
		{
		case '2': command[6] = 'F'; break;
		default: command[6] = '0'; break;
		}
		cout << "\n\t\tColor Set! Press any key to return to Main Menu"; system("pause>0");
		break;
	case '3':
		command[6] = '0';
		command[7] = 'B';
		cout << "\n\tValues Set to defaults! Press any key to return to Main Menu"; system("pause>0");
		break;
	default: break;
	}
}
void credits()
{
	cout << "\n\t\t\t\tCredits\n\n\t      Game Coded by Lord6 Captain : KUSTOM  ASH \n\t\t\tashanmunir@twitter.com"
		 << "\n\t\t   *************links.blogspot.com\n\t      KustomAsh@github.com || fb.com/**********\n\tDOS "
		 <<"commands in system(\"X\") and OUTPUT DESIGN by *****.\n\tAll rights are reserved to Lord AshaL and "
		 <<"Dark ******.\n\tSpecial thanks to my Master John Stewart AKA BRV-*******\n\tFeel free to contact "
		 <<"for help or joining ******** group.\n\nPress any key to return to Main Menu";
}
