#include "BreakoutScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

void Breakout::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* Breakout::createScene() {
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Point(0, 0));

    auto layer = Breakout::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}

bool Breakout::init() {
    if (!Layer::init()) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

    preloadMusic();
    playBgm();

    addBackground();
    addEdge();
    addPlayer();

	addContactListener();
    addKeyboardListener();

    this->schedule(schedule_selector(Breakout::update), 1);
	this->schedule(schedule_selector(Breakout::update2), 1, kRepeatForever, 0);
	this->schedule(schedule_selector(Breakout::update3), 1, kRepeatForever, 0);
	this ->schedule(schedule_selector(Breakout::fires), 0.1f, kRepeatForever, 0);
	
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	time = Label::createWithTTF(ttfConfig, "30");

	time->setPosition(Vec2(origin.x + visibleSize.width * 5/ 6,
		origin.y + visibleSize.height - time->getContentSize().height));
	addChild(time, 1);

	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height - 15));
	addChild(pT, 2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height - 15));
	addChild(sp0, 1);

	auto Listener = EventListenerPhysicsContact::create();
	Listener->onContactBegin = CC_CALLBACK_1(Breakout::onConcactBegan, this);
    return true;
}

void Breakout::preloadMusic() {
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
}

void Breakout::playBgm() {
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

void Breakout::addBackground() {
    auto bgsprite = Sprite::create("black_hole_bg0.jpg");
    bgsprite->setPosition(visibleSize / 2);
    bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, visibleSize.height / bgsprite->getContentSize().height);
    this->addChild(bgsprite, 0);

    auto ps = ParticleSystemQuad::create("black_hole.plist");
    ps->setPosition(visibleSize / 2);
    this->addChild(ps);
}

void Breakout::addEdge() {
    auto edgeSp = Sprite::create();
    auto boundBody = PhysicsBody::createEdgeBox(visibleSize);
	boundBody->setDynamic(false);
    boundBody->setTag(0);
    edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    edgeSp->setPhysicsBody(boundBody);
    this->addChild(edgeSp);
}

void Breakout::addPlayer() {
    player = Sprite::create("player.png");
    player->setAnchorPoint(Vec2(0.5, 0.5));
    player->setPhysicsBody(PhysicsBody::createCircle(35));
	// TODO set bit mask
	player->getPhysicsBody()->setCategoryBitmask(0x01);
	player->getPhysicsBody()->setCollisionBitmask(0x02);
	player->getPhysicsBody()->setContactTestBitmask(0x02);

    player->setPosition(visibleSize / 2);
    player->getPhysicsBody()->setTag(1);
    player->getPhysicsBody()->setAngularVelocityLimit(0);
    addChild(player);
}

void Breakout::addContactListener() {
	auto touchListener = EventListenerPhysicsContact::create();
	touchListener->onContactBegin = CC_CALLBACK_1(Breakout::onConcactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
}

void Breakout::addKeyboardListener() {
    auto keboardListener = EventListenerKeyboard::create();
    keboardListener->onKeyPressed = CC_CALLBACK_2(Breakout::onKeyPressed, this);
    keboardListener->onKeyReleased = CC_CALLBACK_2(Breakout::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keboardListener, this);
}

bool Breakout::onConcactBegan(PhysicsContact& contact) {
	auto xueliang = pT->getPercentage();

	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();

	auto sp1 = (Sprite*)body1->getNode();
	auto sp2 = (Sprite*)body2->getNode();

	if (sp1 != NULL && sp2 != NULL)
	{
		if ( sp1->getPhysicsBody()->getTag() != 50 && sp2->getPhysicsBody()->getTag() != 50)
		{
			if (sp1 == player || sp2 == player) xueliang = (xueliang - 7) > 0 ? xueliang - 7 : 0;
		}
		if (xueliang == 0)
		{
			auto origin = Director::getInstance()->getVisibleOrigin();
			TTFConfig ttfConfig;
			ttfConfig.fontFilePath = "fonts/arial.ttf";
			ttfConfig.fontSize = 72;
			auto fail_label = Label::createWithTTF(ttfConfig, "GAME OVER!");

			fail_label->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height / 2));
			addChild(fail_label, 1);

			Director::getInstance()->pause();
		}
		pT->setPercentage(xueliang);

		if (sp1->getPhysicsBody()->getTag() == 40 || sp2->getPhysicsBody()->getTag() == 40)
		{
			if (sp1 != NULL && sp2 != NULL)
			{
				if (sp1->getTag() == 40 && sp2 == player)
				{
					removeChild(sp1);
					shoot_allow = shoot_allow + 10;
				}

				else if (sp2->getTag() == 40 && sp1 == player)
				{
					removeChild(sp2);
					shoot_allow = shoot_allow + 10;
				}
			}

		}

		else if ( sp1->getPhysicsBody()->getTag() != 50 && sp2->getPhysicsBody()->getTag() != 50)
		{
			ParticleExplosion* fireworks = ParticleExplosion::create();
			fireworks->setPosition(sp1->getPosition());

			fireworks->setDuration(ParticleSystem::DURATION_INFINITY);

			fireworks->setEmitterMode(ParticleSystem::Mode::RADIUS);

			fireworks->setStartRadius(0);
			fireworks->setEndRadius(10);
			cocos2d::Color3B red(255, 0, 0);
			fireworks->setColor(red);
			addChild(fireworks);

			SimpleAudioEngine::getInstance()->playEffect("music/meet_stone.mp3");

			if (sp1 == player)
			{
				removeChild(sp2);
			}

			else
			{
				removeChild(sp1);
			}
		}

		else
		{
			if (sp1->getPhysicsBody()->getTag() == 50)
			{
				bullets[sp1->getTag()] = NULL;
				removeChild(sp1);
				removeChild(sp2);
			}
			else
			{
				bullets[sp2->getTag()] = NULL;
				removeChild(sp1);
				removeChild(sp2);
			}
		}
	}

	return true;
}

void Breakout::update(float f) {
    newEnemys();
}

void Breakout::newEnemys() {
	if (enemys.size() > 50) return;
    int newNum = 2;
    while (newNum--) {
        int type = 0;
        if (CCRANDOM_0_1() > 0.85) { type = 2; }
        else if (CCRANDOM_0_1() > 0.6) { type = 1; }
		else if (CCRANDOM_0_1() > 0.4) { type = 3; }

        Point location = Vec2(0, 0);
        switch (rand() % 4)
        {
        case 0:
            location.y = visibleSize.height;
            location.x = rand() % (int)(visibleSize.width);
            break;
        case 1:
            location.x = visibleSize.width;
            location.y = rand() % (int)(visibleSize.height);
            break;
        case 2:
            location.y = 0;
            location.x = rand() % (int)(visibleSize.width);
            break;
        case 3:
            location.x = 0;
            location.y = rand() % (int)(visibleSize.height);
            break;
        default:
            break;
        }
        addEnemy(type, location);
    }
}

void Breakout::addEnemy(int type, Point p) {
    char path[100];
    int tag;
    switch (type)
    {
    case 0:
        sprintf(path, "stone1.png");
        tag = 10;
        break;
    case 1:
        sprintf(path, "stone2.png");
        tag = 20;
        break;
    case 2:
        sprintf(path, "stone3.png");
        tag = 30;
        break;
	case 3:
		sprintf(path, "item3.png");
		tag = 40;
		break;
    default:
        sprintf(path, "item3.png");
        tag = 40;
        break;
    }
    auto re = Sprite::create(path);
    re->setPhysicsBody(PhysicsBody::createCircle(re->getContentSize().height / 2));
    re->setAnchorPoint(Vec2(0.5, 0.5));
    re->setScale(0.5, 0.5);
    re->setPosition(p);

	re->getPhysicsBody()->setCategoryBitmask(0x02);
	re->getPhysicsBody()->setCollisionBitmask(0x01);
	re->getPhysicsBody()->setContactTestBitmask(0x01);

	re->setTag(tag);

    re->getPhysicsBody()->setTag(tag);
    if (rand() % 100 < 50) {
        re->getPhysicsBody()->setVelocity((player->getPosition() - p) * (0.25));
    }
    else {
        re->getPhysicsBody()->setVelocity((Point(rand() % (int)(visibleSize.width - 100) + 50, rand() % (int)(visibleSize.height - 100) + 50) - p) * (0.25));
    }
    re->getPhysicsBody()->setAngularVelocity(CCRANDOM_0_1() * 10);
    enemys.pushBack(re->getPhysicsBody());
    addChild(re);
}

void Breakout::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        player->getPhysicsBody()->setVelocity(Point(-200, player->getPhysicsBody()->getVelocity().y));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        player->getPhysicsBody()->setVelocity(Point(200, player->getPhysicsBody()->getVelocity().y));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, 200));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, -200));
        break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		fire = true;
		break;
    default:
        break;
    }
}

void Breakout::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(-200, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(200, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, 200));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, -200));
        break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		fire = false;
		break;
    default:
        break;
    }
}

void Breakout::update2(float f)
{
	stringstream ss;
	time_limit = time_limit - f;
	ss << time_limit;
	string tmp;
	tmp = ss.str();

	time -> setString(tmp);
	if (time_limit == 0)
	{
		auto origin = Director::getInstance()->getVisibleOrigin();
		TTFConfig ttfConfig;
		ttfConfig.fontFilePath = "fonts/arial.ttf";
		ttfConfig.fontSize = 72;

		auto win_label = Label::createWithTTF(ttfConfig, "YOU WIN!");

		win_label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height/2));
		addChild(win_label, 1);
		
		Director::getInstance()->pause();
	}
}

void Breakout::update3(float f) {
	shoot_allow = (shoot_allow >= 1) ? shoot_allow - 1 : 0;
}

void Breakout::fires(float f) {
	if (fire == true && shoot_allow > 0)
	{
		auto bullet = Sprite::create("bullet.png");
		bullet->setAnchorPoint(Vec2(0.5, 0.5));
		bullet->setPhysicsBody(PhysicsBody::createCircle(35));
		// TODO set bit mask
		bullet->getPhysicsBody()->setCategoryBitmask(0x01);
		bullet->getPhysicsBody()->setCollisionBitmask(0x02);
		bullet->getPhysicsBody()->setContactTestBitmask(0x02);

		bullet->setPosition(visibleSize / 2);
		bullet->setTag(bullets.size());
		bullet->getPhysicsBody()->setTag(50);
		bullets.push_back(bullet);
		bullets.back()->setPosition(player->getPosition());
		addChild(bullets.back());

		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("fire.wav");

	}

	for (unsigned i = 0; i < bullets.size(); i++) {

		if (bullets[i] != NULL && bullets[i]->getPhysicsBody() ->getPosition().y + 50 > visibleSize.height)
		{
			removeChild(bullets[i]);
			bullets[i] = NULL;
		}

		if (bullets[i] != NULL) {
			bullets[i]->getPhysicsBody()->setVelocity(Point(bullets[i]->getPhysicsBody()->getVelocity().x, 500));
		}
	}
}