/*
 *  Copyright (C) 2018
 *  All rights reserved
 *
 *  Created by Strong at  2018/04/10 20:31:56
 *  Email: 2088201923@qq.com
 */


#include <QCoreApplication>
#include <QVariantMap>
#include <QDebug>

typedef bool (*FilterFun)(QVariant, QVariantMap);

template <typename T>
class Filter {
public:
    Filter() { }

    /* arg1:数据列表;
     * arg2:过滤器函数;
     * arg3: 对应的过滤器函数的判断字段.
     */

    static QList<QVariant> filter(QList<QVariant> datalist, QMap<QString, T> filterFunMap, QMap<QString, QVariantMap > filterDataMap) {
        QMapIterator<QString, T> i(filterFunMap);
        while (i.hasNext()) {
            i.next();

            /* key: 对应的函数指针的字段 */
            QString key = i.key();

            /* value: 对应的函数指针*/
            T value= i.value();

            QVariantMap map = filterDataMap[key];
            if (map.isEmpty())
                continue;

            QList<QVariant> tempDatalist;
            for (int index = 0; index < datalist.count(); index++) {
                bool status = value(datalist.at(index), map);
                if (status)
                    tempDatalist.append(datalist.at(index));
            }

            datalist = tempDatalist;
        }

        return datalist;
    }
};

class Persion {
public:
    Persion() { }
    Persion(int age, bool sex):
        m_age(age), m_sex(sex) { }

    int m_age;
    bool m_sex;
};

/* 注意：转换为QVariant类型，自编类需要这样做 */
Q_DECLARE_METATYPE(Persion)

/* 过滤函数，过滤年龄，传入Qvariant数据与QVariantMap的字段进行比较 */
static bool filterAge(QVariant var, QVariantMap filterDataMap) {
    Persion persion = var.value<Persion>();

    if (!filterDataMap["low"].isValid() || !filterDataMap["high"].isValid())
        return false;

    if (persion.m_age >= filterDataMap["low"].toInt() &&
        persion.m_age <= filterDataMap["high"].toInt())
        return true;
    else
        return false;
}

/* 过滤函数，过滤年龄，传入Qvariant数据与QVariantMap的字段进行比较 */
static bool filterSex(QVariant var, QVariantMap filterDataMap) {
    Persion persion = var.value<Persion>();

    if (!filterDataMap["sex"].isValid())
        return false;

    if (persion.m_sex == filterDataMap["sex"].toBool())
        return true;
    else
        return false;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QVariant> datalist;
    datalist.append(QVariant::fromValue(Persion(1, false)));
    datalist.append(QVariant::fromValue(Persion(2, false)));
    datalist.append(QVariant::fromValue(Persion(3, false)));
    datalist.append(QVariant::fromValue(Persion(1, true)));
    datalist.append(QVariant::fromValue(Persion(2, true)));
    datalist.append(QVariant::fromValue(Persion(3, true)));

    QMap<QString, FilterFun> filterFunMap;
    filterFunMap["filterAge"] = filterAge;
    filterFunMap["filterSex"] = filterSex;

    QVariantMap filterAgeDataMap;
    filterAgeDataMap["type"] = "age";
    filterAgeDataMap["high"] = 3;
    filterAgeDataMap["low"] = 2;

    QVariantMap filterSexDataMap;
    filterSexDataMap["type"] = "sex";
    filterSexDataMap["sex"] = true;

    QMap<QString, QVariantMap> filterDataMap;
    filterDataMap["filterAge"] = filterAgeDataMap;
    filterDataMap["filterSex"] = filterSexDataMap;

    QList<QVariant> resultList = Filter<FilterFun>::filter(datalist, filterFunMap, filterDataMap);

    for (int i = 0; i < resultList.count(); i++) {
        Persion persion = resultList.at(i).value<Persion>();
        qDebug()<<persion.m_age<<persion.m_sex;
    }

    return a.exec();
}


