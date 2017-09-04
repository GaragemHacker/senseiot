FROM python:2

ADD dbconnect-server.py /

RUN pip install paho-mqtt
RUN pip install mysqlclient

CMD [ "python", "./dbconnect-server.py" ]

