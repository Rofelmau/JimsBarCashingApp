#include "CocktailRepository.h"

#include "../DatabaseManager.h"
#include "../Logger.h"
#include "../entities/Cocktail.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace {
    static bool validateDatabase(const QSqlDatabase &db) {
        if (!db.isValid() || !db.isOpen()) {
            Logger::LogError("Database is not open or invalid!");
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
                Logger::LogError("Error inserting new ingredient: " + insertIngredientQuery.lastError().text().toStdString());
                return false;
            }
        }
        return true;
    }

    static void insertCocktailIngredient(const QSqlDatabase &db, int cocktailId, const QString &ingredient) {
        QSqlQuery insertIngredientsQuery(db);
        insertIngredientsQuery.prepare(R"(
            INSERT INTO CocktailIngredients (cocktail_uuid, ingredient_id)
            SELECT :cocktail_uuid, id FROM Ingredients WHERE name = :name
        )");
        insertIngredientsQuery.bindValue(":cocktail_uuid", cocktailId);
        insertIngredientsQuery.bindValue(":name", ingredient);

        if (!insertIngredientsQuery.exec()) {
            Logger::LogError("Error inserting ingredient: " + insertIngredientsQuery.lastError().text().toStdString());
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
    if (!query.exec("SELECT uuid, name FROM Cocktails")) {
        Logger::LogError("Error retrieving cocktails: " + query.lastError().text().toStdString());
        return;
    }

    m_cachedCocktails.clear();

    while (query.next()) {
        const QString uuid = query.value("uuid").toString();
        const QString name = query.value("name").toString();
        QSharedPointer<Cocktail> cocktail(new Cocktail(uuid, name));

        QSqlQuery ingredientQuery(db);
        ingredientQuery.prepare(R"(
            SELECT Ingredients.name FROM Ingredients
            JOIN CocktailIngredients ON Ingredients.id = CocktailIngredients.ingredient_id
            WHERE CocktailIngredients.cocktail_uuid = :uuid
        )");
        ingredientQuery.bindValue(":uuid", uuid);
        if (ingredientQuery.exec()) {
            QStringList ingredients;
            while (ingredientQuery.next()) {
                ingredients.append(ingredientQuery.value("name").toString());
            }
            cocktail->setIngredients(ingredients);
        } else {
            Logger::LogError("Error retrieving ingredients: " + ingredientQuery.lastError().text().toStdString());
            continue;
        }

        m_cachedCocktails.insert(cocktail);
    }
}

QSet<QSharedPointer<Cocktail>> CocktailRepository::getAllCocktails() const
{
    return m_cachedCocktails;
}

QSharedPointer<Cocktail> CocktailRepository::getCocktailByUuid(const QString &uuid) const
{
    for (const QSharedPointer<Cocktail> &cocktail : m_cachedCocktails) {
        if (cocktail->getUuid() == uuid) {
            return cocktail;
        }
    }
    return nullptr;
}

void CocktailRepository::addCocktail(const QString &uuid, const QString &name, const QStringList &ingredients) const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO Cocktails (uuid, name) VALUES (:uuid, :name)");
    query.bindValue(":uuid", uuid);
    query.bindValue(":name", name);

    if (!query.exec()) {
        Logger::LogError("Error adding new cocktail: " + query.lastError().text().toStdString());
        return;
    }

    for (const QString &ingredient : ingredients) {
        if (ensureIngredientExists(db, ingredient)) {
            QSqlQuery insertIngredientQuery(db);
            insertIngredientQuery.prepare(R"(
                INSERT INTO CocktailIngredients (cocktail_uuid, ingredient_id)
                SELECT :uuid, id FROM Ingredients WHERE name = :name
            )");
            insertIngredientQuery.bindValue(":uuid", uuid);
            insertIngredientQuery.bindValue(":name", ingredient);

            if (!insertIngredientQuery.exec()) {
                Logger::LogError("Error inserting ingredient: " + insertIngredientQuery.lastError().text().toStdString());
            }
        }
    }

    loadCocktailsIntoCache();
}

void CocktailRepository::addCocktail(const QString &name, const QStringList &ingredients) const
{
    addCocktail(QUuid::createUuid().toString(), name, ingredients);
}

void CocktailRepository::deleteCocktail(const QString &uuid) const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM Cocktails WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (!query.exec()) {
        Logger::LogError("Error deleting cocktail: " + query.lastError().text().toStdString());
    }

    loadCocktailsIntoCache();
}

void CocktailRepository::editCocktail(const QString &uuid, const QString &name, const QStringList &ingredients) const
{
    QSqlDatabase db = dbManager->database();
    if (!validateDatabase(db)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE Cocktails SET name = :name WHERE uuid = :uuid");
    query.bindValue(":name", name);
    query.bindValue(":uuid", uuid);

    if (!query.exec()) {
        Logger::LogError("Error editing cocktail: " + query.lastError().text().toStdString());
        return;
    }

    QSqlQuery deleteIngredientsQuery(db);
    deleteIngredientsQuery.prepare("DELETE FROM CocktailIngredients WHERE cocktail_uuid = :uuid");
    deleteIngredientsQuery.bindValue(":uuid", uuid);

    if (!deleteIngredientsQuery.exec()) {
        Logger::LogError("Error deleting existing ingredients: " + deleteIngredientsQuery.lastError().text().toStdString());
        return;
    }

    for (const QString &ingredient : ingredients) {
        if (ensureIngredientExists(db, ingredient)) {
            QSqlQuery insertIngredientQuery(db);
            insertIngredientQuery.prepare(R"(
                INSERT INTO CocktailIngredients (cocktail_uuid, ingredient_id)
                SELECT :uuid, id FROM Ingredients WHERE name = :name
            )");
            insertIngredientQuery.bindValue(":uuid", uuid);
            insertIngredientQuery.bindValue(":name", ingredient);

            if (!insertIngredientQuery.exec()) {
                Logger::LogError("Error inserting ingredient: " + insertIngredientQuery.lastError().text().toStdString());
            }
        }
    }

    loadCocktailsIntoCache();
}

QJsonArray CocktailRepository::exportAsJson() const
{
    QJsonArray cocktailsArray;

    for (const QSharedPointer<Cocktail> &cocktail : m_cachedCocktails) {
        QJsonObject cocktailObject;
        cocktailObject["uuid"] = cocktail->getUuid();
        cocktailObject["name"] = cocktail->getName();

        QJsonArray ingredientsArray;
        for (const QString &ingredient : cocktail->getIngredients()) {
            ingredientsArray.append(ingredient);
        }
        cocktailObject["ingredients"] = ingredientsArray;

        cocktailsArray.append(cocktailObject);
    }

    return cocktailsArray;
}

void CocktailRepository::import(const QJsonArray &jsonArray)
{
    for (const QJsonValue &value : jsonArray) {
        if (!value.isObject()) {
            continue;
        }

        QJsonObject cocktailObject = value.toObject();
        QString uuid = cocktailObject["uuid"].toString();
        QString name = cocktailObject["name"].toString();
        QJsonArray ingredientsArray = cocktailObject["ingredients"].toArray();

        QStringList ingredients;
        for (const QJsonValue &ingredientValue : ingredientsArray) {
            ingredients.append(ingredientValue.toString());
        }

        QSharedPointer<Cocktail> existingCocktail = getCocktailByUuid(uuid);
        if (existingCocktail) {
            editCocktail(uuid, name, ingredients);
        } else {
            addCocktail(uuid, name, ingredients);
        }
    }
}
