import os
import random
import shutil


path = "../data/"
path_output = "../processedData/"
n_images = 72
dictionary_creation_percentage = 0.3
training_percentage = 0.5
test_percentage = 0.2

n_dictionary_creation = int(dictionary_creation_percentage * n_images)
n_test = int(test_percentage * n_images)
n_training = n_images - n_test - n_dictionary_creation

print "Dictionary data: %d" % n_dictionary_creation
print "Training data: %d" % n_training
print "Test data: %d" % n_test

os.makedirs(path_output+"/"+"dictionary-creation")
os.makedirs(path_output+"/"+"training")
os.makedirs(path_output+"/"+"test")

for f1 in os.listdir(path):
    if os.path.isdir(path+f1):
        remaining_Images = os.listdir(path+f1)
        random.shuffle(remaining_Images)

        for x in remaining_Images[:n_dictionary_creation]:
            shutil.copy(path+f1+"/"+x, path_output+"dictionary-creation")
        remaining_Images = remaining_Images[n_dictionary_creation:]

        for x in remaining_Images[:n_training]:
            print path+f1+"/"+x
            shutil.copy(path+f1+"/"+x, path_output+"training")
        remaining_Images = remaining_Images[n_training:]

        for x in remaining_Images:
            shutil.copy(path+f1+"/"+x, path_output+"test")
