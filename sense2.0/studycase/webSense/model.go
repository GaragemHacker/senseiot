package main

import (
	"database/sql"
	"time"

	_ "github.com/mattn/go-sqlite3"
)

type Datasets struct {
	Data  []float64 `json:"data"`
	Label string    `json:"label"`
}

//User ...
type User struct {
	User string
	Pass string
}

//ValidLogin user
func (u *User) ValidLogin() bool {
	if u.User == "teste@teste" && u.Pass == "teste" {
		return true
	}
	return false
}

//Device ...
type Device struct {
	Name string `json:"name"`
}

//Node ...
type Node struct {
	Name   string `json:"name"`
	Device string `json:"device"`
}

//Variable ...
type Variable struct {
	Name string `json:"name"`
	Node string `json:"node"`
}

//Value ...
type Value struct {
	ID       int       `json:"id"`
	Variable string    `json:"variable"`
	Value    float64   `json:"value"`
	Date     time.Time `json:"date"`
}

//InitDB database init
func InitDB(filepath string) *sql.DB {
	db, err := sql.Open("sqlite3", filepath)
	if err != nil {
		panic(err)
	}
	if db == nil {
		panic("db nil")
	}
	return db
}

//CreateTableDevice create  table device
func CreateTableDevice(db *sql.DB) {
	// create table if not exists
	sqlTable := `CREATE TABLE IF NOT EXISTS device(name TEXT NOT NULL PRIMARY KEY);`

	_, err := db.Exec(sqlTable)
	if err != nil {
		panic(err)
	}
}

//CreateTableNode create table node
func CreateTableNode(db *sql.DB) {
	// create table if not exists
	sqlTable := `
	CREATE TABLE IF NOT EXISTS node(
		name TEXT NOT NULL PRIMARY KEY,
		device TEXT NOT NULL,
		FOREIGN KEY(device) REFERENCES device(name)
	);
	`
	_, err := db.Exec(sqlTable)
	if err != nil {
		panic(err)
	}
}

//CreateTableVariable create table variable
func CreateTableVariable(db *sql.DB) {
	// create table if not exists
	sqlTable := `
	CREATE TABLE IF NOT EXISTS variable(
		name TEXT NOT NULL PRIMARY KEY,
		node TEXT NOT NULL,
		FOREIGN KEY(node) REFERENCES node(name)
	);
	`
	_, err := db.Exec(sqlTable)
	if err != nil {
		panic(err)
	}
}

//CreateTableValues create table values
func CreateTableValue(db *sql.DB) {
	// create table if not exists
	sqlTable := `
	CREATE TABLE IF NOT EXISTS value(
		id INTEGER NOT NULL PRIMARY KEY,
		variable TEXT NOT NULL,
		value REAL NOT NULL,
		date DATETIME,
		FOREIGN KEY(variable) REFERENCES variable(name)
	);
	`
	_, err := db.Exec(sqlTable)
	if err != nil {
		panic(err)
	}
}

//InsertDevice ...
func InsertDevice(db *sql.DB, device Device) (err error) {
	sql := `INSERT INTO device(name) values(?)`
	stmt, err := db.Prepare(sql)
	if err != nil {
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec(device.Name)
	return
}

//InsertNode ...
func InsertNode(db *sql.DB, node Node) (err error) {
	sql := `INSERT INTO node(device,name) values(?, ?)`
	stmt, err := db.Prepare(sql)
	if err != nil {
		return
	}
	defer stmt.Close()
	_, err = stmt.Exec(node.Device, node.Name)
	return
}

//InsertVariable ...
func InsertVariable(db *sql.DB, variab Variable) (err error) {
	sql := `INSERT INTO variable(node,name) values(?, ?)`
	stmt, err := db.Prepare(sql)
	if err != nil {
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec(variab.Node, variab.Name)
	return
}

//InsertValue ...
func InsertValue(db *sql.DB, value Value) (err error) {
	sql := `INSERT INTO value(variable,value,date) values(?, ?,?)`
	stmt, err := db.Prepare(sql)
	if err != nil {
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec(value.Variable, value.Value, value.Date)
	return err
}

//SelectDevice ...
func SelectDevice(db *sql.DB) []Device {
	sql := "SELECT * FROM device"
	rows, err := db.Query(sql)
	if err != nil {
		panic(err)
	}
	defer rows.Close()

	var result []Device
	for rows.Next() {
		dv := Device{}
		err2 := rows.Scan(&dv.Name)
		if err2 != nil {
			panic(err2)
		}
		result = append(result, dv)
	}
	return result
}

//SelectNode ...
func SelectNode(db *sql.DB, device string) (nodes []Node, err error) {
	sql := "SELECT name FROM node where device = ?"
	rows, err := db.Query(sql, device)
	if err != nil {
		return
	}
	defer rows.Close()

	for rows.Next() {
		nd := Node{}
		err = rows.Scan(&nd.Name)
		if err != nil {
			return
		}
		nodes = append(nodes, nd)
	}
	return
}

//SelectVariable ...
func SelectVariable(db *sql.DB, node string) (variables []Variable, err error) {
	sql := "SELECT name FROM variable where node = ?"
	rows, err := db.Query(sql, node)
	if err != nil {
		return
	}
	defer rows.Close()
	for rows.Next() {

		vb := Variable{}
		err = rows.Scan(&vb.Name)
		if err != nil {
			return
		}
		variables = append(variables, vb)
	}
	return
}

//SelectValue ...
func SelectValue(db *sql.DB, variab string) (values []Value, err error) {
	sql := "SELECT value,date FROM value where variable = ?"
	rows, err := db.Query(sql, variab)
	if err != nil {
		return
	}
	defer rows.Close()

	for rows.Next() {
		vl := Value{}
		err = rows.Scan(&vl.Value, &vl.Date)
		if err != nil {
			return
		}
		values = append(values, vl)
	}
	return
}
