#pragma once
#include "cocos2d.h"

#define database UserDefault::getInstance()

using namespace cocos2d;

class Factory :public cocos2d::Ref {
public:
	//��ȡ��������
	static Factory* getInstance();
	//����һ��������洢�������й���
	Sprite* createMonster();
	//�������е����й��ﶼ����ɫ�ƶ���ͨ�������������еĹ���ܷ���
	void moveMonster(Vec2 playerPos, float time);
	//�Ƴ�����
	void removeMonster(Sprite*);
	//�ж���ײ
	Sprite* collider(Rect rect);
	//��ʼ������֡����
	void initSpriteFrame();

	bool attack_monster(Rect Play);

	Factory();
	Vector<Sprite*> monster;
	cocos2d::Vector<SpriteFrame*> monsterDead;
	static Factory* factory;
	int KO = 0;
};