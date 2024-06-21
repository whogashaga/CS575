import os
import subprocess
from sys import platform
def StartTest():
    for b in [8, 32, 128]:
        for t in [1024, 4096, 16384, 65536, 262144, 1048576, 2097152, 4194304]:
            if platform == "linux" or platform == "linux2":
                os.system(f"/usr/local/apps/cuda/cuda-10.1/bin/nvcc -o proj05 proj05.cu -DNUMTRIALS={t} -DBLOCKSIZE={b}")
            p = subprocess.Popen(f"./proj05",  stderr= open("./proj05.csv",'a'), universal_newlines=True, shell=True)
            p.wait()
if __name__ == '__main__':
    StartTest()