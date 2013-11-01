
#import <string>
#import <Gosu/Color.hpp>

class Player
{
    std::string name;
    double score = 0.0;
    Gosu::Color color;
public:
    Player(std::string name, Gosu::Color col);
    
    // getters
    std::string getName() const { return name; }
    double getScore() const { return score; }
    Gosu::Color getColor() const { return color; }
    
    // setters
    void setScore(double s) { score = s; }
    void addScore(double s) { score += s; }
    void setColor(Gosu::Color col) { color = col; }
};
