import csv
name_start = "parallel_thread_"
size = 4
memLocations = {}
for i in range(size+1):
  filename = name_start+`i`+".csv"
  with open(filename) as file:
    reader = csv.reader(file)
    threadNum = i
    for row in reader:
      memLocation = int(row[1].strip(), 10)
      # memLocation -= memLocation % 64
      if memLocation in memLocations:
        if threadNum in memLocations[memLocation]:
          memLocations[memLocation][threadNum]+=1
        else:
          memLocations[memLocation][threadNum]=1
        pass
      else:
        memLocations[memLocation]={}
        memLocations[memLocation][threadNum]=1
        pass
      pass
    pass
  pass
keys = sorted(memLocations.keys())
print "Threadid,",
for memLocation in keys:
  print memLocation, ',',
  pass
print ""
for threadNum in range(size+1):
  print threadNum,",",
  for memLocation in keys:
    if threadNum in memLocations[memLocation]:
      print memLocations[memLocation][threadNum],",",
    else:
      print "-,",
      pass
  print ''
  pass

