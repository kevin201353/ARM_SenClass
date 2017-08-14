#include "classnameconfig.h"

ClassNameConfig::ClassNameConfig()
{
    m_iClassNum = 0;
    m_iClassLabelNum = 0;
    m_iInode = 0;
}
void ClassNameConfig::AddClass(QString name,QString id)
{
    if(m_iClassNum < MAXCLASS)
    {
        m_ClassNameArray[m_iClassNum].s_strClassName = name;
        m_ClassNameArray[m_iClassNum].s_strClassID = id;
        m_ClassNameArray[m_iClassNum].s_bChooseFlag = false;
        m_iClassNum++;
    }
    else
    {
    }
}

void ClassNameConfig::AddLabel(ClassMould *label)
{
    if(label)
    {
        if (m_iClassLabelNum < 3)
            m_pClassLabel[m_iClassLabelNum++] = label;
    }
}
void ClassNameConfig::AddInode()
{
    if(m_iInode < m_iClassNum-3)
        m_iInode++;
}
void ClassNameConfig::MinusInode()
{
    if(m_iInode > 0)
        m_iInode--;
}
void ClassNameConfig::SetLabelName()
{
    int itemp = m_iInode;
    for(int i = 0;i < m_iClassLabelNum;i++)
    {
        m_pClassLabel[i]->SetClassName(m_ClassNameArray[itemp].s_strClassName);

        if(!m_ClassNameArray[itemp].s_bChooseFlag)
        {
            m_pClassLabel[i]->SetChecked(false);

        }
        else
        {
            m_pClassLabel[i]->SetChecked(true);
        }
        itemp++;
    }
    if(m_iClassNum < m_iClassLabelNum)
    {
        int j = m_iClassLabelNum;
        for(int i = m_iClassLabelNum-m_iClassNum;i > 0;i--)
        {
            m_pClassLabel[--j]->setEnabled(false);
        }
    }
}

void ClassNameConfig::ClearOtherChoose(int inode)
{
    for(int i = 0;i < m_iClassNum;i++)
    {
        m_ClassNameArray[i].s_bChooseFlag = false;
    }
    for(int i = 0;i < m_iClassLabelNum;i++)
    {
        m_pClassLabel[i]->SetCheckedFlag(false);
    }
    if(inode < m_iClassNum)
    {
        m_ClassNameArray[inode].s_bChooseFlag = true;
    }
}
void ClassNameConfig::ChooseOne()
{
    for(int i = 0;i < m_iClassLabelNum;i++)
    {
        if(m_pClassLabel[i]->IsChecked())
        {
            ClearOtherChoose(m_iInode+i);
            break ;
        }
    }
}

QString ClassNameConfig::GetClassName()
{
    for(int i = 0;i < m_iClassNum;i++)
    {
        if(m_ClassNameArray[i].s_bChooseFlag)
            return m_ClassNameArray[i].s_strClassName;
    }
}
QString ClassNameConfig::GetClassID()
{
    for(int i = 0;i < m_iClassNum;i++)
    {
        if(m_ClassNameArray[i].s_bChooseFlag)
            return m_ClassNameArray[i].s_strClassID;
    }
}


void ClassNameConfig::MoveLeft()
{
    int itemp = GetChooseInode();
    if(m_iClassLabelNum <= m_iClassNum)
    {
        if(itemp+m_iClassLabelNum > m_iClassNum )
        {
            //qDebug("Over");
            for(int i = 0;i < m_iClassLabelNum;i++)
            {
                m_pClassLabel[i]->SetClassName(m_ClassNameArray[m_iClassNum-3+i].s_strClassName);
                if(!m_ClassNameArray[m_iClassNum-3+i].s_bChooseFlag)
                {
                    m_pClassLabel[i]->SetChecked(false);

                }
                else
                {
                    m_pClassLabel[i]->SetChecked(true);
                }
            }
            m_iInode = m_iClassNum-3;
        }
        if((itemp-m_iInode) < 0 || abs(m_iInode - itemp) >= m_iClassLabelNum)
        {
            for(int i = 0;i < m_iClassLabelNum;i++)
            {
                m_pClassLabel[i]->SetClassName(m_ClassNameArray[itemp+i].s_strClassName);
                if(!m_ClassNameArray[itemp+i].s_bChooseFlag)
                {
                    m_pClassLabel[i]->SetChecked(false);

                }
                else
                {
                    m_pClassLabel[i]->SetChecked(true);
                }

            }
            m_iInode = itemp;
        }
        int iMove = itemp-m_iInode;
        if(iMove > 0)
        {
            m_pClassLabel[iMove]->SetChecked(false);
            m_pClassLabel[iMove-1]->SetChecked(true);
            m_ClassNameArray[itemp].s_bChooseFlag = false;
            m_ClassNameArray[itemp-1].s_bChooseFlag = true;
        }
    }
    else
    {
        int iMove = itemp-m_iInode;
        if(iMove > 0)
        {
            m_pClassLabel[iMove]->SetChecked(false);
            m_pClassLabel[iMove-1]->SetChecked(true);
            m_ClassNameArray[itemp].s_bChooseFlag = false;
            m_ClassNameArray[itemp-1].s_bChooseFlag = true;
        }
    }
}
void ClassNameConfig::MoveRigth()
{
    int itemp = GetChooseInode();
    if(m_iClassLabelNum <= m_iClassNum)
    {
        if(itemp+m_iClassLabelNum > m_iClassNum )
        {
            //qDebug("Over");
            for(int i = 0;i < m_iClassLabelNum;i++)
            {
                m_pClassLabel[i]->SetClassName(m_ClassNameArray[m_iClassNum-3+i].s_strClassName);
                if(!m_ClassNameArray[m_iClassNum-3+i].s_bChooseFlag)
                {
                    m_pClassLabel[i]->SetChecked(false);

                }
                else
                {
                    m_pClassLabel[i]->SetChecked(true);
                }
            }
            m_iInode = m_iClassNum-3;
        }
        if((itemp-m_iInode) < 0 || abs(m_iInode - itemp) >= m_iClassLabelNum)
        {
            for(int i = 0;i < m_iClassLabelNum;i++)
            {
                m_pClassLabel[i]->SetClassName(m_ClassNameArray[itemp+i].s_strClassName);
                if(!m_ClassNameArray[itemp+i].s_bChooseFlag)
                {
                    m_pClassLabel[i]->SetChecked(false);

                }
                else
                {
                    m_pClassLabel[i]->SetChecked(true);
                }

            }
            m_iInode = itemp;
            return;
        }
        int iMove = itemp-m_iInode;
        if(iMove < m_iClassLabelNum-1)
        {
            m_pClassLabel[iMove]->SetChecked(false);
            m_pClassLabel[iMove+1]->SetChecked(true);
            m_ClassNameArray[itemp].s_bChooseFlag = false;
            m_ClassNameArray[itemp+1].s_bChooseFlag = true;
        }
    }
    else
    {
        int iMove = itemp-m_iInode;
        if(iMove < m_iClassNum-1)
        {
            m_pClassLabel[iMove]->SetChecked(false);
            m_pClassLabel[iMove+1]->SetChecked(true);
            m_ClassNameArray[itemp].s_bChooseFlag = false;
            m_ClassNameArray[itemp+1].s_bChooseFlag = true;
        }
    }
}

int ClassNameConfig::GetChooseInode()
{
    for (int i=0; i < m_iClassNum; i++)
    {
        if (m_ClassNameArray[i].s_bChooseFlag)
        {
            return i;
        }
    }
}
