#!/usr/bin/python3

from scipy.stats import genpareto
import numpy as np
import matplotlib.pyplot as plt
import csv
import re
import os

def plot(platform, b_class, name):

	data_array = []
	dist_location = 0
	dist_scale = 0
	dist_shape = 0

	with open('exec_times/'+platform+'/class_'+b_class+'/'+name+'.dat') as csv_file:
		csv_reader = csv.reader(csv_file, delimiter=',')
		line_count = 0
		for row in csv_reader:
			data_array.append(float(row[0]))

	with open('pwcet/'+platform+'/class_'+b_class+'/pwcet_'+name+'.txt') as pwcet_file:
		for line_str in pwcet_file:
			line = re.findall(r'Location', line_str)
			if len(line) > 0:
				param = line_str.split(':')[1]
				dist_location = float(param)
			line = re.findall(r'Scale', line_str)
			if len(line) > 0:
				param = line_str.split(':')[1]
				dist_scale = float(param)
			line = re.findall(r'Shape', line_str)
			if len(line) > 0:
				param = line_str.split(':')[1]
				dist_shape = float(param)
	
	fig, ax = plt.subplots(1, 1)

	if dist_location != 0 and dist_shape != 0 and dist_scale != 0:
		print(dist_shape, dist_location, dist_scale)
		x = np.linspace(genpareto.ppf(0.5, dist_shape, dist_location, dist_scale), genpareto.ppf(1-1e-9, dist_shape, dist_location, dist_scale), 1000)
		p = 1-genpareto.cdf(x, dist_shape, dist_location, dist_scale)

		ax.plot(x, p, 'r-', lw=5, alpha=0.6)
	y, x, _ = ax.hist(data_array, density=True, histtype='stepfilled', alpha=0.5)
	ax.legend(loc='best', frameon=False)
	ax.set_ylabel('Probability')
	ax.set_xlabel('Execution Time')
	ax.set_ylim([0, y.max()+0.1*y.max()])
	ax.set_xlim([x.min(), x.max()+2*x.std()])
	plt.title(name)
	os.makedirs('plots/'+platform+'/class_'+b_class+'/', exist_ok=True)
	plt.savefig('plots/'+platform+'/class_'+b_class+'/'+name+'.pdf')

platforms = os.listdir("exec_times")
for p in platforms:
	classes = os.listdir("exec_times/"+p)
	for c in classes:
		benchmarks = os.listdir("exec_times/"+p+"/"+c)
		c = c.split('_')[1]
		for b in benchmarks:
			b = b.split('.')[0]
			plot(p, c, b)


