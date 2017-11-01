import sqlite3

conn = sqlite3.connect('database.db')
cursor = conn.cursor()

#Tabela de Comandos
cursor.execute("""
    CREATE TABLE comands (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        comands TEXT NOT NULL,
        topic TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")

print('Tabelas criadas com sucesso')

conn.close()
