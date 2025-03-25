-- Insert initial settings
INSERT INTO Settings (key, value) VALUES ('price_per_cocktail', '5.00');
INSERT INTO Settings (key, value) VALUES ('cup_pawn', '0.50');
INSERT INTO Settings (key, value) VALUES ('selected_cocktail_1', '1');
INSERT INTO Settings (key, value) VALUES ('selected_cocktail_2', '2');
INSERT INTO Settings (key, value) VALUES ('selected_cocktail_3', '3');
INSERT INTO Settings (key, value) VALUES ('selected_cocktail_4', '4');
INSERT INTO Settings (key, value) VALUES ('selected_cocktail_5', '5');
INSERT INTO Settings (key, value) VALUES ('selected_cocktail_6', '6');

-- Insert initial cocktails
INSERT INTO Cocktails (name) VALUES ('Strawberry Virgin');
INSERT INTO Cocktails (name) VALUES ('Erdbeer Mosquito');
INSERT INTO Cocktails (name) VALUES ('Viva');
INSERT INTO Cocktails (name) VALUES ('Ipanema');
INSERT INTO Cocktails (name) VALUES ('Virgin Colada');
INSERT INTO Cocktails (name) VALUES ('Blue Lagoon');

-- Insert initial ingredients
INSERT INTO Ingredients (name) VALUES ('Mangosirup');
INSERT INTO Ingredients (name) VALUES ('Limette');
INSERT INTO Ingredients (name) VALUES ('Orangen');
INSERT INTO Ingredients (name) VALUES ('Gingerale');
INSERT INTO Ingredients (name) VALUES ('Kokossirup');
INSERT INTO Ingredients (name) VALUES ('Ananassaft');
INSERT INTO Ingredients (name) VALUES ('Pfirsichsirup');

-- Insert initial cocktail ingredients
INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) VALUES (1, 5);
INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) VALUES (2, 2);
INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) VALUES (3, 7);
INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) VALUES (4, 4);
INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) VALUES (5, 6);
INSERT INTO CocktailIngredients (cocktail_id, ingredient_id) VALUES (6, 1);

-- Insert initial discounts
INSERT INTO Discounts (name, type, value, cocktail_limit, active) VALUES ('JULEICA', 0, 1.0, -1, 1);
INSERT INTO Discounts (name, type, value, cocktail_limit, active) VALUES ('Gruppenrabatt', 1, 20.0, 4, 1);
INSERT INTO Discounts (name, type, value, cocktail_limit, active) VALUES ('Mitarbeiter', 2, -1.0, -1, 1);
INSERT INTO Discounts (name, type, value, cocktail_limit, active) VALUES ('5%', 3, 5.0, -1, 1);