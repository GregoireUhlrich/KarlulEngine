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
    float typeSprite;
    float x,y;
    
    sf::Texture texture;
    sf::Sprite sprite;
    
    public: 
    
    character();
    character(string name, string file, float x, float y);
    character(string file, int xi, int yi, int dir);
    character(const character& c);
    ~character(){};
    
    string getName() const;
    string getFile() const;
    float getX() const;
    float getY() const;
    int getDir() const;
    float getTypeSprite() const;
    sf::Sprite getSprite() const;
    bool isOnGrid();
    sf::Vector2u getSizeSprite() const;
    
    void setName(string n);
    void setFile(string f);
    void setX(float xNew);
    void setY(float yNew);
    void setDir(int d);
    
    virtual void update(float elapsedTime);
    
    void print() const;
    character& operator=(const character& c);
};

class hero: public character{

    private:
    
    int wantedMove;
    float timeKeyPressed;
    float thresholdMove;
    bool action;
    int keyPressed;
    bool moveEnabled;
    float speed;
    float walk;
    
    public:
    
    hero(string name, string file, float x, float y);
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
    
    void move(float elapsedTime);
    void moveCarefully(float elapsedTime);
    void update(float elpasedTime);
    void updateSprite(float elapsedTime);
    
};

class ListCharacter
{
    private:
    
    int nCharacter;
    vector<character*> list;
    int xSprites, ySprites;
    
    sf::RenderTexture* window;
    
    public:
    
    ListCharacter();
    ListCharacter(sf::RenderTexture* w);
    ~ListCharacter();
    
    bool testPNJ(int ix, int iy, int dir);
    void setWindow(sf::RenderTexture* w);
    void addCharacter(character* c);
    void deleteCharacter(character* c);
    
    void draw();
};

#endif
