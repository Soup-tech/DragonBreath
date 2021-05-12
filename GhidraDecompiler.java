// @TODO Work in progress
// This is a helper function for DragonBreath.py
// @author Soup-tech


import ghidra.app.decompiler.ClangLine;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.decompiler.DecompiledFunction;
import ghidra.app.decompiler.PrettyPrinter;
import ghidra.app.decompiler.component.DecompilerUtils;

import ghidra.app.util.headless.HeadlessScript;

import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.pcode.HighFunction;

import java.util.ArrayList;

public class GhidraDecompiler extends HeadlessScript {

	@Override
  	public void run() throws Exception {

    	// Stop after this headless script
    	setHeadlessContinuationOption(HeadlessContinuationOption.ABORT);

    	// Get the function address from the script arguments
    	String[] args = getScriptArgs();
    	println(String.format("Array length: %d", args.length));

    	if (args.length == 0) {
      		System.err.println("Error: Please specify a function address!");
      		System.err.println("Remember: it's deadbeef no 0xdeadbeef");
      		return;
    	}

    	int functionAddress;
    	
    	try {
      		functionAddress = Integer.parseInt(args[0], 16);
    	}
    	catch (NumberFormatException e) {
      		System.err.println(String.format("Invalid hex address: %s", args[0]));
      		return;
    	}
   		
   		println(String.format("Address: %x", functionAddress));

    	DecompInterface decompInt = new DecompInterface();
    	println("Simplification style: " + decompInt.getSimplificationStyle());
    	println("Debug enables: " + decompInt.debugEnabled());

    	Function f = this.getFunction(functionAddress);
    	if (f == null) {
      		System.err.println(String.format("Function not found at 0x%x", functionAddress));
      		return;
    	}

    	println(String.format("Decompiling %s() at 0x%x", f.getName(), functionAddress));

    	println("Program: " + decompInt.openProgram(f.getProgram())); 

    	DecompileResults decompResults = decompInt.decompileFunction(f, 5, null);
    	println("Decompilation completed: " + decompResults.decompileCompleted()); 

    	DecompiledFunction decompFunction = decompResults.getDecompiledFunction();
    	println(decompFunction.getC());

	    // Print lines prepend with addresses
	    PrettyPrinter pp = new PrettyPrinter(f, decompResults.getCCodeMarkup());
	    ArrayList<ClangLine> lines = pp.getLines();

	    for (ClangLine line : lines) {
	      long minAddress = Long.MAX_VALUE; 
	      long maxAddress = 0; 
	      for (int i = 0; i < line.getNumTokens(); i++) {
	    	
	    	if (line.getToken(i).getMinAddress() == null) {
	        	continue; 
	        }
	        
	        long addr = line.getToken(i).getMinAddress().getOffset();
	        minAddress = addr < minAddress ? addr : minAddress;
	        maxAddress = addr > maxAddress ? addr : maxAddress;
	      	}
	    	
	    	if (maxAddress == 0) {
	        	println(String.format("                      - %s", line.toString()));
	      	} else {
	        	println(String.format("0x%-8x 0x%-8x - %s", minAddress, maxAddress, line.toString()));
	      	}

	    }
	}

  	protected Function getFunction(int address) {

	    Listing listing = currentProgram.getListing();
	    FunctionIterator iter = listing.getFunctions(true);
	    
	    while (iter.hasNext() && !monitor.isCancelled()) {
	      	Function f = iter.next();
	      	if (f.isExternal()) {
	        	continue;
	      	}

	      	Address entry = f.getEntryPoint();
	      	if (entry != null && entry.getOffset() == address) {
	        	return f;
	    	}
	    }
		
		return null;
	}

}
