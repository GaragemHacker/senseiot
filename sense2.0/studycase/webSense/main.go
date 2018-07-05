package main

//CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"html/template"
	"io/ioutil"
	"log"
	"net/http"
	"strconv"
	"strings"
	"time"

	MQTT "github.com/eclipse/paho.mqtt.golang"
	"github.com/gorilla/mux"
	"github.com/gorilla/sessions"
)

//Config struct
type Config struct {
	ID         string `json:"id"`
	Database   string `jons:"database"`
	URLBrocker string `json:"urlBrocker"`
	Port       int    `json:"port"`
	Protocol   string `json:"protocol"`
	Security   bool   `json:"security"`
	User       string `json:"user"`
	Pass       string `json:"pass"`
}

func (c Config) url() string {
	return c.Protocol + "://" + c.URLBrocker + ":" + strconv.Itoa(c.Port)
}

var client MQTT.Client
var config Config
var db *sql.DB
var store = sessions.NewCookieStore([]byte("$2a$10$ox7dudmWy"))
var layout = "02/01/2006 15:04"

//msgRcvd ...
func msgRcvd(client MQTT.Client, msg MQTT.Message) {

	pl := string(msg.Payload())
	tp := strings.Split(string(msg.Topic()), "/")

	fmt.Println("Topic", tp)
	fmt.Println("PayLoad", pl)

	ntp := len(tp)
	if ntp < 4 {
		return
	}
	device := Device{Name: tp[1]}
	node := Node{Device: tp[1], Name: tp[2]}
	variable := Variable{Node: tp[2], Name: tp[3]}

	date := time.Now()

	v, err := strconv.ParseFloat(pl, 32)
	if err != nil {
		fmt.Println("Value is not int:", pl, "\n erro:", err)
		return
	}
	value := Value{
		Variable: tp[3],
		Value:    v,
		Date:     date,
	}

	InsertDevice(db, device)
	if err != nil {
		fmt.Println(err)
	}

	InsertNode(db, node)
	if err != nil {
		fmt.Println(err)
	}

	InsertVariable(db, variable)
	if err != nil {
		fmt.Println(err)
	}

	InsertValue(db, value)
	if err != nil {
		fmt.Println(err)
	}
}

func init() {
	readConfig()
	initDatabase()
}

func main() {

	var router = mux.NewRouter()
	router.PathPrefix("/static/").Handler(http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))
	router.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))

	router.HandleFunc("/", MainHandler)
	router.HandleFunc("/variables", VariablesHandler)
	router.HandleFunc("/values", ValuesHandler)
	router.HandleFunc("/sub", SubHandler)

	//router.HandleFunc("/cicladores", CicladoresHandler)
	//router.HandleFunc("/multimetro", MultimetroHandler)
	//	router.HandleFunc("/multimetro/{nome}/{hora:[0-9]+}", MutimetroPlotHandler)
	//router.HandleFunc("/plot/image/{ciclador}/{canal}/{status}", PlotImageHandler)

	router.HandleFunc("/login", LoginHandler).Methods("POST")
	router.HandleFunc("/logout", LogoutHandler)

	go mqttClient()

	log.Println("Listening...")
	http.ListenAndServe(":8080", router)
}

func MainHandler(w http.ResponseWriter, r *http.Request) {
	//t, _ := template.ParseFiles("template/index.html")

	session, _ := store.Get(r, "login")

	u := session.Values["user"]

	if u != nil && u == "teste@teste" {
		t := template.Must(template.New("product").ParseFiles("template/base.html", "template/index.html"))

		erro := ""
		nodes, err := SelectNode(db, "dev")
		if err != nil {
			erro = err.Error()
		}

		t.ExecuteTemplate(w, "base", map[string]interface{}{
			"menu":  "home",
			"erro":  erro,
			"nodes": nodes,
		})
	} else {
		t, _ := template.ParseFiles("template/login.html")
		t.Execute(w, nil)
	}

	//fmt.Println(cicladores)
}

func VariablesHandler(w http.ResponseWriter, r *http.Request) {
	session, _ := store.Get(r, "login")
	var err error

	u := session.Values["user"]
	var variables []Variable
	erro := ""

	if u != nil && u == "teste@teste" {
		node := r.FormValue("node")
		variables, err = SelectVariable(db, node)

		if err != nil {
			fmt.Println(err)
		}

		t, err := template.ParseFiles("template/variables.html")

		if err != nil {
			fmt.Println(err)
			return
		}

		t.Execute(w, map[string]interface{}{
			"menu":      "home",
			"erro":      erro,
			"vatiables": variables,
		})
		return
	}

	t, _ := template.ParseFiles("template/variables.html")

	t.Execute(w, map[string]interface{}{
		"menu":      "home",
		"erro":      erro,
		"vatiables": "",
	})

	//fmt.Println(cicladores)
}

func ValuesHandler(w http.ResponseWriter, r *http.Request) {
	session, _ := store.Get(r, "login")
	var err error

	u := session.Values["user"]
	var values []Value

	variable := r.FormValue("variable")

	if u != nil && u == "teste@teste" {
		values, err = SelectValue(db, variable)

		if err != nil {
			fmt.Println(err)
		}
	}

	var val []float64
	var lab []string

	for _, v := range values {
		val = append(val, v.Value)
		lab = append(lab, v.Date.Format("02/01/2006 15:04"))
	}

	resp := map[string]interface{}{
		"labels": lab,
		"datasets": Datasets{
			Data:  val,
			Label: variable,
		},
	}

	json, err := json.Marshal(resp)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
	w.Header().Set("Content-Type", "application/json")
	w.Write(json)

	//fmt.Println(cicladores)
}

func SubHandler(w http.ResponseWriter, r *http.Request) {
	session, _ := store.Get(r, "login")
	resp := "0"

	u := session.Values["user"]

	cmd := r.FormValue("cmd")

	topic := "/dev/node"

	if u != nil && u == "teste@teste" {

		if cmd[:1] == "n" {
			topic += cmd[1:2]
		}

		if cmd[2:3] == "r" {
			topic += "/rele" + cmd[3:4]
		}

		payload := "desl"
		if cmd[len(cmd)-1:] == "1" {
			payload = "liga"
		}

		fmt.Println(topic, payload)
		if setComand(topic, payload) == nil {
			resp = "ok"
		}

	}

	json, err := json.Marshal(resp)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
	w.Header().Set("Content-Type", "application/json")
	w.Write(json)

	//fmt.Println(cicladores)
}

func LoginHandler(response http.ResponseWriter, request *http.Request) {

	name := request.FormValue("name")
	pass := request.FormValue("password")

	if name != "" && pass != "" {
		u := &User{
			User: name,
			Pass: pass,
		}
		if !u.ValidLogin() {
			fmt.Println("não fez login.")
		} else {

			session, _ := store.Get(request, "login")
			session.Options = &sessions.Options{
				Path:     "/",
				MaxAge:   3600 * 48,
				HttpOnly: true,
			}
			session.Values["user"] = u.User

			session.Save(request, response)
		}
	}
	http.Redirect(response, request, "/", 302)
}

func LogoutHandler(w http.ResponseWriter, r *http.Request) {
	session, _ := store.Get(r, "login")
	session.Options = &sessions.Options{MaxAge: -1}
	session.Save(r, w)
	http.Redirect(w, r, "/", 302)
}
func mqttClient() {
	//create a ClientOptions struct setting the broker address, clientid, turn
	//off trace output and set the default message handler

	opts := MQTT.NewClientOptions().AddBroker(config.url())
	opts.SetClientID(config.ID)
	if config.Security {
		opts.SetUsername(config.User)
		opts.SetPassword(config.Pass)
	}

	//opts.SetDefaultPublishHandler(f)
	opts.SetCleanSession(false)
	//opts.SetKeepAlive()

	//create and start a client using the above ClientOptions

	opts.OnConnect = func(c MQTT.Client) {

		if token := c.Subscribe("/dev/node1/#", 1, msgRcvd); token.Wait() && token.Error() != nil {
			fmt.Println("erro mqtt", token.Error())
		}

		if token := c.Subscribe("/dev/node2/#", 1, msgRcvd); token.Wait() && token.Error() != nil {
			fmt.Println("erro mqtt", token.Error())
		}
		if token := c.Subscribe("/dev/teste/#", 1, msgRcvd); token.Wait() && token.Error() != nil {
			fmt.Println("erro mqtt", token.Error())
		}

	}

	client = MQTT.NewClient(opts)
	token := client.Connect()
	for token.Wait() && token.Error() != nil {
		fmt.Println("Erro mqtt:", token.Error())
		time.Sleep(5 * time.Second)
		token = client.Connect()
	}

	//subscribe to the topic /go-mqtt/sample and request messages to be delivered
	//at a maximum qos of zero, wait for the receipt to confirm the subscription

	time.Sleep(3 * time.Second)
	/*//unsubscribe from /go-mqtt/sample
	  if token := c.Unsubscribe("/go-mqtt/sample"); token.Wait() && token.Error() != nil {
	    fmt.Println(token.Error())
	    os.Exit(1)
	  }

	  c.Disconnect(250)*/

	forever := make(chan bool)
	<-forever
}

func setComand(t, p string) (err error) {

	payload := []byte(p)
	topic := fmt.Sprintf("%s", t)

	if client.IsConnected() {
		token := client.Publish(topic, 1, false, payload)
		err = token.Error()
	}
	return
}

func initDatabase() {
	db = InitDB(config.Database)
	CreateTableDevice(db)
	CreateTableNode(db)
	CreateTableVariable(db)
	CreateTableValue(db)
}

// read a config.json file and popule return
func readConfig() error {
	raw, err := ioutil.ReadFile("config.json")
	if err != nil {
		return err
	}
	return json.Unmarshal(raw, &config)
}
