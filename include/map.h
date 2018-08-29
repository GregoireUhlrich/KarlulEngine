#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "drawable.h"
#include "character.h"
#include "support.h"
#include "gameEvent.h"
#include "../include/supportWindow.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using namespace std;

// Structure permettant de stocker la position d'un sprite sur la map et dans son PNG //
struct sprite
{
    int x, y;
    int xPNG, yPNG;
};

struct gate
{
    int x, y;
    string name;
};

// Etat de l'éditeur (mode ajout, ecrasement, test du héros etc... //
enum StateMap{ moving, adding, erasing, selecting, heros, nothing};

// Etat de sauvegarde de la carte pour déterminer s'il faut demander confirmation à la fermeture de l'éditeur etc ... //
enum SaveStateMap { initialized, loaded, saved, edited};

// Différentes action à inverser lors d'un Ctrl+Z //
enum actionCtrlZ{ addCtrlZ, eraseCtrlZ, passOrNotCtrlZ};
    
class mapCtrlZ;
// Objet qui gère l'édition de la carte //
class mapi
{
    private:
    
    int x, y, lx, ly; // Position et taille de la carte dans la fenêtre SFML
    int lxMap, lyMap; // Taille de la carte en nombre de sprites
    int xSprites, ySprites; // Taille des sprites en pixel
    
    SaveStateMap saveState; // Etat de sauvegarde de la carte
        
    character* Heros; // Personnage du mode test, sprite ./Graphics/AC001.png
    
    StateMap state; // Etat de l'éditeur.
    
    bool isMouseHere;
    bool isMousePressed;
    bool click, select, select2; // booléens pour savoir à quel moment de la sélection no se trouve
    sf::RectangleShape selectRect; // Rectangle de sélection
    sf::Vector2i posMouse; // position de la souris
    sf::Vector2i oldPosMouse; // Ancienne position de la souris 
    sf::Vector2i posClick; // position du click 
    int delta; // Nombre de crans de la molette de la souris
    double iZoom; // Ratio de zoom pour un cran passé de la molette
    double minZoom, maxZoom; // 2chelles min et max pour le zoom
    bool allPrio; // si vrai: une seule priorité est affichée à l'écran (c'est mal fait oui)
    bool showPass; // si vrai: on montre les ordres de passages
    int currentPrio; // priorité de travail 
    
    bool grid; // si vrai: on affiche la grille sur la carte
    sf::RectangleShape* gridX; // Rectangles pour afficher la grille
    sf::RectangleShape* gridY;
    
    string stringFile; // Nom du fichier en cours pour la carte
    string stringDirPNG;
    
    vector<vector<vector<sf::Sprite > > > spriteVec;// Contient tous les sprites par prio puis numéro de texture
    int nTotTexture; // Nombre de textures ouvertes dans le programme
    vector<sf::Texture> textureVec;
    vector<string> fileTextureVec;
    vector<vector<int> > iTextureVec;
    vector<vector<vector<vector<int> > > > indexSpriteVec;// indice de la Texture dans "Texture" en fonction de la prio et numéro de Texture
    // index des sprites dans "Sprites" en fonction de la prio et de la position x,y
    int nPrio; // Nombre de priorités au total
    vector<int> nTextureVec;
    vector<vector<int> > nSpriteVec;
    int nExceptions; // nombre d'exceptions de passage
    vector<vector<vector<int> > > passOrNotVec;
    
    // Variables pour le ctrlC (Rectangle de sélection, priorité, sprites à afficher ...) //
    int xCtrlC;
    int yCtrlC;
    int nxCtrlC;
    int nyCtrlC;
    int prioCtrlC;
    sf::Sprite*** spriteCtrlC;
    string*** fileCtrlC;
    bool ghostSpriteCtrlC;
    
    sf::RenderWindow* window; // pointeur vers la fenêtre
    sf::RenderTexture mapWindow; // Texture sur laquelle on affiche les sprites
    sf::View viewMap; // Vue de la carte
    sf::RectangleShape boundary; // Rectangle sur la bordure de la carte
    sf::Sprite spriteToAdd; // image fantome de l'ensemble de sprites sélectionnés en mode add
    bool ghostSprite; // si vrai: on affiche le sprite fantome
    
    sf::Vector2u sizeWindow; // Taille de la fenêtre en pixels
    sf::Sprite toDraw; // Sprite final à afficher sur la fenêtre après avoir généré la carte
    bool isImLeft; // si vrai: on affiche l'image PNG à gauche de l'application
    imagePNG* imL; // imagePNG
    bool isImRight; // si vrai: on affiche l'image PNG à gauche de l'application
    imagePNG* imR; // imagePNG
    int sizeLim; // taille de la limite avec la zone de boutons
    sf::RectangleShape limL; 
    sf::RectangleShape limR; 
    sf::Sprite spriteMap;
    
    mapCtrlZ* ctrlZObject;
    
    int nGates;
    gate* gates;
    
    int nEvents;
    gameEvent** events;
    
    public:
    
    mapi(sf::RenderWindow* w, character* H, string f, int height); // f is the file of the map, height is the height of the map in pixel
    ~mapi();
    
    void initPNG(string f, char chirality); // creates a PNG on the left or the right of the window depending on chirality ('L' or 'R')
    void deletePNG(char chirality); // remove a PNG
    
    sf::Vector2u getSizeMap() const; // return sf::Vector2u(lxMap, lyMap)
    sf::Vector2u getSizeSprites() const;
    SaveStateMap getSaveState() const;
    int getNTextures() const;
    string* getFileTextures() const;
    bool isSaved() const;
    string getFile() const;
    string getImageFile() const;
    void setFile(string f);
    void setDirPNG(string f);
    string getDirPNG() const;
    void setState(StateMap s);
    void setSaveState(SaveStateMap s);
    void setGrid(bool g);
    void setPrio(int newPrio);
    void setAllPrio(bool a);
    void setShowPass(bool s);
    void setSelect(bool s, bool s2);
    void setSizeMap(sf::Vector2u s);
    void setLxMap(int lxNew);
    void setLyMap(int lyNew);
    void setPassOrNot(int pass, int prio, int ix, int iy);
    void pastePassOrNot(int dir, int prio, int xmin, int ymin, int lxS, int lyS);
    void resetTextureSprite();
    void setTakeMidX(bool s);
    void setTakeMidY(bool s);
    void setPosHeros(int xi, int yi);
    void setDirHeros(int dir);
    int loadMap();
    void saveMap();
    void initMap();
    void reinitMap();
    
    void ctrlC();
    void ctrlV();
    void ctrlX();
    void ctrlZ();
    void suppr();
    void freeSpritesCtrlC();
    
    sf::Vector2f convertPos(sf::Vector2i p);
    sf::Vector2i invConvertPos(sf::Vector2f p);
    
    int addTexture(string t);
    void removeTexture(int prio, int iT, string t);
    
    void addSprite(sprite s, int prio, string t);
    void addSprite2(sprite s, int prio, string t, int nxS, int nyS, int lxS, int lyS);
    void removeSprite(int prio, int ix, int iy);
    
    int testMouse(sf::Vector2i posMouse);
    void mousePressed(sf::Vector2i posMouse);
    void mouseReleased();
    void mouseWheel(int d);
    void windowResized(sf::Vector2u newSizeWindow);
    
    void setJoueur(bool s);
    void setHerosPos(sf::Vector2i p);
    bool testDir(int dir);
    bool downOK();
    bool upOK();
    bool leftOK();
    bool rightOK();
    
    void keyPressed(sf::Keyboard::Key k);
    
    void update(double eT);
    void draw();
    
    void closeWindow();
    
};

class mapCtrlZ
{
    private:
    
    mapi* M;
    int xSprites, ySprites;
    int memoryCtrlZ;
    int nCtrlZ;
    actionCtrlZ* action;
    int nAddCtrlZ;
    int** nSpriteAdd;
    int** posAddCtrlZ;
    int* prioAddCtrlZ;
    
    double thresholdTime;
    double elapsedTime;
    int nEraseCtrlZ;
    int* nSpriteErase;
    int** prioSpriteEraseCtrlZ;
    int** passSpriteEraseCtrlZ;
    sprite** spriteEraseCtrlZ;
    string** fileEraseCtrlZ;
    
    int nPassOrNotCtrlZ;
    int** nPassCtrlZ;
    int** initPosPassCtrlZ;
    int* passCtrlZ;
    int* prioPassCtrlZ;
    
    public:
    
    mapCtrlZ(mapi* Mi);
    ~mapCtrlZ();
    
    void addElapsedTime(double eT);
    
    void saveAdding(sprite s, int prio, int lxS, int lyS);
    void clearAdding();
    
    void saveErasing(sprite s, int prio, int pass, string t);
    void clearErasing();
    
    void saveChangingPass(int prio, int dir, int ix, int iy, int lx, int ly);
    void clearChangingPass();
    
    void CtrlZ();
    
};

#endif
