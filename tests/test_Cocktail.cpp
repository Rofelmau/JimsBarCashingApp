#include <gtest/gtest.h>

#include "../src/entities/Cocktail.h"

TEST(CocktailTest, DefaultConstructor) {
    Cocktail cocktail;
    EXPECT_FALSE(cocktail.getUuid().isEmpty());
    EXPECT_EQ(cocktail.getName(), "");
    EXPECT_TRUE(cocktail.getIngredients().isEmpty());
}

TEST(CocktailTest, ParameterizedConstructor) {
    QString uuid = "123e4567-e89b-12d3-a456-426614174000";
    Cocktail cocktail(uuid, "Mojito");
    EXPECT_EQ(cocktail.getUuid(), uuid);
    EXPECT_EQ(cocktail.getName(), "Mojito");
    EXPECT_TRUE(cocktail.getIngredients().isEmpty());
}

TEST(CocktailTest, SetAndGetIngredients) {
    QString uuid = "123e4567-e89b-12d3-a456-426614174000";
    Cocktail cocktail(uuid, "Mojito");
    QStringList ingredients = {"Rum", "Mint", "Sugar", "Lime", "Soda"};
    cocktail.setIngredients(ingredients);
    EXPECT_EQ(cocktail.getIngredients(), ingredients);
}

TEST(CocktailTest, CopyConstructor) {
    QString uuid = "123e4567-e89b-12d3-a456-426614174000";
    Cocktail original(uuid, "Mojito");
    QStringList ingredients = {"Rum", "Mint", "Sugar", "Lime", "Soda"};
    original.setIngredients(ingredients);

    Cocktail copy(original);
    EXPECT_EQ(copy.getUuid(), original.getUuid());
    EXPECT_EQ(copy.getName(), original.getName());
    EXPECT_EQ(copy.getIngredients(), original.getIngredients());
}

TEST(CocktailTest, AssignmentOperator) {
    QString uuid = "123e4567-e89b-12d3-a456-426614174000";
    Cocktail original(uuid, "Mojito");
    QStringList ingredients = {"Rum", "Mint", "Sugar", "Lime", "Soda"};
    original.setIngredients(ingredients);

    Cocktail assigned;
    assigned = original;
    EXPECT_EQ(assigned.getUuid(), original.getUuid());
    EXPECT_EQ(assigned.getName(), original.getName());
    EXPECT_EQ(assigned.getIngredients(), original.getIngredients());
}

TEST(CocktailTest, EqualityOperator) {
    QString uuid1 = "123e4567-e89b-12d3-a456-426614174000";
    QString uuid2 = "123e4567-e89b-12d3-a456-426614174001";
    Cocktail cocktail1(uuid1, "Mojito");
    Cocktail cocktail2(uuid1, "Mojito");
    EXPECT_TRUE(cocktail1 == cocktail2);

    Cocktail cocktail3(uuid2, "Daiquiri");
    EXPECT_FALSE(cocktail1 == cocktail3);
}

TEST(CocktailTest, HashFunction) {
    QString uuid = "123e4567-e89b-12d3-a456-426614174000";
    Cocktail cocktail(uuid, "Mojito");
    uint hash = qHash(cocktail);
    EXPECT_NE(hash, 0u);
}
