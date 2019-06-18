import matplotlib.pyplot as plt
from tkinter import *
import subprocess
import numpy as np


for n_points in range(1,250):
	print(n_points)
	
	x_pure = np.linspace(-n_points,n_points,n_points)
	y_pure = x_pure
	noise_points = [i[0] for i in (n_points*np.random.rand(n_points,1) - n_points/2)]
	y_pure = y_pure + noise_points

	#GENERATE RANDOM POINTS WITH NOISE
	with open('input_points.txt','w') as f:
		for point in range(n_points):
			f.write(str(x_pure[point])+","+str(y_pure[point])+"\n")
	
	p1 = subprocess.Popen(["./a.out",str(100), str(n_points)])
	p1.wait()
	