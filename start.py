from multiprocessing import Process
import os
import subprocess

def run_process(PPath, OPath):
	p = subprocess.Popen("./chain %s %s" % (PPath, OPath), shell=True)
	p.wait()


if __name__ == "__main__":
	for i in range(1):
		PPath = "./data/PData_%s" % str(i + 1)
		OPath = "./data/OData_%s" % str(i + 1)
		p = Process(target=run_process,args=(PPath, OPath))
		p.start()

	p.join()
