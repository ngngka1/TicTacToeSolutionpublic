#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <array>
#include <limits>
#include "ListNode.cpp"

using namespace std;

class Game
{
private:
    static Game *instance;
    Game()
    {
        grid = {
            {'O', 'X', 'O'},
            {'X', '_', '_'},
            {'_', '_', '_'}};
    }

public:
    vector<vector<char>> grid;
    static Game *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Game();
        }
        return instance;
    }
};

Game *Game::instance = nullptr;

class Player
{
public:
    char symbol;
    int num; // indicates player1 or player2
    Player(char playerSymbol, int playerNum)
    {
        symbol = playerSymbol;
        num = playerNum;
    }
};

void inputPlayer(Player *&user, Player *&opponent, map<int, Player *> *players)
{
    char playerSymbol1, playerSymbol2;
    int option;
    do
    {
        cin.clear();
        cin.sync();
        cout << "Please input the symbol representing Player 1: (The player who makes the first move)\n";
        cin >> playerSymbol1; // only the first char of the line is read
        // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (cin.fail());

    do
    {
        cin.clear();
        cin.sync();
        cout << "Please input the symbol representing Player 2:\n";
        cin >> playerSymbol2; // only the first char of the line is read
        // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (cin.fail());

    do
    {
        cin.clear();
        cin.sync();
        cout << "Are you player 1 or 2? (1/2)\n";
        cin >> option;
        // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (cin.fail() || !(option >= 1 && option <= 2));

    if (option == 1)
    {
        user = new Player(playerSymbol1, 1);
        opponent = new Player(playerSymbol2, 2);
        (*players)[1] = user;
        (*players)[2] = opponent;
    }
    else
    {
        user = new Player(playerSymbol2, 2);
        opponent = new Player(playerSymbol1, 1);
        (*players)[2] = user;
        (*players)[1] = opponent;
    }
}

void inputGrid() // WIP
{
    return;
}

bool winCheck(const Game *game, const char checkedPlayerChess)
{
    for (int i = 0; i < 3; i++)
    {
        if // checking if there are horizontal wins
            (
                game->grid[i][0] == game->grid[i][1] &&
                game->grid[i][1] == game->grid[i][2] &&
                game->grid[i][2] == checkedPlayerChess)
        {
            return true;
        }
        if // checking if there are vertical wins
            (
                game->grid[0][i] == game->grid[1][i] &&
                game->grid[1][i] == game->grid[2][i] &&
                game->grid[2][i] == checkedPlayerChess)
        {
            return true;
        }
    }

    if (game->grid[0][0] == game->grid[1][1] && game->grid[1][1] == game->grid[2][2] && game->grid[2][2] == checkedPlayerChess)
        return true;
    if (game->grid[2][0] == game->grid[1][1] && game->grid[1][1] == game->grid[0][2] && game->grid[0][2] == checkedPlayerChess)
        return true;

    return false;
}

array<int, 2> getWinablePosition(Game *game, const char currChess)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (game->grid[i][j] == '_')
            {
                game->grid[i][j] = currChess;
                if (winCheck(game, currChess))
                {
                    game->grid[i][j] = '_';
                    return {i, j};
                }
                game->grid[i][j] = '_';
            }
        }
    }
    return {-1, -1};
}

stack<array<int, 2>> validAttack(Game *game, const char currChess, const char nextChess)
{
    stack<array<int, 2>> validAttackPositions;
    stack<array<int, 2>> unvisited;
    set<int> closed;
    bool diags[5]; // false refers to invalid move (which is blocked by opponent(nextChess))
    // [0] _ [1]
    //  _ [2] _
    // [3] _ [4]
    int count = 0;
    for (int rowNum = 0; rowNum < 3; rowNum++) // horizontal wins
    {
        for (int colNum = 0; colNum < 3; colNum++)
        {
            if (game->grid[rowNum][colNum] == '_')
            {
                unvisited.push({rowNum, colNum});
            }
            else if (game->grid[rowNum][colNum] == nextChess)
            {
                closed.insert(rowNum);
                closed.insert(colNum);
            }
            if (abs(rowNum - colNum) % 2 == 0)
            {
                diags[count] = game->grid[rowNum][colNum] != nextChess;
                count++;
            }
        }
    }
    while (unvisited.size() > 0)
    {
        int targetRow = unvisited.top()[0];
        int targetCol = unvisited.top()[1];
        unvisited.pop();

        if (closed.find(targetRow) == closed.end() && closed.find(targetCol) == closed.end())
        {
            validAttackPositions.push({targetRow, targetCol});
        }
        else if ((abs(targetRow - targetCol) % 2) == 0)
        {
            // Note: situations where two chess exists are eliminated already
            if (targetRow != targetCol || targetRow == 1) // case of increasing diagonal '/'
            {
                if (diags[3] ^ diags[2] ^ diags[1]) // if (3 blank) or (2 blank 1 currChess)
                    validAttackPositions.push({targetRow, targetCol});
            }
            if (targetRow == targetCol || targetRow == 1)
            {
                if (diags[0] ^ diags[2] ^ diags[4]) // if (3 blank) or (2 blank 1 currChess)
                    validAttackPositions.push({targetRow, targetCol});
            }
        }
    }

    return validAttackPositions;
}

stack<array<int, 2>> getValidMovePosition(Game *game, char currChess, const char nextChess) // nextChess is the chess to be placed NEXT round
{
    array<int, 2> winablePosition = getWinablePosition(game, currChess);
    if (winablePosition[0] != -1)
    {
        stack<array<int, 2>> return_value;
        return_value.push({winablePosition[0], winablePosition[1]});
        return return_value;
    }

    array<int, 2> opponentWinablePosition = getWinablePosition(game, nextChess);
    if (opponentWinablePosition[0] != -1)
    {
        stack<array<int, 2>> return_value;
        return_value.push({opponentWinablePosition[0], opponentWinablePosition[1]});
        return return_value;
    }

    return validAttack(game, currChess, nextChess);
}

ListNode *backtrack(ListNode *end, Game *game, const Player *user, const Player *opponent, const map<int, Player *> *players, int rounds)
{
    if (rounds >= 4)
    {
        if (winCheck(game, opponent->symbol) || rounds > 9)
        {
            return nullptr;
        }
        else if (winCheck(game, user->symbol))
        {
            return end;
        }
    }

    char currChess = (*players).at((rounds & 1) + 1)->symbol;
    char nextChess = (*players).at(((rounds + 1) & 1) + 1)->symbol;
    stack<array<int, 2>> validMovePositions = getValidMovePosition(game, currChess, nextChess);
    // cout << "validattackposition size: " << validMovePositions.size() << endl;
    if (validMovePositions.size() == 0)
    {
        return nullptr;
    }

    while (validMovePositions.size() > 0)
    {
        int targetRow = validMovePositions.top()[0];
        int targetCol = validMovePositions.top()[1];
        validMovePositions.pop();
        ListNode *currhead = new ListNode(targetRow, targetCol, currChess, end);

        game->grid[targetRow][targetCol] = currChess;
        currhead->next = backtrack(end, game, user, opponent, players, rounds + 1);
        game->grid[targetRow][targetCol] = '_';
        if (currhead->next != nullptr)
        {
            return currhead;
        }
        else
        {
            delete currhead;
        }
    }
    return nullptr;
}

int main()
{
    Game *game = Game::getInstance();
    Player *user;
    Player *opponent;
    map<int, Player *> *players = new map<int, Player *>();
    int rounds = 0;

    inputPlayer(user, opponent, players);
    inputGrid();

    ListNode *end = new ListNode(-1, -1, '_', nullptr);
    for (int i = 0; i < 3; i++) // This function will be changed when game grid input is possible
    {
        for (int j = 0; j < 3; j++)
        {
            if (game->grid[i][j] != '_')
            {
                rounds++;
            }
        }
    }
    // Linked list / stack is viable data structure but I just chose the former one
    ListNode *head = backtrack(end, game, user, opponent, players, rounds); // round 0 refers to no chess placed
    delete user;
    delete opponent;
    delete end;
    if (head == nullptr)
    {
        cout << "no solution found\n";
    }

    ofstream outputFile;
    outputFile.open("step.csv"); // documentation purpose
    while (head != end)
    {
        game->grid[head->rowNum][head->colNum] = head->placedChess;
        outputFile << "\nrow: " << head->rowNum << "\ncolumn: " << head->colNum << "\nPlaced Chess: " << head->placedChess << endl;
        cout << "steps: \n";
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cout << game->grid[i][j] << " ";
            }
            cout << "\n";
        }
        head = head->next;
    }
    outputFile.close();
    cout << "\nPress any key to exit\n";
    cin.clear();
    cin.sync();
    getchar();
    return 0;
}