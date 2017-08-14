#include "myjson.h"
#include "log.h"
extern Log  *g_pLog;

myJson::~myJson()
{
    m_cInitFlag = 0;
}
myJson::myJson()
{
    m_cInitFlag = 0;
}

int myJson::ReadJson(int *tmp, const char *value)
{
    if(value == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(value))
    {
        if(!m_Value[value].isNull())
            *tmp = m_Value[value].asInt();
    }
    else
    {
        return 0;
    }
    return 1;
}
int myJson::ReadJson(unsigned int *tmp, const char *value)
{
    if(value == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(value))
    {
        if(!m_Value[value].isNull())
            *tmp = m_Value[value].asUInt();
    }
    else
    {
        return 0;
    }
    return 1;
}
int myJson::ReadJson(bool *tmp, const char *value)
{
    if(value == NULL)
        return -1;
    if(!m_cInitFlag)
        return -2;
    if(m_Value.isMember(value))
    {
        if(!m_Value[value].isNull())
            *tmp = m_Value[value].asBool();
    }
    else
    {
        return 0;
    }
    return 1;
}
int myJson::ReadJson(char *tmp, const char *array, const char *value)
{
    if(value == NULL || array == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(array))
    {
        Json::Value val_array = m_Value[array];
        if(val_array.isNull())
            return 0;
        if(!val_array.isArray())
            return 0;
        int iSize = val_array.size();
        for(int i = 0;i < iSize;i++)
        {
            if(val_array[i].isMember(value))
            {
                if(!val_array[i][value].isNull())
                    strcpy(tmp,val_array[i][value].asCString());
                break;
            }
        }
    }
    else
        return 0;
    return 1;
}
int myJson::ReadJson_v(char *tmp,const char *data1,const char *data2)
{
    if(data1 == NULL || data2 == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(data1))
    {
        Json::Value val_array = m_Value[data1];
        if(val_array.isNull())
            return 0;
        if(val_array.isMember(data2))
        {
            if(!val_array[data2].isNull())
                strcpy(tmp,val_array[data2].asCString());
        }
    }
    else
        return 0;
    return 1;
}
int myJson::ReadJson_v(int *tmp,const char *data1,const char *data2)
{
    if(data1 == NULL || data2 == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(data1))
    {
        Json::Value val_array = m_Value[data1];
        if(val_array.isNull())
            return 0;
        if(val_array.isMember(data2))
        {
            if(!val_array[data2].isNull())
                *tmp = val_array[data2].asInt();
        }
    }
    else
        return 0;
    return 1;
}
int myJson::ReadJson(int *tmp, const char *array, const char *value)
{
    if(value == NULL || array == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(array))
    {
        Json::Value val_array = m_Value[array];
        if(val_array.isNull())
            return 0;
        if(!val_array.isArray())
            return 0;
        int iSize = val_array.size();
        for(int i = 0;i < iSize;i++)
        {
            if(val_array[i].isMember(value))
            {
                if(!val_array[i][value].isNull())
                    *tmp = val_array[i][value].asInt();
                break;
            }
        }
    }
    else
        return 0;
    return 1;
}
int myJson::ReadJson(char (*tmp)[100], const char *array, const char *value, int MaxNum)
{
    int i = 0;
    if(value == NULL || array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(array))
    {
        Json::Value val_array = m_Value[array];
        if(val_array.isNull())
            return 0;
        if(!val_array.isArray())
            return 0;
        int iSize = val_array.size();
        for(i = 0;i < iSize && i < MaxNum;i++)
        {
            if(val_array[i].isMember(value))
            {
                if(!val_array[i][value].isNull())
                {
                    strcpy(tmp[i],val_array[i][value].asCString());
                }
                else
                {
                    //printf("Vlaue:NULL\n");
                    strcpy(tmp[i],"NULL");
                }
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson(int tmp[], const char *array, const char *value, int MaxNum)
{
    int i = 0;
    if(value == NULL || array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(array))
    {
        Json::Value val_array = m_Value[array];
        if(val_array.isNull())
            return 0;
        if(!val_array.isArray())
            return 0;
        int iSize = val_array.size();
        for(i = 0;i < iSize && i < MaxNum;i++)
        {
            if(val_array[i].isMember(value))
            {
                if(!val_array[i][value].isNull())
                {
                    tmp[i] = val_array[i][value].asInt();
                }
                else
                {
                    //printf("Vlaue:NULL\n");
                    tmp[i] = 0;
                }
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson(bool tmp[], const char *array, const char *value, int MaxNum)
{
    int i = 0;
    if(value == NULL || array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(array))
    {
        Json::Value val_array = m_Value[array];
        if(val_array.isNull())
            return 0;
        if(!val_array.isArray())
            return 0;
        int iSize = val_array.size();
        for(i = 0;i < iSize && i < MaxNum;i++)
        {
            if(val_array[i].isMember(value))
            {
                if(!val_array[i][value].isNull())
                {
                    tmp[i] = val_array[i][value].asBool();
                }
                else
                {
                    //printf("Vlaue:NULL\n");
                    tmp[i] = 0;
                }
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson(char *tmp, const char *value)
{
    if(value == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(value))
    {
        if(!m_Value[value].isNull())
            strcpy(tmp,m_Value[value].asCString());
    }
    else
    {
        return 0;
    }
    return 1;
}
int myJson::ReadJson(char (*tmp)[100],const char *data ,const char *array, const char *value, int MaxNum)
{
    int i = 0;
    if(value == NULL ||data == NULL ||array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(data))
    {
        Json::Value Jsondata = m_Value[data];
        if(Jsondata.isNull())
            return 0;
        if(Jsondata.isArray())
            return 0;
        if(Jsondata.isMember(array))
        {
            Json::Value val_array = Jsondata[array];
            if(val_array.isNull())
                return 0;
            if(!val_array.isArray())
                return 0;
            int iSize = val_array.size();
            for(i = 0;i < iSize && i < MaxNum;i++)
            {
                if(val_array[i].isMember(value))
                {
                    if(!val_array[i][value].isNull())
                    {
                        strcpy(tmp[i],val_array[i][value].asCString());
                    }
                    else
                    {
                        //printf("Vlaue:NULL\n");
                        strcpy(tmp[i],"NULL");
                    }
                }
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson(bool tmp[],const char *data ,const char *array, const char *value, int MaxNum)
{
    int i = 0;
    if(value == NULL ||data == NULL ||array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(data))
    {
        Json::Value Jsondata = m_Value[data];
        if(Jsondata.isNull())
            return 0;
        if(Jsondata.isArray())
            return 0;
        if(Jsondata.isMember(array))
        {
            Json::Value val_array = Jsondata[array];
            if(val_array.isNull())
                return 0;
            if(!val_array.isArray())
                return 0;
            int iSize = val_array.size();
            for(i = 0;i < iSize && i < MaxNum;i++)
            {
                if(val_array[i].isMember(value))
                {
                    if(!val_array[i][value].isNull())
                    {
                        tmp[i] = val_array[i][value].asBool();
                    }
                    else
                    {
                        //printf("Vlaue:NULL\n");
                        tmp[i] = false;
                    }
                }
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson(int tmp[], const char *data,const char *array, const char *value, int MaxNum)
{
    int i = 0;
    if(value == NULL ||data == NULL ||array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(data))
    {
        Json::Value Jsondata = m_Value[data];
        if(Jsondata.isNull())
            return 0;
        if(Jsondata.isArray())
            return 0;
        if(Jsondata.isMember(array))
        {
            Json::Value val_array = Jsondata[array];
            if(val_array.isNull())
                return 0;
            if(!val_array.isArray())
                return 0;
            int iSize = val_array.size();
            for(i = 0;i < iSize && i < MaxNum;i++)
            {
                if(val_array[i].isMember(value))
                {
                    if(!val_array[i][value].isNull())
                    {
                        tmp[i] = val_array[i][value].asInt();
                    }
                    else
                    {
                        //printf("Vlaue:NULL\n");
                        tmp[i] = 0;
                    }
                }
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson_v(char (*tmp)[100], const char *data, const char *array, int MaxNum)
{
    int i = 0;
    if(data == NULL ||array == NULL || MaxNum <= 0)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }
    if(m_Value.isMember(data))
    {
        Json::Value Jsondata = m_Value[data];
        if(Jsondata.isNull())
            return 0;
        if(Jsondata.isArray())
            return 0;
        if(Jsondata.isMember(array))
        {
            Json::Value val_array = Jsondata[array];
            if(val_array.isNull())
                return 0;
            if(!val_array.isArray())
                return 0;
            int iSize = val_array.size();
            for(i = 0;i < iSize && i < MaxNum;i++)
            {
                strcpy(tmp[i],Jsondata[array][i].asCString());
            }
        }
    }
    else
        return 0;
    return i;
}
int myJson::ReadJson_v(bool *tmp, const char *data, const char *value)
{
    if(data == NULL ||value == NULL)
        return -1;
    if(!m_cInitFlag)
    {
        return -2;
    }

    if(m_Value.isMember(data))
    {
        Json::Value Jsondata = m_Value[data];
        if(Jsondata.isNull())
            return 0;
        if(Jsondata.isArray())
            return 0;
        if(Jsondata.isMember(value))
        {
            Json::Value val_array = Jsondata[value];
            if(!val_array.isNull())
                *tmp = val_array.asBool();
        }
        else
            *tmp = false;
    }
    else
        return 0;
}
int myJson::WriteJson(int tmp, const char *Key)
{
    if(!m_WriteValue.isMember(Key))
    {
        m_WriteValue[Key] = tmp;
    }
    else
    {
        return 0;
    }
    return 1;
}
int myJson::WriteJson(unsigned int tmp, const char *Key)
{
    if(!m_WriteValue.isMember(Key))
    {
        m_WriteValue[Key] = tmp;
    }
    else
    {
        return 0;
    }
    return 1;
}
int myJson::WriteJson(const char *tmp, const char *Key)
{
    if(!m_WriteValue.isMember(Key))
    {
        m_WriteValue[Key] = tmp;
    }
    else
    {
        return 0;
    }
    return 1;
}
char *myJson::Json2String(char *buf)
{
    if(!buf)
    {
        return NULL;
    }
    strcpy(buf,m_WriteValue.toStyledString().c_str());
    ValueClear();
    return buf;
}
void myJson::ValueClear()
{
    m_WriteValue.clear();
}
int myJson::Parse(const char *strJson)
{
    if(strJson == NULL)
    {
        m_cInitFlag = 0;
        return -1;
    }
    if(!strlen(strJson))
    {
        m_cInitFlag = 0;
        return -2;
    }
    m_Value.clear();
    if(!m_Reader.parse(strJson,m_Value))
    {
        g_pLog->WriteLog(0,"[Error]->Json.parse [%s]",strJson);
        m_cInitFlag = 0;
        return -2;
    }
    m_cInitFlag = 1;
    return 0;
}
