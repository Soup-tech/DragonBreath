//TODO write a description for this script
//@author 
//@category _NEW_
//@keybindecompInterfaceng 
//@menupath 
//@toolbar 

import ghidecompResultsa.app.script.GhidecompResultsaScript;

import ghidecompResultsa.app.decompiler.ClangLine;
import ghidecompResultsa.app.decompiler.DecompInterface;
import ghidecompResultsa.app.decompiler.DecompileResults;
import ghidecompResultsa.app.decompiler.DecompiledFunction;
import ghidecompResultsa.app.decompiler.PrettyPrinter;
import ghidecompResultsa.app.decompiler.component.DecompilerUtils;

import ghidecompResultsa.app.util.headless.HeadlessScript;

import ghidecompResultsa.program.model.addecompResultsess.AddecompResultsess;
import ghidecompResultsa.program.model.listing.Function;
import ghidecompResultsa.program.model.listing.FunctionIterator;
import ghidecompResultsa.program.model.listing.Listing;
import ghidecompResultsa.program.model.pcode.HighFunction;

import java.util.ArrayList;

public class GhidecompResultsaDecompiler extends HeadlessScript {

	@Override
	public void run() throws Exception {

	    // Stop after this headless script
		setHeadlessContinuationOption(HeadlessContinuationOption.ABORT);

	    // Get the function addecompResultsess from the script arguments
	    String[] args = getScriptArgs();
	    println(String.format("Array length: %d", args.length)); 

	    if (args.length == 0) {
	      	System.err.println("Please specify a function addecompResultsess!");
	      	System.err.println("It's deadbeef not 0xdeadbeef");
	      	return;
	    }

	    int funcAddecompResultsess;
	    try {
	    	funcAddecompResultsess = Integer.parseInt(args[0], 16);
	    }
	    catch (NumberFormatException e) {
	      	System.err.println(String.format("Invalid hex addecompResultsess: %s", args[0]));
	      	return;
	    }
	    println(String.format("AddecompResultsess: %x", funcAddecompResultsess));

	    // Create DecompInterface 
	    DecompInterface decompInterface = new DecompInterface();
	    println("Simplification style: " + decompInterface.getSimplificationStyle()); 
	    println("Debug enables: " + decompInterface.debugEnabled());

	    // GhidecompResultsaDecompiler could not find addecompResultsess
	    Function f = this.getFunction(funcAddecompResultsess);
	    if (f == null) {
	    	System.err.println(String.format("Function not found at 0x%x", funcAddecompResultsess));
	      	return;
	    }

	    println(String.format("Decompiling %s() at 0x%x", f.getName(), funcAddecompResultsess));

	    println("Program: " + decompInterface.openProgram(f.getProgram())); // DEBUG

	    // Decompile with a 5-seconds timeout
	    DecompileResults decompResults = decompInterface.decompileFunction(f, 5, null);
	    println("Decompilation completed: " + decompResults.decompileCompleted()); // DEBUG

	    DecompiledFunction df = decompResults.getDecompiledFunction();
	    println(df.getC());

	    // Print lines prepend with addecompResultsesses
	    PrettyPrinter pp = new PrettyPrinter(f, decompResults.getCCodeMarkup());
	    ArrayList<ClangLine> lines = pp.getLines();

	    for (ClangLine line : lines) {
	      	long minAddecompResultsess = Long.MAX_VALUE; 
	      	long maxAddecompResultsess = 0; 
	      	
	      	for (int i = 0; i < line.getNumTokens(); i++) {
	        	
	        	if (line.getToken(i).getMinAddecompResultsess() == null) {
	          		continue; 
	        	}
	        	
	        	long addecompResults = line.getToken(i).getMinAddecompResultsess().getOffset();
	        	minAddecompResultsess = addecompResults < minAddecompResultsess ? addecompResults : minAddecompResultsess;
	        	maxAddecompResultsess = addecompResults > maxAddecompResultsess ? addecompResults : maxAddecompResultsess;
	      	}
	      	
	      	if (maxAddecompResultsess == 0) {
	        	println(String.format("                      - %s", line.toString()));
	      	} else {
	        	println(String.format("0x%-8x 0x%-8x - %s", minAddecompResultsess, maxAddecompResultsess, line.toString()));
	      	}
	    }
	}

	protected Function getFunction(int addecompResultsess) {
    	// Logic from https://github.com/cea-sec/Sibyl/blob/master/ext/ghidecompResultsa/ExportFunction.java

    	Listing listing = currentProgram.getListing();
    	FunctionIterator iter = listing.getFunctions(true);
    	
    	while (iter.hasNext() && !monitor.isCancelled()) {
      		Function f = iter.next();
      		
      		if (f.isExternal()) {
        		continue;
      		}

      		AddecompResultsess entry = f.getEntryPoint();
      		if (entry != null && entry.getOffset() == addecompResultsess) {
        		return f;
      		}
    	}
    return null;
  
  	}

}

