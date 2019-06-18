import matplotlib.pyplot as plt

x = [i for i in range(1,250)]
y=[]
print("Reading file")
with open("time.txt") as f:
	content = f.readlines()
for i in content:
	print(float(i))
	y.append(float(i))
print("Read file")

plt.plot(x,y)
plt.show()