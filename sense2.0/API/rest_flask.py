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

class Temperature_node1(Resource):
    def get(self):

    	topicNode = 'dev/node1/temperature'

    	conn = db_connect.connect() # connect to database
    	query = conn.execute("SELECT * FROM temperature WHERE topic='%s' "%(topicNode)) # This line performs query and returns json result

    	return {'temp': [i[0] for i in query.cursor.fetchall()]} # Fetches first column that is Employee ID

class Temperature_date_range_node1(Resource):
    def get(self, temp_date_init, temp_date_end):

    	topicNode = 'dev/node1/temperature'

    	conn = db_connect.connect()
    	query = conn.execute("SELECT * FROM temperature WHERE topic='%s' AND DATETIME(created_at) >= '%s' AND DATETIME(created_at) <='%s' " %(topicNode, temp_date_init, temp_date_end))
    	result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
    	return jsonify(result)

api.add_resource(Temperature_node1, '/node1/temperature') # Route_1
api.add_resource(Temperature_date_range_node1,'/node1/temperature/<temp_date_init>/<temp_date_end>') #Route_2



class currentrms_node1(Resource):
    def get(self):

    	topicNode = 'dev/node1/current'

    	conn = db_connect.connect() # connect to database
    	query = conn.execute("SELECT * FROM currentrms WHERE topic='%s' "%(topicNode)) # This line performs query and returns json result

    	return {'temp': [i[0] for i in query.cursor.fetchall()]} # Fetches first column that is Employee ID

class currentrms_date_range_node1(Resource):
    def get(self, temp_date_init, temp_date_end):

    	topicNode = 'dev/node1/current'

    	conn = db_connect.connect()
    	query = conn.execute("SELECT * FROM currentrms WHERE topic='%s' AND DATETIME(created_at) >= '%s' AND DATETIME(created_at) <='%s' " %(topicNode, temp_date_init, temp_date_end))
    	result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
    	return jsonify(result)



api.add_resource(currentrms_node1, '/node1/currentrms') # Route_1
api.add_resource(currentrms_date_range_node1,'/node1/currentrms/<temp_date_init>/<temp_date_end>') #Route_2

if __name__ == '__main__':
     app.run(host='0.0.0.0',port=5002)




