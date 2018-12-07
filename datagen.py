import sys
import numpy as np
import matplotlib.pyplot as plt

def genExponetial(num, minValue, maxValue, param):
	ret = []
	for i in range(num):
		x = y = -1
		while x < minValue or x > maxValue:
			x = int(np.random.exponential(param))
		while y < minValue or y > maxValue:
			y = int(np.random.exponential(param))

		ret.append([x, y])

	return np.array(ret)


def zipf(minValue, maxValue, param):
	HsubV = 0
	V = 100
	# calculate the V-th harmonic number HsubV. WARNING: V>1
	for i in range(1, V):
		HsubV += 1.0 / pow(i, param)

	r = np.random.random() * HsubV
	i = np.random.random()
	sum = 1.0
	while sum < r:
		i += 1 + np.random.random()
		sum += 1.0 / pow(i, param)

	# i follows zipf distribution and lies between 1 and V
	# x lies between 0. and 1. and then between minValue and maxValue
	x = (i - 1) / (V - 1)
	x = (maxValue - minValue) * x + minValue

	return x


def genZipf(num, minValue, maxValue, param):
	ret = []
	for i in range(num):
		x = y = -1
		while x < minValue or x > maxValue:
			x = int(zipf(minValue, maxValue, param))
		while y < minValue or y > maxValue:
			y = int(zipf(minValue, maxValue, param))

		ret.append([x, y])

	return np.array(ret)


def genWeight(data, r, ratio):
	weight = []
	for index, i in enumerate(data):
		k = 0

		for j in data:
			dist = np.linalg.norm(i - j)
			if dist < r:
				k += 1

		temp = int(np.ceil(np.random.normal(ratio * k, 1)))
		weight.append(temp if temp >= 1 else 1)

	weight = np.array(weight).reshape(-1, 1)
	return np.hstack((weight, data))


def writeFile(path, data):
	with open(path, "w") as f:
		for item in data:
			f.write(" ".join(map(str, item.tolist())) + "\n")


if __name__ == '__main__':
	# 命令行参数：分布选项，P数量，O数量，P输出文件，O输出文件，点坐标最小值，点坐标最大值，分布参数
	args = sys.argv
	distOption = args[1] if len(sys.argv) > 1 else "exp"
	Pnum = int(args[2]) if len(sys.argv) > 2 else 1000
	Onum = int(args[3]) if len(sys.argv) > 3 else 500
	PPath = args[4] if len(sys.argv) > 4 else "./data/PData"
	OPath = args[5] if len(sys.argv) > 5 else "./data/OData"
	minValue = int(args[6]) if len(sys.argv) > 6 else 0
	maxValue = int(args[7]) if len(sys.argv) > 7 else 1000
	param = float(args[8]) if len(sys.argv) > 8 else 1000

	print("********************************************")
	print("* Generator Configurations")
	print("********************************************")
	print("* Distribution: %s" % distOption)
	print("* Distribution Parameter: %s" % param)
	print("* Number of Service Providers(P): %s" % Pnum)
	print("* Number of Users(O): %s" % Onum)
	print("* Minimum Coordinate: %s" % minValue)
	print("* Maximum Coordinate: %s" % maxValue)
	print("********************************************")

	P = np.array([])
	O = np.array([])

	try:
		if distOption == "exp":
			for i in range(4):
				P_temp = genExponetial(Pnum, minValue, maxValue, param)
				O_temp = genExponetial(Onum, minValue, maxValue, param)
				
				P = genWeight(P_temp, (maxValue - minValue) / 20, 10)
				O = genWeight(O_temp, (maxValue - minValue) / 20, 5)
				writeFile("%s_%s" % (PPath, i + 1), P)
				writeFile("%s_%s" % (OPath, i + 1), O)

			print("\n...Generation Complete!")

		elif distOption == "zipf":
			for i in range(4):
				P = genZipf(Pnum * 4, minValue, maxValue, param)
				O = genZipf(Onum * 4, minValue, maxValue, param)

				P = genWeight(P_temp, (maxValue - minValue) / 20, 10)
				O = genWeight(O_temp, (maxValue - minValue) / 20, 5)
				writeFile("%s_%s" % (PPath, i + 1), P)
				writeFile("%s_%s" % (OPath, i + 1), O)

			print("\n...Generation Complete!")

		else:
			raise Exception("Invalid distribution option")

	except Exception as e:
		print("ERROR: %s" % e)
		raise e

	finally:
		plt.subplot(221, title="Positions of P")
		plt.scatter(P[:,1:2], P[:,2:3])
		plt.subplot(222, title="Positions of O")
		plt.scatter(O[:,1:2], O[:,2:3])
		plt.show()
