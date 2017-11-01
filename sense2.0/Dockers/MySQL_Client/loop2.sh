#!/bin/bash

i=0;

while :
do

    t="$(( $RANDOM % 20 + 15 ))";
    h="$(( $RANDOM % 20 + 60 ))";

    top1="dev/node1/temperature";
    top2="dev/node1/humidity";

    mosquitto_pub -h echo1001.me -p 1883 -t $top1 -m $t;
    mosquitto_pub -h echo1001.me -p 1883 -t $top2 -m $h;

    echo -e "[\033[32m MSG ENVIADA \033[37m] Topico: $top1 | msg: $t  ==> $i:"

    echo -e "[\033[32m MSG ENVIADA \033[37m] Topico: $top2    | msg: $h  ==> $i:"
    ((i=i+1));
    sleep 5;
done
