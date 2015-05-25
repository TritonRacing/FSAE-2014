/*
 * This file reads in a data output from a gyroscope and accelerometer sensor
 * then parses the data and grabs x, y, and z componenets from each and passes
 * that inforamtion so that it could be used to graph.
 */
package formulasae;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;

/**
 *
 * @author amirhajimirsadeghi
 */
public class GyroAccelData {
    //FIELDS
    public AccelOutput[] accelOutputs;
    public GyroOutput[] gyroOutputs;
    public int[] milliseconds;
    
    //CONSTRUCTORS
    public GyroAccelData(String dataFilePath) throws FileNotFoundException, IOException{
        //gets a URL from the output file path, use that to open a file,
        //and turn that file into a file stream so we can extract data
        URL url = getClass().getResource(dataFilePath);
        File file = new File(url.getPath());
        FileInputStream input = new FileInputStream(file);
        //this will be used to extract line by line strings
        BufferedReader br = new BufferedReader(new InputStreamReader(input));
        
        ArrayList<String> strLines = new ArrayList<>();
        String strLine;
        int lineCount=0;
        
        String firstLine = br.readLine();
        firstLine = br.readLine();
        
        while ((strLine = br.readLine()) != null)   {
            strLines.add(strLine);
            lineCount++;
            
        }
        accelOutputs = new AccelOutput[lineCount];
        gyroOutputs = new GyroOutput[lineCount];
        
        for(int i = 0; i < lineCount; i++){
            String[] elements = strLines.get(i).split("\t");
            double accelX = Double.parseDouble(elements[0]);
            double accelY = Double.parseDouble(elements[1]);
            double accelZ = Double.parseDouble(elements[2]);
            double gyroX = Double.parseDouble(elements[3]);
            double gyroY = Double.parseDouble(elements[4]);
            double gyroZ = Double.parseDouble(elements[5]);
            
            milliseconds[i] = Integer.parseInt(elements[6]);
            accelOutputs[i] = new AccelOutput(accelX,accelY,accelZ);
            gyroOutputs[i] = new GyroOutput(gyroX,gyroY,gyroZ);
            
                    
        }
        
        br.close();
    }
    
    public AccelOutput[] getAccelOutputs(){
        return accelOutputs;
    }
    
    public GyroOutput[] getGyroOutputs(){
        return gyroOutputs;
    }
    public int[] getMilliseconds(){
        return milliseconds;
    }
}
