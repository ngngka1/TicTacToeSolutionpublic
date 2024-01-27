#include <iostream>
#include <vector>
#include <unordered_map>
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
                {'_', '_', '_'},
                {'_', '_', '_'},
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
        ListNode* next;
        ListNode(int rowNumArg, int colNumArg, ListNode* nextarg)
        {
            rowNum = rowNumArg;
            colNum = colNumArg;
            next = nextarg;
        }
}

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
        players[1] = user;
        players[2] = opponent;

    }
    else
    {
        user = new Player(playerSymbol2, 2);
        opponent = new Player(playerSymbol1, 1);
        players[2] = user;
        players[1] = opponent;
    }
}

void inputGrid()
{
    return; // wip
}

bool winCheck(const Game* game, const Player* checkedPlayer)
{
    for (int i = 0; i < 3; i++)
    {
        if // checking if there are horizontal wins
        (
            game->grid[i][0] == game->grid[i][1] && 
            game->grid[i][1] == game->grid[i][2] && 
            game->grid[i][2] == checkedPlayer->symbol
        )
            {
                return true;
            }
        if // checking if there are vertical wins
        (
            game->grid[0][i] == game->grid[1][i] && 
            game->grid[1][i] == game->grid[2][i] && 
            game->grid[2][i] == checkedPlayer->symbol
        )
            {
                return true;
            }
    }

    if (game->grid[0][0] == game->grid[1][1] && game->grid[1][1] == game->grid[2][2] && game->grid[2][2] == checkedPlayer->symbol)
        return true;
    if (game->grid[2][0] == game->grid[1][1] && game->grid[1][1] == game->grid[0][2] && game->grid[0][2] == checkedPlayer->symbol)
        return true;

    return false;
}

bool validMove(int rowNum, int colNum, Game* game, const map<int, Player*>* players, int rounds)
{
    return; // wip
}

ListNode* backtrack(const ListNode* end, Game* game, const Player* user, const Player* opponent, const map<int, Player*>* players, int rounds)
{
    if (rounds >= 4)
    {
        if (winCheck(user) && !winCheck(opponent))
        {
            return head;
        }
    }

    char nextChess = players[(rounds & 1) + 1]->symbol;

    for (int rowNum = 0; rowNum < 3; rowNum++)
    {
        for (int colNum = 0; colNum < 3; colNUm++)
        {
            if (game->grid[rowNum][colNum] == '_' && )
            {
                ListNode* currhead = new ListNode(rowNum, colNum, end);
                game->grid[rowNum][colNum] = nextChess;
                currhead->next = backtrack(currhead.next, game, user, opponent, players, rounds + 1);
                if (currhead->next == end)
                {
                    return currhead;
                }
                else
                {
                    delete currhead;
                }
                game->grid[rowNum][colNum] = '_';

            }
        }
    }
    return nullptr;
}

int main()
{
    Game* game = Game::getInstance();
    Player* user;
    Player* opponent;
    map<int, Player*>* players;

    inputPlayer(user, opponent, players);
    inputGrid();

    ListNode* end;
    end = (ListNode*)malloc(sizeof(ListNode));
    if (end == nullptr)
    {
        cout << "malloc failed"
        return 1;
    }
    backtrack(end, game->grid, user, opponent, players, 0) // round 0 refers to no chess placed



    return 0;

}