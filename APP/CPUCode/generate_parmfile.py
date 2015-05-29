#!/usr/bin/env python
import sys

def yn_question(question):
	valid = {"yes":"yes", "y":"yes", "ye":"yes", "no":"no", "n":"no"}

	while 1:
		answer = raw_input(question).lower()
		if answer in valid.keys():
			return valid[answer]
		else:
			print "Unknown input. Please respond with 'yes' or 'no'"


print "=========Parameter File generator for Explicit Heat Equation======"
filename = raw_input("Enter name for your parmfile: ")

nx = int(input("Number of points in the fast dimension(x): "))
ny = int(input("Number of points in the medium dimension(y): "))
nz = int(input("Number of points in the slow dimension(z): "))

iteration = int(input("Number of iterations: "))
dt = float(input("DT value: "))
dx = float(input("DX value: "))
dy = float(input("DY value: "))
dz = float(input("DZ value: "))

use_input_file = yn_question("Use input file? ")

if use_input_file == "yes":
	input_flag = 1
	input_dir = raw_input("Enter input file path: ")
else:
	input_flag = 0


use_model_file = yn_question("Use model file? ")

if use_model_file == "yes":
	model_flag = 2
	model_dir = raw_input("Enter model file path")
else:
	use_default_model = yn_question("Use default alpha value?")
	if use_default_model == "yes":
		model_flag = 0
	else:
		model_flag = 1
		model_value = float(input("Enter alpha value: "))

print "Confirming input...."
print "File name : " + filename
print "nx : " + str(nx)
print "ny : " + str(ny)
print "nz : " + str(nz)
print "iteration : " + str(iteration)
print "dt : " + str(dt)
print "dx : " + str(dx)
print "dy : " + str(dy)
print "dz : " + str(dz)
print "input flag : " + str(input_flag)
if input_flag == 1:
	print "input location : " + input_dir
print "model flag : " + str(model_flag)
if model_flag == 2:
	print "model location : " + model_dir
if model_flag == 1:
	print "model value : " + str(model_value)

write_file = yn_question("Continue? ")

if write_file == "no" :
	sys.exit()

fp = open(filename, 'w')
fp.write(str(nx)+"\n")
fp.write(str(ny)+"\n")
fp.write(str(nz)+"\n")
fp.write(str(iteration)+"\n")
fp.write(str(dt)+"\n")
fp.write(str(dx)+"\n")
fp.write(str(dy)+"\n")
fp.write(str(dz)+"\n")
fp.write(str(input_flag)+"\n")
if input_flag == 1:
	fp.write(input_dir+"\n");
fp.write(str(model_flag)+"\n")
if model_flag == 1:
	fp.write(str(model_value)+"\n");
if model_flag == 2:
	fp.write(model_dir+"\n");

