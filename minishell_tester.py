import pexpect

testcmds = ['echo hello', 'exit']
minishell = pexpect.spawn('/home/frafal/minishell/minishell')
bash = pexpect.spawn('bash')

def result(str):
    if (minishell.expect(str) == 0):
        return 'OK'
    return 'KO'

def test(cmd, testnum):
    print(f"\nTest {testnum:03d}")
    print(f"{'Command:':10}{cmd}")
    minishell.sendline(cmd)
    # bash.sendline(cmd)
    # print(f"{'Expected:':10}{bash.read}")


def main():
    print('\nTest 01')
    print('Command:\t\techo hello')
    minishell.sendline('echo hello')
    print('Expected:\t\thello')
    print(result('hello'))

    print('\nTest 02')
    print('Command:\t\texit')
    minishell.sendline('exit')
    print('Expected:\t\texit')
    print(result('exit'))

    for testnum, cmd in enumerate(testcmds):
        test(cmd, testnum)

if __name__ == "__main__":
    main()
