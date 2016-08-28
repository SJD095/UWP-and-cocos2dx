#include "HelloWorldScene.h"
#include "Monster.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
#include <string>

char last = 'D';

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	addChild(player, 3);

	database->setIntegerForKey("kill", 0);

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	//倒计时
	time = Label::createWithTTF(ttfConfig, "0");
	//倒计时周期性调用调度器
	schedule(schedule_selector(HelloWorld::random_create_monster), 5.0f, kRepeatForever, 0);

	schedule(schedule_selector(HelloWorld::monster_move_and_check), 2.0f, kRepeatForever, 0);

	schedule(schedule_selector(HelloWorld::check), 0.1f, kRepeatForever, 0);
	//倒计时的数字
	dtime = 0;
	time->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - time->getContentSize().height));
	addChild(time, 1);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 1);

	//静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	//攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i, 0, 113, 113)));
		attack.pushBack(frame);
	}

	//死亡动画
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(23);
	for (int i = 0; i < 23; i++) {
		int j = i;
		if (i == 22) j = 0;
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * j, 0, 79, 90)));
		dead.pushBack(frame);
	}

	//运动动画
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i + 12, 0, 65, 101)));
		run.pushBack(frame);
	}

	//Label
	auto menuLabel1 = Label::createWithTTF(ttfConfig, "W");
	auto menuLabel2 = Label::createWithTTF(ttfConfig, "S");
	auto menuLabel3 = Label::createWithTTF(ttfConfig, "A");
	auto menuLabel4 = Label::createWithTTF(ttfConfig, "D");
	auto menuLabel5 = Label::createWithTTF(ttfConfig, "X");
	//menuItem
	auto item1 = MenuItemLabel::create(menuLabel1, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'W'));
	auto item2 = MenuItemLabel::create(menuLabel2, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'S'));
	auto item3 = MenuItemLabel::create(menuLabel3, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'A'));
	auto item4 = MenuItemLabel::create(menuLabel4, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'D'));
	auto item5 = MenuItemLabel::create(menuLabel5, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'X'));

	item3->setPosition(Vec2(origin.x + item3->getContentSize().width, origin.y + item3->getContentSize().height));
	item4->setPosition(Vec2(item3->getPosition().x + 3 * item4->getContentSize().width, item3->getPosition().y));
	item2->setPosition(Vec2(item3->getPosition().x + 1.5*item2->getContentSize().width, item3->getPosition().y));
	item1->setPosition(Vec2(item2->getPosition().x, item2->getPosition().y + item1->getContentSize().height));
	item5->setPosition(Vec2(origin.x + visibleSize.width - item5->getContentSize().width, item1->getPosition().y));

	auto menu = Menu::create(item1, item2, item3, item4, item5, NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 1);

	TMXTiledMap* map = TMXTiledMap::create("map.tmx");
	map->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	map->setAnchorPoint(Vec2(0.5, 0.5));
	map->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(map, 0);

	return true;
}


void HelloWorld::moveEvent(Ref*, char cid) {
	switch (cid) {
	case 'W':
		{
			int move = player->getPosition().y + 30 >= visibleSize.height ? visibleSize.height - player->getPosition().y : 30;
			auto walk = Animation::createWithSpriteFrames(run, 0.025f);
			auto playerwalk = Animate::create(walk);
			auto moveUp = MoveBy::create(0.3, Point(0, move));
			player->runAction(moveUp);
			player->runAction(playerwalk);
			break;
		}
	case 'A':
		{
			if (last == 'D')
			{
				player->setFlippedX(true);
				last = 'A';
			}

			int move = player->getPosition().x - 30 <= 0 ? -player->getPosition().x + 15 : -30;
			auto walk = Animation::createWithSpriteFrames(run, 0.025f);
			auto playerwalk = Animate::create(walk);
			auto moveLeft = MoveBy::create(0.3, Point(move, 0));
			player->runAction(moveLeft);
			player->runAction(playerwalk);
			break;
		}
	case 'S':
		{
			int move = player->getPosition().y - 30 <= 0 ? -player->getPosition().y + 15 : -30;
			auto walk = Animation::createWithSpriteFrames(run, 0.025f);
			auto playerwalk = Animate::create(walk);
			auto moveUp = MoveBy::create(0.3, Point(0, move));
			player->runAction(moveUp);
			player->runAction(playerwalk);
			break;
		}
	case 'D':
		{
			if (last == 'A')
			{
				player->setFlippedX(false);
				last = 'D';
			}

			int move = player->getPosition().x + 30 >= visibleSize.width ? visibleSize.width - player->getPosition().x : 30;
			auto walk = Animation::createWithSpriteFrames(run, 0.025f);
			auto playerwalk = Animate::create(walk);
			auto moveUp = MoveBy::create(0.3, Point(move, 0));
			player->runAction(moveUp);
			player->runAction(playerwalk);
			break;
		}
	}
}


void HelloWorld::actionEvent(Ref*, char cid) {
	auto fac = Factory::getInstance();
	switch (cid) {
	case 'X':
		{
			auto atk = Animation::createWithSpriteFrames(attack, 0.025f);
			auto playerattack = Animate::create(atk);
			player->runAction(playerattack);
			
			if (fac->attack_monster(player->getBoundingBox()))
			{
				char str[5];
				itoa(database->getIntegerForKey("kill"), str, 10);
				time->setString(str);

				int hp = pT->getPercentage();
				hp = hp + 20 > 100 ? 100 : hp + 20;
				pT->setPercentage(hp);
			}
			break;
		}
	}
}

void HelloWorld::stopAc() {
	Director::getInstance()->pause();
}

void HelloWorld::random_create_monster(float dt) {
	auto fac = Factory::getInstance();
	
	if (fac->monster.size() < 15)
	{
		auto monster = fac->createMonster();
		float x = random(origin.x, visibleSize.width);
		float y = random(origin.y, visibleSize.height);
		monster->setPosition(x, y);
		fac->monster.pushBack(monster);
		addChild(monster, 3);
	}
	//time->setString(str);
}

int tmp = 0;

void HelloWorld::monster_move_and_check(float dt)
{
	auto fac = Factory::getInstance();
	auto playerPos = player->getPosition();
	fac->moveMonster(playerPos, 1);
}

void HelloWorld::check(float dt)
{
	auto fac = Factory::getInstance();
	if (fac->collider(player->getBoundingBox()) != NULL)
	{
		fac->removeMonster(fac->collider(player->getBoundingBox()));

		auto die = Animation::createWithSpriteFrames(dead, 0.025f);
		auto playerdie = Animate::create(die);
		player->runAction(playerdie);

		int hp = pT->getPercentage();
		hp = hp - 20 > 0 ? hp - 20 : 0;
		pT->setPercentage(hp);
	}
}