#import "Game.hpp"

Game::Game(std::vector<Player> players)
:players(players)
{
}

void TurnbasedGame::endTurn()
{
    if (getWinner()) return;
    currentPlayer++;
    if (currentPlayer >= players.size()) {
        currentPlayer = 0;
    }
}

unmanaged_ptr<Player> ScoredGame::getWinner()
{
    if (winner) return winner;
    for (auto& player : players) {
        if (player.getScore() > winningScore) {
            winner = &player;
            return winner;
        }
    }
    return nullptr;
}

