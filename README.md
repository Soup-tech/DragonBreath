<html>
  <header>
    <h1>DragonBreath</h1>
    <h2>NASA SRE Research Project</h2>
    <img src='assets/Dracarys.gif' width='100%'>
    <h3>This repository is the compilation of all the scripts I have developed while working for the NASA Reverse Engineering Study.</h3>
    <h3><i>"It simply isn't an adventure worth telling if there aren't any dragons."<br>- J.R.R Tolkien</i></h3>
  </header>
  
  <body>
    <!-- Table of Contents -->
    <h1>Table of Contents</h1>
    <ul>
      <li><h3><a href="DragonBreath">DragonBreath</a></h3></li>
      <li><h3><a href="Smaug">Smaug</a></h3></li>
    </ul>
    <h2><a id="DragonBreath">DragonBreath</a></h2>
    <p>The best (and my personal favorite) script that was written for this research project. DragonBreath collects all the decompiled code contained within an application.</p>
    <p>This script decompiles all the functions in the executable that is currently loaded into Ghidra. Place the script in the ghidra_scripts directory. Change the OUTPUT_DIR variable to where you would like to write all the source files to and run the script.</p>
    <code><p>Example Output from Aeon 0.2a</p></code>
    <pre>
dragonBreath.py> Running...
                              ______________                               
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
                            `.___,'   `.__,'   `.__,'  DragonBreath
[+] Decompiling: _init @ 0x00101000
[+] Decompiling: FUN_00101020 @ 0x00101020
[+] Decompiling: _start @ 0x001014c0
[+] Decompiling: deregister_tm_clones @ 0x001014f0
[+] Decompiling: register_tm_clones @ 0x00101520
[+] Decompiling: __do_global_dtors_aux @ 0x00101560
[+] Decompiling: frame_dummy @ 0x001015a0
[+] Decompiling: main @ 0x001015a9
[+] Decompiling: establishConnection @ 0x00101b3c
[+] Decompiling: sayEHLO @ 0x00101d30
[+] Decompiling: authorizePlain @ 0x00101f1a
[+] Decompiling: sayWho @ 0x00102129
[+] Decompiling: sayWhere @ 0x001022cb
[+] Decompiling: sendMessageHeader @ 0x00102466
[+] Decompiling: sendData @ 0x001026d9
[+] Decompiling: sayQuit @ 0x00102881
[+] Decompiling: logEntry @ 0x001029c6
[+] Decompiling: parseReply @ 0x00102a32
[+] Decompiling: getConfig @ 0x00102a8b
[+] Decompiling: setup @ 0x00102bba
[+] Decompiling: rmSpaces @ 0x00102f74
[+] Decompiling: logTimestamp @ 0x00102ffd
[+] Decompiling: getMessageHeader @ 0x00103090
[+] Decompiling: extractMailAddr @ 0x001033a0
[+] Decompiling: authCat @ 0x001034b2
[+] Decompiling: encodeB64 @ 0x001035d2
[+] Decompiling: __libc_csu_init @ 0x00103960
[+] Decompiling: __libc_csu_fini @ 0x001039d0
[+] Decompiling: _fini @ 0x001039d8
[+] Decompiling: _ITM_deregisterTMCloneTable @ 0x00107038
[+] Decompiling: __gmon_start__ @ 0x001070c8
[+] Decompiling: _ITM_registerTMCloneTable @ 0x00107120
dragonBreath.py> Finished!
	   </pre>
  </body> 
  <h2><a id="Smaug">Smaug</a></h2>
    <p>Smaug was an experimental Python script that I worked on in my free time while working for the NASA IV&V WVSGC project. Smaug is meant to be an encapsulation of all the work I had done while working on the project. Smaug, using the revised FlawFinder ruleset, would scan both source code and decompiled byte code and attempt to match the vulnerability detected in the original source code with the corresponding vulnerability in the decompiled machine code.</p>
</html>

