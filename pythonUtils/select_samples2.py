import numpy
import sys
import os
import re
from sklearn.model_selection import train_test_split
from sklearn.model_selection import ShuffleSplit
from sklearn.model_selection import StratifiedKFold

def main():

	if len(sys.argv)<6:
		print 'Usage  <input dataset [.npy |.csv]> <input files path [.txt]> <seed [int] > <number of samples [int]> <testSize [0.0 - 1.0 (float)]>'
		exit(1)

	
	currentDir = os.getcwd()
	symbolBetweenFolder = '/'
	strAux = sys.argv[2]

	occorruences = numpy.array([])
	for m in re.finditer(symbolBetweenFolder, strAux):
		occorruences = numpy.append(occorruences,m.start())
	

	if len(occorruences) == 1:
		start = 0
		if strAux[0] == '.':
			start += 1
		databaseName = strAux[start:int(occorruences[0])]
	elif len(occorruences) > 1:
		lastIndex = int(len(occorruences)-1)
		secondLastIndex = int(len(occorruences)-2)
		start = int(occorruences[secondLastIndex])
		end = int(occorruences[lastIndex])
		databaseName = strAux[start:end]
	else:
		databaseName = ''


	path2files = 	currentDir + symbolBetweenFolder + databaseName
	seed = int(sys.argv[3])
	n = int(sys.argv[4])
	testSize = float(sys.argv[5])
	ntrain = int(n*(1-testSize))
	ntest = n-ntrain;


	indexExtension =  sys.argv[1].rfind(".")
	extension = (sys.argv[1])[indexExtension+1:]
	if(extension == "npy"):
		indata  = numpy.load(sys.argv[1])
	elif(extension == 'csv' or extension == 'txt') :
		indata  = numpy.loadtxt(sys.argv[1])
	else:
		print "weird extension"
		return

	indata  = numpy.load(sys.argv[1])
	inpath  = numpy.loadtxt(sys.argv[2], dtype='str')
	

	classes =  numpy.unique(indata[:,-1])
	nclasses = len(classes)
	samplesPerClassTrain = [0]*(nclasses)
	samplesPerClassTest = [0]*(nclasses)

	for i in xrange(nclasses):
		samplesPerClassTrain[i] = ntrain/nclasses
		samplesPerClassTest[i] = ntest/nclasses

	selectedTrain = []
	selectedTest = []
	numpy.random.seed(seed)
	#import pdb; pdb.set_trace()	
	for i, c in enumerate(classes):
		samples = numpy.where(indata[:,-1] == c)[0]
		numpy.random.shuffle(samples)
		selectedTrain.extend(samples[0: samplesPerClassTrain[i] ])
		selectedTest.extend(samples[ samplesPerClassTrain[i] : samplesPerClassTrain[i]+samplesPerClassTest[i] ])
	
	pathTrain =  numpy.chararray(inpath[selectedTrain].shape,itemsize=100)
	pathTest =  numpy.chararray(inpath[selectedTest].shape,itemsize=100)
	
	k = 0
	for i in selectedTrain:
		pathTrain[k] = path2files +  symbolBetweenFolder + inpath[i]
		k += 1

	k = 0
	for i in selectedTest:
		pathTest[k] = path2files +  symbolBetweenFolder + inpath[i]
		k += 1
	
	numpy.savetxt("train_paths.txt", pathTrain, fmt='%s')
	numpy.savetxt("test_paths.txt", pathTest, fmt='%s')


if __name__=='__main__':
	main()
