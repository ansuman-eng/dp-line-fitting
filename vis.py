import matplotlib.pyplot as plt
from tkinter import *
import subprocess
import numpy as np

n_points = 10
plt.ion()



def getVal(event):
	#print(Throttle.get())
	plt.clf()


	#CALL THE CPP CODE
	c_str = str(Throttle.get())
	subprocess.Popen(["./a.out",c_str, str(n_points)])

	#SCATTER THE INPUT POINTS
	plt.scatter(x_pure,y_pure)
	x_line=[]
	y_line=[]

	m_vals=[]
	c_vals=[]
	#READ THE LINES FROM THE OUTPUT FILE
	with open('output_points.txt') as f:
		for line in f:
			m,c = line.split(" ")
			m_vals.append(float(m))
			c_vals.append(float(c))

	m_vals=m_vals[::-1]
	c_vals=c_vals[::-1]
	#FIND THE INTERSECTION POINTS
	if(len(m_vals)==1):
		m=m_vals[0]
		c=c_vals[0]

		x_1 = x_pure[0]-5
		y_1 = m*x_1+c

		x_line.append(x_1)
		y_line.append(y_1)

		x_1 = x_pure[len(x_pure)-1]+5
		y_1 = m*x_1+c
		x_line.append(x_1)
		y_line.append(y_1)

	else:
		for i in range(len(m_vals)-1):
			m1=m_vals[i]
			c1=c_vals[i]
			m2=m_vals[i+1]
			c2=c_vals[i+1]
			x = (c2-c1)/(m1-m2)
			y = (m1*c2 - m2*c1)/(m1-m2)
			if(i==0):
				x_1 = x_pure[0]-5
				y_1 = m1*x_1+c1
				x_line.append(x_1)
				y_line.append(y_1)

			x_line.append(x)
			y_line.append(y)

			if(i==len(m_vals)-2):
				x_1 = x_pure[len(x_pure)-1]+5
				y_1 = m2*x_1+c2
				x_line.append(x_1)
				y_line.append(y_1)

	#PLOT THEM
	for i in range(len(x_line)-1):
		plt.plot([x_line[i],x_line[i+1]],[y_line[i],y_line[i+1]])
	plt.show()

n_points = int(input("Enter the number of points..."))

x_pure = np.linspace(-n_points,n_points,n_points)
y_pure = x_pure
noise_points = [i[0] for i in (n_points*np.random.rand(n_points,1) - n_points/2)]
y_pure = y_pure + noise_points

#GENERATE RANDOM POINTS WITH NOISE
with open('input_points.txt','w') as f:
	for point in range(n_points):
		f.write(str(x_pure[point])+","+str(y_pure[point])+"\n")


master = Tk()
Throttle = Scale(master, from_=0, to=100*n_points,resolution=0.1, orient=HORIZONTAL, width=40, length=1000, command=getVal)
Throttle.set(0)
Throttle.pack()

mainloop()