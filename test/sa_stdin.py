import subprocess
from subprocess import Popen, PIPE, STDOUT
p = Popen(['../testing3'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
data = "> chr1\nmississippi\n> chr2\nmississippimississippi\n\n@read1\niss\n@read2\nmis\n@read3\nssi\n@read4\nssippi\n\n"
stdout_data = p.communicate(input=data.encode())[0]

print(stdout_data.decode())


