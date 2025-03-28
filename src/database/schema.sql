-- Settings table
CREATE TABLE IF NOT EXISTS Settings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    key TEXT UNIQUE,
    value TEXT
);

-- Cocktails table
CREATE TABLE IF NOT EXISTS Cocktails (
    uuid TEXT PRIMARY KEY,
    name TEXT
);

-- Ingredients table
CREATE TABLE IF NOT EXISTS Ingredients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT
);

-- CocktailIngredients table
CREATE TABLE IF NOT EXISTS CocktailIngredients (
    cocktail_uuid TEXT,
    ingredient_id INTEGER,
    FOREIGN KEY(cocktail_uuid) REFERENCES Cocktails(uuid),
    FOREIGN KEY(ingredient_id) REFERENCES Ingredients(id)
);

-- Discounts table
CREATE TABLE IF NOT EXISTS Discounts (
    uuid TEXT PRIMARY KEY,
    name TEXT,
    type INTEGER,
    value REAL,
    cocktail_limit INTEGER,
    active INTEGER DEFAULT 1
);

-- SalesDiscounts table
CREATE TABLE IF NOT EXISTS SalesDiscounts (
    sale_id INTEGER,
    discount_uuid TEXT,
    quantity INTEGER,
    FOREIGN KEY(sale_id) REFERENCES Sales(id),
    FOREIGN KEY(discount_uuid) REFERENCES Discounts(uuid)
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
    cocktail_uuid TEXT,
    quantity INTEGER,
    FOREIGN KEY(sale_id) REFERENCES Sales(id),
    FOREIGN KEY(cocktail_uuid) REFERENCES Cocktails(uuid)
);

-- Weather table
CREATE TABLE IF NOT EXISTS Weather (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    condition INTEGER,
    temperature INTEGER,
    timestamp TEXT DEFAULT (datetime('now', 'utc')) -- ISO 8601 format in UTC
);

-- CashRegister table
CREATE TABLE IF NOT EXISTS CashRegister (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    balance REAL NOT NULL DEFAULT 0.0,
    last_updated TEXT DEFAULT (datetime('now', 'utc'))
);

-- CashRegisterLog table
CREATE TABLE IF NOT EXISTS CashRegisterLog (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    change_amount REAL NOT NULL,
    new_balance REAL NOT NULL,
    reason TEXT,
    timestamp TEXT DEFAULT (datetime('now', 'utc'))
);

-- Add a table for database versioning
CREATE TABLE IF NOT EXISTS DatabaseVersion (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    version INTEGER NOT NULL
);
