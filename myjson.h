#ifndef MYJSON_H
#define MYJSON_H

#include <json/json.h>
#include <string.h>
#include <stdio.h>


#define JSONHEAD "{\"Cmd\":\"name\",\"Terminal\":4,\"Manage\":5}"
typedef struct test
{
    char s_strCmd[10];
    int s_iTerminal;
    int s_iManage;
}t;
class myJson
{
public:
    myJson();
    ~myJson();
    int Parse(const char *strJson);
    int ReadJson(int *tmp, const char *value);
    int ReadJson(unsigned int *tmp,const char *value);
    int ReadJson(char *tmp,const char *value);
    int ReadJson(bool *tmp,const char *value);
    int ReadJson(char *tmp,const char *array,const char *value);
    int ReadJson(int *tmp,const char *array,const char *value);
    int ReadJson(char (*tmp)[100], const char *array, const char *value, int MaxNum);
    int ReadJson(int tmp[], const char *array, const char *value, int MaxNum);
    int ReadJson(bool tmp[], const char *array, const char *value, int MaxNum);
    int ReadJson(char (*tmp)[100],const char *data ,const char *array, const char *value, int MaxNum);
    int ReadJson(int tmp[], const char *data,const char *array, const char *value, int MaxNum);
    int ReadJson(bool tmp[],const char *data ,const char *array, const char *value, int MaxNum);


    int ReadJson_v(char *temp,const char *data1,const char *data2);
    int ReadJson_v(int *temp,const char *data1,const char *data2);
    int ReadJson_v(char (*tmp)[100],const char *data ,const char *array,int MaxNum);
    int ReadJson_v(bool *tmp,const char *data,const char *value);

    int WriteJson(int tmp, const char *Key);
    int WriteJson(unsigned int tmp, const char *Key);
    int WriteJson(const char * tmp, const char *Key);
    char *Json2String(char *buf);
    void ValueClear();
private:
    Json::Reader m_Reader;
    Json::Value  m_Value;
    Json::Value  m_WriteValue;
    char m_cInitFlag;

};

#endif // JSON_H
