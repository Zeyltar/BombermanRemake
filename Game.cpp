#include "Game.h"

int Game::mFPS = 60;
int Game::mScreenWidth = 256;
int Game::mScreenHeight = 224;
int Game::mCol = 10;
int Game::mRow = 8;
float Game::mGameScale = 2.f;
sf::Vector2f Game::mGameTableOrigin(49.f, 41.f);
std::vector<SceneNode::_ptr> Game::mGameTable = std::vector<SceneNode::_ptr>();
sf::Vector2i Game::mCellSize(16, 16);

Game::Game()
	: mWindow(sf::VideoMode(mScreenWidth* (int)mGameScale, mScreenHeight* (int)mGameScale), "BOMBERMAN!")
	, mView(mWindow.getDefaultView())
	, mTextures()
	, mScene()
	, mBackground_ptr()
	, mFloor_ptr()
	, mIsRunning(false)
	, mWallsNb(5)
	, mEnemiesNb(3)
{
	mWindow.setFramerateLimit(mFPS);

	std::vector<std::string> resources({ "borders","characters","enemies", "tileset" });
	{
		for (auto&& name : resources)
		{
			mTextures.load(name, "./assets/" + name + ".png");
		}
	}

	mColors["floor"] = sf::Color(240, 176, 208, 255);
	mColors["background"] = sf::Color(192, 144, 96, 128);
	mColors["object0"] = sf::Color(192, 144, 96, 255);
	mColors["object1"] = sf::Color(128, 64, 48, 255);
	mColors["object2"] = sf::Color(48, 0, 0, 255);
	
	initializeBorders();
	initializeFloor();
	initializeGameTable();
	initializePlayer(0, 0);
	createRandomWalls(mWallsNb);
	createEnemies(mEnemiesNb);
	
}

Game::~Game()
{
	mWindow.close();
}

void Game::run()
{
	mIsRunning = true;
	sf::Clock clock;
	sf::Time timePerFrame = sf::seconds(1.f / mFPS);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mIsRunning)
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
		}
		renderFrame();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mIsRunning = false;
		default:
			break;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
	}

}

void Game::renderFrame()
{
	mWindow.setView(mView);
	mWindow.clear(mColors["floor"]);

	mWindow.draw(mScene);
	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}

void Game::initializeBorders()
{
	mScreen_ptr = std::make_shared<SpriteNode>();
	mScreen_ptr->getSprite().setTexture(mTextures.get("borders"));
	mScreen_ptr->getSprite().setTextureRect({ 1 + (mScreenWidth + 1), 1, mScreenWidth, mScreenHeight });
	mScreen_ptr->getSprite().scale(sf::Vector2f(1.f, 1.f) * mGameScale);
	mScene.attachChild(mScreen_ptr);
}

void Game::initializeFloor()
{
	mFloor_ptr = std::make_shared<SceneNode>();
	for (int x = 0; x < mCol; x++)
	{
		for (int y = 0; y < mRow; y++)
		{
			sf::Vector2f position = mGameTableOrigin;
			position.x += x * mCellSize.x;
			position.y += y * mCellSize.y;
			position *= mGameScale;

			SpriteNode::_ptr floor_ptr = std::make_shared<SpriteNode>();
			floor_ptr->getSprite().setTexture(mTextures.get("tileset"));
			floor_ptr->getSprite().setTextureRect({ 103, 1, mCellSize.x, mCellSize.y });
			floor_ptr->getSprite().scale(mGameScale, mGameScale);
			floor_ptr->getSprite().setColor(mColors["background"]);
			floor_ptr->getSprite().setPosition(position);

			mFloor_ptr->attachChild(floor_ptr);
		}
	}
	mScene.attachChild(mFloor_ptr);
}

void Game::initializeGameTable()
{
	for (int x = 0; x < mCol; x++)
	{
		for (int y = 0; y < mRow; y++)
		{
			SceneNode::_ptr cell_ptr = std::make_shared<SceneNode>();
			mGameTable.push_back(cell_ptr);
			mScene.attachChild(cell_ptr);
		}
	}
}

void Game::initializePlayer(int x, int y)
{
	if (x >= mCol || x < 0 || y >= mRow || y < 0)
		throw std::invalid_argument("player out of world");

	mPlayer_ptr = make_shared<GameObject>("Player", mCellSize, x, y);
	mPlayer_ptr->getSprite().setTexture(mTextures.get("characters"));
	mPlayer_ptr->getSprite().setTextureRect({ 1, 1, mCellSize.x, mCellSize.y });
	mPlayer_ptr->getSprite().scale(mGameScale, mGameScale);
	mPlayer_ptr->getSprite().setColor(mColors["object1"]);
	mPlayer_ptr->setSelf();
	mScene.attachChild(mPlayer_ptr);
}

void Game::createRandomWalls(int amount)
{
	int count = 0;
	while (count < amount)
	{
		
		std::srand(std::time(0));
		int x = rand() % mCol;
		int y = rand() % mRow;

		while (mGameTable[y * mCol + x]->getChildren().size() > 0)
		{
			x = rand() % mCol;
			y = rand() % mRow;
		}

		GameObject::_ptr go_ptr = std::make_shared<GameObject>("Wall", mCellSize, x, y);
		go_ptr->getSprite().setTexture(mTextures.get("tileset"));
		go_ptr->getSprite().setTextureRect({ 18, 1, mCellSize.x, mCellSize.y });
		go_ptr->getSprite().scale(mGameScale, mGameScale);
		go_ptr->getSprite().setColor(mColors["object0"]);
		go_ptr->setSelf();
		
		std::cout << "x: " << x << " y: " << y << std::endl;

		mGameTable[y * mCol + x]->attachChild(go_ptr);
		count++;
	}
}

void Game::createEnemies(int amount)
{
	int count = 0;
	while (count < amount)
	{

		std::srand(std::time(0));
		int x = rand() % mCol;
		int y = rand() % mRow;

		while (mGameTable[y * mCol + x]->getChildren().size() > 0)
		{
			x = rand() % mCol;
			y = rand() % mRow;
		}

		GameObject::_ptr go_ptr = std::make_shared<GameObject>("Enemy", mCellSize, x, y);
		go_ptr->getSprite().setTexture(mTextures.get("enemies"));
		go_ptr->getSprite().setTextureRect({ 52, 18, mCellSize.x, mCellSize.y });
		go_ptr->getSprite().scale(mGameScale, mGameScale);
		go_ptr->getSprite().setColor(mColors["object1"]);
		go_ptr->setSelf();

		std::cout << "x: " << x << " y: " << y << std::endl;

		mGameTable[y * mCol + x]->attachChild(go_ptr);
		count++;
	}
}
