import subprocess,os

print("[COMPILER] Starting compilation...")

def GetLib(x):
    return "-l"+x

commandArgs = []
commandArgs.append(r'g++ *.cpp *.h')
#commandArgs.append(r'-I"F:\OpenGL Libraries\freeglut\include"')
#commandArgs.append(r'-L"F:\OpenGL Libraries\freeglut\lib\x64"')
#commandArgs.append(r'-I"F:\OpenGL Libraries\glew-2.1.0\include"')
#commandArgs.append(r'-L"F:\OpenGL Libraries\glew-2.1.0\lib\Release\x64"')
#commandArgs.append(r'-I"F:\OpenGL Libraries\glfw-3.3.8.bin.WIN64\include"')
#commandArgs.append(r'-L"F:\OpenGL Libraries\glfw-3.3.8.bin.WIN64\lib-mingw-w64"')
#commandArgs.append(GetLib('opengl32'))
#commandArgs.append(GetLib('pthread'))
#commandArgs.append(GetLib('freeglut'))
#commandArgs.append(GetLib('glfw3'))
#commandArgs.append(GetLib('glu32'))
#commandArgs.append(GetLib('freeGLUT'))
#commandArgs.append(GetLib('glew32'))
#commandArgs.append(GetLib('ws2_32'))
#commandArgs.append(GetLib('gdi32'))
commandArgs.append('-static-libstdc++')
commandArgs.append('-std=c++17')
commandArgs.append('-fconcepts')
commandArgs.append('-g')
commandArgs.append('-o RunProg.exe')

command = ""
for i in commandArgs:
    command += i + " "

result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).stdout.decode('utf-8')

if (result != ""):
    print("[COMPILER] A compilation error has occured")
    print()
    print(result)
    print()
    print()
    print("-----------------------------")
    print("[SYSTEM] Press enter to close")
    print()
    input()
else:
    print("[COMPILER] Compilation completed!")
    print("[SYSTEM]   Press 'Enter' to run program")
    input()
    os.system("cls")
    os.system("\"" + os.getcwd() + "\\RunProg.exe" + "\"")
    print()
    print()
    print("-----------------------------")
    print("[SYSTEM] Press enter to close")
    print()
    input()
    
