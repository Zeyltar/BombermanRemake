#include "SceneNode.h"
#include "Game.h"
SceneNode::SceneNode()
    : mParent(SceneNode::_wkptr())
    , mChildren()
{}


void SceneNode::attachChild(SceneNode::_ptr child_ptr)
{
    child_ptr->mParent = weak_from_this();
    /* ^^^ "this" ne peut pas être stocké tel quel dans un "weak_ptr" */
    mChildren.push_back(child_ptr);
}


vector<SceneNode::_ptr> SceneNode::getChildren()
{
    return mChildren;
}


SceneNode::_ptr SceneNode::detachChild(SceneNode::_ptr child_ptr)
{
    SceneNode::_ptr detached_ptr = nullptr;
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (mChildren[i] == child_ptr)
        {
            // Extraction du noeud trouvé
            mChildren.erase(mChildren.begin() + i);
            /*^^^ "erase" n'est pas optimal*/
            detached_ptr = child_ptr;
            detached_ptr->mParent = SceneNode::_wkptr();
            // Pas besoin de continuer à chercher
            break;
        }
    }
    // Retourne le noeud extrait ou "nullptr" si introuvable
    return detached_ptr;
}


void SceneNode::draw
(sf::RenderTarget& window, sf::RenderStates states) const
{
    // Applique la transformation géométrique de l'instance sur la vue globale
    states.transform *= this->getTransform();
    // Dessine le graphisme que contient le noeud (s'il y en a un)
    this->drawNode(window, states);
    // Propage "draw" aux sous-noeuds
    for (SceneNode::_ptr child_ptr : mChildren)
    {
        child_ptr->draw(window, states);
    }
}


// Pas de graphisme associé dans la version générique
void SceneNode::drawNode
(sf::RenderTarget& window, sf::RenderStates states) const
{}


SpriteNode::SpriteNode()
    : mSprite()
{}


sf::Sprite& SpriteNode::getSprite()
{
    return mSprite;
}


void SpriteNode::drawNode
(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(mSprite, states);
}


DiskNode::DiskNode()
    : mCircleShape()
{}


sf::CircleShape& DiskNode::getCircleShape()
{
    return mCircleShape;
}


void DiskNode::drawNode
(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(mCircleShape, states);
}


TextNode::TextNode()
    : mText()
{}


sf::Text& TextNode::getText()
{
    return mText;
}


void TextNode::drawNode
(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(mText, states);
}


AnimatedNode::AnimatedNode
(sf::Vector2i subSize, sf::Time duration, sf::Texture& texture, bool repeat)
    : mSubSize(subSize)
    , mDuration(duration)
    , mElapsed(sf::Time::Zero)
    , mRepeat(repeat)
    , mNbLi()
    , mNbCo()
    , mNbFrame()
{
    getSprite().setTexture(texture);
    getSprite().setTextureRect({ 0,0,mSubSize.x,mSubSize.y });
    getSprite().setOrigin((sf::Vector2f)(mSubSize / 2));
    mNbLi = getSprite().getTexture()->getSize().y / subSize.y;
    mNbCo = getSprite().getTexture()->getSize().x / subSize.x;
    mNbFrame = mNbLi * mNbCo;
};

void AnimatedNode::processNode()
{
    if (mElapsed > mDuration)
    {
        if (mRepeat)
        {
            mElapsed = sf::Time::Zero;
        }
        else
        {
            return;
        }
    }
    // Calcul du numéro de l'image à afficher
    int frameIndex = (mElapsed / mDuration) * mNbFrame;
    // Décalage dans la matrice
    sf::IntRect rect;
    rect.top = (frameIndex / mNbCo) * mSubSize.y;
    rect.left = (frameIndex % mNbCo) * mSubSize.x;
    rect.width = mSubSize.x;
    rect.height = mSubSize.y;
    getSprite().setTextureRect(rect);
    // Décalage dans le temps
    mElapsed += sf::seconds(1.f / Game::mFPS);
}

GameObject::GameObject(string tag, sf::Vector2i subSize, int x, int y)
    : mSprite()
    , mTag(tag)
    , mSubSize(subSize)
    , mX(x)
    , mY(y)
{
}

sf::Sprite& GameObject::getSprite()
{
    return mSprite;
}

void GameObject::setSelf()
{
    sf::Vector2f position = Game::mGameTableOrigin;
    position.x += mX * mSubSize.x;
    position.y += mY * mSubSize.y;
    position *= Game::mGameScale;

    mSprite.setPosition(position);
}

void GameObject::drawNode
(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(mSprite, states);
}
