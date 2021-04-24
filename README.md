<html>
  <header>
    <h1>DragonBreath</h1>
    <h2>Mass Decompiling Executables Using Ghidra</h2>
    <h3>Need the decompiled output of every function from Ghidra? Let DragonBreath find all the functions and decompile them for you! So kick back and enjoy the roasting of your enemies... Er, I mean executables...</h3>
    <img src='assets/Dracarys.gif' width='100%'>
  </header>
  
  <body>
  <h2>Prerequisite</h2>
  <p>Python3 : https://www.python.org/downloads/</p>
  <p>Ghidra : https://ghidra-sre.org/</p>
  <p><strong>Important:</strong> You will need to copy GhidraDecompiler.java into your ghidra_scripts folder and have Ghidra recognize it as a usable script.</p>
  <h2>Usage</h2>
    <p>./dragonBreath.py [Top Level Directory] [Ghidra Project Directory] [analyzeHeadless Absolute Path] [option] ... [-h help | -q quiet]
    <p> <strong>Top Level Directory:</strong> The directory where all of your binaries are stored. Currently, the script will detect any executables within the depth of two directories.</p>
    <p> <strong>Ghidra Project Directory:</strong> The directory to your Ghidra Project Folder. This needs to be created in Ghidra GUI before this script can be run.</p>
    <p> <strong>analyzeHeadless Absolute Path: </strong> The absolute path to the analyzeHeadless exectuable. This will be under your Ghidra folder in the support directory.</p>
   <h2>Output</h2>
   <p> Your decompiled output will be stored in a folder in the same level as the executable. DragonBreath will store the output in a folder starting with src_Ghidra. Currently, DragonBreath does not look for and clean up this folder before running, meaning if you want to rerun DragonBreath on an executable, you'll have to decide whether or not to delete all of the files from the first run.</p>
  </body>
  
</html>
