//��Ҫ�Ķ���ģ��
//PenΪ������


#pragma once
#include <unordered_map>

#include <dpp/dpp.h>

//����ʱ��Сʱ�ļ�ֵ��Ӧ����Ϣӳ��
class {

};

//��ֽ-��ŷ���Ƶ������(��Ϣ���У���Ϣӳ��)
class test {
public:
	test();

	~test() = default;
public:


private:
	std::unordered_map<dpp::snowflake, dpp::snowflake> Link;
};

//webhook version
class ObjWeb {
public:
	ObjWeb(dpp::webhook WK);

	~ObjWeb() = default;
public:


private:
	std::pair<dpp::snowflake, dpp::webhook> Send;

	std::unordered_map<dpp::snowflake, dpp::snowflake> Link;
};