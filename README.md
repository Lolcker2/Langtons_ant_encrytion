.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ר
|																							    __                          |
|		 __.		   ___		  __	__.   _________.   __________   _________.    __    __.	\.\  ________               |
|		|  |		  /	  \		 |  \  |  |  |   ______|  (____	 ____) |    ___  |   |  \  |  |  |/ /  ____  |              |
|		|  | 		 /	_  \	 |	 \ |  |  |  |   ___	  	  |	 |	   |   |   | |   |   \ |  |     \_(___ \_|              |
|		|  |		/  /_\  \	 |	 .`|  |  |  |  (_  |	  |  |     |   |   | |   |   .`   |     _'.___`_                |
|		|  |___	   /  _____  \   |	| \   |  |  |___|  |      |  |     |   |___| |   |  | \   |    | \____) \               |
|		|______)  /__/	   \__\  |__|  \__|  |_________|      |__|     |_________|   |__|  \__|    |________/               |
|                                                                                                                           |
|		 ___		__	  __.  ________	      ______.   __    __.   _______   _________.   ______     ______.   ______      |
|       /	\      |  \  |  | (___  ___)     |   ___|  |  \  |  |  |   ____) |    __   |  |   _  \   |   ___|  |   __ \     |
|      /  _	 \     |   \ |  |	 |  |		 |  |__    |   \ |  |  |  |      |   |  |  |  |  | \  \  |  |__    |  (__) )    |
|     /	 /_\  \    |   .`|  |    |  |        |	 __)   |   .`|  |  |  |      |   |  |  |  |  |  )  ) |   __)   |   _  /     |
|	 / 	_____  \   |  | \   |    |  |        |	|___   |  | \   |  |  |____  |   |__|  |  |  |_/  /  |  |___   |  | \  \    |
|	/__/     \__\  |__|  \__|    |__|        |______|  |__|  \__|  |_______) |_________|  |______/   |______|  |__|  \__\   |
|                                                                                                                           |
|                                                                                                                           |
L~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~J
														   _____________________
														__/LANGTON'S ANT ENCODER\___
													   / .__                    __. \ 
													   | |[#] MADE BY: EYAL LOCKER| |
												        V	                       V	
														

the Langton's ant encoder is a program that encodes a word.
it does that via couple steps:
	
	1.) get the word, and encode it (1/2 encryptions) via "LR code".
	2.) run the famous algorithem "Langton's ant" (I have made sagnificant changes to it), (2/2 encryptions).
	3.) the algorithem returns a matrix of numbers, parse that into ascii characters.
	4.) Ooo there is a fourth step, who knew....., anyways, by this stage ur done!!
	
	
I decided to call the whole algorithem "LANGTON'S ANT ECODER".
The algorithem behaves like a "One way hash" algorithem, although, Im preaty sure that it aint one.
There is a visual part, and it is the langton's ant first douzen step.

In the program, many files exist, let me indreduce those:

	
- UTILS.INC:
	In the file "utils.inc", all of the procs are declared.
	here is a list of those procs:

	 - Modulo:
		takes 2 inputs (a, b).
		returns a mod b, in c# -> (a%b)
		
		additional notes:
			it overwrites the value of bp, but at the end, rewinds back to that prev value.
			pushes the result to the stack.
		
		
	- Fake_push:
		takes no inputs
		returns void
	  
		additional notes:
			it appends the value of cl into a list called "Malloc", thats programed to act similar to a stack,
			that stack uses size of db for it's elements.
			
			
	- Fake_pop:
		takes no inputs
		returns void
		
		additional notes:
			it pops the lastest value from the list called "Malloc".
			becase Malloc can be accessed like a traditional list, then there is no reason to return the poped value.
			
			
	- Movement:
		takes no inputs
		returns 2 variables, (the new position of the ant)
		
		additional notes:
			based on a global variable called "dir", it moves the ant.
			the dir - moves chart is the following:
							
							0
					1		    5
						 dir
					2   		4
							3
							
			meaning that if for example, dir is 0,
			then the ant would move upwards, but if dir is 1,
			then it'll move in a diagonal line (up & left).
			
			
	- Rotation:
		takes 1 input
		reutrns void
		
		additional notes:
			turns the ant based on a key (the input).
			turns by changing the gloabal variable called "dir".
			the key - dir chart is the following:
				
							N
					L	    	R			
							^
						 key
					X		    Y
							U
							
			[*] the arrow is the direction of the ant (dir),
				and those keys are in an offset from it,
				the amount & direction of the offset, determents the next dir (after the turn)
				
			meaning that if for example, the key is 'N', then dir will stay the same,
			but it the key is 'X', then the ant will turn 120 degrees to the left (each offset is by 60 degrees).
				
	
	- Pos_to_index:
		takes no inputs
		returns / set a variable (an index for the buffer)
		
		additional notes:
			uses a position vector in a matrix and turns it into an index for a 1D buffer that contains the matrix.
			for example:
				for the matrix:
				 _ _ _
				|_|_|_|
				|_|_|_|
				|_|_|_|
											X    Y
				and for the position vector [2, 2], 
				it'll return 9 as it's the 9th address from the start of the buffer
	
	
	- Index_to_pos:
		takes no inputs
		returns / set a variable (a position for computetions)
		
		additional notes:
			basically, the inverse of 'Pos_to_index', meaning, it takes a buffer index, and converts it into a position in the matrix thats saved in the buffer.
			for example:
				for the matrix:
				 _ _ _
				|_|_|_|
				|_|_|_|
				|_|_|_|
				
				and for the buffer index 9
											X    Y
				it'll return the position vector [2, 2]
				since thats the position of that cell in regards to matrix, instead of to the buffer
				
	- Dec_to_base
		takes 2 inputs (number , base)
		returns a number (number at numberical base 'base')
		
		additional notes:
			translate the number 'number' from numberical base '10' AKA DECIMAL, into numberical base 'base'.
			for example:
				for the inputs:
					number -> 60
					base -> 6
					it'll return 10, since 10 at numberical base 6 is the same as the number 60 in decimal.
					
					
				
