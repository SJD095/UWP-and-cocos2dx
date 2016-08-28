#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
	
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258 - 48 * i, 0, 42, 42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364, 0, 42, 42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp)
{
	Animation* anim = Animation::createWithSpriteFrames(monsterDead, 0.2f);
	Animate* ani = Animate::create(anim);
	Sequence* seq = Sequence::create(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), ani,  NULL);
	sp->runAction(seq);
	this -> monster.eraseObject(sp, true);
}
void Factory::moveMonster(Vec2 playerPos, float time)
{
	for (auto iter = this->monster.begin(); iter != this->monster.end(); iter++)
	{
		auto monsterPos = (*iter) -> getPosition();
		auto vector_to_player = playerPos - monsterPos;
		vector_to_player.normalize();
		(*iter)->runAction(MoveBy::create(time, vector_to_player * 10));
	}
}

Sprite* Factory::collider(Rect rect)
{
	for (auto sp : this->monster)
	{
		if (rect.containsPoint(sp->getPosition()))
		{
			return sp;
		}
	}
	return NULL;
}

bool Factory::attack_monster(Rect rect)
{
	Rect attack_rect = Rect(rect.getMinX() - 40, rect.getMinY() - 40, rect.getMaxX() + 40, rect.getMaxY() + 40);
	Sprite* sp = collider(attack_rect);
	if (sp != NULL)
	{
		removeMonster(sp);

		this -> KO = database->getIntegerForKey("kill");
		++KO;
		database->setIntegerForKey("kill", KO);

		return true;
	}
	return false;
}