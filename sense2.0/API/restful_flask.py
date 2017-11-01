from flask import Flask, request
from flask_restful import Resource, Api
from sqlalchemy import create_engine
from json import dumps
#from flask.ext.jsonpify import jsonify
#from flask_jsonpify import jsonify
from flask import jsonify
#import timestring

db_connect = create_engine('sqlite:///database.db')
app = Flask(__name__)
api = Api(app)

# Metodos GET
class Temperature_node1(Resource):
    def get(self):

    	topicNode = 'dev/node1/temperature'

        # Acesso ao Banco de Dados
    	conn = db_connect.connect() # connect to database
    	query = conn.execute("SELECT * FROM temperature WHERE topic='%s' "%(topicNode)) # This line performs query and returns json result

    	return {'temp': [i[0] for i in query.cursor.fetchall()]} # Fetches first column that is Employee ID

# Metodos GET
class Temperature_date_range_node1(Resource):
    def get(self, date_init, date_end):

    	topicNode = 'dev/node1/temperature'

        # Acesso ao Banco de Dados
    	conn = db_connect.connect()
    	query = conn.execute("SELECT * FROM temperature WHERE topic='%s' AND DATETIME(created_at) >= '%s' AND DATETIME(created_at) <='%s' " %(topicNode, date_init, date_end))
    	result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
    	return jsonify(result)

# Metodos GET
class Temperature_node1_instant(Resource):
    def get(self):

        topicNode = 'dev/node1/temperature'

        # Acesso ao Banco de Dados
        conn = db_connect.connect()
        query = conn.execute("SELECT * FROM temperature WHERE topic='%s' ORDER BY id DESC LIMIT 1" %(topicNode))
        result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
        return jsonify(result)

# Metodos GET
class currentrms_node1(Resource):
    def get(self):

    	topicNode = 'dev/node1/current'

        # Acesso ao Banco de Dados
    	conn = db_connect.connect() # connect to database
    	query = conn.execute("SELECT * FROM currentrms WHERE topic='%s' "%(topicNode)) # This line performs query and returns json result
        result = {'currentrms': [i[0] for i in query.cursor.fetchall()]} # Fetches first column
    	conn.close()

        return jsonify(result)

# Metodos GET
class currentrms_date_range_node1(Resource):
    def get(self, date_init, date_end):

    	topicNode = 'dev/node1/current'

        # Acesso ao Banco de Dados
    	conn = db_connect.connect()
    	query = conn.execute("SELECT * FROM currentrms WHERE topic='%s' AND DATETIME(created_at) >= '%s' AND DATETIME(created_at) <='%s' " %(topicNode, date_init, date_end))
    	result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
    	conn.close()

        return jsonify(result)

# Metodos GET
class currentrms_node1_instant(Resource):
    def get(self):

        topicNode = 'dev/node1/current'

        # Acesso ao Banco de Dados
        conn = db_connect.connect()
        query = conn.execute("SELECT * FROM currentrms WHERE topic='%s' ORDER BY id DESC LIMIT 1" %(topicNode))
        result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
        conn.close()

        return jsonify(result)

# Metodos POST
class comand_node1_desl(Resource):
    #def post(self,message):
    def post(self):

        topicNode = 'cmd/node1/rele1'
        message = 'desl'

        # Acesso ao Banco de Dados
        conn = db_connect.connect()
        conn.execute("INSERT INTO comands (comands, topic) VALUES (?,?)", (message,topicNode))
        conn.close()

        return "OFF"

# Metodos POST
class comand_node1_liga(Resource):
    #def post(self,message):
    def post(self):

        topicNode = 'cmd/node1/rele1'
        message = 'liga'

        # Acesso ao Banco de Dados
        conn = db_connect.connect()
        conn.execute("INSERT INTO comands (comands, topic) VALUES (?,?)", (message,topicNode))
        conn.close()

        return "ON"


# Metodos GET
api.add_resource(Temperature_node1, '/node1/temperature') # Route_1
api.add_resource(Temperature_date_range_node1,'/node1/temperature/<date_init>/<date_end>') #Route_2
api.add_resource(Temperature_node1_instant, '/node1/temperature/instant') # Route_3

# Metodos GET
api.add_resource(currentrms_node1, '/node1/currentrms') # Route_1
api.add_resource(currentrms_date_range_node1,'/node1/currentrms/<date_init>/<date_end>') #Route_2
api.add_resource(currentrms_node1_instant, '/node1/currentrms/instant') # Route_3

# Metodos POST
api.add_resource(comand_node1_desl, '/node1/comand/desl') # Route_1
api.add_resource(comand_node1_liga, '/node1/comand/liga') # Route_1


if __name__ == '__main__':
     app.run(host='0.0.0.0',port=5003)




