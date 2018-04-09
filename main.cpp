#include <QCoreApplication>
#include <QVariantMap>
#include <QDebug>

typedef bool (*FilterFun)(QVariant);

static bool filter1(QVariant var) { return (var.toInt() > 5); }
static bool filter2(QVariant var) { return (var.toInt() > 7); }

template <typename T>
class Filter {
public:
    Filter() { }

    static QList<QVariant> filter(QList<QVariant> datalist, QList<T> filterList) {
        for (int i = 0; i < filterList.count(); i++) {
           T filterFun= filterList.at(i);

            for (int index = 0; index < datalist.count(); index++) {
                bool status = filterFun(datalist.first());
                if (!status) {
                    datalist.removeFirst();
                    index = 0;
                }
            }
        }

        qDebug()<<datalist;
        return datalist;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<FilterFun> filterList;
    filterList<<filter1<<filter2;

    QList<QVariant> datalist;
    datalist<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10;

    Filter<FilterFun>::filter(datalist, filterList);

    return a.exec();
}


