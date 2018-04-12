# coding=utf8
# the above tag defines encoding for this document and is for Python 2.x compatibility

import re

regex = r"(\d+,\d+)\s+mem_load_uops_l3_hit_retired_xsnp_hitm"

test_str = ''

with open('cat.txt', 'r') as file:
    for line in file:
        test_str += line



matches = re.finditer(regex, test_str)
index = 0
for matchNum, match in enumerate(matches):
    matchNum = matchNum + 1
    
    # print ("Match {matchNum} was found at {start}-{end}: {match}".format(matchNum = matchNum, start = match.start(), end = match.end(), match = match.group()))
    
    for groupNum in range(0, len(match.groups())):
        groupNum = groupNum + 1
        index += 1
        print ("{index},{group}".format(index=index, group = match.group(groupNum)))

# Note: for Python 2.7 compatibility, use ur"" to prefix the regex and u"" to prefix the test string and substitution.

