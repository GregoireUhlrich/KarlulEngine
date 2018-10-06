#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;


///*** Class for the heros ***///
class character{

    protected:
    
    string name;
    string file;
    int direction;
    int sx, sy, dx, dy;
    int xSprites, ySprites;
    bool onGrid;
    double typeSprite;
    double x,y;
    
    sf::Texture texture;
    sf::Sprite sprite;
    
    public: 
    
    character();
    character(string name, string file, double x, double y);
    character(string file, int xi, int yi, int dir);
    character(const character& c);
    ~character(){};
    
    string getName() const;
    string getFile() const;
    double getX() const;
    double getY() const;
    int getDir() const;
    double getTypeSprite() const;
    sf::Sprite getSprite() const;
    bool isOnGrid() const;
    sf::Vector2u getSizeSprite() const;
    
    void setName(string n);
    void setFile(string f);
    void setX(double xNew);
    void setY(double yNew);
    void setDir(int d);
    
    virtual void update(double elapsedTime);
    
    void print() const;
    character& operator=(const character& c);
};

class hero: public character{

    private:
    
    int wantedMove;
    double timeKeyPressed;
    double thresholdMove;
    bool action;
    int keyPressed;
    bool moveEnabled;
    double speed;
    double walk;
    
    public:
    
    hero(string name, string file, double x, double y);
    ~hero(){};
    
    int getWantedMove() const;
    int getKeyPressed() const;
    
    void setWantedMove(int wM);
    void setAction(bool a);
    bool pullAction();
    void releaseKey();
    void setKeyPressed(int key);
    void enableMove();
    void disableMove();
    
    void move(double elapsedTime);
    void moveCarefully(double elapsedTime);
    void update(double elpasedTime);
    void updateSprite(double elapsedTime);
    
};

class ListCharacter
{
    private:
    
    int nCharacter;
    vector<character*> list;
    
    sf::RenderWindow* window;
    
    public:
    
    ListCharacter();
    ListCharacter(sf::RenderWindow* w);
    ~ListCharacter();
    
    void setWindow(sf::RenderWindow* w);
    void addCharacter(character* c);
    void deleteCharacter(character* c);
    
    void draw();
};

#endif
