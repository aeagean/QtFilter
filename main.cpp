#include <QCoreApplication>
#include <QVariantMap>
#include <QDebug>

class Filter;

typedef bool (Filter::*FilterFun)(QVariant);

class Filter {
public:
    Filter() { }

    QList<QVariant> filter(Filter* obj, QList<QVariant> datalist, QList<FilterFun> filterList) {
        for (int i = 0; i < filterList.count(); i++) {
           FilterFun filterFun = filterList.at(i);

            for (int index = 0; index < datalist.count(); index++) {
                bool status = obj->*filterFun(datalist.first());
                if (!status) {
                    datalist.removeFirst();
                    index = 0;
                }
            }
        }

        qDebug()<<datalist;
        return datalist;
    }

    bool filter1(QVariant var) { return (var.toInt() > 5); }
    bool filter2(QVariant var) { return (var.toInt() > 7); }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<FilterFun> filterList;
    filterList<<&Filter::filter1;//<<filter2;

    QList<QVariant> datalist;
    datalist<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10;

    Filter fil;
    fil.filter(&fil, datalist, filterList);

    return a.exec();
}


