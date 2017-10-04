
from flask import Flask, request
from flask_restful import Resource, Api
from sqlalchemy import create_engine
from json import dumps
from flask.ext.jsonpify import jsonify

db_connect = create_engine('sqlite:///database.db')
app = Flask(__name__)
api = Api(app)

class Temperature(Resource):
    def get(self):
        conn = db_connect.connect() # connect to database
        query = conn.execute("select * from temperature") # This line performs query and returns json result
        return {'temp': [i[0] for i in query.cursor.fetchall()]} # Fetches first column that is Employee ID


class Temperature_date(Resource):
    def get(self, temp_date):
        conn = db_connect.connect()
        query = conn.execute("SELECT temperature, DATE(created_at) FROM temperature WHERE DATE(created_at) = '%s' " %temp_date)
        result = {'data': [dict(zip(tuple (query.keys()) ,i)) for i in query.cursor]}
        return jsonify(result)

api.add_resource(Temperature, '/temperature') # Route_1
#api.add_resource(Temperature_id, '/temperature/<temp_id>') # Route_2
api.add_resource(Temperature_date, '/temperature/<temp_date>')
#api.add_resource(Temperature_range,'/temperature/<temp_min>/<temp_max>'


if __name__ == '__main__':
     app.run(host='0.0.0.0',port=5004)

