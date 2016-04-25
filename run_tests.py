import os, sys, subprocess, argparse
	
# cmds = [
	# ['./main', '-d', 'n10.dat', '-g', 'n10.graph', '-n', '10'],
	# ['./main', '-d', 'n50.dat', '-g', 'n50.graph', '-n', '50'],
	# ['./main', '-d', 'n100.dat', '-g', 'n100.graph', '-n', '100'],
	# ['./main', '-d', 'n500.dat', '-g', 'n500.graph', '-n', '500'],
	# ['./main', '-d', 'n1000.dat', '-g', 'n1000.graph', '-n', '1000']
# ]

sizes = [10, 20, 30, 40, 50, 100, 150, 200, 250, 500, 1000]

def gen_cmd(size):
	n = str(size)
	cmd = ['./main', '-d', 'n' + n + '.dat', '-g', 'n' + n + '.graph', '-n', n]
	return cmd

def main():
	cmds = []
	for i in sizes:
		os.system('python gen_data.py ' + str(i) + ' > n' + str(i) + '.dat')
		cmds.append(gen_cmd(i))
	for cmd in cmds:
		for i in range(2):
			with open('results.txt', 'a') as out:
				subprocess.call(cmd, stdout=out)
#
# Call this module as a main program
if __name__ == '__main__':
	main()