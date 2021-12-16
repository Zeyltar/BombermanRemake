#pragma once

#include <memory>
#include <vector>
using namespace std;

#include <SFML/Graphics.hpp>


class SceneNode
	: public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	, public std::enable_shared_from_this<SceneNode>
{
public:
	typedef shared_ptr<SceneNode> _ptr;
	typedef weak_ptr<SceneNode> _wkptr;

	SceneNode();

	vector<_ptr> getChildren();
	void attachChild(_ptr child_ptr);
	_ptr detachChild(_ptr child_ptr);

private:

	virtual void draw
	(sf::RenderTarget& window, sf::RenderStates states) const;
	virtual void drawNode
	(sf::RenderTarget& window, sf::RenderStates states) const;
	/* ^^^ "drawNode" doit être "const" car il est appelé par "draw" */
	_wkptr mParent;
	vector<_ptr> mChildren;
};


class SpriteNode : public SceneNode
{
public:
	typedef shared_ptr<SpriteNode> _ptr;
	SpriteNode();
	sf::Sprite& getSprite();
private:
	virtual void drawNode
	(sf::RenderTarget& window, sf::RenderStates states) const;
	sf::Sprite mSprite;
};


class DiskNode : public SceneNode
{
public:
	typedef shared_ptr<DiskNode> _ptr;
	DiskNode();
	sf::CircleShape& getCircleShape();
private:
	virtual void drawNode
	(sf::RenderTarget& window, sf::RenderStates states) const;
	sf::CircleShape mCircleShape;
};


class TextNode : public SceneNode
{
public:
	typedef shared_ptr<TextNode> _ptr;
	TextNode();
	sf::Text& getText();
private:
	virtual void drawNode
	(sf::RenderTarget& window, sf::RenderStates states) const;
	sf::Text mText;
};


class AnimatedNode : public SpriteNode
{
public:
	typedef shared_ptr<AnimatedNode> _ptr;
	AnimatedNode
	(sf::Vector2i subSize, sf::Time duration, sf::Texture& texture, bool repeat = false);
	void processNode();
private:
	sf::Vector2i mSubSize;
	sf::Time mDuration;
	sf::Time mElapsed;
	bool mRepeat;

	int mNbLi;
	int mNbCo;
	int mNbFrame;
};

class GameObject : public SpriteNode
{
public:
	typedef shared_ptr<GameObject> _ptr;
	GameObject
	(string tag, sf::Vector2i subSize, int x, int y);
	int mX, mY;
	sf::Sprite& getSprite();

	void setSelf();
private:
	virtual void drawNode
	(sf::RenderTarget& window, sf::RenderStates states) const;
	string mTag;
	sf::Vector2i mSubSize;
	sf::Sprite mSprite;
};

