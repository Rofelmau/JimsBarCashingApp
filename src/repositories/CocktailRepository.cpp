#include "CocktailRepository.h"

#include "../DatabaseManager.h"
#include "../entities/Cocktail.h"

#include <QSet>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

namespace {
    static bool validateDatabase(const QSqlDatabase &db) {
        if (!db.isValid() || !db.isOpen()) {
            qDebug() << "Database is not open or invalid!";
            return false;
        }
        return true;
    }

    static bool ensureIngredientExists(const QSqlDatabase &db, const QString &ingredient) {
        QSqlQuery checkIngredientQuery(db);
        checkIngredientQuery.prepare("SELECT id FROM Ingredients WHERE name = :name");
        checkIngredientQuery.bindValue(":name", ingredient);
        if (!checkIngredientQuery.exec() || !checkIngredientQuery.next()) {
            QSqlQuery insertIngredientQuery(db);
            insertIngredientQuery.prepare("INSERT INTO Ingredients (name) VALUES (:name)");
            insertIngredientQuery.bindValue(":name", ingredient);
            if (!insertIngredientQuery.exec()) {
                qDebug() << "Error inserting new ingredient: " << insertIngredientQuery.lastError();
                return false;
            }
        }
        return true;
    }

    static void insertCocktailIngredient(const QSqlDatabase &db, int cocktailId, const QString &ingredient) {
        QSqlQuery insertIngredientsQuery(db);
        insertIngredientsQuery.prepare("INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) "
                                       "SELECT :cocktail_id, id FROM Ingredients WHERE name = :name");
        insertIngredientsQuery.bindValue(":cocktail_id", cocktailId);
        insertIngredientsQuery.bindValue(":name", ingredient);

        if (!insertIngredientsQuery.exec()) {
            qDebug() << "Error inserting ingredient: " << insertIngredientsQuery.lastError();
        }
    }
}

CocktailRepository::CocktailRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent)
    : QObject(parent)
    , dbManager(dbManager)
{
    loadCocktailsIntoCache();
}

void CocktailRepository::loadCocktailsIntoCache() const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM Cocktails")) {
        qDebug() << "Error retrieving cocktails: " << query.lastError();
        return;
    }

    m_cachedCocktails.clear();

    while (query.next()) {
        if (!query.value("id").isValid() || query.value("id").isNull()) {
            qWarning() << "Invalid ID from database!";
            continue;
        }

        const int id = query.value("id").toInt();
        const QString name = query.value("name").toString();
        QSharedPointer<Cocktail> cocktail(new Cocktail(id, name));

        // Retrieve ingredients
        QSqlQuery ingredientQuery(db);
        ingredientQuery.prepare("SELECT Ingredients.name FROM Ingredients "
                                "JOIN CocktailIngredients ON Ingredients.id = CocktailIngredients.ingredient_id "
                                "WHERE CocktailIngredients.cocktail_id = :id");
        ingredientQuery.bindValue(":id", id);
        if (ingredientQuery.exec()) {
            QStringList ingredients;
            while (ingredientQuery.next()) {
                ingredients.append(ingredientQuery.value("name").toString());
            }
            cocktail->setIngredients(ingredients);
        } else {
            qDebug() << "Error retrieving ingredients: " << ingredientQuery.lastError();
        }

        m_cachedCocktails.insert(cocktail);
    }
}

QSet<QSharedPointer<Cocktail>> CocktailRepository::getAllCocktails() const
{
    return m_cachedCocktails;
}

QSharedPointer<Cocktail> CocktailRepository::getCocktailById(int id) const
{
    for (const QSharedPointer<Cocktail> &cocktail : m_cachedCocktails) {
        if (cocktail->getId() == id) {
            return cocktail;
        }
    }
    return nullptr;
}

void CocktailRepository::addCocktail(const QString &name, const QStringList &ingredients) const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO Cocktails (name) VALUES (:name)");
    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "Error adding new cocktail: " << query.lastError();
        return;
    }

    int cocktailId = query.lastInsertId().toInt();

    for (const QString &ingredient : ingredients) {
        if (ensureIngredientExists(db, ingredient)) {
            insertCocktailIngredient(db, cocktailId, ingredient);
        }
    }

    loadCocktailsIntoCache();
}

void CocktailRepository::deleteCocktail(int id) const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM Cocktails WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error deleting cocktail: " << query.lastError();
    }

    // Delete ingredients
    QSqlQuery ingredientQuery(db);
    ingredientQuery.prepare("DELETE FROM CocktailIngredients WHERE cocktail_id = :id");
    ingredientQuery.bindValue(":id", id);

    if (!ingredientQuery.exec()) {
        qDebug() << "Error deleting ingredients: " << ingredientQuery.lastError();
    }

    loadCocktailsIntoCache();
}

void CocktailRepository::editCocktail(const int id, const QString &name, const QStringList &ingredients) const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE Cocktails SET name = :name WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error editing cocktail: " << query.lastError();
    }

    // Delete existing ingredients
    QSqlQuery deleteIngredientsQuery(db);
    deleteIngredientsQuery.prepare("DELETE FROM CocktailIngredients WHERE cocktail_id = :id");
    deleteIngredientsQuery.bindValue(":id", id);

    if (!deleteIngredientsQuery.exec()) {
        qDebug() << "Error deleting existing ingredients: " << deleteIngredientsQuery.lastError();
    }

    // Insert new ingredients
    for (const QString &ingredient : ingredients) {
        if (ensureIngredientExists(db, ingredient)) {
            insertCocktailIngredient(db, id, ingredient);
        }
    }

    loadCocktailsIntoCache();
}
