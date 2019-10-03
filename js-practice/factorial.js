function FirstFactorial(num) { 
  if (num == 1) {
    return 1;
  }
  else {
    return FirstFactorial(num) * FirstFactorial(num-1);
  }
}
   
// keep this function call here 
FirstFactorial(readline());                            
  
