import subprocess,os

print("[COMPILER] Starting compilation...")
result = subprocess.run("g++ *.cpp *.h -o RunProg.exe", stdout=subprocess.PIPE, stderr=subprocess.STDOUT).stdout.decode('utf-8')

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
    
