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

path = os.environ.get("PATH")
pwd = os.environ.get("PWD")
prompt = "minishell$ "
testcmds_hardcoded = [[["echo hello", "hello"]],
            [["exit", "exit"]],
            [["cat nonexistingfile", "cat: nonexistingfile: No such file or directory"]],
            [
             ["env | grep \"^PATH=\"", "PATH=" + path],
             ["export PATH=$PWD:$PATH", prompt],
             ["env | grep \"^PATH=\"", "PATH=" + pwd + ":" + path],
             ["ls", "this is my ls"]
             ]
            ]

testcmds = [
    ["/bin/ls"],
    [""],
    ["/bin/uname"],
    ["/bin/uname -a"],
    ["     "],
    ["abc"],
    ["echo hello world    how are you??"],
    [" echo -n hello"]
    ]

def result(minishell, str):
    try:
        if (str == prompt):
            minishell.expect_exact("\r\n" + str, timeout=1)
        else:
            minishell.expect_exact("\r\n" + str + "\r\n", timeout=1)
        return bcolors.OKGREEN + "OK" + bcolors.ENDC
    except:
        return bcolors.FAIL + "KO" + bcolors.ENDC

def referenceresult(minishell, str):
    try:
        minishell.expect_exact(prompt, timeout=1)
        if (minishell.before.decode() == str):
            return bcolors.OKGREEN + "OK" + bcolors.ENDC
        return bcolors.FAIL + "KO" + bcolors.ENDC
    except:
        return bcolors.FAIL + "KO" + bcolors.ENDC

def test(cmdlist, testnum):
    minishell = pexpect.spawn(sys.argv[1])
    logfile = "testlogs/minishell_tester_" + str(testnum).zfill(3) + ".log"
    os.makedirs(os.path.dirname(logfile), exist_ok=True)
    minishell.logfile_read = open(logfile, "wb")
    print(bcolors.HEADER + bcolors.BOLD + f"\nTest {testnum:03d}" + bcolors.ENDC)
    for cmd in cmdlist:
        print(f"{'Command:':10}{cmd[0]}")
        minishell.sendline(cmd[0])
        print(f"{'Expected:':10}{cmd[1]}")
        print(result(minishell, cmd[1]))
    minishell.logfile_read.close()

def referencetest(cmdlist, testnum):
    minishell = pexpect.spawn(sys.argv[1])
    minishell_logfile = "testlogs/" + str(testnum).zfill(3) + "_testoutput_minishell.log"
    os.makedirs(os.path.dirname(minishell_logfile), exist_ok=True)
    minishell.logfile_read = open(minishell_logfile, "wb")

    bash = pexpect.spawn("bash")
    bash_logfile = "testlogs/" + str(testnum).zfill(3) + "_testoutput_bash.log"
    bash.logfile_read = open(bash_logfile, "wb")

    print(bcolors.HEADER + bcolors.BOLD + f"\nTest {testnum:03d}" + bcolors.ENDC)
    for i, cmd in enumerate(cmdlist):
        bash.sendline("export PS1=\"minishell$ \"")
        bash.expect_exact("\r\n" + prompt)
        bash.sendline(cmd)
        bash.expect_exact(cmd + "\r\n")
        bash.expect_exact(prompt)
        print(f"{'Command:':10}{cmd}")
        print(f"{'Expected:':10}{bash.before.decode()}")
        minishell.sendline(cmd)
        minishell.expect_exact(cmd + "\r\n")
        print(referenceresult(minishell, bash.before.decode()))
    minishell.logfile_read.close()
    bash.logfile_read.close()
    minishell.sendline("exit")
    bash.sendline("exit")

def main():
    for testnum, cmdlist in enumerate(testcmds):
        referencetest(cmdlist, testnum)

if __name__ == "__main__":
    main()
