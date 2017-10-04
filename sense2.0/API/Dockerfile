FROM python:2

ADD rest_flask.py /home/
ADD subscriber.py /home/
ADD database.db /home/
ADD create_table.py /home/

RUN pip install paho-mqtt
RUN pip install mysqlclient
RUN pip install flask flask-jsonpify flask-sqlalchemy flask-restful
RUN pip freeze

CMD [ "python", "/home/create_table.py" ]
CMD [ "python", "/home/subscriber.py" ]
CMD [ "python", "/home/rest_flask.py" ]

#EXPOSE 5003 1883
