#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto spritecache = SpriteFrameCache::getInstance();

	spritecache->addSpriteFramesWithFile("level-sheet.plist");

	auto bg_sky = Sprite::create("level-background-0.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg_sky, 0);

	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height + origin.y - 170));
	this->addChild(stone, 0);

	mouse = Sprite::createWithSpriteFrameName("gem-mouse-1.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(Vec2(100 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(mouse, 0);

	auto MenuShoot = MenuItemFont::create("Shoot", CC_CALLBACK_1(GameSence::Shoot, this));
	auto menu = Menu::create(MenuShoot, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2 + origin.x + 300, visibleSize.height + origin.y - 170));
	this->addChild(menu, 1);

	return true;
}

void GameSence::Shoot(cocos2d::Ref* pSender)
{
	auto fadeOut = FadeOut::create(1.0f);
	auto mouseback = MoveTo::create(2, CCPoint(random()%100, random()%500));
	auto moveTo = MoveTo::create(0.2, mouse->getPosition());

	auto stone_tmp = Sprite::create("stone.png");
	stone_tmp->setPosition(stone->getPosition());
	this->addChild(stone_tmp);

	auto diamond = Sprite::createWithSpriteFrameName("diamond-0.png");
	diamond->setPosition(mouse->getPosition());
	this->addChild(diamond);

	GameSence::mouse->runAction(mouseback);
	auto seq = Sequence::create(moveTo, fadeOut, nullptr);
	stone_tmp->runAction(seq);
	
	return;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();

	auto chess = Sprite::create("cheese.png");
	chess->setPosition(location);
	this->addChild(chess, 0);

	auto fadeOut = FadeOut::create(3.0f);
	chess->runAction(fadeOut);

	auto moveTo = MoveTo::create(2, location);
	auto seq = Sequence::create(moveTo, nullptr);
	mouse->runAction(seq);

	return true;
}
