import sqlite3

conn = sqlite3.connect('database.db')
cursor = conn.cursor()

cursor.execute("""
    CREATE TABLE temperature (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        temperature TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")


cursor.execute("""
    CREATE TABLE humidity (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        humidity TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")

print('Tabelas criadas com sucesso.')

conn.close()
