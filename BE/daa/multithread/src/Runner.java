// Name: Sameer Deshmukh
// Class: BE I
// Roll: 405119
// Title: Multithreaded multiplication of 2 large integers

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
  static Vector<Long> shared_vector = new Vector<Long>();
  
  public void compute() {
    Scanner in = new Scanner(System.in);
    Vector<Thread> threadPool = new Vector<Thread>();
    
    System.out.println("\nEnter num1: ");
    num1 = in.nextLong();
    System.out.println("\nEnter num2: ");
    num2 = in.nextLong();
    
//    System.out.println("num1 " + num1);
//    System.out.println("num2 " + num2);
//    System.out.println("num2/10 " + num2/10);
//    System.out.println("num2%10" + num2%10);
    long i = 1;
    
    
    while (true) {
      Thread thread = new Thread(new Multiplier(i,num2%10,num1));
      thread.start();
      threadPool.addElement(thread);
      i *= 10;
      num2 = num2 / 10;
      if (num2 == 0) {
        break;
      }
    }
    
    for (int j = 0; j < threadPool.size(); ++j) {
      try {
        threadPool.get(j).join();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
    
    long product = 0;
    
    for (int j = 0; j < Implementation.shared_vector.size(); ++j) {
      product += Implementation.shared_vector.get(j);
    }
    
    System.out.println("Product is : " + product);
    in.close();
  }
}

public class Runner {
  public static void main(String[] args) {
	  Implementation i = new Implementation();
	  i.compute();	  
	}
}

// OUTPUT:
//
// Enter num1: 
// 3456552
//
// Enter num2: 
// 456325
//
// Product is : 1577311091400