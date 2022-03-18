#include <iostream>
#include <string>
#include "Table.h"

Table::Table()
{
    table = init();
    tableId = init();
    piecesId = init();

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int res;
            do
            {
                res = rand() % 256;
            } while (isTaken(res, i, j));

            tableId[i][j] = res;
        }
    }

    table[0][1] = -1;
    table[0][3] = -1;
    table[0][5] = -1;
    table[0][7] = -1;

    table[1][0] = -1;
    table[1][2] = -1;
    table[1][4] = -1;
    table[1][6] = -1;

    table[2][1] = -1;
    table[2][3] = -1;
    table[2][5] = -1;
    table[2][7] = -1;

    table[5][0] = 1;
    table[5][2] = 1;
    table[5][4] = 1;
    table[5][6] = 1;

    table[6][1] = 1;
    table[6][3] = 1;
    table[6][5] = 1;
    table[6][7] = 1;

    table[7][0] = 1;
    table[7][2] = 1;
    table[7][4] = 1;
    table[7][6] = 1;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (table[i][j] != 0)
            {
                int res;
                do
                {
                    res = rand() % 256;
                } while (isTaken(res, i, j));

                piecesId[i][j] = res;
            }
        }
    }
}

int Table::getPlayer()
{
    return playerTurn;
}

bool Table::isIdFromPiece(int id)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (table[i][j] != 0 && piecesId[i][j] == id)
            {
                return true;
            }
        }
    }
    return false;
}

std::pair<int, int> Table::getCoordsFromPiece(int id)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (table[i][j] != 0 && piecesId[i][j] == id)
            {
                return std::make_pair(i, j);
            }
        }
    }
}

void Table::getPossibleMovesFromId(int id, int **table)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (piecesId[i][j] == id)
            {
                table = availableMoves(i, j, playerTurn, table);
                return;
            }
        }
    }
}

int Table::isTaken(int res, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tableId[i][j] == res)
            {
                return -1;
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (piecesId[i][j] == res)
            {
                return -1;
            }
        }
    }

    return 0;
}

int **Table::init()
{
    int **table = (int **)malloc(sizeof(int *) * 8);
    for (int i = 0; i < 8; i++)
    {
        table[i] = (int *)malloc(sizeof(int *) * 8);
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            table[i][j] = 0;
        }
    }
    return table;
}

int Table::get(int x, int y, int **table)
{
    return table[x][y];
}

int Table::getTable(int x, int y)
{
    return get(x, y, table);
}
int Table::getTableId(int x, int y)
{
    return get(x, y, tableId);
}
int Table::getpiecesId(int x, int y)
{
    return get(x, y, piecesId);
}

int **Table::getTable()
{
    return Table::table;
}

int **Table::getTableId()
{
    return Table::tableId;
}
int **Table::getpiecesId()
{
    return Table::piecesId;
}

void Table::setTable(int **table)
{
    Table::table = table;
}

void Table::PrintTable(int **table)
{
    std::cout << "\n\n\n";
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            if (table[i][j] < 0)
            {
                std::cout << std::to_string(table[i][j]) << " ";
            }
            else
            {
                std::cout << " " << std::to_string(table[i][j]) << " ";
            }
        }
        std::cout << "\n";
    }
}

int **Table::availableMoves(int x, int y, int player, int **table)
{

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (player == 1)
            {
                if (checkIfValidWhite(x, y, i, j) > 0)
                {
                    table[i][j] = 1;
                }
            }
            else
            {
                if (checkIfValidBlack(x, y, i, j) > 0)
                {
                    table[i][j] = 1;
                }
            }
        }
    }

    return table;
}

int Table::typeMoveFromTo(int x1, int y1, int x2, int y2, int player)
{
    if (player == 1 && playerTurn == 1)
    {
        return checkIfValidWhite(x1, y1, x2, y2);
    }
    if (player == -1 && playerTurn == -1)
    {
        return checkIfValidBlack(x1, y1, x2, y2);
    }
    return -1;
}

int Table::moveFromTo(int x1, int y1, int x2, int y2, int player)
{
    if (player == 1 && playerTurn == 1)
    {
        if (checkIfValidWhite(x1, y1, x2, y2) > 0)
        {
            makeMove(x1, y1, x2, y2, player);
            playerTurn = -1;
            return 1;
        }
    }
    if (player == -1 && playerTurn == -1)
    {
        if (checkIfValidBlack(x1, y1, x2, y2) > 0)
        {
            makeMove(x1, y1, x2, y2, player);
            playerTurn = 1;
            return 1;
        }
    }
    return -1;
}

int Table::makeMove(int x1, int y1, int x2, int y2, int player)
{
    table[x1][y1] = 0;
    table[x2][y2] = player;

    piecesId[x2][y2] = piecesId[x1][y1];
    piecesId[x1][y1] = 0;
    if (x1 - x2 == 2 || x1 - x2 == -2)
    {
        int x_attack;
        int y_attack;
        if (player == 1)
        {
            x_attack = x1 - 1;
            if ((y1 - y2) < 0)
            {
                y_attack = y1 + 1;
            }
            else
            {
                y_attack = y1 - 1;
            }
        }
        else
        {
            x_attack = x1 + 1;
            if ((y1 - y2) < 0)
            {
                y_attack = y1 + 1;
            }
            else
            {
                y_attack = y1 - 1;
            }
        }

        table[x_attack][y_attack] = 0;
        piecesId[x_attack][y_attack] = 0;

        return 2;
    }

    return 1;
}

int Table::checkIfValidWhite(int x1, int y1, int x2, int y2)
{
    if (x1 <= 7 && x1 >= 0 && y1 <= 7 && y1 >= 0 && x2 <= 7 && x2 >= 0 && y1 <= 7 && y1 >= 0)
    {
        if (table[x2][y2] == 0)
        { // check if landing is empty
            if (table[x1][y1] == 1)
            { // check if white
                if (x2 == x1 - 1 && y2 == y1 - 1)
                {
                    return 1; // move
                }
                if (x2 == x1 - 1 && y2 == y1 + 1)
                {
                    return 1; // move
                }

                if (x2 == x1 - 2 && y2 == y1 - 2)
                {
                    if (table[x1 - 1][y1 - 1] == -1)
                    {
                        return 2; // manact
                    }
                    return -5; // nu esita piesa peste care sa sara
                }

                if (x2 == x1 - 2 && y2 == y1 + 2)
                {
                    if (table[x1 - 1][y1 + 1] == -1)
                    {
                        return 2; // manact
                    }
                    return -5;
                }

                return -4; // invalid type move
            }
            return -3; // not white #cancelled
        }
        return -2; // occupied space
    }
    return -1; // out of bound;
}

int Table::checkIfValidBlack(int x1, int y1, int x2, int y2)
{
    if (x1 <= 7 && x1 >= 0 && y1 <= 7 && y1 >= 0 && x2 <= 7 && x2 >= 0 && y1 <= 7 && y1 >= 0)
    { // check de bun simt
        if (table[x2][y2] == 0)
        { // check if landing is empty
            if (table[x1][y1] == -1)
            { // check if black

                if (x2 == x1 + 1 && y2 == y1 - 1)
                {
                    return 1; // move
                }
                if (x2 == x1 + 1 && y2 == y1 + 1)
                {
                    return 1; // move
                }

                if (x2 == x1 + 2 && y2 == y1 - 2)
                {
                    if (table[x1 + 1][y1 - 1] == 1)
                    {
                        return 2; // manact
                    }
                    return -5; // nu esita piesa peste care sa sara
                }

                if (x2 == x1 + 2 && y2 == y1 + 2)
                {
                    if (table[x1 + 1][y1 + 1] == 1)
                    {
                        return 2; // manact
                    }
                    return -5;
                }
                return -4; // invalid type move
            }
            return -3; // not white #cancelled
        }
        return -2; // occupied space
    }
    return -1; // out of bound;
}