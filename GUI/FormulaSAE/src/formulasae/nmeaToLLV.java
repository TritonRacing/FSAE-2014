/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package formulasae;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.net.URL;
import java.util.Calendar;
import java.util.Date;

/**
 * This class will handle reading in a text file that has lines of nmea 
 * sentences and turnig it into longitude, Lagitude, and Velocity data.
 * @author amirhajimirsadeghi
 */
public class nmeaToLLV {
    //FIELDS
    private double longitude= 0;
    private double latitude = 0;
    private double velocity = 0;
    private Date time;
    
    
    //CONSTRUCTOR
    public nmeaToLLV(String nmeaFilePath) throws FileNotFoundException{
        URL url = getClass().getResource(nmeaFilePath);
        File file = new File(url.getPath());
        InputStream input = new FileInputStream(file);
    }
    
    void extractData(String nmeaSentence){
        //Array to hold different parts of the sentence
        String[] sentenceArray = nmeaSentence.split(",");

        //EXTRACT TIME FROM SENTENCE
        int fullTime = Integer.parseInt(sentenceArray[1]);
        int second = fullTime%100;
        fullTime = fullTime/100;
        int minute = fullTime%100;
        fullTime = fullTime/100;
        int hour = fullTime;
        
        //initialize time of sentence
        Calendar timeHolder = Calendar.getInstance();
        timeHolder.set(Calendar.HOUR_OF_DAY,hour);
        timeHolder.set(Calendar.MINUTE, minute);
        timeHolder.set(Calendar.SECOND,second);
        time = timeHolder.getTime();
        
    }
    
}
