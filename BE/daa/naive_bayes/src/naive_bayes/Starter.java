package naive_bayes;

import java.util.Vector;

class Data {
  public String workType, qual;
  public Integer age, exp;
}

public class Starter {
  Vector<Data> database;
  
  private void loadData() {
    String[] workType = {
        "Consultancy", 
        "Service", 
        "Research", 
        "Service", 
        "Consultancy",
        "Research",
        "Research",
        "Service",
        "Consultancy",
        "Research"};
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
    String[] qual = {
        "PhD",
        "MTech",
        "MTech",
        "BTech",
        "MTech",
        "PhD",
        "BTech",
        "MTech",
        "BTech",
        "PhD"
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
    
    for(int i = 0; i < 10; ++i) {
      Data tuple = new Data();
      tuple.age = age[i];
      tuple.exp = exp[i];
      tuple.qual = qual[i];
      tuple.workType = workType[i];
      database.add(tuple);
    }
  }
  
  Starter() {
    loadData();
    NaiveBayes classifier = new NaiveBayes();
    classifier.fit(database, new String[] {"exp", "workType", "age", "qual"});
    
    Data predictor = new Data();
    predictor.age = 30;
    predictor.exp = 8;
    predictor.qual = "MTech";
    predictor.workType = "";
    
    System.out.println("Classifier says : " + classifier.predict(predictor, "workType").workType);
  }
}
