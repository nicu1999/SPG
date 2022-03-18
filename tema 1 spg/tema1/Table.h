#pragma once
#include <iostream>

class Table
{
public:
	Table();
	~Table();
	void PrintTable(int **table);
	int moveFromTo(int x1, int y1, int x2, int y2, int player);
	int checkIfValidWhite(int x1, int y1, int x2, int y2);
	int checkIfValidBlack(int x1, int y1, int x2, int y2);
	int makeMove(int x1, int y1, int x2, int y2, int player);
	int typeMoveFromTo(int x1, int y1, int x2, int y2, int player);
	int **availableMoves(int x, int y, int player, int **table);
	int **getTable();
	int **getpiecesId();
	int **getTableId();
	void setTable(int **table);
	static int **init();
	int getTable(int x, int y);
	int getTableId(int x, int y);
	int getpiecesId(int x, int y);
	int isTaken(int id, int x, int y);
	void getPossibleMovesFromId(int id, int **table);
	bool isIdFromPiece(int id);
	std::pair<int, int> Table::getCoordsFromPiece(int id);
	int getPlayer();

private:
	int get(int x, int y, int **table);
	int **table;
	int **tableId;
	int **piecesId;
	int playerTurn = 1;
};
