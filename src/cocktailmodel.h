#ifndef COCKTAILMODEL_H
#define COCKTAILMODEL_H

#include <QAbstractListModel>
#include <QVector>

struct Cocktail {
    QString name;
    QString color;
    QString image;
};

class CocktailModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum CocktailRoles {
        NameRole = Qt::UserRole + 1,
        ColorRole,
        ImageRole
    };

    explicit CocktailModel(const QString &filePath, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    void loadCocktails(const QString &filePath);
    QVector<Cocktail> m_cocktails;
};

#endif // COCKTAILMODEL_H