#import "Player.hpp"
#import <vector>
#import <limits>
#import "unmanaged_ptr.hpp"

class Game
{
protected:
    std::vector<Player> players;
public:
    Game(std::vector<Player> players);
    const std::vector<Player>& getPlayers() const { return players; }
};

class ScoredGame : public Game
{
    double winningScore = std::numeric_limits<double>::max();
    unmanaged_ptr<Player> winner;
public:
    using Game::Game;
    void setWinningScore(double score) { winningScore = score; }
    unmanaged_ptr<Player> getWinner();
};

class TurnbasedGame : public ScoredGame
{
private:
    size_t currentPlayer = 0;
public:
    using ScoredGame::ScoredGame;
    void endTurn();
    unmanaged_ptr<Player> getCurrentPlayer() { return &players.at(currentPlayer); }
};

