name_start = "parallel_thread_"
size = 10
memLocations = {}
for i in range(size+1):
  filename = name_start+`i`+".txt"
  if i == size:
    filename = name_start+"main.txt"
  with open(filename) as file:
    isFirst = True
    threadNum = size
    for line in file:
      if isFirst and i != size:
        isFirst = False
        threadNum = int(line)
        pass
      else:
        isFirst = False
        memLocation = int(line.strip(), 10)
        memLocation -= memLocation % 64
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
  pass
for memLocation in memLocations:
  if len(memLocations[memLocation]) < 2:
    continue
    pass
  print '------------------'
  print memLocation
  for threadNum in memLocations[memLocation]:
    print threadNum, memLocations[memLocation][threadNum]
    pass
  print '------------------'
  pass

