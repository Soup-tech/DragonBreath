#!/usr/bin/python3
import manyDecompile as md

def main():
	temp = md.manyDecompile()
	
	temp.setBinaryPath("/home/hades/Desktop/vuln-software/0verkill-0.16/0verkill")
	temp.setDecompiler("/opt/retdec/bin/retdec-decompiler.py")
	temp.setDirectory("/home/hades/Desktop/vuln-software")
	
	temp.getBinaries()

if __name__ == "__main__":
	main()