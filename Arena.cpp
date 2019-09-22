#include "Arena.h"
#include "Player.h"
#include "Robot.h"


#include <iostream>
using namespace std;

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRobots = 0;
}

Arena::~Arena()
{
	cerr << "Entering Arena destructor" << endl;
		
	delete m_player;
	for (int z = 0; z < MAXROBOTS; z++) { // NOTICE - m_nROBOTS OR MAXROBOTS? WHEN A ROBOT DIES IN A GAME, IS IT BEING DELETED?
		if (m_robots[z] != nullptr)
			delete m_robots[z];
	}
	cerr << "Leaving Arena destructor" << endl;
}

int Arena::rows() const
{
	return m_rows;  
}

int Arena::cols() const
{
	
	return m_cols;  
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::robotCount() const
{
	return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
	int n=0;
	for (int j = 0; j < MAXROBOTS; j++)
	{
		if (m_robots[j] != nullptr)
		{
			if (m_robots[j]->row() == r && m_robots[j]->col() == c)
				n++;
		}
	}
	return n;  
}

void Arena::display(string msg) const
{
	// Position (row,col) in the arena coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';

	// Indicate each robot's position
	for (int i = 0; i < MAXROBOTS; i++)
	{
		if (m_robots[i] != nullptr)
		{
			char& gridChar = grid[m_robots[i]->row() - 1][m_robots[i]->col() - 1];

			switch (gridChar)
			{
			case '.':
				gridChar = 'R';
				break;
			case 'R':
				gridChar = '2';
				break;
			case '2':
				gridChar = '3';
				break;
			case '3':
				gridChar = '4';
				break;
			case '4':
				gridChar = '5';
				break;
			case '5':
				gridChar = '6';
				break;
			case '6':
				gridChar = '7';
				break;
			case '7':
				gridChar = '8';
				break;
			default:
				gridChar = '9';
				break;
			}
		}
	}

	// Indicate player's position
	if (m_player != nullptr)
	{
		// Set the char to '@', unless there's also a robot there,
		// in which case set it to '*'.
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (gridChar == '.')
			gridChar = '@';
		else
			gridChar = '*';

	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++)
	{
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, robot, and player info
	cout << endl;
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << robotCount() << " robots remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else
	{
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isDead())
			cout << "The player is dead." << endl;
	}
}

bool Arena::addRobot(int r, int c)
{

	if (m_nRobots == MAXROBOTS)
		return false;

	//Robot* newRobot = new Robot(this, r, c);
	//m_robots[m_nRobots] = newRobot;

	m_robots[m_nRobots] = new Robot(this, r, c);
	m_nRobots++;
	return true;

}

bool Arena::addPlayer(int r, int c)
{
	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Dynamically allocate a new Player and add it to the arena
	m_player = new Player(this, r, c);
	return true;
}

void Arena::damageRobotAt(int r, int c) 
{

	for (int i = 0; i < MAXROBOTS; i++)
	{
		if (m_robots[i] != nullptr)
		{
			if (m_robots[i]->row() == r && m_robots[i]->col() == c)
			{
				bool flag = (m_robots[i]->takeDamageAndLive());
				m_robots[i]->takeDamage();
				if (!flag)
				{
					delete m_robots[i];
					m_robots[i] = nullptr;
					m_nRobots--;
				}
				break;
			}
		}
	}

}

bool Arena::moveRobots()
{
	for (int k = 0; k < MAXROBOTS; k++)
	{
		if (m_robots[k] != nullptr)
		{
			m_robots[k]->move();
			if (m_robots[k]->row() == m_player->row() && m_robots[k]->col() == m_player->col())
				m_player->setDead();
		}
	}
	// return true if the player is still alive, false otherwise
	return !m_player->isDead();
}


void Arena::initializeRobots()
{
	for (int i = 0; i < MAXROBOTS; i++)
		m_robots[i] = nullptr;
}