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
    
    //CONSTRUCTOR
    public nmeaToLLV(String nmeaFilePath) throws FileNotFoundException{
        URL url = getClass().getResource(nmeaFilePath);
        File file = new File(url.getPath());
        InputStream input = new FileInputStream(file);
    }
    
}
