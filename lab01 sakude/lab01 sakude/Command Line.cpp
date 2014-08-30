if (key_input==ENTER)	 // Identify Enter
  {   switch(buffer[0]) { 	// Example command entry:”cor 2” 
   case 'c': // Set color which number starts from buffer position 3
		cor=atoi(&buffer[3]);
		if (cor>=0 && cor<16)
			color=cor;
		   	 ClearString(buffer);  // Erase buffer
		break;	
  	
}
key_input=-1;
}