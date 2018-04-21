import csv
import os
threadDicts = {}

threadId = 0
for root,dirs,files in os.walk('.'):
  for file in files:
    if file.endswith(".csv"):
      with open(file, "r") as f:
        reader = csv.reader(f)
        rows = list(reader)
        for row in rows:
          memAddr = int(row[0])
          memAddr -= (memAddr % 64)
          count = int(row[1])
          if memAddr not in threadDicts:
            threadDicts[memAddr]={}
            threadDicts[memAddr][threadId]=count
          else:
            if threadId in threadDicts[memAddr]:
              threadDicts[memAddr][threadId]+=count
            else:
              threadDicts[memAddr][threadId]=count
        threadId += 1

for memAddr in threadDicts:
  if len(threadDicts[memAddr]) > 1:
    print memAddr
