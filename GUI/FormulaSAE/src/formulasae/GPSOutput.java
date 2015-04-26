/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package formulasae;

import java.util.Date;

/**
 *
 * @author amirhajimirsadeghi
 */
public class GPSOutput {
    //FIELDS
    private String longitude;
    private String latitude;
    private double velocity = 0;
    private Date time;
    
   //CONSTRUCTOR
    public GPSOutput(String longInput, String latInput, 
                     double velInput, Date timeInput){
        longitude = longInput;
        latitude = latInput;
        velocity = velInput;
        time = timeInput;
    }
    //longitude in double
    public String getLongitude(){
        return longitude;
    }
    //longitude as double
    public String getLatitude(){
        return latitude;
    }
    
    public String mapInput(){
        return longitude+", "+latitude;
    }
    
    //velocity as ddouble
    public double getVelocity(){
        return velocity;
    }
    
    //time in miliseconds since january 1,1970
    public long getTime(){
        return time.getTime();
    }
}
