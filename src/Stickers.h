//��Ҫ�Ķ���ģ��
//PenΪ������


#pragma once
#include <unordered_map>

#include <dpp/dpp.h>

//
class Obj{
public:
	Obj();

	~Obj() = default;
private:

};

//webhook version
class ObjWeb {
public:
	ObjWeb(dpp::webhook WK);

	~ObjWeb() = default;
public:


private:
	dpp::webhook WebHook;
};