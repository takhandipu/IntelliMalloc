import re
import sys

size = int(sys.argv[1])
for power in range(8):
    d = 2**power
    regex1 = r"([\d,]+)\s+mem_load_uops_l3_hit_retired_xsnp_hitm"
    regex2 = r"([\d\.]+)\s+seconds"

    test_str =  ""

    name = ""+`d`+"_"+`size`+".txt"
    # print name
    # '''
    with open(name, "r") as file:
        for line in file:
            test_str += line
            pass
        pass

    matches1 = re.finditer(regex1, test_str)
    matches2 = re.finditer(regex2, test_str)

    print d, ",",

    for matchNum, match in enumerate(matches1):
        matchNum = matchNum + 1
        for groupNum in range(0, len(match.groups())):
            groupNum = groupNum + 1
            print "{groupNum}".format(groupNum = match.group(groupNum)),",",
            pass
        pass
    for matchNum, match in enumerate(matches2):
        matchNum = matchNum + 1
        for groupNum in range(0, len(match.groups())):
            groupNum = groupNum + 1
            print "{groupNum}".format(groupNum = match.group(groupNum))
            pass
        pass
    # '''
    pass






