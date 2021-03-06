package formulasae;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

/**
 * This class will handle reading in a text file that has lines of nmea 
 * sentences and outputing an array of GPSOutput objects that hold extracted
 * data.
 * @author amirhajimirsadeghi
 */
public class nmeaToLLV {
    //FIELDS
    public GPSOutput[] fileOutputs;
    
    
    //CONSTRUCTOR
    //initializes the nmea to readable data in GPSOutput objects
    public nmeaToLLV(String nmeaFilePath) throws FileNotFoundException, IOException{
        //gets a URL from the output file path, use that to open a file,
        //and turn that file into a file stream so we can extract data
        URL url = getClass().getResource(nmeaFilePath);
        File file = new File(url.getPath());
        FileInputStream input = new FileInputStream(file);
        //this will be used to extract line by line strings
        BufferedReader br = new BufferedReader(new InputStreamReader(input));
        
        ArrayList<String> strLines = new ArrayList<>();
        String strLine;
        int lineCount=0;
        
        String firstLine = br.readLine();
        //Read File Line By Line
        while ((strLine = br.readLine()) != null)   {
            strLines.add(strLine);
            lineCount++;
            
        }
        fileOutputs = new GPSOutput[lineCount];
        for(int i =0; i< strLines.size();i++){
            String[] nmeaArray = strLines.get(i).split(",");
            
            if(nmeaArray[2].toLowerCase().equals("v")){
                continue;
            }
            fileOutputs[i] = extractData(nmeaArray);
        }
        //Close the input stream
        br.close();
        
        /*turn InputStream into array of strings and use iput stream
          to extract data from each sentence*/
    }
    
    final GPSOutput extractData(String[] sentenceArray){
        

        //EXTRACT TIME FROM SENTENCE
        String[] fullTimeComp = sentenceArray[1].split(".");
        
        int millisecond = 0;
        
        if(fullTimeComp.length > 1){
            //handle milliseconds
            millisecond = Integer.parseInt(fullTimeComp[1]);
        }
        //handle other time
        int fullTime = Integer.parseInt(fullTimeComp[0]);
        int second = fullTime%100;
        fullTime = fullTime/100;
        int minute = fullTime%100;
        fullTime = fullTime/100;
        int hour = fullTime;
        
        //handle the date
        String day = sentenceArray[9];
        int dayVals = Integer.parseInt(day);
        int year = dayVals%100;
        dayVals = dayVals/100;
        int month = dayVals%100;
        dayVals = dayVals/100;
        
        //initialize time from the sentence
        Calendar timeHolder = Calendar.getInstance();
        timeHolder.set(Calendar.YEAR,year);
        timeHolder.set(Calendar.MONTH,month);
        timeHolder.set(Calendar.DAY_OF_MONTH,dayVals);
        timeHolder.set(Calendar.HOUR_OF_DAY,hour);
        timeHolder.set(Calendar.MINUTE, minute);
        timeHolder.set(Calendar.SECOND,second);
        timeHolder.set(Calendar.MILLISECOND,millisecond);
        Date time = timeHolder.getTime();
        
        //initialize latitude Value
        String latitudeVal = sentenceArray[3];
        latitudeVal = latitudeVal.substring(0,2)+" "+latitudeVal.substring(2,latitudeVal.length());
        if(sentenceArray[4].equals("N")){
            latitudeVal = "+"+latitudeVal;
        }
        else{
            latitudeVal = "-"+latitudeVal;
        }
        
        //initialize longitude value
        String longitudeVal = sentenceArray[5];
        longitudeVal = longitudeVal.substring(0,3)+" "+longitudeVal.substring(3,longitudeVal.length());
        if(sentenceArray[6].equals("E")){
            longitudeVal = "+"+longitudeVal;
        }
        else{
            longitudeVal = "-"+longitudeVal;
        }
        
        //Velocity Calculator
        String knotString = sentenceArray[7];
        double knots = Double.parseDouble(knotString);
        double mphAccurate = knots*1.15078;
        int mphRounded = (int)mphAccurate;
        
        return new GPSOutput(latitudeVal,longitudeVal,mphRounded,time);
    }
    
    public GPSOutput[] getOutputs(){
        return fileOutputs;
    }
}
