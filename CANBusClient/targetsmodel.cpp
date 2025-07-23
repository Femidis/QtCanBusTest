#include "targetsmodel.h"


TargetsModel::TargetsModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int TargetsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_targets.size();

}

QVariant TargetsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    const targetObject& target = m_targets[index.row()];
    if (role == ClsRole)
        return target.cls;
    else if (role == PrbRole)
        return target.prb;
    else if (role == XRole)
        return target.x;
    else if (role == YRole)
        return target.y;
    else if (role == WRole)
        return target.w;
    else if (role == HRole)
        return target.h;

    return QVariant();

}


QHash<int, QByteArray> TargetsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ClsRole] = "class";
    roles[PrbRole] = "probability";
    roles[XRole] = "x";
    roles[YRole] = "y";
    roles[WRole] = "w";
    roles[HRole] = "h";
    return roles;
}


void TargetsModel::addTarget(const targetObject &target){

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_targets.append(target);
    endInsertRows();
}


int TargetsModel::getSize()
{
    return m_targets.size();
}

void TargetsModel::renewTargets(QVector<targetObject> tgObjs)
{
    auto n  =  tgObjs.size();
    for (int i=0;i<n;i++){

        addTarget(tgObjs[i]);

    }

    tgObjs;
}
