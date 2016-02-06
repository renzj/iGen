fp2 = open("IATTest_arrivalExe_output.log", "r")
fp1 = open("IATTest_Tool_output.log", "r")
lines1 = fp1.readlines()
lines2 = fp2.readlines()
sum = 0
for i in range(0,len(lines1) - 1):
	if i == 0:
		continue
	sum = sum + long(lines2[i]) - long(lines1[i])

print sum
print sum / (len(lines1) - 1)
