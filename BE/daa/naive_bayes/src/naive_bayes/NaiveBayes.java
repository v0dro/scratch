package naive_bayes;
import java.util.*;
import java.lang.Math;

class ClsData {
  public Double ageMean, qualMean, expMean, ageStd, qualStd, expStd;
}

public class NaiveBayes {
  private Vector<Data> trainingData;
  private HashMap<Integer, Vector<Data> > classifiedData;
  private HashMap<Integer, ClsData> summarizedData;
  
  NaiveBayes() {
    trainingData = new Vector<Data>();
    classifiedData = new HashMap<Integer, Vector<Data> >();
    summarizedData = new HashMap<Integer, ClsData>();
  }
  
  private Double mean(Vector<Integer> numbers) {
    Double sum = 0.0;
    for (int i = 0; i < numbers.size(); ++i) { sum += numbers.get(i); }
    
    return sum/numbers.size();
  }
  
  private Double std(Vector<Integer> numbers) {
    Double avg = mean(numbers);
    Double variance = 0.0;
    
    for (int i = 0; i < numbers.size(); ++i) {
      variance += Math.pow(numbers.get(i) - avg, 2.0);
    }
    variance = variance / (numbers.size() - 1);
    
    return Math.sqrt(variance);
  }
  
  private void createClasses() {
    // Create a Hash of Qualification -> Data pairs.
    for (int i = 0; i < trainingData.size(); ++i) {
      Integer workType = trainingData.get(i).workType;
      Data tuple = trainingData.get(i);
      
      if (classifiedData.containsKey(workType)) { // Key already exists so just add to the value Vector<Data>
        classifiedData.get(workType).add(tuple);
      }
      else { // said key does not exist in the HashMap so create it.
        Vector<Data> classifiedSet = new Vector<Data>();
        classifiedSet.add(tuple);
        classifiedData.put(workType,classifiedSet);
      }
    }
  }
  
  private Vector<Integer> getAllAges(Vector<Data> dataset) {
    Vector<Integer> numbers = new Vector<Integer>();
    
    for (int i = 0; i < dataset.size(); ++i) {
      numbers.add(dataset.get(i).age);
    }
    
    return numbers;
  }
  
  private Vector<Integer> getAllExps(Vector<Data> dataset) {
    Vector<Integer> numbers = new Vector<Integer>();
    
    for (int i = 0; i < dataset.size(); ++i) {
      numbers.add(dataset.get(i).exp);
    }
    
    return numbers;    
  }
  
  private Vector<Integer> getAllQual(Vector<Data> dataset) {
    Vector<Integer> numbers = new Vector<Integer>();
    
    for (int i = 0; i < dataset.size(); ++i) {
      numbers.add(dataset.get(i).qual);
    }
    
    return numbers;
  }
  
  private void summarizeByClass() {
    Iterator it = classifiedData.entrySet().iterator();
    
    // the values in each class are a Vector stored in the form of (mean,std)
    while (it.hasNext()) {
      Map.Entry pair       = (Map.Entry) it.next();
      ClsData clf   = new ClsData();
      Vector<Data> value   = (Vector<Data>)pair.getValue();
      Integer workType     = (Integer)pair.getKey();
      
      clf.ageMean     = mean(getAllAges(value));
      clf.ageStd      = std(getAllAges(value));
      clf.expMean     = mean(getAllExps(value));
      clf.expStd      = std(getAllExps(value));
      clf.qualMean    = mean(getAllQual(value));
      clf.qualStd     = std(getAllQual(value));
      
      summarizedData.put(workType, clf);
    }
  }
  
  private Double calculateProbability(Integer x, Double mean, Double std) {
    // Use gaussian theorem to calculate probability.
    Double exp = Math.exp(-(Math.pow(x-mean,2)/(2*Math.pow(std,2))));
    return (1 / (Math.sqrt(2*Math.PI) * std)) * exp;
  }
  
  public void fit(Vector<Data> database) {
    this.trainingData = database;
    createClasses();
    summarizeByClass();
  }
  
  private HashMap<Integer, Double> calculateClassProbabilities(Data predictor) {
    HashMap<Integer, Double> map = new HashMap<Integer, Double>();
    Iterator it = summarizedData.entrySet().iterator();
    Double probability;
    
    while (it.hasNext()) {
      probability = 1.0;
      Map.Entry<Integer, ClsData> pair = (Map.Entry<Integer, ClsData>)it.next();
      ClsData value = pair.getValue();
      probability *= calculateProbability(predictor.age, value.ageMean, value.ageStd);
      probability *= calculateProbability(predictor.exp, value.expMean, value.expStd);
      probability *= calculateProbability(predictor.qual, value.qualMean, value.qualStd);
      
      map.put(pair.getKey(), probability);
    }
    
    return map;
  }
  
  private Integer classWithMaxProbability(HashMap<Integer, Double> probabilities) {
    Double max = Collections.max(probabilities.values());
    Integer index = 0;
    
    for (int i = 0; i < probabilities.values().size(); ++i) {
      if (max == probabilities.values().toArray()[i]) {
        index = i;
        break;
      }
    }
    
    return index;
  }
  
  public Data predict(Data predictor) {
    HashMap<Integer, Double> probabilities = calculateClassProbabilities(predictor);
    predictor.workType =  classWithMaxProbability(probabilities);
    
    return predictor;
  }
  
  public Collection<Double> predict_proba(Data predictor) {
    HashMap<Integer, Double> probabilities = calculateClassProbabilities(predictor);
    
    return probabilities.values();
  }
}
