
import os
import subprocess

decompiler = ['Ghidra','IDA','JEB','retdec','src']
tld = "C:\\NASA-R3-2021\\Test_Suites\\vuldeepecker\\vuldeepecker-analysis"
os.chdir(tld)
dirs = os.listdir()

for d in dirs:
    os.chdir(d)
    l = os.listdir()
    n = []
    for i in l:
        if (i not in decompiler and '.i64' not in i):
            n.append(i)
    
    app = n[0]    
    print(app)
    print("Running IDC on: " + app)
    subprocess.Popen(['C:\\NASA-R3-2021\\IDA Pro 7.6\\ida64','-A','-S C:\\NASA-R3-2021\\produce_c_file.py',app])
    os.chdir('..')
    

