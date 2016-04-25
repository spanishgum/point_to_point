import os, sys, subprocess, argparse
	
cmds = [
	['./main', '-d', 'n10.dat', '-g', 'n10.graph', '-n', '10'],
	['./main', '-d', 'n50.dat', '-g', 'n50.graph', '-n', '50'],
	['./main', '-d', 'n100.dat', '-g', 'n100.graph', '-n', '100'],
	['./main', '-d', 'n500.dat', '-g', 'n500.graph', '-n', '500'],
	['./main', '-d', 'n1000.dat', '-g', 'n1000.graph', '-n', '1000']
]
	
def main():
	global cmds
	
	for cmd in cmds:
		with open('results.txt', 'a') as out:
			subprocess.call(cmd, stdout=out)
#
# Call this module as a main program
if __name__ == '__main__':
	main()
	sys.exit(0)
	
######################################## old version
	
def usage():
	sys.stderr.write('Usage: ./main [-t0] [-t1] [-d datfile -g graphfile -n N]\n')
	sys.exit()

def depracated_main():
	parser = get_parser()
	args = parser.parse_args()
	
	if not args.dfile or not args.gfile or not args.nsize:
		usage()
	else:
		dfile = args.dfile
		gfile = args.gfile
		nsize = int(args.nsize)
	os.system('./main -d ' + dfile + ' -g ' + gfile + ' -n ' + nsize)

def get_parser():
	p = argparse.ArgumentParser(description='N3Parser that splits triples with line breaks')
	p.add_argument('-d', '--dfile', help='specify input file')
	p.add_argument('-g', '--gfile', help='specify graph file')
	p.add_argument('-n', '--nsize', help='specify data size')
	return p
