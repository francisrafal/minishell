import pexpect

def result(str):
    if (child.expect(str) == 0):
        return 'OK'
    return 'KO'

child = pexpect.spawn('/home/frafal/minishell/minishell')
print('\nTest 01')
print('Command:\t\techo hello')
child.sendline('echo hello')
print('Expected:\t\thello')
print(result('hello'))

print('\nTest 02')
print('Command:\t\texit')
child.sendline('exit')
print('Expected:\t\texit')
print(result('exit'))
