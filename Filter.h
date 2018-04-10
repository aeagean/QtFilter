/*
 *  Copyright (C) 2018
 *  All rights reserved
 *
 *  Created by Strong at  2018/04/10 20:31:56
 *  Email: 2088201923@qq.com
 */
#ifndef FILTER_H
#define FILTER_H

#include <QVariantMap>
#include <QDebug>

/* 过滤函数必须按此格式写 */
typedef bool (*FilterFun)(QVariant, QVariantMap);

/* T == FilterFun */
template <typename T>
class Filter {
public:
    Filter() { }

    /* arg1:数据列表;
     * arg2:过滤器函数;
     * arg3: 对应的过滤器函数的判断字段.
     */
    static QList<QVariant> filter(QList<QVariant> datalist,
                                  QMap<QString, T> filterFunMap,
                                  QMap<QString, QVariantMap > filterDataMap)
    {
        QMapIterator<QString, T> i(filterFunMap);
        while (i.hasNext()) {
            i.next();

            /* key: 对应的函数指针的字段 */
            QString key = i.key();

            /* value: 对应的函数指针*/
            T value= i.value();

            QVariantMap map = filterDataMap[key];
            if (map.isEmpty()) {
                qDebug()<<"Could not find the corresponding field!";
                continue;
            }

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

#endif // FILTER_H
