// Program for spawning threads to multiply two large multi digit integers.

import java.util.Scanner;
import java.util.Vector;

class Multiplier implements Runnable {
  long i;
  long multiplier, multiplicand;
  
  public Multiplier(long i, long multiplier, long multiplicand) {
    this.i = i;
    this.multiplier = multiplier;
    this.multiplicand = multiplicand;
  }
  
  @Override
  public void run() {
    Implementation.shared_vector.addElement(i*multiplier*multiplicand);
  }
}

class Implementation {

  long num1, num2;
  static Vector shared_vector = new Vector();
  
  public void compute() {
    Scanner in = new Scanner(System.in);
    Vector<Thread> threadPool = new Vector<Thread>();
    
    System.out.println("\nEnter num1: ");
    num1 = in.nextLong();
    System.out.println("\nEnter num2: ");
    num2 = in.nextLong();
    
    long i = 1;
    
    while ((num2 = num2 % 10) != 0) {
      Thread thread = new Thread(new Multiplier(i,num2,num1));
      thread.start();
      threadPool.addElement(thread);
    }
    
    for (int j = 0; j < threadPool.size(); ++j) {
      try {
        threadPool.get(j).join();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
    
    in.close();
  }
}

public class Runner {
	public static void main() {
	  Implementation i = new Implementation();
	  i.compute();	  
	}
}