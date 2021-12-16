#pragma once

#include <map>
#include <memory>
#include <list>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SceneNode.h"
#include "ResourceHolder.h"

class Game
{
public:
    Game();
    ~Game();
    void run();
    static int mFPS;
    static int mScreenWidth;
    static int mScreenHeight;
    static int mCol;
    static int mRow;
    static float mGameScale;
    static sf::Vector2f mGameTableOrigin;
    static sf::Vector2i mCellSize;
    static std::vector<SceneNode::_ptr> mTable;
private:
    void processEvents();
    void renderFrame();
    void createRandomWalls(int amount);
    void createEnemies(int amount);
    // Affichage
    sf::RenderWindow mWindow;
    sf::View mView;
    // Ressources
    ResourceHolder<sf::Texture> mTextures;
    std::map<std::string, sf::Color> mColors;
    // Contenu
    SceneNode mScene;
    SpriteNode::_ptr mScreen_ptr;
    SceneNode::_ptr mBackground_ptr;
    SceneNode::_ptr mFloor_ptr;

    GameObject::_ptr mPlayer_ptr;
    int mWallsNb;
    int mEnemiesNb;
    // Etat du jeu

    bool mIsRunning;
};