#!/bin/bash

# Setze die Pfade zu den SQL-Dateien und zur Datenbank
SCHEMA_FILE="$1"
DATA_FILE="$2"
DB_FILE="$3"

# Überprüfe, ob die Datenbank existiert
if [ ! -f "$DB_FILE" ]; then
    echo "Datenbank existiert nicht. Erstelle neue Datenbank..."
    sqlite3 "$DB_FILE" < "$SCHEMA_FILE"
    sqlite3 "$DB_FILE" < "$DATA_FILE"
else
    echo "Datenbank existiert bereits, überspringe Erstellung."
fi
