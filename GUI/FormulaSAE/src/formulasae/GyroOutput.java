/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package formulasae;

/**
 *
 * @author amirhajimirsadeghi
 */
public class GyroOutput {
    //FIELDS
    private double x;
    private double y;
    private double z;
    
    //CONSTRUCTORS
    public GyroOutput(double inX,double inY,double inZ){
        x = inX;
        y = inY;
        z = inZ;
    }
    
    //Getters
    public double getX(){
        return x;
    }
    public double getY(){
        return y;
    }
    public double getZ(){
        return z;
    }
    
}
