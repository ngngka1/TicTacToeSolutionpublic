#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <string>
#include <limits>

using namespace std;

class Game
{
    private:
        static Game* instance;
        Game()
        {
            grid = {
                {'_', 'X', '_'},
                {'_', 'O', '_'},
                {'_', '_', '_'}
            };
        }

    public:
        vector<vector<char>> grid;
        static Game* getInstance()
        {
            
            if (instance == nullptr)
            {
                instance = new Game();
            }
            return instance;
        }
};

Game* Game::instance = nullptr;

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

class ListNode
{
    public:
        int rowNum;
        int colNum;
        char placedChess;
        ListNode* next;
        ListNode(int rowNumArg, int colNumArg, char placedChessArg, ListNode* nextarg)
        {
            rowNum = rowNumArg;
            colNum = colNumArg;
            placedChess = placedChessArg;
            next = nextarg;
        }
};

void inputPlayer(Player* &user, Player* &opponent, map<int, Player*>* players)
{
    char playerSymbol1, playerSymbol2;
    int option;
    bool inputting = true;
    while (inputting)
    {
        cout << "Please input the symbol representing Player 1: (The player who makes the first move)\n";
        cin >> playerSymbol1; // only the first char of the line is read
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!cin.fail())
        {
            break;
        }
    }
    
    while (inputting)
    {
        cout << "Please input the symbol representing Player 2:\n";
        cin >> playerSymbol2; // only the first char of the line is read
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!cin.fail())
        {
            break;
        }
    }

    while (inputting)
    {
        cout << "Are you player 1 or 2? (1/2)\n";
        cin >> option;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option == 1 || option == 2)
        {
            inputting = false;
        }
    }

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

bool winCheck(const Game* game, const char checkedPlayerChess)
{
    for (int i = 0; i < 3; i++)
    {
        if // checking if there are horizontal wins
        (
            game->grid[i][0] == game->grid[i][1] && 
            game->grid[i][1] == game->grid[i][2] && 
            game->grid[i][2] == checkedPlayerChess
        )
            {
                return true;
            }
        if // checking if there are vertical wins
        (
            game->grid[0][i] == game->grid[1][i] && 
            game->grid[1][i] == game->grid[2][i] && 
            game->grid[2][i] == checkedPlayerChess
        )
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

vector<int> getWinablePosition(Game* game, const char currChess)
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
    return {};
}

vector<vector<int>> validAttack(Game* game, const char currChess, const char nextChess)
{
    vector<vector<int>> validAttackPositions = {};
    vector<vector<int>> blankGrid = {};
    
    for (int i = 0; i < 3; i++) // horizontal wins
    {
        if (game->grid[i][0] != nextChess && game->grid[i][1] != nextChess && game->grid[i][2] != nextChess)
        {
            for (int colNum = 0; colNum < 3; colNum++)
            {
                if (game->grid[i][colNum] == '_')
                {
                    blankGrid.push_back({i, colNum});
                }
            }
            if (blankGrid.size() == 2)
            {
                validAttackPositions.push_back(blankGrid[0]);
                validAttackPositions.push_back(blankGrid[1]);
            }
            blankGrid = {};
        }

        if (game->grid[0][i] != nextChess && game->grid[1][i] != nextChess && game->grid[2][i] != nextChess)
        {
            for (int rowNum = 0; rowNum < 3; rowNum++)
            {
                if (game->grid[rowNum][i] == '_')
                {
                    blankGrid.push_back({rowNum, i});
                }
            }
            if (blankGrid.size() == 2)
            {
                validAttackPositions.push_back(blankGrid[0]);
                validAttackPositions.push_back(blankGrid[1]);
            }
            blankGrid = {};
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (game->grid[i * 2][0] != game->grid[1][1] && game->grid[1][1] != game->grid[2][i * 2] && game->grid[1][1] != nextChess)
        {
            // brute force but sadly I cant come up with a clean and efficient code
            if (game->grid[i * 2][0] == '_') blankGrid.push_back({i * 2, 0});
            if (game->grid[1][1] == '_') blankGrid.push_back({1, 1});
            if (game->grid[0][i * 2] == '_') blankGrid.push_back({0, i * 2});
            if (blankGrid.size() == 2)
            {
                validAttackPositions.push_back(blankGrid[0]);
                validAttackPositions.push_back(blankGrid[0]);
            }
            blankGrid = {};
        }
    }
    return validAttackPositions;
}

vector<vector<int>> getValidMovePosition(Game* game, char currChess, const char nextChess) // nextChess is the chess to be placed NEXT round
{
    vector<int> winablePosition = getWinablePosition(game, currChess);
    if (winablePosition.size() > 0)
    {
        return {winablePosition};
    }

    vector<int> opponentWinablePosition = getWinablePosition(game, nextChess);
    if (opponentWinablePosition.size() > 0)
    {
        return {opponentWinablePosition};
    }

    vector<vector<int>> validAttackPositions = validAttack(game, currChess, nextChess);
    return validAttackPositions;
}

ListNode* backtrack(ListNode* end, Game* game, const Player* user, const Player* opponent, const map<int, Player*>* players, int rounds)
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
    vector<vector<int>> validMovePositions;
    validMovePositions = getValidMovePosition(game, currChess, nextChess);
    if (validMovePositions.size() == 0)
    {
        return nullptr;
    }

    for (auto pos: validMovePositions)
        {
            ListNode* currhead = new ListNode(pos[0], pos[1], currChess, end);
            game->grid[pos[0]][pos[1]] = currChess;
            currhead->next = backtrack(end, game, user, opponent, players, rounds + 1);
            game->grid[pos[0]][pos[1]] = '_';
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
    Game* game = Game::getInstance();
    Player* user;
    Player* opponent;
    map<int, Player*>* players = new map<int, Player*>();
    int rounds = 0;

    inputPlayer(user, opponent, players);
    inputGrid();

    ListNode* end;
    end = (ListNode*)malloc(sizeof(ListNode));
    if (end == nullptr)
    {
        cout << "malloc failed";
        return 1;
    }
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
    ListNode* head = backtrack(end, game, user, opponent, players, rounds); // round 0 refers to no chess placed
    delete user;
    delete opponent;
    free(end);
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




    return 0;

}