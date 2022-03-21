#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import csv
import re
import os

def run_stat(platform, b_class, name):

    data_array = []
    dist_location = 0
    dist_scale = 0
    dist_shape = 0

    with open('exec_times/'+platform+'/class_'+b_class+'/'+name+'.dat') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            data_array.append(float(row[0]))
	
    output = name + "\n" + "-----\n"
    output += "Mean:   " + str(np.mean(data_array)) + "\n"
    output += "Min:    " + str(np.min(data_array)) + "\n"
    output += "Max:    " + str(np.max(data_array)) + "\n"
    output += "Var:    " + str(np.var(data_array)) + "\n"
    output += "StdDev: " + str(np.std(data_array)) + "\n"
    output += "COV:    " + str(np.std(data_array)/np.mean(data_array)) + "\n"

    with open('stat/'+platform+'/class_'+b_class+'/'+name+'.txt', "w") as output_file:
        output_file.write(output)
    

platforms = os.listdir("exec_times")
for p in platforms:
	classes = os.listdir("exec_times/"+p)
	for c in classes:
		benchmarks = os.listdir("exec_times/"+p+"/"+c)
		c = c.split('_')[1]
		for b in benchmarks:
			b = b.split('.')[0]
			run_stat(p, c, b)

