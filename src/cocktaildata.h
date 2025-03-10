#ifndef COCKTAILDATA_H
#define COCKTAILDATA_H

#include <QObject>
#include <QVariantList>

class CocktailData : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList cocktails READ cocktails NOTIFY cocktailsChanged)

public:
    explicit CocktailData(QObject *parent = nullptr);
    QVariantList cocktails() const;

signals:
    void cocktailsChanged();

private:
    void loadCocktails();
    QVariantList m_cocktails;
};

#endif // COCKTAILDATA_H