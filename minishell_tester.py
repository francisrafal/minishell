import pexpect
import sys
import os

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

testcmds = [["echo hello", "hello"],
            ["exit", "exit"],
            ["cat nonexistingfile", "cat: nonexistingfile: No such file or directory"]]

def result(minishell, str):
    try:
        minishell.expect("\r\n" + str + "\r\n", timeout=1)
        return bcolors.OKGREEN + "OK" + bcolors.ENDC
    except:
        return bcolors.FAIL + "KO" + bcolors.ENDC

def test(cmd, testnum):
    minishell = pexpect.spawn(sys.argv[1])
    logfile = "testlogs/minishell_tester_" + str(testnum).zfill(3) + ".log"
    os.makedirs(os.path.dirname(logfile), exist_ok=True)
    minishell.logfile = open(logfile, "wb")
    print(bcolors.HEADER + bcolors.BOLD + f"\nTest {testnum:03d}" + bcolors.ENDC)
    print(f"{'Command:':10}{cmd[0]}")
    minishell.sendline(cmd[0])
    print(f"{'Expected:':10}{cmd[1]}")
    print(result(minishell, cmd[1]))
    minishell.logfile.close()


def main():
    for testnum, cmd in enumerate(testcmds):
        test(cmd, testnum)

if __name__ == "__main__":
    main()
