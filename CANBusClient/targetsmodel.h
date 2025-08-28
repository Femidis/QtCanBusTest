#ifndef TARGETSMODEL_H
#define TARGETSMODEL_H

#include <QAbstractListModel>
#include "canIClient.h"

class TargetsModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum  {
        ClsRole = Qt::UserRole + 1,
        PrbRole,
        XRole,
        YRole,
        WRole,
        HRole
    };

    explicit TargetsModel(QObject *parent = nullptr);



    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addTarget(const targetObject &target);

    int getSize();

public slots:
    void renewTargets(QVector<targetObject>);

private:
    QVector<targetObject> m_targets;


};


#endif // TARGETSMODEL_H
