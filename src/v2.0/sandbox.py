#!/usr/bin/python3
import manyDecompile as md

def main():
	temp = md.manyDecompile()
	
	temp.setBinaryPath("/home/hades/Desktop/vuln-software/0verkill-0.16/0verkill")
	temp.fetchMethods()
	print(temp.getMethods())

if __name__ == "__main__":
	main()