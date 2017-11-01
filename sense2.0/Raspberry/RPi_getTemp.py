#!/usr/bin/env python3

from subprocess import check_output

from re import findall

def get_temp():
    temp = check_output(["vcgencmd","measure_temp"]).decode("UTF-8")
    return(findall("\d+\.\d+",temp)[0])		

print(get_temp())
