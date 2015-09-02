package naive_bayes;

import java.util.Vector;

class Data {
  public Integer age, exp, qual, workType;
}

public class Starter {
  Vector<Data> database;
  
  private void loadData() {
//    Consultancy = 0
//    Service = 1
//    Research = 2
    Integer[] workType = {
        0, 
        1, 
        2, 
        1, 
        0,
        2,
        2,
        1,
        0,
        2};
    Integer[] age = {
        30,
        21,
        26,
        28,
        40,
        35,
        27,
        32,
        45,
        36
    };

//  BTech = 0
//  MTech = 1
//    PhD = 2
    Integer[] qual = {
        2,
        1,
        1,
        0,
        1,
        2,
        0,
        2,
        0,
        2
    };
    Integer[] exp = {
        9,
        1,
        2,
        10,
        14,
        10,
        6,
        9,
        17,
        7
    };
    
    for(int i = 0; i < workType.length; ++i) {
      Data tuple = new Data();
      tuple.age = age[i];
      tuple.exp = exp[i];
      tuple.qual = qual[i];
      tuple.workType = workType[i];
      database.add(tuple);
    }
  }
  
  Starter() {
    database = new Vector<Data>();
    loadData();
    NaiveBayes classifier = new NaiveBayes();
    classifier.fit(database);
    
    Data predictor = new Data();
    predictor.age = 30;
    predictor.exp = 8;
    predictor.qual = 1;
    predictor.workType = -1;
    
    System.out.println("Classifier says : " + classifier.predict(predictor).workType);
  }
}
