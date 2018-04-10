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

            for (int index = 0; index < datalist.count();) {
                bool status = value(datalist.at(index), map);
                if (!status) {
                    datalist.removeAt(index);
                    index = 0;
                }
                else {
                    index++;
                }
            }
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

Q_DECLARE_METATYPE(Persion)

static bool filterAge(QVariant var, QVariantMap filterDataMap) {
    Persion persion = var.value<Persion>();

    if (persion.m_age >= filterDataMap["low"].toInt() &&
        persion.m_age <= filterDataMap["high"].toInt())
        return true;
    else
        return false;
}

static bool filterSex(QVariant var, QVariantMap filterDataMap) {
    Persion persion = var.value<Persion>();

    if (persion.m_sex >= filterDataMap["sex"].toBool())
        return true;
    else
        return false;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QMap<QString, FilterFun> filterFunMap;
    filterFunMap["filterAge"] = filterAge;
    filterFunMap["filterSex"] = filterSex;

    QList<QVariant> datalist;
    datalist.append(QVariant::fromValue(Persion(1, false)));
    datalist.append(QVariant::fromValue(Persion(2, false)));
    datalist.append(QVariant::fromValue(Persion(3, false)));
    datalist.append(QVariant::fromValue(Persion(1, true)));
    datalist.append(QVariant::fromValue(Persion(2, true)));
    datalist.append(QVariant::fromValue(Persion(3, true)));

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


