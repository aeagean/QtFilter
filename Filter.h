#ifndef FILTER_H
#define FILTER_H

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

    static QList<QVariant> filter(QList<QVariant> datalist, QMap<QString, T> filterFunMap, QMap<QString, QVariantMap > filterDataMap)
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
