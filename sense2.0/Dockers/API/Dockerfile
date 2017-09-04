FROM python:2

ADD rest_flask.py /
ADD subscriber.py /
ADD database.db /


RUN pip install paho-mqtt
RUN pip install mysqlclient
RUN pip install flask flask-jsonpify flask-sqlalchemy flask-restful
RUN pip freeze

CMD [ "python", "/rest_flask.py" ]
CMD [ "python", "/subscriber.py" ]

#EXPOSE 5002 1883
