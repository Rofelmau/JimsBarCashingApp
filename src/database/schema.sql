-- Settings table
CREATE TABLE IF NOT EXISTS Settings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    key TEXT UNIQUE,
    value TEXT
);

-- Cocktails table
CREATE TABLE IF NOT EXISTS Cocktails (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT
);

-- Ingredients table
CREATE TABLE IF NOT EXISTS Ingredients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT
);

-- CocktailIngredients table
CREATE TABLE IF NOT EXISTS CocktailIngredients (
    cocktail_id INTEGER,
    ingredient_id INTEGER,
    FOREIGN KEY(cocktail_id) REFERENCES Cocktails(id),
    FOREIGN KEY(ingredient_id) REFERENCES Ingredients(id)
);

-- Discounts table
CREATE TABLE IF NOT EXISTS Discounts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT,
    type INTEGER,
    value REAL,
    cocktail_limit INTEGER,
    active INTEGER DEFAULT 1
);

-- SalesDiscounts table
CREATE TABLE IF NOT EXISTS SalesDiscounts (
    sale_id INTEGER,
    discount_id INTEGER,
    FOREIGN KEY(sale_id) REFERENCES Sales(id),
    FOREIGN KEY(discount_id) REFERENCES Discounts(id)
);

-- Sales table
CREATE TABLE IF NOT EXISTS Sales (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT DEFAULT (datetime('now', 'utc')), -- ISO 8601 format in UTC
    payment_method INTEGER,
    price_per_cocktail REAL,
    total_price REAL,
    returned_cups INTEGER DEFAULT 0
);

-- SalesDetails table
CREATE TABLE IF NOT EXISTS SalesDetails (
    sale_id INTEGER,
    cocktail_id INTEGER,
    quantity INTEGER,
    FOREIGN KEY(sale_id) REFERENCES Sales(id),
    FOREIGN KEY(cocktail_id) REFERENCES Cocktails(id)
);