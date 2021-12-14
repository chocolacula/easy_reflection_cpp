import json
import sys

with open(sys.argv[1]) as file:
    data = json.load(file)
    print(data['version'], end ='')