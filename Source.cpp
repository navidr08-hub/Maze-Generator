// Maze-Generation Algorithm visual representation

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stack>
#include <array>

using namespace std;


class BlueMaze : public olc::PixelGameEngine
{
public:
	typedef struct Cell {
	private:
		bool visited;
		int row, col;

	public:
		Cell(int a, int b) {
			this->row = a; this->col = b;
			this->visited = false;
		}

		int get_row() { return this->row; }
		int get_col() { return this->col; }

		void setVisited(bool b) { this->visited = b; }
		bool getVisited() { return this->visited; }

	}Cell;

	int rows, columns, visits = 0;
	vector<Cell> grid;
	stack<Cell> path;

	BlueMaze()	{sAppName = "BlueMaze";}

	BlueMaze(int x, int y)	{
		sAppName = "BlueMaze";
		this->rows = x;
		this->columns = y;

		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				grid.push_back(Cell(i, j));
		
		int v = rand() % y;
		grid[v].setVisited(true);
		path.push(grid[v]);
		visits++;
	}

	void DrawCell(Cell * ptr, olc::Pixel color = olc::Pixel(olc::WHITE)) {
		int x = ptr->get_col() * 3, y = ptr->get_row() * 3;
		Draw(x, y, color);
		Draw(x + 1, y, color);
		Draw(x, y + 1, color);
		Draw(x + 1, y + 1, color);
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		// creating the grid
		for (int i = 0; i < ScreenHeight(); i++) 
			for (int j = 0; j < ScreenWidth(); j++) 
				Draw(i, j, olc::Pixel(olc::BLACK));

		for (int i = 0; i < grid.size(); i++)
			DrawCell(&grid[i], olc::Pixel(olc::BLUE));
		
		// Making the first cell green
		DrawCell(&path.top(), olc::Pixel(olc::GREEN));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		this_thread::sleep_for(10ms);

		if (visits == grid.size()) { // checking if maze is complete
			DrawCell(&path.top());
			return true;
		}

		Cell* ptr = &(path.top());

		DrawCell(ptr);

		array<int, 4> moves = { -1, -1, -1, -1 }; // move[0] = top, move[1] = left, move[2] = right, move[3] = bottom
		vector<int> ok; // directions available

		// checking which adjacent cells are within boundaries of maze

		if (ptr->get_row() - 1 >= 0) // top
			moves[0] = ((ptr->get_row() - 1) * this->columns) + ptr->get_col();
		if (ptr->get_col() - 1 >= 0) // left
			moves[1] = (ptr->get_row() * this->columns) + ptr->get_col() - 1;
		if (ptr->get_col() + 1 < this->columns) // right
			moves[2] = (ptr->get_row() * this->columns) + ptr->get_col() + 1;
		if (ptr->get_row() + 1 < this->rows) // bottom
			moves[3] = ((ptr->get_row() + 1) * this->columns) + ptr->get_col();

		// storing unvisited spots within adjacent cells
		for (int i = 0; i < sizeof(moves) / sizeof(moves[0]); i++) {
			if (moves[i] == -1) 
				continue;
			if (grid[moves[i]].getVisited() == false)
				ok.push_back(moves[i]);
		}
		
		// "randomly" selecting one of the valid spots available
		int next;
		if (ok.size() != 0)
			next = ok[rand() % ok.size()];
		else {
			DrawCell(&path.top());
			path.pop();
			DrawCell(&path.top(), olc::Pixel(olc::GREEN));
			return true;
		}

		// eliminating the wall to the selected adjacent cell
		for (int i = 0; i < sizeof(moves) / sizeof(moves[0]); i++) {	
			if (next == moves[i]) {
				switch (i)
				{
				case 0:
					Draw(ptr->get_col() * 3, (ptr->get_row() * 3) - 1, olc::Pixel(olc::WHITE));
					Draw((ptr->get_col() * 3) + 1, (ptr->get_row() * 3) - 1, olc::Pixel(olc::WHITE));
					break;
				case 1:
					Draw((ptr->get_col() * 3) - 1, ptr->get_row() * 3, olc::Pixel(olc::WHITE));
					Draw((ptr->get_col() * 3) - 1, (ptr->get_row() * 3) + 1, olc::Pixel(olc::WHITE));
					break;
				case 2:
					Draw((ptr->get_col() * 3) + 2, ptr->get_row() * 3, olc::Pixel(olc::WHITE));
					Draw((ptr->get_col() * 3) + 2, (ptr->get_row() * 3) + 1, olc::Pixel(olc::WHITE));
					break;
				case 3:
					Draw(ptr->get_col() * 3, (ptr->get_row() * 3) + 2, olc::Pixel(olc::WHITE));
					Draw((ptr->get_col() * 3) + 1, (ptr->get_row() * 3) + 2, olc::Pixel(olc::WHITE));
					break;
				default:
					break;
				}
			}
		}

		// coloring the new cell that was selected
		DrawCell(&grid[next]);

		// setting new cell's visited to true and adding to visits
		grid[next].setVisited(true);
		path.push(grid[next]);
		visits++;

		// making current cell green to visually represent back-tracking algorithm
		DrawCell(&path.top(), olc::Pixel(olc::GREEN));

		return true;
	}
};


int main()
{
	cout << "Dimensions of the square maze: ";
	int size;
	scanf_s("%d", &size);

	if (size < 2 && size > 100) { 
		cout << "Size must be between 2 - 100";
		return 0; 
	} // maze must be square and larger than 1x1.

	BlueMaze maze(size, size);
	if (maze.Construct((size * 3) - 1, (size * 3) - 1, 8, 8))
		maze.Start();

	return 0;
}
