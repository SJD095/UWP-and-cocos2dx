#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}





// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
    this->addChild(textField, 2);

    auto button = Button::create();
    button->setTitleText("Login");
    button->setTitleFontSize(30);
    button->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	button->addClickEventListener(CC_CALLBACK_1(LoginScene::login, this));
    this->addChild(button, 2);

	auto auto_button = Button::create();
	auto_button->setTitleText("Auto Login");
	auto_button->setTitleFontSize(30);
	auto_button->setPosition(Size(visibleWidth / 2, visibleHeight / 2 - 40));
	auto_button->addClickEventListener(CC_CALLBACK_1(LoginScene::auto_login, this));
	this->addChild(auto_button, 2);

    return true;
}
//���Login��������¼
void LoginScene::login(cocos2d::Ref* pSender)
{
	string username = "username=" + textField->getStringValue();
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::OnHttpRequestCompleted, this));

	database->setStringForKey("cookies", textField->getStringValue());

	const char* postData = username.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST username");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
void LoginScene::OnHttpRequestCompleted(HttpClient *sender, HttpResponse * response)
{
	if (!response)
	{
		return;
	}

	string response_data = "";

	std::vector<char> *buffer = response->getResponseData();
	for (int i = 0; i < buffer->size(); i++)
	{
		response_data += (*buffer)[i];
	}
	//�洢����HTTP��Ӧ���ĵ�header����
	string header_data = "";

	std::vector<char> *buffer2 = response->getResponseHeader();
	for (int i = 0; i < buffer2->size(); i++)
	{
		header_data += (*buffer2)[i];
	}

	int subscript = header_data.find("GAMESESSIONID");
	string cookie = "";
	for (; header_data[subscript] != '\r'; subscript++)
	{
		cookie += header_data[subscript];
	}
	//�洢��½�������Ϊ�û������cookie���´�����Ӧ�ÿ�ֱ�ӵ��Auto Login��½
	database->setStringForKey("cookies", cookie);

//  header sample
//  Server: Apache - Coyote / 1.1
//	Set - Cookie : GAMESESSIONID = 8ca610711bc37b6c2ddf9ae08fcc6dfb
//	Content - Type : application / json; charset = UTF - 8
//	Transfer - Encoding: chunked
//	Date : Sun, 29 May 2016 13 : 53 : 35 GMT

	rapidjson::Document d;
	d.Parse<0>(response_data.c_str());

	if (d["result"].GetBool() == true)
	{
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.8, scene, Color3B(0, 0, 0)));
	}
	
	return;
}
//ʹ��cookie��½
void LoginScene::auto_login(cocos2d::Ref* pSender)
{
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	//��ʵ�����﷢��cookie�Ĳ��ֲ�û����Ч�����������ƺ��������Login�׶η��͵�cookie�����������username,���ؽ������result��Ӧֵһ��Ϊfalse
	headers.push_back("Cookies: " + database->getStringForKey("cookies"));
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::OnHttpRequestCompleted2, this));

	request->setTag("POST cookies");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginScene::OnHttpRequestCompleted2(HttpClient *sender, HttpResponse * response)
{
	if (!response)
	{
		return;
	}

	string response_data = "";

	std::vector<char> *buffer = response->getResponseData();
	for (int i = 0; i < buffer->size(); i++)
	{
		response_data += (*buffer)[i];
	}

	rapidjson::Document d;
	d.Parse<0>(response_data.c_str());

	//��������POST�������������ݶ�û���ṩusername���ʷ���json�ַ���result��Ӧ��ֵ��Ϊfalse,����GameScene�еĲ������ᷢ��cookie�����漰�û���������ȡ����result��Ӧֵ�ļ���
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.8, scene, Color3B(0, 0, 0)));

	return;
}

