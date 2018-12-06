from multiprocessing import Process
import os
import subprocess

def run_process(PPath, OPath):
	p = subprocess.Popen("./weighted_chain/main %s %s" % (PPath, OPath), shell=True)
	p.wait()


if __name__ == "__main__":
	for i in range(4):
		PPath = "../data/PData_%s" % i
		OPath = "../data/OData_%s" % i
		p = Process(target=run_process,args=(PPath, OPath))
		p.start()

	p.join()
