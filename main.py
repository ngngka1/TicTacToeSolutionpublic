# It is planned that this program will be converted into javascript and user can input the game grid easier with a webpage layout.

# Tic tac toe is a game where the first player has the chance to not lose 100% if he knows all winning combinations.
import time
import psutil
import csv
import sys
import os
import copy

            
def backtrack(game: list, players: dict, rounds: int, user: str) -> list:
    result = []
    if rounds >= 4 and win_check(game, user):
        for row in game:
            result.append(row[:]) # this is to avoid elements in result have the same reference to elements in game
        return [result]
    for row_num, row in enumerate(game):
        for col_num, element in enumerate(row):
            if element == "_" and valid_move(row_num, col_num, game[:][:], players, rounds):
                game[row_num][col_num] = players[rounds % 2]
                result = backtrack(game, players, rounds + 1, user)
                game[row_num][col_num] = "_"
                if result:
                    temp = []
                    for row in game:
                        temp.append(row[:])
                    return [temp] + result
    return result
            
def win_check(game: list, player: str):
    for row in game: # check horizontal win
        if row[0] == row[1] == row[2] == player:
            return True
        
    if (game[0][0] == game[1][0] == game[2][0] == player # check vertical win
    or game[0][1] == game[1][1] == game[2][1] == player
    or game[0][2] == game[1][2] == game[2][2] == player):
        return True
    
    if (game[0][0] == game[1][1] == game[2][2] == player # check diagonal win
    or game[0][2] == game[1][1] == game[2][0] == player):
        return True
    
    return False

def block(row_num, col_num, game: list, opponent): # this return the pos the player has to block the opponent from winning
    pos = []
    for row_num, row in enumerate(game):
        for col_num, element in enumerate(row):
            if game[row_num][col_num] == "_":
                game[row_num][col_num] = opponent
                if win_check(game, opponent):
                    pos = (row_num, col_num)
                game[row_num][col_num] = "_"
            if pos:
                return pos == [row_num, col_num]
    return True # return True because don't need to block the opponent

def attack(target_row, target_col, game, curr_player, opponent): #
    valid_attack = []
    for i, row in enumerate(game):
        for j, element in enumerate(row):
            if (game[i][j] == "_" 
                and (
                    game[i][j + 1 - len(row)] == game[i][j + 2 - len(row)] != opponent # horizontal
                    or game[i + 1 - len(game)][j] == game[i + 2 - len(game)][j] != opponent # vertical # this line returned true when i =0, j = 1 because direction
                    or (i == j and game[i + 1 - len(game)][j + 1 - len(row)] == game[i + 2 - len(game)][j + 2 - len(row)] != opponent)
                    or (abs(i-j) == 2 and game[len(game) - i - 1][j + 1 - len(row)] == game[len(game) - i - 2][j + 2 - len(row)] != opponent)
                    )
                ):
                valid_attack.append((i, j))
                
    for element in valid_attack:
        if element == (0, 1): 
            print("WRONG")
        if element == (target_row, target_col):
            return True
    return False

def valid_move(row_num, col_num, game, players, rounds): # this function returns true if the move is valid(block or attack)
    if rounds >= 4:
        if block(row_num, col_num, game[:][:], players[(rounds + 1) % 2]):
            return False
    return attack(row_num, col_num, game[:][:], players[rounds % 2], players[(rounds + 1) % 2])
    
        
    
def init_file():
    stepfile = open("step.csv", "w")
    stepfile.close()
    permufile = open("permutation.csv", "w")
    permufile.close()
    
def main():
    time_start = round(time.time(), 8)
    process = psutil.Process()
    init_file()
    game = [
        ["O", "_", "X"],
        ["_", "_", "_"],
        ["_", "_", "_"]
    ]
    
    PLAYERS = {
        0: "O", # player 1
        1: "X" # player 2
    }
    
    rounds = 0
    for row in game:
        for element in row:
            if element != "_":
                rounds += 1
    
    user = PLAYERS[0]

    result = backtrack(game[0:], PLAYERS, rounds, user)
    print(result)

    
    with open("step.csv", "a", newline='') as file:
        writer = csv.writer(file, delimiter=" ")
        writer.writerow([f"You are Player 1 ({user})"])
        for i, gamegrid in enumerate(result):
            writer.writerow([f"Round {i + rounds}:"])
            for row in gamegrid:
                writer.writerow(row)
                
        with open("permutation.csv", "a", newline='') as file:
            writer = csv.writer(file)
            for row in result[-1]:
                writer.writerow(" ".join(row))
            file.write("\n")
                
    time_elapsed = (round(time.time(), 8) - time_start) * 1000
    print(f"%.4f ms" % time_elapsed)
    
    memory_usage = process.memory_info().rss / 1024 / 1024 # Convert to kilobytes
    print("Memory usage:", memory_usage, "MB")
    
if __name__ == "__main__":
    main()