#######################################
# Created by Maximillian A. Campbell ##
#######################################
# ======= Intro =======
print("""                              ______________                               
                        ,===:'.,            `-._                           
Art by                       `:.`---.__         `-._                       
 John VanderZwaag              `:.     `--.         `.                     
                                 \.        `.         `.                   
                         (,,(,    \.         `.   ____,-`.,                
                      (,'     `/   \.   ,--.___`.'                         
                  ,  ,'  ,--.  `,   \.;'         `                         
                   `{D, {    \  :    \;                                    
                     V,,'    /  /    //                                    
                     j;;    /  ,' ,-//.    ,---.      ,                    
                     \;'   /  ,' /  _  \  /  _  \   ,'/                    
                           \   `'  / \  `'  / \  `.' /                     
                            `.___,'   `.__,'   `.__,'  DragonBreath""")


# ======= Libraries =======
import ghidra.app.decompiler.DecompInterface
import ghidra.program.model.symbol
from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

# ======= Variables =======

# Loading Application
program = getCurrentProgram()
ifc = DecompInterface()

# Output Directory
OUTPUT_DIR = "/tmp" # Change me!

# ======= Functions ======
def decompileFunction(function):
	outputFile = open(OUTPUT_DIR + func.getName() + '.c','w')
	results = ifc.decompileFunction(function,0,ConsoleTaskMonitor())
	outputFile.write(results.getDecompiledFunction().getC())
	outputFile.close()


# ======= Main =======

ifc.openProgram(program)
func = getFirstFunction() # Entry point

while func is not None:
	try: function = getGlobalFunctions(func.getName())[0]	
	except:
		func = getFunctionAfter(func)
		continue

	print("[+] Decompiling: {} @ 0x{}".format(func.getName(),func.getEntryPoint()))
	decompileFunction(function)
	func = getFunctionAfter(func)

