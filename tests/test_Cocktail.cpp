#include <gtest/gtest.h>

#include "../src/entities/Cocktail.h"

TEST(CocktailTest, DefaultConstructor) {
    Cocktail cocktail;
    EXPECT_EQ(cocktail.getId(), -1);
    EXPECT_EQ(cocktail.getName(), "");
    EXPECT_TRUE(cocktail.getIngredients().isEmpty());
}

TEST(CocktailTest, ParameterizedConstructor) {
    Cocktail cocktail(1, "Mojito");
    EXPECT_EQ(cocktail.getId(), 1);
    EXPECT_EQ(cocktail.getName(), "Mojito");
    EXPECT_TRUE(cocktail.getIngredients().isEmpty());
}

TEST(CocktailTest, SetAndGetIngredients) {
    Cocktail cocktail(1, "Mojito");
    QStringList ingredients = {"Rum", "Mint", "Sugar", "Lime", "Soda"};
    cocktail.setIngredients(ingredients);
    EXPECT_EQ(cocktail.getIngredients(), ingredients);
}

TEST(CocktailTest, CopyConstructor) {
    Cocktail original(1, "Mojito");
    QStringList ingredients = {"Rum", "Mint", "Sugar", "Lime", "Soda"};
    original.setIngredients(ingredients);

    Cocktail copy(original);
    EXPECT_EQ(copy.getId(), original.getId());
    EXPECT_EQ(copy.getName(), original.getName());
    EXPECT_EQ(copy.getIngredients(), original.getIngredients());
}

TEST(CocktailTest, AssignmentOperator) {
    Cocktail original(1, "Mojito");
    QStringList ingredients = {"Rum", "Mint", "Sugar", "Lime", "Soda"};
    original.setIngredients(ingredients);

    Cocktail assigned;
    assigned = original;
    EXPECT_EQ(assigned.getId(), original.getId());
    EXPECT_EQ(assigned.getName(), original.getName());
    EXPECT_EQ(assigned.getIngredients(), original.getIngredients());
}

TEST(CocktailTest, EqualityOperator) {
    Cocktail cocktail1(1, "Mojito");
    Cocktail cocktail2(1, "Mojito");
    EXPECT_TRUE(cocktail1 == cocktail2);

    Cocktail cocktail3(2, "Daiquiri");
    EXPECT_FALSE(cocktail1 == cocktail3);
}

TEST(CocktailTest, HashFunction) {
    Cocktail cocktail(1, "Mojito");
    uint hash = qHash(cocktail);
    EXPECT_NE(hash, 0u);
}
